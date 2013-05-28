/*
 ============================================================================
 Name        : Generic-Card-Game.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "Table.h"
#include "Player.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "userActions.h"


Player *player=NULL;
pthread_t inputThreadID=-1;
time_t turnStartTime=0;
char logFilePath[50];
pthread_mutex_t internalMut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cardPlayedCond=PTHREAD_COND_INITIALIZER;
int cardToPlayIndex=-1;
shm_vars shm;

/**
 * @brief A function used to create the log file and/or set logFilePath
 *
 */
void initLogFile(char *name){

	sprintf(logFilePath,"../%s.log",name);//get file name

	if(isDealer(player)){//only dealer creates file
		pthread_mutex_lock(shm.tableResourcesMutex);
		FILE * logFileFD=fopen(logFilePath,"w");//created or truncated
		fprintf(logFileFD,"when|who|what|result\n");
		fclose(logFileFD);
		pthread_mutex_unlock(shm.tableResourcesMutex);

	}
}

/**
 * @brief A function that writes on the file a given action with a given result.
 *
 */
void logToFile(char *action,char *result){
	char name [50];
	if(isDealer(player)){//dealer has special prefix
		sprintf(name,"Dealer-%s",getPlayerName(player));
	}
	else{
		sprintf(name,"%s",getPlayerName(player));
	}

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );//obtain current time
	pthread_mutex_lock(shm.tableResourcesMutex);
	FILE *logFileFD=fopen(logFilePath,"a");
	fprintf(logFileFD,"%s|%s|%s|%s\n",asctime(timeinfo),name,action,result);
	fclose(logFileFD);
	pthread_mutex_unlock(shm.tableResourcesMutex);
}
/**
 * @brief A function used to log the info related to the placement of a card on the table
 *
 *
 */
void logPlayActionToFile(char *playedCardName){
	logToFile("play",playedCardName);//log the play occurence
	char hand[200];
	int handSize=getHandSize(player);
	int i=0;

	//fill hand array
	Card *c=getHandCard(player,i);
	sprintf(hand,"%s",getName(c));
	for(i=1;i<handSize;i++){
		c=getHandCard(player,i);
		if(isValidCard(c)){
			sprintf(hand,"%s-%s",hand,getName(c));
		}
	}
	//log it
	logToFile("hand",hand);

}
/**
 *
 * @brief A function used to leave the game and clean all used resources
 *
 */
int leaveGame(){

	removePlayer(&player,shm.t);
	if(getNrPlayers(shm.t)==0){//if we left and there is no other player
		pthread_cond_destroy(shm.tableResourcesCond);
		pthread_mutex_destroy(shm.tableResourcesMutex);
		munmap(shm.smBlock,shm.smSize);
		shm_unlink(shm.smName);
		free(shm.smName);
		shm.smID=-1;
		shm.smSize=-1;
		shm.smBlock=NULL;
		deletePlayer(&player);
		shm.tableResourcesCond=NULL;
		shm.tableResourcesMutex=NULL;
	}
	else{
		pthread_cond_broadcast(shm.tableResourcesCond);//say we're leaving
		munmap(shm.smBlock,shm.smSize);
		shm.smID=-1;
		shm.smSize=-1;
		shm.smBlock=NULL;
		deletePlayer(&player);
		shm.tableResourcesCond=NULL;
		shm.tableResourcesMutex=NULL;
		inactivate(shm.t);
		shm.t=NULL;
	}

	exit(0);//kill all threads
	return 0;
}


/**
 * @brief A function that attempts to open a shared memory block with a given name exclusively or not. shm global var is set
 *
 *
 */
void attemptToOpenBlock(char *blockName,bool exclusive){

	int memBlockID;
	if(exclusive){
		memBlockID=shm_open(blockName,O_CREAT|O_RDWR|O_EXCL,0666);
	}
	else{
		memBlockID=shm_open(blockName,O_RDWR,0666);
	}
	shm.smID=memBlockID;
	shm.smName=blockName;
}
/**
 * @brief A function that creates a memory block to hold the structures for a given game
 *
 *
 */
void createMemBlock(int nrPlayers,char *tableName,int blockSize,bool *excl){
	char *blockName=malloc((strlen(tableName)+2)*sizeof(char));
	sprintf(blockName,"/%s",tableName);

	attemptToOpenBlock(blockName,true);//try to open it in exclusive mode;
	if(shm.smID==-1){//if could not open it
		(*excl)=false;//excl is false
		return attemptToOpenBlock(blockName,false);//open in non exclusive mode
	}



	if(ftruncate(shm.smID,blockSize)<0){//if opened in exclusive mode get space
		shm.smID=-1;
		free(shm.smName);
		shm.smName=NULL;
		return;
	}
	shm.smSize=blockSize;
	(*excl)=true;
	return;
}
/**
 *
 * @brief A function that creates and maps a shared memory block with the requirements of the game
 *
 *
 */
