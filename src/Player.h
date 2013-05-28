/*
 * Player.h
 *
 *  Created on: May 26, 2013
 *      Author: root
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include "Card.h"

#define FIFO_NAME_LEN 50
#define FIFO_TEMPLATE_STR "/tmp/cardFifo_%d"

typedef struct _player Player;

//Player Ops
Player * createPlayer(char *name);
void deletePlayer(Player **p);
int getID(Player *p);
int getNrCards(Player *p);
void printHandCards(Player *p);
char *getFifoName(Player *p);
int getHandSize(Player *p);
Card getCard(Player *p,int index);
int getCardsFromFifo(Player *p,int nrOfCards,bool block);
void addCardToHand(Player *p,Card c);
int emptyHandCardSpot(Player *p);
char *getPlayerName(Player *p);
bool isDealer(Player *p);
Card * getHandCard(Player *p,int index);
//TODO continue prototypes



#endif /* PLAYER_H_ */
