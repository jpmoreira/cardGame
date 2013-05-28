/*
 * PlayerTable.h
 *
 *  Created on: May 27, 2013
 *      Author: root
 */

#ifndef PLAYERTABLE_H_
#define PLAYERTABLE_H_
#include "Player.h"
#include "Table.h"

/**
 * @brief A function that adds a player to a table if it's possible
 * @param newPlayer a pointer to the player to be added
 * @param t a pointer to the table where the player is to be added
 * @return a bool stating if the operation was successfully done
 *
 */
bool addPlayer(Table *t,Player *newPlayer);
void removePlayer(Player **p,Table *t);
bool playCard(Player *p,Table *t,int index);

#endif /* PLAYERTABLE_H_ */
