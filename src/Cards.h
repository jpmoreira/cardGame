
#ifndef CARDS_H_
#define CARDS_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum{
	false,
	true
}bool;
typedef enum{
	Hearts,
	Spades,
	Diamonds,
	Clubs,
	No_Suit

}Suit;
typedef enum{
	Ace,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	No_Value

}CardValue;
struct _card;
struct _table;

typedef struct _card Card;
typedef struct _table Table;

Suit getSuit(Card * card);
CardValue getCardValue(Card *card);
Card * createCard(Suit theSuit,CardValue theValue);
char* getName(Card *c);




int getNrPlayers(Table * table);
int getAvailableSeats(Table *t);
bool isFull(Table *t);
bool isValidCard(Card *c);
int getTurn(Table *t);
int getRound(Table *t);
void updateTurn(Table *t);
Card *getUsedCard(Table * table,int index);
Card *getRoundCards(Table *t,int round);
Card getTableCard(Table *table,int index);
Card getDeckCard(Table *table,int index);

//TODO player maybe to be implemented with functions to get hand cards etc
#endif
