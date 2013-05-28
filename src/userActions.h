/*
 * userActions.h
 *
 *  Created on: May 28, 2013
 *      Author: root
 */

#include "Table.h"
#include "globalVars.h"

/**
 * @brief A function called when the user wants to display the turn of the game
 *
 */
void displayTurn_action();
/**
 * @param A function that plays a card to the current table after prompting the user to choose the card
 *
 */
void playCard_action();
/**
 * @param A function that prints the table cards to the stdout
 *
 */
void displayTableCards_action();
/**
 * @param A function that prints the player's cards to the stdout
 *
 */
void displayUsedCards_action();
/**
 *
 * @param A function that prints the time passed since the beggining of the players turn. Zero is printed if it's not this player turn.
 */
void displayElapsedTime_action();
