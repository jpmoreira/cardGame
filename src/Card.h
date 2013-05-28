
#ifndef CARDS_H_
#define CARDS_H_

#include <stdio.h>
#include <stdlib.h>


#define NR_SUITS 4
#define NR_CARD_VALUES 13

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


typedef struct _card Card;






//Card Ops
Suit getSuit(Card * card);
CardValue getCardValue(Card *card);
Card _createCard(Suit theSuit,CardValue theValue);
Card * createCard(Suit theSuit,CardValue theValue);
char* getName(Card *c);
bool printCard(Card *c,bool printNonValid);
void fillArrayWithCard(Card* array,Card c,int size);
bool isValidCard(Card *c);
bool readCardFromFifo(int fifoFD,Card *destination);








//TODO player maybe to be implemented with functions to get hand cards etc
#endif
