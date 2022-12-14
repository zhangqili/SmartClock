#include "lock.h"
#include "as608.h"
#include "stdio.h"

uint8_t LOCK = 0;
SearchResult searchResult;
uint8_t LOCKStatus = 0;
uint8_t attempts=0;

uint8_t CHECK_FR()
{
    searchResult.mathscore = 0;
    searchResult.pageID = 0;
    do
    {
        if (!press_FR(&searchResult))
        {
            LOCK = 0;
            attempts=0;
        }
        else
        {
            LOCKStatus = 1;
            attempts++;
        }
    } while (LOCK);

    return 0;
}

uint8_t ADD_FR(uint8_t id)
{
    Add_FR(id);
}
uint8_t DEL_FR(void)
{
}