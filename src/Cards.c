#include "Cards.h"

struct _card{
	Suit cardSuit;
	CardValue cardValue;
	char name[4];
	char pseudoName[4];//thats what's returned when getName is called. this way no problem is it's changed
};
struct _table{
	int availableSeats;
	int nrPlayers;
	int turn;//number of the player that is supposed to play next (-1 if no one is supposed to play)
	int round;//the round (1st round,2nd round..)
	Card usedCards[52];
	Card * tableCards;
	Card deck[52];
};

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
			c->name[1]='*';
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
			c->name[2]='*';
			break;
	}

	ensureNameCoerence(c);
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
bool validCard(Card *c){
	if(c==NULL || c->cardSuit==No_Suit || c->cardValue==No_Value){return false;}
	return true;
}



/**
 * @brief A function that returns the number of players a given table currently has
 * @param table a table struct whose number of players is to be returned
 * @return the number of players the given table currently has. -1 is returned if a null table pointer is passed
 */
int getNrPlayers(Table * table){

	if(table==NULL){
		return -1;
	}
	return table->nrPlayers;

}
/**
 * @brief A function that returns the number of available seats on a given Table
 * @param t a pointer to a table whose number of available seats is to be returned
 * @return the number of available seats on the given table. -1 is returned if a null table pointer is passed.
 */
int getAvailableSeats(Table *t){
	if(t==NULL){
		return -1;
	}
	return t->availableSeats;

}
/**
 * @brief A function that returns true if a given table is full or false otherwise.
 * @param t a pointer to a table which is to be tested for available seats
 * @return a bool representing if a given table is filled to capacity. True is returned if a null pointer is passed.
 */
bool isFull(Table *t){
	if(t==NULL){
		return true;
	}
	if(t->availableSeats==0){
		return true;
	}
	return false;


}
/**
 * @brief A function that returns the number of the player that is supposed to play.
 * @param t a pointer to a table object whose turn is to be returned
 * @return the number identifying the number of the next player to play. -1 is returned is no player is supposed to play or if the passed table pointer is a null pointer
 */
int getTurn(Table *t){
	if(t==NULL){
		return -1;
	}
	return t->turn;
}
/**
 *@brief A function that returns the current round on a given table.
 *@param t a pointer to a table struct whose round is to be returned.
 *@return an integer with the round number. -1 is returned if a null pointer was passed or if the game has not yet started
 *
 *
 */
int getRound(Table *t){
	if(t==NULL){return -1;}
	return t->round;
}
/**
 * @brief Updates the turn to the next player nr.
 * @param t the Table for which the turn is to be update
 *
 */
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
/**
 *
 * @brief A function that returns the used card at a given index
 * @param table a pointer to a table struct whose used cards are to be returned
 * @param index the index of the card to be returned
 * @return a pointer to the used card at a given index. Null is returned if there is no card used at that position
 */
Card *getUsedCard(Table * table,int index){
	if(index<0 && index>51){
		Card c;
		c.cardValue=No_Value;
		c.cardSuit=No_Suit;
		return c;
	}
}
/**
 *
 * @brief A function that returns a malloced array containing the set cards that was played on a given round
 * @param round an integer with the number of the round whose cards are to be returned. First round is round zero.
 * @return A malloced array containing all the cards played. The cards are given in the order they were played.
 */
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



/**
 * @brief A function that returns a table card for a particular table.
 * @param table a pointer to a table struct whose table cards are to be returned.
 * @param index an integer with the place of the card to be returned on the table
 * @return the card at the desired position an "invalid" Card struct is returned if there are no cards or an invalid index was passed. To test Card validity please call isValidCard on the returned card
 *
 */
Card getTableCard(Table *table,int index){
	if(index>=table->nrPlayers ||index<0|| table->tableCards==NULL){//if no valid index is given or if there are no cards in the table
		Card c;
		c->cardSuit=No_Suit;
		c->cardValue=No_Value;
		fillName(&c);
		return c;
	}
	return table->tableCards[index];


}
/**
 *
 * @brief A function that returns the card on a given position on the pack
 * @param table a table struct pointer representing the table whose pack's card is to be returned
 * @param index the index of the card inside the deck
 * @return a copy of the card that is on a given postion.If an invalid index is porvided or if there is no card at the desired position a non valid card struct is returned. The validity of a Card struct should be asserted by
 */
Card getDeckCard(Table *table,int index){

}
Card getHandCard(Table *table,int playerNr,int index);
