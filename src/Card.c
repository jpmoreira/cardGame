#ifndef CARD_C_
#define CARD_C_

#include "Card.h"
#include "_card_def.h"
#include <unistd.h>







/**
 * @brief returns the Suit of a given Card pointer
 * @param card a pointer to a card whose suit is to be returned
 * @return the suit of the card
 *
 */
Suit getSuit(Card * card){
	return card->cardSuit;

}
/**
 * @brief Returns the value of a given card
 * @param card a pointer to a card whose value is to be returned
 * @return the value of the card
 */
CardValue getCardValue(Card *card){
	return card->cardValue;
}
/**
 * @brief A function that ensures the pseudoName parameter is the same as the name parameter on a given card. This is necessary because a pointer to the pseudoName is passed when getName is called. And it might be changed externally
 * @param c A pointer to a card whose pseudoName is to be coerent with the name parameter
 *
 */
void ensureNameCoerence(Card *c){
	int i=0;
	for(;i<4;i++){
		c->pseudoName[i]=c->name[i];
	}
}
/**
 * @brief A function that fills the name of a given card taking into account his suit and value
 * @param c a pointer to a card whose name is to be filled
 */
void fillName(Card *c){

	c->name[0]=' ';//by default first element is a space
	c->name[3]='\0';
	switch (c->cardValue) {
		case Ace:
			c->name[1]='A';
			break;
		case Two:
			c->name[1]='2';
			break;
		case Three:
			c->name[1]='3';
			break;
		case Four:
			c->name[1]='4';
			break;
		case Five:
			c->name[1]='5';
			break;
		case Six:
			c->name[1]='6';
			break;
		case Seven:
			c->name[1]='7';
			break;
		case Eight:
			c->name[1]='8';
			break;
		case Nine:
			c->name[1]='9';
			break;
		case Ten:
			c->name[0]='1';
			c->name[1]='0';
			break;
		case Jack:
			c->name[1]='J';
			break;
		case Queen:
			c->name[1]='Q';
			break;
		case King:
			c->name[1]='K';
			break;
		default:
			c->name[1]='?';
			break;
	}











	switch (c->cardSuit) {
		case Hearts:
			c->name[2]='h';
			break;
		case Spades:
			c->name[2]='s';
			break;
		case Diamonds:
			c->name[2]='d';
			break;
		case Clubs:
			c->name[2]='c';
			break;
		default:
			c->name[2]='?';
			break;
	}

	ensureNameCoerence(c);
}
/**
 * @brief A function that creates a Card struct with the specified parameters. The struct is allocated on the stack.
 * @param theSuit the suit of the card to be created
 * @param theValue the value of the card to be created
 * @return A card struct initialized with the specified parameters
 */
Card _createCard(Suit theSuit,CardValue theValue){
	Card c;
	c.cardSuit=theSuit;
	c.cardValue=theValue;
	fillName(&c);
	c.malloced=false;
	return c;
}
/**
 * @brief A function that creates a Card struct with the provided values. The card is allocated on the heap and therefore the claimed memory should be freed as soon as it is no longer needed. This can be done by calling the function destroyCard
 * @param theSuit the suit of the card to be created
 * @param theValue the value of the card to be created
 * @return a pointer to the recently created card. NULL is returned if memory could not be allocated for the card.
 *
 */
Card * createCard(Suit theSuit,CardValue theValue){
	Card *theCard=malloc(sizeof(Card));
	if(theCard!=NULL){
		theCard->cardSuit=theSuit;
		theCard->cardValue=theValue;
		fillName(theCard);
		theCard->malloced=true;


	}
	return theCard;
}
/**
 *
 * @brief Returns the name of a given card
 * @param c a pointer to a card whose name is to be returned
 * @return a null terminated c string with 3 elements with the name of the given card
 *
 */
char* getName(Card *c){
	ensureNameCoerence(c);//ensures pseudoName is the same as  name
	return c->pseudoName;
}
/**
 * @brief A function that returns a bool stating if a given card is a playable card
 * @param c a pointer to a card whose validity is to be tested
 * @return a bool stating if the given card is valid or not.
 *
 */
bool isValidCard(Card *c){
	if(c==NULL || c->cardSuit==No_Suit || c->cardValue==No_Value){return false;}
	return true;
}
/**
 * @brief A function that deletes a given card. If the memory allocation was done on the heap, the memory is freed and the pointer set to null.
 * @param c a pointer to a card pointer that we want to have deleted
 *
 */
void deleteCard(Card **c){
	Card *theCardPtr=*c;
	theCardPtr->cardSuit=No_Suit;
	theCardPtr->cardValue=No_Value;
	if((*c)->malloced==true){
		free(theCardPtr);
		*c=NULL;
	}

}
/**
 * @brief A function that prints a card on the stdout
 * @param c A card struct pointer to the card that is supposed to be printed
 * @param printNonValid a bool that should be true in case non valid cards should be printed and false otherwise.
 * @return true is returned if something was printed. False is returned otherwise
 */
bool printCard(Card *c,bool printNonValid){
	if(c==NULL){return false;}
	if(printNonValid || isValidCard(c)){
		printf("%s",c->name);
		return true;
	}
	return false;
}

/**
 * @brief A function that fills a Card array with various instances of a given card
 * @param array the card array to be filled in.
 * @param c an instance of a card to fill the entire array
 * @param size the size of the array to be filled
 *
 */
void fillArrayWithCard(Card* array,Card c,int size){
	if(array==NULL){return;}
	int i=0;
	for(;i<size;i++){
		array[i]=c;
	}
}

/**
 *
 * @brief A function that reads the cards from a fifo
 */
bool readCardFromFifo(int fifoFD,Card *destination){
	int retRead=read(fifoFD,destination,sizeof(Card));
	if(retRead!=sizeof(Card)){return false;}
	return true;

}

#endif
