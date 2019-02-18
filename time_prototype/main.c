#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


int main()
{
	int time_val = 4526565;
	
	int hours = time_val / 3600000;
	int minutes = (time_val /60000) %60;
	int seconds = (time_val/1000)%60;

	printf("%02d:%02d:%02d\n", hours, minutes, seconds);

	return 0;
}
