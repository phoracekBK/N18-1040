#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

uint32_t thread_index = 0;


gpointer thread_fce (gpointer data)
{
	int i = 0;
	while(TRUE)
	{
		printf("i:%d- %s -> %f\n",i, (char*) data, ((double)sqrt(i))/2.0);
		i++;
	}

	if(data != NULL)
		free(data);

	return NULL;
}


gboolean g_thread (gpointer param)
{
	char* num = (char*) malloc(sizeof(12));
	sprintf(num,"%d", thread_index);
	
	GThread * thread = g_thread_new ("load", thread_fce, num);
	
	thread_index++;

	return TRUE;
}

int main(int argv, char ** argc)
{
	gtk_init(&argv, &argc);
/*
	g_timeout_add (10,
                    g_thread,
                    NULL);
*/

	for(int i = 0; i< 50000; i++)
	{

		char* num = (char*) malloc(sizeof(12));
		sprintf(num,"%d", thread_index);
	
		GThread * thread = g_thread_new ("load", thread_fce, num);
	
		thread_index++;
	}


	gtk_main();
	
	return 0;
}
