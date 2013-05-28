/*
 * _player_def.h
 *
 *  Created on: May 26, 2013
 *      Author: root
 */

#ifndef PLAYER_DEF_H_
#define PLAYER_DEF_H_

struct _player{
	char *name;
	char *pseudo_name;
	int id;
	int handSize;
	int nrCards;
	Card *hand;
	char* fifoName;
	char* pseudo_fifoName;
};


#endif /* PLAYER_DEF_H_ */
