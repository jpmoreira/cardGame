
#include "Player.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "_card_def.h"
#include "_player_def.h"

/**
 * @brief A function that creates a player and sets all it's required parameters
 * @return a pointer to the recently initialized player
 */
Player * createPlayer(char* name){
	Player *p=malloc(sizeof(Player));
	p->name=malloc((strlen(name)+1)*sizeof(char));
	p->pseudo_name=malloc((strlen(name)+1)*sizeof(char));
	sprintf(p->name,"%s",name);
	sprintf(p->pseudo_name,"%s",name);
	p->handSize=-1;
	p->nrCards=0;
	p->id=-1;
	p->hand=NULL;
	p->fifoName=NULL;
	return p;

}
void deletePlayer(Player **p){
	if((*p)->fifoName){//if it has a fifo name
		unlink((*p)->fifoName);
		free((*p)->fifoName);
	}
	free((*p)->name);
	free(*p);
	*p=NULL;

}
int getID(Player *p){
	if(p==NULL){
		return -1;
	}
	return p->id;
}
int getHandSize(Player *p){
	if(p==NULL){
		return -1;
	}
	return p->handSize;
}
int getNrCards(Player *p){
	if(p==NULL){
		return -1;
	}
	return p->nrCards;
}
void printHandCards(Player *p){
	if(p==NULL){return;}
	int i=0;
	printf("Hand of player %d:\n",p->id);
	for(;i<p->handSize;i++){
		printCard(&(p->hand[i]),true);
		if(i<p->nrCards-1){
			printf("-");
		}
	}
	printf("\n");
}
char *getFifoName(Player *p){
	if (p==NULL){return NULL;}

	p->pseudo_fifoName=p->fifoName;
	return p->pseudo_fifoName;
}
int getCardsFromFifo(Player *p,int nrOfCards,bool block){
	if(p==NULL){return 0;}

	int fd;
	if(block){fd=open(p->fifoName,O_RDONLY);}
	else{fd=open(p->fifoName,O_RDONLY|O_NONBLOCK);}

	if(fd==-1){return 0;}

	int cardsRead=0;
	while(cardsRead<nrOfCards ){
		Card c=_createCard(No_Suit,No_Value);
		int readRet=read(fd,&c,sizeof(Card));
		if(readRet==sizeof(Card)){//if we read a hole Card
			addCardToHand(p,c);
			cardsRead++;
		}
	}
	return cardsRead;
}
int emptyHandCardSpot(Player *p){
	if(p==NULL	|| p->hand==NULL){return -1;}
	int i=0;
	for(;i<p->handSize;i++){
		Card *c=&(p->hand[i]);
		if(!(isValidCard(c))){
			return i;
		}
	}
	return -1;
}
void addCardToHand(Player *p,Card c){

	int index=0;
	if(p==NULL || p->hand==NULL){return;}
	if(index>=p->handSize){return;}
	while(isValidCard(&(p->hand[index]))){index++;}
	if(index>=p->handSize){return;}
	p->hand[index]=c;
	p->nrCards++;
}
char *getPlayerName(Player *p){
	if (p==NULL || p->name==NULL){return NULL;}
	sprintf(p->pseudo_name,"%s",p->name);
	return p->pseudo_name;
}
bool isDealer(Player *p){
	if(p==NULL || p->id!=0){return false;}
	return true;
}
Card * getHandCard(Player *p,int index){
	if(index>=p->handSize || index<0){return NULL;}
	return &(p->hand[index]);
}

