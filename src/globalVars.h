#ifndef _GV_H_
#define _GV_H_
typedef struct{
	Table *t;
	pthread_mutex_t *tableResourcesMutex;
	pthread_cond_t *tableResourcesCond;
	void *smBlock;
	int smSize;
	int smID;
	char *smName;
} shm_vars;

extern shm_vars shm;
extern pthread_mutex_t internalMut;
extern pthread_cond_t cardPlayedCond;
extern int cardToPlayIndex;

extern Player *player;
extern pthread_t inputThreadID;
extern time_t turnStartTime;
#endif
