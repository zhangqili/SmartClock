#include"main.h"
#include "as608.h"

#define MAX_UNLOCK_ATTEMPT 5

extern uint8_t LOCK;
extern SearchResult searchResult;
extern uint8_t LOCKStatus;
extern uint8_t attempts;

uint8_t CHECK_FR();
uint8_t ADD_FR(uint8_t id);
uint8_t DEL_FR(void);
