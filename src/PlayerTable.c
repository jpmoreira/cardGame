/*
 * PlayerTable.c
 *
 *  Created on: May 27, 2013
 *      Author: root
 */


#include "PlayerTable.h"
#include "_table_def.h"
#include "_player_def.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
bool addPlayer(Table *t,Player *newPlayer){
	if(t==NULL || newPlayer==NULL || t->availableSeats<=0){return false;}
	newPlayer->id=++(t->lastID);
	newPlayer->hand=malloc(t->cardsPerPlayer*sizeof(Card));
	newPlayer->fifoName=malloc(FIFO_NAME_LEN*sizeof(char));
	newPlayer->pseudo_fifoName=malloc(FIFO_NAME_LEN*sizeof(char));
	sprintf(newPlayer->fifoName,FIFO_TEMPLATE_STR,newPlayer->id);
	sprintf(newPlayer->pseudo_fifoName,FIFO_TEMPLATE_STR,newPlayer->id);
	unlink(newPlayer->fifoName);
	mkfifo(newPlayer->fifoName,0666);


	newPlayer->nrCards=0;
	newPlayer->handSize=t->cardsPerPlayer;
	t->availableSeats--;
	t->nrPlayers++;
	fillArrayWithCard(newPlayer->hand,_createCard(No_Suit,No_Value),newPlayer->handSize);
	return true;


}
void removePlayer(Player **p,Table *t){
	if((*p)==NULL || t==NULL || (*p)->id==-1){return;}

	t->nrPlayers--;
	t->availableSeats++;
	unlink((*p)->fifoName);
	char *name=getPlayerName(*p);
	char *tempName=malloc((strlen(name)+1)*sizeof(char));
	sprintf(tempName,"%s",name);
	deletePlayer(p);
	*p=createPlayer(tempName);
	free(tempName);
}


bool playCard(Player *p,Table *t,int index){
	if(p==NULL || t==NULL || index>=t->cardsPerPlayer || index<0){return false;}
	Card *c=createCard(getSuit(&(p->hand[index])),getCardValue(&(p->hand[index])));
	if(!isValidCard(c)){return false;}
	p->hand[index]=_createCard(No_Suit,No_Value);
	int position=getFirstNonFilledTableCardPosition(t);
	if(position==-1){
		cleanTableCards(t);
		position=getFirstNonFilledTableCardPosition(t);
	}
	t->tableCards[position]=*c;
	updateTurn(t);
	return true;


}



//TODO ver se ja imprime as table cards
