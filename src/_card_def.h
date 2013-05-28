#ifndef _CARD_DEF_H
#define _CARD_DEF_H


struct _card{
	Suit cardSuit;
	CardValue cardValue;
	char name[4];
	char pseudoName[4];//thats what's returned when getName is called. this way no problem is it's changed
	bool malloced;
};

/**
 * @brief An internal function of the card structure that fills the name accordingly to the cardSuit and cardValue
 *
 */
void fillName(Card *c);

#endif
