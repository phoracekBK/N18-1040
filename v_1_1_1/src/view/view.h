#ifndef _VIEW_H_
#define _VIEW_H_

#include "modul_tree/controler_interface.h"

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <settings_button.h>
#include <aclib.h>

/** @ingroup configuration Main window title string. */
#define MAIN_WINDOW_TITLE "Host-bk"

/** @ingroup configuration Application system icon path (icon shown in system application list panel widget). */
#define PROGRAM_ICON_PATH "./icons/bk.png"

#define BK_ICON_PATH "./icons/bk.png"
#define MACHINE_ICON_PATH "./icons/machine_icon.png"
#define KM_ICON_PATH "./icons/km_icon.png"
#define STC_ICON_PATH "./icons/stc_icon.png"



struct _gui_;
typedef struct _gui_ gui;


struct _gui_info_window_;
typedef struct _gui_info_window_ gui_info_window;


gui * gui_new();
void gui_signals(gui * this);
void gui_finalize(GtkWidget * widget, gpointer param);



gui_info_window * gui_info_window_new(char * title, char * info_label, void (*callback) (void*), void * param);
void gui_info_window_callback(GtkWidget * widget, gpointer param);



#endif
