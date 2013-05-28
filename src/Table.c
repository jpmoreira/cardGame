#include "Table.h"
#include "_card_def.h"
#include "_player_def.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "_table_def.h"




int getNrPlayers(Table * table){

	if(table==NULL){
		return -1;
	}
	return table->nrPlayers;

}

int getAvailableSeats(Table *t){
	if(t==NULL){
		return -1;
	}
	return t->availableSeats;

}

bool isFull(Table *t){
	if(t==NULL){
		return true;
	}
	if(t->availableSeats==0){
		return true;
	}
	return false;


}
int getTurn(Table *t){
	if(t==NULL){
		return -1;
	}
	return t->turn;
}

int getRound(Table *t){
	if(t==NULL){return -1;}
	return t->round;
}
void updateTurn(Table *t){

	if(t==NULL){
		return;
	}
	if(t->turn==-1){//if the game is about to start
		t->turn=0;//set the first player as the one that should play
	}
	int previousTurn=t->turn;
	int nrPlayers=t->nrPlayers;
	if(previousTurn>=nrPlayers-1){
		t->turn=0;
		return;
	}

	t->turn=previousTurn+1;
}

Card getUsedCard(Table * table,int index){
	if(index<0 || index>51|| table==NULL || table->usedCards==NULL){
		Card c;
		c.cardValue=No_Value;
		c.cardSuit=No_Suit;
		return c;
	}
	return table->usedCards[index];


}
Card *getRoundCards(Table *t,int round){

	if(round>=t->round){return NULL;}//if round has not yet been played or if it's the currently playing round
	int i=round*t->nrPlayers;
	int limit=i+t->nrPlayers;
	Card *array=malloc(t->nrPlayers*sizeof(Card));

	if(array==NULL){return array;}//if malloc didn't work

	for(;i<limit;i++){
		array[i]=t->usedCards[i];
	}
	return array;

}

Card getTableCard(Table *table,int index){
	if(table==NULL || index>=table->nrPlayers ||index<0|| table->tableCards==NULL){//if no valid index is given or if there are no cards in the table
		Card c;
		c.cardSuit=No_Suit;
		c.cardValue=No_Value;
		fillName(&c);
		return c;
	}
	return table->tableCards[index];


}

Card getDeckCard(Table *table,int index){
	if(table==NULL || index<0 || index>51|| table->deck==NULL){
		Card c;
		c.cardSuit=No_Suit;
		c.cardValue=No_Value;
		fillName(&c);
		return c;
	}
	return table->deck[index];

}

Table * createTable(int nrSeats,int cardsPerPlayer){

	if(nrSeats<2){return NULL;}
	Table *t=malloc(sizeof(Table));
	if(t==NULL){return t;}
	t->malloced=true;
	t->availableSeats=nrSeats;
	t->nrPlayers=0;
	t->round=-1;
	t->lastID=-1;
	t->active=true;


	if(cardsPerPlayer>=DECK_SIZE/nrSeats){
		t->cardsPerPlayer=DECK_SIZE/nrSeats;
	}
	else if(cardsPerPlayer<0){
			free(t);
			return NULL;
		}
	else{
		t->cardsPerPlayer=cardsPerPlayer;
	}


	//t->tableCards=malloc(t->nrPlayers*sizeof(Card));
//	if(t->tableCards==NULL){//if could not allocate space for the tableCards
//		free(t);//free what we have allocated so far
//		return NULL;
//	}
	Card c=_createCard(No_Suit,No_Value);
	fillArrayWithCard(t->tableCards,c,nrSeats);
	fillArrayWithCard(t->usedCards,c,DECK_SIZE);
	fillDeck(t);
	t->turn=-1;


	return t;


}

void deleteTable(Table **t){
	if((*t)->malloced){
//		free((*t)->tableCards);
//		(*t)->tableCards=NULL;
		free(*t);
		*t=NULL;
	}

}

void fillDeck(Table *t){
	if(t==NULL){return;}
	Suit s=Hearts;
	CardValue c=Ace;
	int index=0;
	for(;s<NR_SUITS;s++){
		for(c=Ace;c<NR_CARD_VALUES;c++){
			Card card=_createCard(s,c);
			t->deck[index++]=card;
		}

	}
}

void printDeck(Table *t){
	if(t==NULL){return;}
	int i=0;
	printf("start-");
	for(;i<DECK_SIZE;i++){
		bool printed=printCard(&(t->deck[i]),false);
		if(printed){printf("-");}
	}
	printf("end\n");

}


