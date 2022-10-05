#include "time.h"

Time CURRENT_TIME={0,0,0}, ALARM_TIME={0,0,0};

void CheckTime(Time* t)
{
	if(t->second >= 60)
	{
		t->minute++;
		t->second=0;
	}
	if(t->minute >= 60)
	{
		t->hour++;
		t->minute=0;
	}
	if(t->hour >= 24)
	{
		t->hour=0;
	}
}

void Tick(Time* t)
{
	t->second++;
	CheckTime(t);
}