bool createAndMapBlock(int nrPlayers,char *tableName){
	int blockSize=get_Table_required_mem_size(nrPlayers)+sizeof(pthread_mutex_t)+sizeof(pthread_cond_t);
	bool excl=false;
	createMemBlock(nrPlayers,tableName,blockSize,&excl);//creates the block

	if(shm.smID==-1){return false;}//if cant create return
	shm.smBlock=mmap(NULL,blockSize,PROT_READ|PROT_WRITE,MAP_SHARED,shm.smID,0);
	shm.tableResourcesMutex=shm.smBlock;
	shm.tableResourcesCond=shm.smBlock+sizeof(pthread_mutex_t);
	if(excl){//if it's a newly created block
		//initialize cond vars and mutex
		pthread_mutexattr_t mattr;
		 pthread_mutexattr_init(&mattr);
		 pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
		 pthread_mutex_init(shm.tableResourcesMutex, &mattr);

		 pthread_condattr_t cattr;
		 pthread_condattr_init(&cattr);
		 pthread_condattr_setpshared(&cattr,PTHREAD_PROCESS_SHARED);
		 pthread_cond_init(shm.tableResourcesCond,&cattr);



		 //shuffle and add player
		 pthread_mutex_lock(shm.tableResourcesMutex);//lock the mutex right away
		 shm.t=_createTable(shm.tableResourcesCond+sizeof(pthread_cond_t),nrPlayers,DECK_SIZE/nrPlayers);
		 shuffleDeck(shm.t);
		 addPlayer(shm.t,player);
		 pthread_mutex_unlock(shm.tableResourcesMutex);
	}
	else{
		//if it was already created
		shm.t=grabTable(shm.tableResourcesCond+sizeof(pthread_cond_t),nrPlayers,DECK_SIZE/nrPlayers);
		pthread_mutex_lock(shm.tableResourcesMutex);//try to obtain lock
		addPlayer(shm.t,player);
		pthread_cond_broadcast(shm.tableResourcesCond);//say we have new player added
		pthread_mutex_unlock(shm.tableResourcesMutex);
	}
		return true;


}

/**
 * @brief A function that distributes the cards to the players
 *
 *
 */
void dealCards(){
	int dealed=0;
	open(getFifoName(player),O_RDONLY|O_NONBLOCK);//open our own fifo so we dont block reading
	while(dealed!=getNrPlayers(shm.t)){//while we havent given cards to everyone
		pthread_mutex_lock(shm.tableResourcesMutex);//lock the mutex
		while(getAvailableSeats(shm.t)!=0){//while there are empty seats
			if(!isActive(shm.t)){//if someone left unlock mutex and return
				pthread_mutex_unlock(shm.tableResourcesMutex);
				return;
			}
			pthread_cond_wait(shm.tableResourcesCond,shm.tableResourcesMutex);//wake me up when someone arrives
		}
		giveCards(shm.t,DECK_SIZE/getNrPlayers(shm.t),dealed++);
		pthread_mutex_unlock(shm.tableResourcesMutex);
	}
	logToFile("deal","-");//log the deal operation

}
/**
 * @brief A function that gets all the cards from the fifo and logs the operation to the log file
 *
 *
 */
bool getCards(bool block){
	if(!isActive(shm.t)){return false;}
	getCardsFromFifo(player,DECK_SIZE/getNrPlayers(shm.t),block);

	char hand[200];
	int handSize=getHandSize(player);
	int i=0;
	Card *c=getHandCard(player,i);
	sprintf(hand,"%s",getName(c));
	for(i=1;i<handSize;i++){//for all hand cards
		c=getHandCard(player,i);
		sprintf(hand,"%s-%s",hand,getName(c));
	}
	logToFile("recieved cards",hand);

	return true;


}
/**
 *
 * @brief A function that checks the validity of the command line parameters
 */
bool checkParametersValidity(int argc,char **argv){
	if(argc!=4){//if not enough parameters
		printf("Bad parameters. Usage\n tpc <player's name> <shm name> <n. players>\n");
		return false;
	}
	int nrPlayers=atoi(argv[3]);
	if(nrPlayers<=1 || nrPlayers>52){//invalid nr of players
		printf("Bad nr of Players. Nr of players must be a positive number between 2 and 52.\n");
		return false;
	}
	return true;
}
/**
 * @brief A function that verifies if it's our turn of playing
 *
 *
 */
bool isOurTurn(){
	pthread_mutex_lock(shm.tableResourcesMutex);
	int turn=getTurn(shm.t);
	pthread_mutex_unlock(shm.tableResourcesMutex);
	if(turn==getID(player)){return true;}
	return false;
}
/**
 * @brief A function that print the menu to the stdout
 *
 */