void shuffleDeck(Table *t){
	if(t==NULL){return;}
	int nrSwaps=0;
	srand(time(NULL));
	while(nrSwaps<10){nrSwaps=rand()%100;}

	int card1Index=0;
	int card2Index=0;
	srand(time(NULL));
	while(nrSwaps){
		card1Index=rand()%DECK_SIZE;
		card2Index=rand()%DECK_SIZE;
		if(card2Index!=card1Index){
			Card temp=t->deck[card1Index];
			t->deck[card1Index]=t->deck[card2Index];
			t->deck[card2Index]=temp;
			nrSwaps--;
		}
	}

}

void printUsedCards(Table *t){
	if (t==NULL){return;}
	int i=0;
	bool printed;
	printf("Used Cards: ");
	for(;i<DECK_SIZE;i++){
		printed=printCard(&(t->usedCards[i]),false);
		if(printed && i<DECK_SIZE-1){printf("-");}
	}
	printf("\n");

}

void printTableCards(Table *t){
	if(t==NULL){return;}
	int i=0;
	printf("Table cards:");
	for(;i<t->nrPlayers;i++){
		printCard(&(t->tableCards[i]),true);
		if(t<t->nrPlayers-1){
			printf("-");
		}
	}
	printf("\n");

}

Table *grabTable(void *mb,int nrPlaces,int cardsPerPlayer){
	Table *t=mb;
	return t;
}

Table * _createTable(void *mb,int nrPlaces,int cardsPerPlayer){
	Table *t=mb;
	t->availableSeats=nrPlaces;
	t->active=true;
	t->nrPlayers=0;
	t->round=-1;
	t->turn=-1;
	t->lastID=-1;
	Card c=_createCard(No_Suit,No_Value);
	fillDeck(t);
	fillArrayWithCard(t->usedCards,c,DECK_SIZE);
	//t->tableCards=mb+sizeof(Table);//tableCards is placed at the end of the Table struct
	fillArrayWithCard(t->tableCards,c,nrPlaces);//fill it
	t->malloced=false;

	if(cardsPerPlayer>=DECK_SIZE/nrPlaces){
		t->cardsPerPlayer=DECK_SIZE/nrPlaces;
	}
	else if(cardsPerPlayer<0){
			return NULL;
		}
	else{
		t->cardsPerPlayer=cardsPerPlayer;
	}


	return t;


}

int get_Table_required_mem_size(int nrPlaces){
	//return sizeof(Table)+nrPlaces*sizeof(Card);//size of table + the size of the table card's array
	return 2*sizeof(Table);
}

bool giveCards(Table *t,int nrCards,int playerID){
	if(nrCards<=nrOfCardsInDeck(t)){
		if(playerID<0 || playerID>t->lastID+1){
			return false;
		}

		char fifoName[FIFO_NAME_LEN];
		sprintf(fifoName,FIFO_TEMPLATE_STR,playerID);
		int fd=open(fifoName,O_WRONLY|O_TRUNC);
		if(fd==-1){
			return false;
		}
		int count=0;
		while(count<nrCards){//place stuff on the fifo
			Card c=removeCardFromDeckTop(t);
			write(fd,&c,sizeof(Card));
			count++;
		}
		close(fd);
		return true;

	}
	return false;
}


int nrOfCardsInDeck(Table *t){
	if(t==NULL || t->deck==NULL){
		return 0;
	}
	int count=0;
	int i=0;
	for(;i<DECK_SIZE;i++){
		Card c=t->deck[i];
		if(isValidCard(&c)){
			count++;
		}
	}
	return count;
}

Card removeCardFromDeckTop(Table *t){
	if (t==NULL || t->deck==NULL){return _createCard(No_Suit,No_Value);}
	Card c;
	int i=0;
	for(;i<DECK_SIZE;i++){
		Card cardFromDeck=t->deck[i];
		if(isValidCard(&cardFromDeck)){
			c=cardFromDeck;
			t->deck[i]=_createCard(No_Suit,No_Value);
			return c;
		}
	}
	return _createCard(No_Suit,No_Value);
}

bool isActive(Table *t){
	return t->active;
}
void inactivate(Table *t){
	t->active=false;
}
void setTurn(Table *t, int turn){
	if(t==NULL || turn >=t->nrPlayers){return;}
	t->turn=turn;
}

void cleanTableCards(Table *t){
	int i=0;
	for(; i<t->nrPlayers;i++){
		int place=getFirstNonFilledUsedCard(t);
		t->usedCards[place]=t->tableCards[i];
		t->tableCards[i]=_createCard(No_Suit,No_Value);
		place++;
	}
}
int getFirstNonFilledUsedCard(Table *t){
	int i=0;
	for(;i<DECK_SIZE;i++){
		if(!isValidCard(&(t->usedCards[i]))){return i;}
	}
	return -1;
}
int getFirstNonFilledTableCardPosition(Table *t){
	int i=0;
	for(;i<t->nrPlayers;i++){
		if(!isValidCard(&(t->tableCards[i]))){return i;}
	}
	return -1;
}

