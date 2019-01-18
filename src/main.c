#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "view/view.h"



int main(int argv, char ** argc)
{
#if CONTROLER_TEST_MODE == false
	multi_lang_init();

	gtk_init(&argv, &argc);
	int controler_init_res = controler_init();

	if(controler_init_res == 0)
	{
		gui * view = gui_new();
		gui_signals(view);
	}
	else
	{
		gui_info_window_new("Chyba!", "Inicializace jádra neproběhla v pořádku!\nProgramu bude niní ukončen.", NULL, NULL);
	}

	gtk_main();
	
	/* safety exit the core instance */
	if(controler_init_res == 0)
		controler_finalize();	
	
	
#else
	controler_unit_test();
#endif
	return 0;
}
