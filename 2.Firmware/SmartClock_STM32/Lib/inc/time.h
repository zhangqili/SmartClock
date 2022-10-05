typedef struct
{
	int hour;
	int minute;
	int second;
} Time;

extern Time CURRENT_TIME, ALARM_TIME;
extern _Bool isALarmOn;

void Tick(Time* t);
void CheckTime(Time* t);
