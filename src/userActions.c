#include "userActions.h"

void displayTurn_action(){
	pthread_mutex_lock(shm.tableResourcesMutex);
	int turn=getTurn(shm.t);
	pthread_mutex_unlock(shm.tableResourcesMutex);
	printf("\nThe current turn is: %d\n",turn);
}


void playCard_action(){
	printf("Your Cards: \n");
	printHandCards(player);
	printf("\n Choose the index of the card you want to play: \n");
	int cardIndex;
	scanf("%i",&cardIndex);
	while(!isValidCard(getHandCard(player,cardIndex))){//while we dont habe a good index for the card
		printf("\n Invalid Index. Please enter another one\n");
		scanf("%i",&cardIndex);
	}
	pthread_mutex_lock(&internalMut);
	cardToPlayIndex=cardIndex;//set the value of the global var
	pthread_cond_broadcast(&cardPlayedCond);
	pthread_mutex_unlock(&internalMut);

}
void displayTableCards_action(){
	pthread_mutex_lock(shm.tableResourcesMutex);
	printTableCards(shm.t);
	pthread_mutex_unlock(shm.tableResourcesMutex);

}
void displayUsedCards_action(){
	pthread_mutex_lock(shm.tableResourcesMutex);
	printUsedCards(shm.t);
	pthread_mutex_unlock(shm.tableResourcesMutex);
}
void displayElapsedTime_action(){

	if(isOurTurn()){//if its not our turn
		time_t now=time(0);
		printf("Time Elapsed: %d\n",(int)(now-turnStartTime));
	}
	else{
		printf("Time Elapsed: 0\n");
		return;
	}
}
