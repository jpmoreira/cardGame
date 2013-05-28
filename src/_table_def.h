/*
 * _table_def.h
 *
 *  Created on: May 27, 2013
 *      Author: root
 */

#ifndef TABLE_DEF_H_
#define TABLE_DEF_H_

#include "_card_def.h"
struct _table{
	int lastID;
	int availableSeats;
	int nrPlayers;
	int turn;//number of the player that is supposed to play next (-1 if no one is supposed to play)
	int round;//the round (1st round,2nd round..)
	int cardsPerPlayer;
	Card usedCards[DECK_SIZE];
	Card tableCards[DECK_SIZE];
	Card deck[DECK_SIZE];
	bool active;
	bool malloced;
};

#endif /* TABLE_DEF_H_ */
