#ifndef TABLE_H_
#define TABLE_H_

#include "Card.h"
#include "Player.h"
#define DECK_SIZE 52

struct _table;
typedef struct _table Table;

//Table Ops

/**
 * @brief A function that returns the number of players a given table currently has
 * @param table a table struct whose number of players is to be returned
 * @return the number of players the given table currently has. -1 is returned if a null table pointer is passed
 */
int getNrPlayers(Table * table);
/**
 * @brief A function that returns the number of available seats on a given Table
 * @param t a pointer to a table whose number of available seats is to be returned
 * @return the number of available seats on the given table. -1 is returned if a null table pointer is passed.
 */
int getAvailableSeats(Table *t);
/**
 * @brief A function that returns true if a given table is full or false otherwise.
 * @param t a pointer to a table which is to be tested for available seats
 * @return a bool representing if a given table is filled to capacity. True is returned if a null pointer is passed.
 */
bool isFull(Table *t);
/**
 * @brief A function that returns the number of the player that is supposed to play.
 * @param t a pointer to a table object whose turn is to be returned
 * @return the number identifying the number of the next player to play. -1 is returned is no player is supposed to play or if the passed table pointer is a null pointer
 */
int getTurn(Table *t);
/**
 *@brief A function that returns the current round on a given table.
 *@param t a pointer to a table struct whose round is to be returned.
 *@return an integer with the round number. -1 is returned if a null pointer was passed or if the game has not yet started
 *
 *
 */
int getRound(Table *t);
/**
 * @brief Updates the turn to the next player nr.
 * @param t the Table for which the turn is to be update
 *
 */
void updateTurn(Table *t);
/**
 *
 * @brief A function that returns the used card at a given index
 * @param table a pointer to a table struct whose used cards are to be returned
 * @param index the index of the card to be returned
 * @return a pointer to the used card at a given index. Null is returned if there is no card used at that position
 */
Card getUsedCard(Table * table,int index);
/**
 *
 * @brief A function that returns a malloced array containing the set cards that was played on a given round
 * @param round an integer with the number of the round whose cards are to be returned. First round is round zero.
 * @return A malloced array containing all the cards played. The cards are given in the order they were played.
 */
Card *getRoundCards(Table *t,int round);
/**
 * @brief A function that returns a table card for a particular table.
 * @param table a pointer to a table struct whose table cards are to be returned.
 * @param index an integer with the place of the card to be returned on the table
 * @return the card at the desired position an "invalid" Card struct is returned if there are no cards or an invalid index was passed. To test Card validity please call isValidCard on the returned card
 *
 */
Card getTableCard(Table *table,int index);
/**
 *
 * @brief A function that returns the card on a given position on the pack
 * @param table a table struct pointer representing the table whose pack's card is to be returned
 * @param index the index of the card inside the deck
 * @return a copy of the card that is on a given postion.If an invalid index is porvided or if there is no card at the desired position a non valid card struct is returned. The validity of a Card struct should be asserted by
 */
Card getDeckCard(Table *table,int index);
/**
 * @brief a function that creates a table with a given number of players
 * @param nrPlaces the number of available seats on the table
 * @param cardsPerPlayer the number of cards each player should have
 * @return A pointer to a table struct initialized with the desires parameters
 *
 *
 */
Table * createTable(int nrPlaces,int cardsPerPlayer);
/**
 *
 * @brief A method that initializes a table struct and it's content. Intended for use on a SM block. A pointer to a memory block whose size may be obtained from calling required_mem_size with the adequate parameters
 * @param mb a pointer to a memory block with a given size
 * @param nrPlaces the number of available places on the table
 * @param cardsPerPlayer the number of cards a player should have
 * @return a pointer to the newly created table struct. When it's no longer needed deleteTable should be called on it
 */
Table * _createTable(void *t,int nrPlaces,int cardsPerPlayer);
/**
 * @brief A function that returns the number of bytes required to store a Table struct, and all auxiliar structures.
 * @param nrPlaces the number of available seats the desired table is supposed to have.
 * @return the number of bites required to store all the information related to the table struct
 *
 */
int get_Table_required_mem_size(int nrPlaces);
/**
 * @brief A function that frees the memory used for a given table struct
 * @param t a pointer to a pointer to a table struct that is to be deleted
 *
 *
 */
void deleteTable(Table **t);

/**
 * @brief A function that given a player id and a table distributes a given number of cards to that player from the table deck
 * @param t the table whose deck is to provide the cards to be distributed to the player
 * @param nrCards the number of cards that are to be distributed
 * @param playerID the id of the player to whom the card are to be delivered
 * @return a bool stating if the operation was successfully done
 */
bool giveCards(Table *t,int nrCards,int playerID);



//deck operations
/**
 * @brief A function that fills the deck of a given table with one instance of each different card which is possible to have.
 * @param t a table struct pointer to a table whose deck is to be filled.
 *
 *
 */
void fillDeck(Table *t);
/**
 * @brief A function that prints the content of a a table's deck
 * @param t a table struct pointer to the table whose deck is to be printed
 */
void printDeck(Table *t);
/**
 * @brief A function that shuffles the deck of a given table.
 * @param t a pointer to a table struct whose deck is to be shuffled
 *
 *
 */
void shuffleDeck(Table *t);
/**
 * @brief A function that prints the cards already used in a particular table
 * @param t a pointer to a table struct whose used cards are to be printed;
 *
 */
void printUsedCards(Table *t);
/**
 * @brief A function that prints the cards that are currently on the table
 * @param t a pointer to a table struct whose cards are to be printed
 */
void printTableCards(Table *t);
int nrOfCardsInDeck(Table *t);
Card removeCardFromDeckTop(Table *t);
bool isActive(Table *t);

/**
 * @brief A function used to remove a player from a table. Sets it to inactive
 *
 */
void removePlayer(Player **p,Table *t);
/**
 * @brief A function to deactivate a table
 *
 */
void inactivate(Table *t);
/**
 * @brief A function to set the turn of a given table
 *
 */
void setTurn(Table *t,int nr);
/**
 * @brief A function that clean all the cards on the table
 *
 */
void cleanTableCards(Table *t);
/**
 * @brief A function that returns the index of the first empty place on the used cards array
 *
 *
 */
int getFirstNonFilledUsedCard(Table *t);
/**
 * @brief A function that returns the index of the first empty place on the table card's array
 *
 */
int getFirstNonFilledTableCardPosition(Table *t);
/**
 * @brief A function that returns a table out of a properly set shared memory block
 *
 */
Table *grabTable(void *mb,int nrPlaces,int cardsPerPlayer);

#endif