void printMenu(){
	printf("Player %s (%d)\n",getPlayerName(player),getID(player));
	printf("\n##########MENU#########\n");
	printf("1-> See Hand Cards\n");
	printf("2-> See Table Cards\n");
	printf("3-> See Used Cards\n");
	printf("4-> See Turn\n");
	printf("5-> End Game\n");
	printf("6-> See time Elapsed\n");
	printf("7-> Play Card\n");
}
/**
 * @brief A function that given the turn a random suitable number
 *
 */
void defineTurn(){
	srand(time(NULL));
	pthread_mutex_lock(shm.tableResourcesMutex);
	int nr=rand()%getNrPlayers(shm.t);
	setTurn(shm.t,nr);
	pthread_mutex_unlock(shm.tableResourcesMutex);

}







/**
 * @brief A function that operates as the main program loop. It plays cards if the order was issued in syncronization with the other players. It also leaves the game if any of the players abandon it
 *
 */
void mainLoop(){

	while(1){

		Card *playedCard=NULL;
		bool played=false;
		pthread_mutex_lock(shm.tableResourcesMutex);
		if(!isActive(shm.t)){//if someone left
			pthread_mutex_unlock(shm.tableResourcesMutex);//free mutex and leave
			leaveGame();
		}

		int turn=getTurn(shm.t);


		if(turn!=getID(player)){//if it's not our turn sleep
			pthread_cond_wait(shm.tableResourcesCond,shm.tableResourcesMutex);
		}
		if(turn==getID(player)){//if it's our turn
			turnStartTime=time(0);
			pthread_mutex_lock(&internalMut);//lock internal
			played=playCard(player,shm.t,cardToPlayIndex);
			while(!played){//if the card was not a playable one
				pthread_mutex_unlock(shm.tableResourcesMutex);//unlock the external one while we wait
				pthread_cond_wait(&cardPlayedCond,&internalMut);
				pthread_mutex_lock(shm.tableResourcesMutex);//lock again so we can play
				Card *temp=getHandCard(player,cardToPlayIndex);
				playedCard=createCard(getSuit(temp),getCardValue(temp));
				played=playCard(player,shm.t,cardToPlayIndex);
			}
			pthread_cond_broadcast(shm.tableResourcesCond);
			pthread_mutex_unlock(&internalMut);
			cardToPlayIndex=-1;

		}
		pthread_mutex_unlock(shm.tableResourcesMutex);
		if(played){//if we played in the previous loop then log info
			logPlayActionToFile(getName(playedCard));
			deleteCard(&playedCard);
		}



	}

}

/**
 *
 * @brief A function launched in another thread that gets user input and acts accordingly
 *
 */
void* getUserInput(void * arg){

	int i=-1;
	while(1){

		printMenu();

		scanf("%i",&i);

		switch (i) {
			case 1:
				printHandCards(player);
				break;
			case 2:
				displayTableCards_action();
				break;
			case 3:
				displayUsedCards_action();
				break;
			case 4:
				displayTurn_action();
				break;
			case 5:
				leaveGame();
				break;
			case 6:
				displayElapsedTime_action();
				break;
			case 7:
				playCard_action();
				break;
			default:
				break;
		}


	}
	return NULL;

}



int main(int argc, char **argv) {

	//Are parameters ok?
	bool parameters=checkParametersValidity(argc,argv);
	if(!parameters){return 1;}

	int nrPlayers=atoi(argv[3]);
	char *tableName=argv[2];

	player=createPlayer(argv[1]);
	if(player==NULL){return 2;}

	printf("Sucessfully created player struct\n");

	printf("Creating shared memory block\n");

	createAndMapBlock(nrPlayers,tableName);

	if(player==NULL || shm.smBlock==NULL || shm.tableResourcesCond==NULL || shm.tableResourcesMutex==NULL || shm.t==NULL){
		printf("Ups! Something went wrong\n Program will terminate\n");
		return 3;
	}

	initLogFile(argv[2]);//init the log file

	if(isDealer(player)){
		printf("Will deal cards...\n");
		dealCards();
		printf("Cards dealt\nWill define turn\n");
		defineTurn();
		pthread_cond_broadcast(shm.tableResourcesCond);
		printf("Turn defined\n");
	}

	printf("Will get cards\n");
	if(isDealer(player)){
		getCards(false);
	}
	else{getCards(true);}

	printf("Got cards\n");

	printf("Launching user listner thread\n");

	//launch thread for input
	pthread_create(&inputThreadID,NULL,getUserInput,NULL);


	printf("Will start Game\n");
	mainLoop();//enter the main program loop


	leaveGame();


	return 0;
}



