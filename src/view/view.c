#include "view.h"


#define DEFAULT_TEXT_EDITOR "zenity --text-info --width 800 --height 600 2>/dev/null"



/* color definition to main window */
const double bg[3] = {0.97, 0.89, 0};
const double fg[3] = {0.18, 0.18, 0.18};

const double settings_btn_bg[3] = {0.7, 0.7, 0.7};
const double settings_btn_fg_s[3] = {0.7, 0.7, 0.7};




enum _gui_job_list_items_
{
	JOB_STATE= 0,
	JOB_NAME,
	JOB_ORDER,
	JOB_SHEET_NUMBER,
	JOB_STAMP_NUMBER,
	JOB_DATE_TIME,

	JOB_LIST_N
};

enum _gui_job_report_list_item_
{
	REP_FINISH_STATE = 0,
	REP_NAME,
	REP_REJECTED_SHEET_NUMBER,
	REP_SHEET_NUMBER,
	REP_STAMP_NUMBER,
	REP_DATE_TIME,

	REPO_ITEM_N
};

enum _gui_control_btn_
{
	GC_BTN_PRINT = 0,
	GC_BTN_PAUSE,
	GC_BTN_CONTINUE,
	GC_BTN_CANCEL,
	GC_BTN_ERROR_RESET,
	GC_BTN_FEED,
	GC_BTN_DELETE_JOB,

	GC_BTN_N
};


enum _gui_hot_lis_
{
	Q_HOT_MAIN = 0,
	Q_HOT_BACKUP,
	Q_HOT_FEEDBACK,
	PCI_HOT_IN,
	PCI_HOT_OUT,
	GIS_HOT,
	JOB_LOG_DIR,

	HOT_LIST_N
};

enum _gui_settings_btn_
{
	S_BTN_NETWORK = 0,
	S_BTN_LANG,
	S_BTN_PRINT_PARAMETERS,
	S_BTN_HOTFOLDERS,
	S_BTN_IO_VISION,
	S_BTN_BACK,

	S_BTN_N
};

enum _gui_params_list_
{
	PAR_MAX_STACKED_SHEETS = 0,
	PAR_MAX_REJECTED_SHEET_SEQ,

	PAR_LIST_N
};



struct _gui_base_;
typedef struct _gui_base_ gui_base;

struct _gui_control_page_;
typedef struct _gui_control_page_ gui_control_page;

struct _gui_settings_page_;
typedef struct _gui_settings_page_ gui_settings_page;

struct _gui_network_page;
typedef struct _gui_network_page gui_network_page;

struct _gui_lang_page_;
typedef struct _gui_lang_page_ gui_lang_page;

struct _gui_print_params_page_;
typedef struct _gui_print_params_page_ gui_print_params_page;

struct _gui_hotfolder_page_;
typedef struct _gui_hotfolder_page_ gui_hotfolder_page;


struct _gui_io_vision_page_;
typedef struct _gui_io_vision_page_ gui_io_vision_page;



/**
** @ingroup Gui
*/
struct _gui_
{
	bool blink;

	GtkWidget * main_win;
	GtkWidget * window_container;
	GtkWidget * page_stack;
	GtkWidget * drawing_area;

	GtkWidget * status_bar;
	c_string * error_string;

	gui_base * gui_base_ref;
	gui_control_page * control_page;
	gui_settings_page * settings_page;
	gui_io_vision_page * io_vision_page;
	gui_network_page * network_page;
	gui_lang_page * lang_page;
	gui_hotfolder_page * hotfolder_page;
	gui_print_params_page * print_params_page; 
};

struct _gui_control_page_
{
	GtkWidget * page;
	GtkWidget * job_list;

	GtkListStore * job_report_list_store; 
	GtkWidget * job_report_list;
	GtkWidget * log_report_scroll_area;

	GtkListStore * job_list_store;
	GtkWidget ** btn;

	GtkWidget * info_box;
	
	array_list * report_csv_list;
	
	GtkWidget * ena_switch;
	GtkWidget * xbf_pulse;

	GtkWidget * print_mode_combo;
/*
	GtkWidget * gis_status_label;
	GtkWidget * machine_status_label;
	GtkWidget * core_status_label;
*/
	GtkWidget * btn_settings;
	GtkWidget * btn_export;

	gui_base * gui_base_ref;
};

struct _gui_settings_page_
{
	GtkWidget * page;

	settings_button * btn[S_BTN_N];

	gui_base * gui_base_ref;
};


struct _gui_io_vision_page_
{
	GtkWidget * page;
	GtkWidget * io_vision;
	settings_button * btn_return;

	gui_base * gui_base_ref;
};

struct _gui_network_page
{
	GtkWidget * page;
	
	GtkWidget * iij_tcp_port_entry;
	GtkWidget * iij_ip_address_entry;
	GtkWidget * iij_tcp_port_label;
	GtkWidget * iij_ip_address_label;
	GtkWidget * iij_network_switch;
	GtkWidget * iij_conn_label;
	GtkWidget * iij_ip_addr_corect_label;
	GtkWidget * iij_tcp_port_corect_label;

	GtkWidget * pci_connection_label;
	GtkWidget * iij_connection_label;
	GtkWidget * quadient_connection_label;
	GtkWidget * pci_connection_entry;
	GtkWidget * iij_connection_entry;
	GtkWidget * quadient_connection_entry;

	settings_button * btn_return;

	gui_base * gui_base_ref;
};

struct _gui_lang_page_
{
	GtkWidget * page;

	GtkWidget * lang_list;
	GtkWidget * list_label;

	settings_button * btn_return;
	gui_base * gui_base_ref;
};

struct _gui_print_params_page_
{
	GtkWidget * page;
	
	GtkWidget ** par_lbl;
	GtkWidget ** par_entry;
	

	GtkWidget * sheet_source_lbl;
	GtkWidget * print_confirm_lbl;
	GtkWidget * sheet_source_combo;
	GtkWidget * print_confirm_switch;


	settings_button * btn_return;
	gui_base * gui_base_ref;
};

struct _gui_hotfolder_page_
{
	GtkWidget * page;

	GtkWidget ** hot_entry;
	GtkWidget ** hot_btn;
	GtkWidget ** hot_lbl;

	settings_button * btn_return;
	gui_base * gui_base_ref;
};


struct _gui_info_window_
{
	GtkWidget * dialog;
	GtkWidget * info_label;
	GtkWidget * button;

	GtkWidget * container;

	void (*callback)(void*); 
	void * param;
};

/**
** @ingroup Gui
** Structure with shared parameters for graphic interface
*/
struct _gui_base_
{
	GdkRectangle work_area_geometry;
};






void gui_create_main_window(gui * this);
gboolean gui_on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer param);
gboolean gui_cyclic_interupt(gpointer param);
void gui_set_language(gui * this);
void gui_lang_page_set_interface_language_callback(GtkComboBox * widget, gpointer * param);
void gui_pack(gui * this);
char * gui_hotfolder_page_def_file_chooser(char * title);

gui_control_page * gui_control_page_new(gui_base * gui_base_ref);
void gui_control_page_language(gui_control_page * this);
gboolean gui_control_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param);
void gui_control_page_load_jobs(gui_control_page * this);
GtkTreeViewColumn * gui_control_page_new_tree_column(char * label, int index);
void gui_control_page_delete_columns(GtkWidget * list);
void gui_control_page_load_report_csv_list(gui_control_page * this);
void gui_control_page_update_report_csv_list(gui_control_page * this);
void gui_control_page_open_report_csv (GtkTreeView *tree_view, gpointer param);

void gui_control_page_btn_feed_sheet_callback(GtkWidget * widget, gpointer param);
void gui_control_page_btn_print_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_reset_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_pause_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_go_to_settings_callback(GtkButton *button, gpointer param);
void gui_control_page_set_machine_mode_callback (GtkComboBox *widget, gpointer param);
void gui_control_page_set_ena_callback(GtkSwitch *widget, gboolean state, gpointer param);
void gui_control_page_manual_sheet_feed(GtkWidget * widget, gpointer param);

gui_settings_page * gui_settings_page_new(gui_base * gui_base_ref);
void gui_settings_page_language(gui_settings_page * this, lang * multi_lang);
void gui_settings_page_btn_go_to_io_vision_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_lang_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_print_params_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_hotfolder_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_network_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_back_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_return_callback(GtkButton *button, GdkEventButton * event, gpointer param);

gui_io_vision_page * gui_io_vision_page_new(gui_base * gui_base_ref);
void gui_io_vision_draw_socket(cairo_t* cr, double x, double y, double w, double h, char* label);
gboolean gui_io_vision_draw_event(GtkWidget * widget, cairo_t * cr, gpointer param);

gui_network_page * gui_network_page_new(gui_base * gui_base_ref);
void gui_network_page_language(gui_network_page * this, lang * multi_lang);
gboolean gui_setting_page_iij_network_control_callback(GtkSwitch *widget, gboolean state, gpointer param);
void gui_network_page_set_pci_address(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_network_page_set_quadient_address(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_network_page_set_iij_address(GtkWidget *widget, GdkEvent  *event, gpointer param);


gui_hotfolder_page * gui_hotfolder_page_new(gui_base * gui_base_ref);
void gui_hotfolder_page_language(gui_hotfolder_page * this, lang * multi_lang);
void gui_hotfolder_page_select_q_main_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_q_feedback_path_callback(GtkWidget * widget, gpointer parama);
void gui_hotfolder_page_select_q_backup_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_pci_in_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_pci_out_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_gis_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_report_path_callback(GtkWidget * widget, gpointer param);

gui_print_params_page * gui_print_params_page_new(gui_base * gui_base_ref);
void gui_print_params_page_language(gui_print_params_page * this, lang * multi_lang);
void gui_print_params_page_max_stacked_sheet_callback (GtkWidget *widget, GdkEvent  *event, gpointer   param);
void gui_print_params_set_max_rejected_sheet_seq(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_print_params_set_sheet_source_callback (GtkComboBox *widget, gpointer param);
gboolean gui_print_params_set_print_confirmation_state_callback (GtkSwitch *widget, gboolean state, gpointer param);


gui_lang_page * gui_lang_page_new(gui_base * gui_base_ref);
void gui_lang_page_language(gui_lang_page * this, lang * multi_lang);

gui_info_window * gui_info_window_new(char * title, char * info_label, void (*callback)(void*), void * param);
void gtk_info_window_finalize();
void gui_info_window_callback(GtkWidget * widget, gpointer param);

gui_base * gui_base_new();
GdkPixbuf * gui_base_load_icon(char * addr);








/**
** @ingroup Gui
** Contructor for gui layer.
** Definition of main window and window pages and connection with core layer
*/
gui * gui_new()
{
	gui * this = (gui*) malloc(sizeof(gui));

	this->gui_base_ref = gui_base_new();	

	/* create main window */
	gui_create_main_window(this);
	
	/* status bar */
	this->status_bar = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->status_bar), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height/12);
	this->error_string = c_string_new();
	this->blink = false;
	
	/* create control page */
	this->control_page = gui_control_page_new(this->gui_base_ref);

	/* create settings page */
	this->settings_page = gui_settings_page_new(this->gui_base_ref);
	
	/* create io vision page*/
	this->io_vision_page = gui_io_vision_page_new(this->gui_base_ref);

	/* create network settings page */
	this->network_page = gui_network_page_new(this->gui_base_ref);

	/* create language settings page */
	this->lang_page = gui_lang_page_new(this->gui_base_ref);

	/* create hotfolder settings page */
	this->hotfolder_page = gui_hotfolder_page_new(this->gui_base_ref);

	/* create print parameters settings page */
	this->print_params_page = gui_print_params_page_new(this->gui_base_ref); 

	/* set language */
	gui_set_language(this);

	/* create container and put everything together */
	gui_pack(this);	

	/* cyclic interupt for continues reading of core status */
	g_timeout_add(1000, gui_cyclic_interupt, this);

	gtk_widget_show_all(GTK_WIDGET(this->main_win));

	return this;
}

/* todo loading job list */
gboolean gui_cyclic_interupt(gpointer param)
{
	gui * this = (gui*) param;

	this->blink = !this->blink;

	//const char * visible_page = gtk_stack_get_visible_child_name (GTK_STACK(this->page_stack));

	gtk_combo_box_set_active(GTK_COMBO_BOX(this->control_page->print_mode_combo), controler_get_machine_mode());
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->print_mode_combo), controler_machine_status_val() == MACHINE_STATE_WAIT);

	bool pause_en = (controler_machine_status_val() != MACHINE_STATE_WAIT) && (controler_machine_status_val() != MACHINE_STATE_NEXT) && 
						(controler_machine_status_val() != MACHINE_STATE_ERROR) && (controler_machine_status_val() != MACHINE_STATE_PAUSE) && 
						(controler_machine_status_val() != MACHINE_STATE_READY_TO_START) && (controler_machine_status_val() != MACHINE_STATE_WAIT_FOR_CONFIRMATION);


	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PAUSE]), pause_en);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CONTINUE]), controler_machine_status_val() == MACHINE_STATE_PAUSE);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PRINT]), controler_machine_status_val() == MACHINE_STATE_WAIT);

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CANCEL]), (controler_machine_status_val() != MACHINE_STATE_WAIT) && (controler_machine_status_val() != MACHINE_STATE_ERROR));
	gtk_widget_set_visible(GTK_WIDGET(this->control_page->btn[GC_BTN_FEED]), controler_machine_status_val() == MACHINE_STATE_WAIT_FOR_CONFIRMATION);

	/* set status of network control widget depend on network connection state */
	gboolean conn = ((controler_iij_is_connected() == STATUS_CLIENT_CONNECTED) ? FALSE : TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_tcp_port_entry), conn);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_ip_address_entry), conn);

	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->pci_connection_entry), controler_pci_network_connected() != STATUS_CLIENT_CONNECTED);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->quadient_connection_entry), controler_quadient_network_connected() != STATUS_CLIENT_CONNECTED);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_connection_entry), controler_iij_network_connected() != STATUS_CLIENT_CONNECTED);

	gtk_switch_set_active(GTK_SWITCH(this->network_page->iij_network_switch), !conn);

	/* can set only if the machine state is in wait state */
	for(int i = 0; i < HOT_LIST_N; i++)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->hotfolder_page->hot_btn[i]), (controler_machine_status_val() == MACHINE_STATE_WAIT));
		gtk_widget_set_sensitive(GTK_WIDGET(this->hotfolder_page->hot_entry[i]), (controler_machine_status_val() == MACHINE_STATE_WAIT));
	}

	/* load report csv list */
	gui_control_page_load_report_csv_list(this->control_page);

	/* check state of core */
	if(controler_get_error_val() > 0)
	{
		c_string_set_string(this->error_string, (char*) controler_get_error_str());
	}
	else
	{
		if(c_string_len(this->error_string) > 0)
			c_string_clear(this->error_string);
	}


	
	//if(strcmp(visible_page, "control_page") == 0)
	{
		//if(this->core_ref->job_list_changed > 0)
			gui_control_page_load_jobs(this->control_page);
	}
 	
	gtk_widget_queue_draw(GTK_WIDGET(this->drawing_area));

	return TRUE;
}

void gui_set_language(gui * this)
{
	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	if(multi_lang != NULL)
	{
		gtk_window_set_title(GTK_WINDOW(this->main_win), multi_lang->win_title);

		gui_control_page_language(this->control_page);

		settings_button_set_text(this->io_vision_page->btn_return, multi_lang->set_btn_back_to_settngs);
		settings_button_set_text(this->hotfolder_page->btn_return, multi_lang->set_btn_back_to_settngs);
		settings_button_set_text(this->print_params_page->btn_return, multi_lang->set_btn_back_to_settngs);
		settings_button_set_text(this->network_page->btn_return, multi_lang->set_btn_back_to_settngs);
		settings_button_set_text(this->lang_page->btn_return, multi_lang->set_btn_back_to_settngs);

		gui_settings_page_language(this->settings_page, multi_lang);

		gui_print_params_page_language(this->print_params_page, multi_lang);

		gui_hotfolder_page_language(this->hotfolder_page, multi_lang);

		gui_network_page_language(this->network_page, multi_lang);

		gui_lang_page_language(this->lang_page, multi_lang);
	}
}

gboolean gui_status_bar_draw(GtkWidget * widget, cairo_t *cr, gpointer param)
{
	gui * this = (gui*) param;	

	if(c_string_len(this->error_string) > 0)
	{
		if(this->blink == true)
		{
	        	cairo_set_source_rgb(cr, 1, 0, 0);
		}
		else
		{
	       	 	cairo_set_source_rgb(cr, bg[0], bg[1], bg[2]);
		}

       		cairo_rectangle(cr, 0, 0, this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height/12);
 	        cairo_fill(cr);

		cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(cr, 19);
		cairo_text_extents_t extents;
		cairo_text_extents(cr, c_string_get_char_array(this->error_string), &extents);
        	cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_move_to(cr, 30, this->gui_base_ref->work_area_geometry.height/24.0 + ((double)extents.height/2.0)-2);
		cairo_show_text(cr, c_string_get_char_array(this->error_string));
        	cairo_fill(cr);
	}

	return FALSE;
}


gboolean gui_on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer param)
{
	gui * this = (gui*) param;

	cairo_set_source_rgb(cr, fg[0], fg[1], fg[2]);
        cairo_rectangle(cr, 0, 0, this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height/12);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, bg[0], bg[1], bg[2]);
        cairo_rectangle(cr, 0, this->gui_base_ref->work_area_geometry.height/12, this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height/12);
        cairo_fill(cr);

	cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);

/*
	cairo_set_line_width(cr, 3);
	cairo_move_to(cr,  this->gui_base_ref->work_area_geometry.width/16*3, this->gui_base_ref->work_area_geometry.height/24*5+50);
	cairo_line_to(cr,  this->gui_base_ref->work_area_geometry.width/16*3, this->gui_base_ref->work_area_geometry.height-50);
	cairo_stroke(cr);
*/

	return FALSE;
}

/**
** @ingroup Gui
** In this function are called all signal connection for graphic interface
*/
void gui_signals(gui * this)
{
	/* main window signals */
	g_signal_connect(G_OBJECT(this->main_win), 
				"destroy", 
				G_CALLBACK(gui_finalize), 
				this);
	g_signal_connect(G_OBJECT(this->drawing_area), 
				"draw", 
				G_CALLBACK(gui_on_draw_event), 
				this);
	g_signal_connect(G_OBJECT(this->status_bar), 
				"draw", 
				G_CALLBACK(gui_status_bar_draw), 
				this);

	/* io_vision signals */
	g_signal_connect(G_OBJECT(this->io_vision_page->io_vision), 
				"draw", 
				G_CALLBACK(gui_io_vision_draw_event), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->io_vision_page->btn_return)), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_return_callback), 
				this);

	/* settings page return buttons */
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->network_page->btn_return)), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_return_callback), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->lang_page->btn_return)), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_return_callback), 
				this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->hotfolder_page->btn_return)), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_return_callback), 
				this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->print_params_page->btn_return)), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_return_callback), 
				this);

	/* control page signals */
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_PRINT]),
				"clicked", 
				G_CALLBACK(gui_control_page_btn_print_callback), 
				this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_PAUSE]), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_pause_callback), 
				this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_CONTINUE]), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_continue_callback), 
				this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_CANCEL]), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_cancel_callback), 
				this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_ERROR_RESET]),
				"clicked", 
				G_CALLBACK(gui_control_page_btn_reset_callback),
				this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn_settings), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_go_to_settings_callback), 
				this);
	g_signal_connect(G_OBJECT(this->control_page->job_report_list), 
				"cursor-changed", 
				G_CALLBACK(gui_control_page_open_report_csv), 
				this);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_FEED]), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_feed_sheet_callback),
				NULL);
	g_signal_connect(G_OBJECT(this->control_page->info_box), 
				"draw", 
				G_CALLBACK(gui_control_info_box_draw_callback), 
				this->control_page);

	g_signal_connect(G_OBJECT(this->control_page->print_mode_combo), 
				"changed", 
				G_CALLBACK(gui_control_page_set_machine_mode_callback), 
				this->control_page);

	g_signal_connect(G_OBJECT(this->control_page->ena_switch), 
				"state-set", 
				G_CALLBACK(gui_control_page_set_ena_callback), 
				NULL);
	g_signal_connect(G_OBJECT(this->control_page->xbf_pulse), 
				"clicked", 
				G_CALLBACK(gui_control_page_manual_sheet_feed),
				NULL);

	/* settings page signals */
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_IO_VISION])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_go_to_io_vision_callback), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_LANG])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_go_to_lang_settings_callback), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_NETWORK])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_go_to_network_settings_callback), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_PRINT_PARAMETERS])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_go_to_print_params_settings_callback), 
				this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_HOTFOLDERS])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_go_to_hotfolder_settings_callback), 
				this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_BACK])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_back_callback),
				this);

	/* language settings page signals */
	g_signal_connect(G_OBJECT(this->lang_page->lang_list), 
				"changed", 
				G_CALLBACK(gui_lang_page_set_interface_language_callback), 
				this);
	
	/* network settings page signals */
	g_signal_connect(G_OBJECT(this->network_page->iij_network_switch), 
				"state-set", 
				G_CALLBACK(gui_setting_page_iij_network_control_callback), 
				this->network_page);
	g_signal_connect(G_OBJECT(this->network_page->iij_connection_entry), 
				"key-release-event", 
				G_CALLBACK(gui_network_page_set_iij_address), 
				NULL);
	g_signal_connect(G_OBJECT(this->network_page->pci_connection_entry), 
				"key-release-event", 
				G_CALLBACK(gui_network_page_set_pci_address),
				NULL);
	g_signal_connect(G_OBJECT(this->network_page->quadient_connection_entry),
				"key-release-event", 
				G_CALLBACK(gui_network_page_set_quadient_address), 
				NULL);


	/* hotfolder settings page signals */
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_MAIN]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_q_main_path_callback), 
				this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_FEEDBACK]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_q_feedback_path_callback), 
				this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_BACKUP]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_q_backup_path_callback),
				this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[PCI_HOT_IN]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_pci_in_path_callback), 
				this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[PCI_HOT_OUT]), 
				"clicked",
				 G_CALLBACK(gui_hotfolder_page_select_pci_out_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[GIS_HOT]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_gis_path_callback), 
				this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[JOB_LOG_DIR]), 
				"clicked", 
				G_CALLBACK(gui_hotfolder_page_select_report_path_callback), 
				this);

	/* print parametrs settings page */
	g_signal_connect(G_OBJECT(this->print_params_page->par_entry[PAR_MAX_STACKED_SHEETS]), 
				"key-release-event", 
				G_CALLBACK(gui_print_params_page_max_stacked_sheet_callback), 
				this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->par_entry[PAR_MAX_REJECTED_SHEET_SEQ]), 
				"key-release-event", 
				G_CALLBACK(gui_print_params_set_max_rejected_sheet_seq), 
				this->print_params_page);

	g_signal_connect(G_OBJECT(this->print_params_page->sheet_source_combo), 
				"changed", 
				G_CALLBACK(gui_print_params_set_sheet_source_callback), 
				this->print_params_page);

	g_signal_connect(G_OBJECT(this->print_params_page->print_confirm_switch), 
				"state_set", 
				G_CALLBACK(gui_print_params_set_print_confirmation_state_callback), 
				this->print_params_page);
}

/**
** @ingroup Gui
** Destructor function as the callback function called when the application window is closed.
*/
void gui_finalize(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	free(this->gui_base_ref);
	gtk_main_quit();
}

/**
** @ingroup Gui
** Function for create and initializing the main window for graphic interface of application
*/
void gui_create_main_window(gui * this)
{
	this->main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_icon(GTK_WINDOW(this->main_win), gui_base_load_icon(PROGRAM_ICON_PATH));
	gtk_window_set_default_size(GTK_WINDOW(this->main_win), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);
	gtk_window_set_position(GTK_WINDOW(this->main_win), GTK_WIN_POS_CENTER);
	//gtk_window_set_resizable(GTK_WINDOW(this->main_win), FALSE);
	gtk_window_set_decorated (GTK_WINDOW(this->main_win), FALSE);


	this->drawing_area = gtk_drawing_area_new();
        gtk_widget_set_size_request(GTK_WIDGET(this->drawing_area), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);

	this->page_stack = gtk_stack_new();
        gtk_widget_set_size_request(GTK_WIDGET(this->page_stack), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);
}


char * gui_def_file_chooser(gui* this, char * title)
{
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	gint res;
  	char * dirname = NULL;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	dialog = gtk_file_chooser_dialog_new (title,
                                      GTK_WINDOW(this->main_win),
                                      action,
                                      multi_lang->file_chooser_cancel_btn,
                                      GTK_RESPONSE_CANCEL,
                                      multi_lang->file_chooser_select_btn,
                                      GTK_RESPONSE_APPLY,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));

	if (res == GTK_RESPONSE_APPLY)
  	{
  		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
  		dirname = gtk_file_chooser_get_filename (chooser);
  	}

	gtk_widget_destroy (dialog);
	return dirname;
}

/**
** @ingroup Gui
** Function pack all part of the graphic interface together.
*/
void gui_pack(gui * this)
{
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->control_page->page, "control_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->settings_page->page, "settings_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->io_vision_page->page, "io_vision_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->lang_page->page, "lang_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->network_page->page, "network_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->hotfolder_page->page, "hotfolder_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->print_params_page->page, "print_params_page");

	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "control_page");

	this->window_container = gtk_fixed_new();

	gtk_fixed_put(GTK_FIXED(this->window_container), this->drawing_area, 0, 0);
	gtk_fixed_put(GTK_FIXED(this->window_container), this->status_bar, 0, this->gui_base_ref->work_area_geometry.height/12);
	gtk_fixed_put(GTK_FIXED(this->window_container), this->page_stack, 0, 0);

	gtk_container_add(GTK_CONTAINER(this->main_win), this->window_container);
}

/* todo ena button */
gui_control_page * gui_control_page_new(gui_base * gui_base_ref)
{
	gui_control_page * this = (gui_control_page *) malloc(sizeof(gui_control_page));

	this->gui_base_ref = gui_base_ref;

	/* window dimensions */
	double width = gui_base_ref->work_area_geometry.width;
	double height = gui_base_ref->work_area_geometry.height;

	this->report_csv_list = array_list_new();

	this->ena_switch = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->ena_switch), 100, 35);
	//gtk_switch_set_active(GTK_SWITCH(this->ena_switch), controler_Get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0);	

	this->xbf_pulse = gtk_button_new_with_label("XBF");
	gtk_widget_set_size_request(GTK_WIDGET(this->xbf_pulse), 100, 35);

	this->info_box = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->info_box), width, height);

	//print, cancel print, pause print, error reset, delete job
	this->btn = (GtkWidget**) malloc(sizeof(GtkWidget*)*GC_BTN_N);

	this->job_report_list = gtk_tree_view_new();
	
	this->log_report_scroll_area = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(this->log_report_scroll_area), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(GTK_WIDGET(this->log_report_scroll_area), width/2, height/2-185);
	gtk_container_add(GTK_CONTAINER(this->log_report_scroll_area), this->job_report_list);

	this->btn_settings = gtk_button_new_with_label("Nastavení");
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_settings), 350, 35);	

	this->btn_export = gtk_button_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_export), 350, 35);

	this->job_list_store = NULL;
	this->job_report_list_store = NULL;

	this->job_list = gtk_tree_view_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->job_list), width/2, height/8);

	this->print_mode_combo = gtk_combo_box_text_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->print_mode_combo), 350, 35);


	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), width, height);

	gtk_fixed_put(GTK_FIXED(this->page), this->info_box, 0, 0);
	gtk_fixed_put(GTK_FIXED(this->page), this->print_mode_combo, width/4*3+(width/4-350)/2, (5*height/24)+100);


	gtk_fixed_put(GTK_FIXED(this->page), this->job_list, width/4, (5*height/24)+50);
	gtk_fixed_put(GTK_FIXED(this->page), this->btn_settings, width/4*3+(width/4-350)/2, height-130);	
	gtk_fixed_put(GTK_FIXED(this->page), this->btn_export, width/4*3-350, height-130);	
	gtk_fixed_put(GTK_FIXED(this->page), this->log_report_scroll_area, width/4, (5*height/24)+(height/8)+220);
	gtk_fixed_put(GTK_FIXED(this->page), this->ena_switch, width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+350);
	gtk_fixed_put(GTK_FIXED(this->page), this->xbf_pulse, width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+410);

	for(int i = 0; i < GC_BTN_N; i++)
	{
		if(i == GC_BTN_PRINT)
			this->btn[i] = gtk_button_new_with_label ("print");
		else if(i == GC_BTN_PAUSE)
			this->btn[i] = gtk_button_new_with_label ("pause");
		else if(i == GC_BTN_CONTINUE)
			this->btn[i] = gtk_button_new_with_label ("continue");
		else if(i == GC_BTN_CANCEL)
			this->btn[i] = gtk_button_new_with_label ("stop");
		else if(i == GC_BTN_ERROR_RESET)
			this->btn[i] = gtk_button_new_with_label("reset");
		else if(i == GC_BTN_DELETE_JOB)
			this->btn[i] = gtk_button_new_with_label ("Delete");
		else if(i == GC_BTN_FEED)
			this->btn[i] = gtk_button_new_with_label ("Feed");

		gtk_widget_set_size_request(GTK_WIDGET(this->btn[i]), 75,70);

		if(i!=GC_BTN_DELETE_JOB)
		{
			gtk_fixed_put(GTK_FIXED(this->page), this->btn[i], width/4+(120*i), (5*height/24)+(height/8)+90);
		}
		else
		{
			gtk_fixed_put(GTK_FIXED(this->page), this->btn[i], 3*width/4-80,(5*height/24)+(height/8)+90);
		}
	}

	return this;
}

/*todo*/
void gui_control_page_manual_sheet_feed(GtkWidget * widget, gpointer param)
{
	/*
	core * this = (core*) param;
	c_pulse_reset(this->ena_pulse);
	this->manual_freed = 1;
	*/
}

/* todo */
void gui_control_page_set_ena_callback(GtkSwitch *widget, gboolean state, gpointer param)
{
/*
	core * this = (core*) param;

	if(state == TRUE)
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 1);
	else	
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 0);
*/
	
}

gboolean gui_control_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	int width = this->gui_base_ref->work_area_geometry.width;
	int height = this->gui_base_ref->work_area_geometry.height;
	int left_horizontal_offset = 40;	
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15);

	/* widget labels */
	cairo_move_to(cr, width/4, (5*height/24)+40);
	cairo_show_text(cr, multi_lang->g_job_list_lbl);


	cairo_move_to(cr, width/4*3+(width/4-350)/2, (5*height/24)+80);
	cairo_show_text(cr, multi_lang->par_machine_mode_lbl);

	cairo_move_to(cr, width/4, (5*height/24)+(height/8)+210);
	cairo_show_text(cr, multi_lang->g_report_csv_list_lbl);

	cairo_set_font_size(cr, 18);
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);


	/* left side */
	char temp_buff[16];

	cairo_move_to(cr,left_horizontal_offset , 5*height/24+80);
	cairo_show_text(cr, multi_lang->g_counters_label);
	
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15);
	cairo_move_to(cr, left_horizontal_offset, 5*height/24+130);
	cairo_show_text(cr, multi_lang->g_feeder_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+160);
	cairo_show_text(cr, multi_lang->g_cnt_main_feed_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+200);
	cairo_show_text(cr, multi_lang->g_cnt_companion_feed_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+260);
	cairo_show_text(cr, multi_lang->g_reject_bin_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+290);
	cairo_show_text(cr, multi_lang->g_cnt_rejected_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+360);
	cairo_show_text(cr, multi_lang->g_stacker_lbl);


	cairo_move_to(cr, left_horizontal_offset, 5*height/24+390);
	cairo_show_text(cr, multi_lang->g_cnt_tab_insert_blb);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+430);
	cairo_show_text(cr, multi_lang->g_cnt_stakced_lbl);


	cairo_set_font_size(cr, 19);

	sprintf(temp_buff, "%d", controler_get_feeded_sheets());
	cairo_text_extents_t ext_main_counter;
	cairo_text_extents(cr, temp_buff, &ext_main_counter);
	cairo_move_to(cr, width/4-80 - ext_main_counter.width, 5*height/24+160);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_feeded_companion_sheets());
	cairo_text_extents_t ext_companion_counter;
	cairo_text_extents(cr, temp_buff, &ext_companion_counter);
	cairo_move_to(cr, width/4-80 - ext_companion_counter.width, 5*height/24+200);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_rejected_sheets());
	cairo_text_extents_t ext_reject_counter;
	cairo_text_extents(cr, temp_buff, &ext_reject_counter);
	cairo_move_to(cr, width/4-80 - ext_reject_counter.width, 5*height/24+290);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_tab_inserts());
	cairo_text_extents_t ext_ti_counter;
	cairo_text_extents(cr, temp_buff, &ext_ti_counter);
	cairo_move_to(cr, width/4-80 - ext_ti_counter.width, 5*height/24+390);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d/%d",controler_get_total_sheet_number(), controler_get_stacked_sheets());
	cairo_text_extents_t ext_stacked_counter;
	cairo_text_extents(cr, temp_buff, &ext_stacked_counter);
	cairo_move_to(cr, width/4-80 - ext_stacked_counter.width, 5*height/24+430);
	cairo_show_text(cr, temp_buff);

	/* right side */

	cairo_set_font_size(cr, 15);
	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+200);
	cairo_show_text(cr, multi_lang->g_status_machine_lbl);

	cairo_text_extents_t ext_machine_status;
	cairo_text_extents(cr, controler_machine_get_state_str(controler_machine_status_val()), &ext_machine_status);
	cairo_move_to(cr, width-(width/4-350)/2-ext_machine_status.width, 5*height/24+200);
	cairo_show_text(cr, controler_machine_get_state_str(controler_machine_status_val()));

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+260);
	cairo_show_text(cr, multi_lang->g_status_gis_lbl);

	cairo_text_extents_t ext_gis_status;
	cairo_text_extents(cr, controler_get_gis_status(), &ext_gis_status);
	cairo_move_to(cr, width-(width/4-350)/2 - ext_gis_status.width, 5*height/24+260);
	cairo_show_text(cr, controler_get_gis_status());

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+320);
	cairo_show_text(cr, "TCP/IP status PCI:");
	
	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+380);
	cairo_show_text(cr, "TCP/IP status IIJ:");

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+440);
	cairo_show_text(cr, "TCP/IP status QUADIENT:");
	
	/* network connection status points */
	if(controler_pci_network_connected() != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);

	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+320, 10, 0, 2*M_PI);
	cairo_fill(cr);

	if(controler_iij_network_connected() != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);

	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+380, 10, 0, 2*M_PI);
	cairo_fill(cr);


	if(controler_quadient_network_connected() != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+440, 10, 0, 2*M_PI);
	cairo_fill(cr);

	return FALSE;
}


void gui_control_page_language(gui_control_page * this)
{
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	/* columns for the job list tree widget */
	gui_control_page_delete_columns(this->job_list);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_state, JOB_STATE)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_name, JOB_NAME)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_order, JOB_ORDER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_sheet_number, JOB_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_stamp_number, JOB_STAMP_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_date_time, JOB_DATE_TIME)));


	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(this->print_mode_combo));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang->par_mm_setup_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang->par_mm_print_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang->par_mm_inspection_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang->par_mm_print_inspection_comb);

	/* columns for job report csv list tree widget */

	gui_control_page_delete_columns(this->job_report_list);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_state, REP_FINISH_STATE)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_name, REP_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_rejected_sheets, REP_REJECTED_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_sheet_number, REP_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_stamp_number, REP_STAMP_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_date_time, REP_DATE_TIME)));

}

void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param)
{
	controler_print_cancel();
}

void gui_control_page_btn_reset_callback(GtkButton *button, gpointer param)
{
	controler_print_reset_error();
	controler_refresh_dir_list();
}

void gui_control_page_btn_go_to_settings_callback(GtkButton *button, gpointer param)
{	
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "settings_page");
}



void gui_control_page_btn_pause_callback(GtkButton *button, gpointer param)
{
	if(controler_machine_status_val() != MACHINE_STATE_ERROR)
	{
		/* set pause */
		controler_print_pause();
	}
}

void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	if(controler_print_continue() == 0)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_CONTINUE]), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_PAUSE]), TRUE);
	}
}

void gui_control_page_btn_print_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;

	if(controler_machine_status_val() == MACHINE_STATE_WAIT)
	{
		/* start new print */
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (this->job_list));
		GtkTreeIter iter;
        	GtkTreeModel *model;
        	gchar *job_name;

        	if (gtk_tree_selection_get_selected (selection, &model, &iter))
        	{
        	        gtk_tree_model_get (model, &iter, JOB_NAME, &job_name, -1);

			if(controler_print_start((const char*) job_name) == 0)
			{
				gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_PAUSE]), TRUE);
				gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_CONTINUE]), FALSE);
				gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_PRINT]), FALSE);
			}

        	        g_free(job_name);
        	}
	}
}

void gui_control_page_delete_columns(GtkWidget * list)
{
	if(gtk_tree_view_get_n_columns (GTK_TREE_VIEW(list)) > 0)
        {
               	while(gtk_tree_view_remove_column (GTK_TREE_VIEW(list),
        	GTK_TREE_VIEW_COLUMN(gtk_tree_view_get_column(GTK_TREE_VIEW(list), 0))) > 0){}
	}
}

GtkTreeViewColumn * gui_control_page_new_tree_column(char * label, int index)
{
	GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes(label,
			                                                       (GtkCellRenderer*) gtk_cell_renderer_text_new(),
                                                                             "text", index, NULL);
	gtk_tree_view_column_set_min_width (column, 20);
	gtk_tree_view_column_set_expand (column, TRUE);
	gtk_tree_view_column_set_resizable (column, TRUE);

	return column;
}


void gui_control_page_load_jobs(gui_control_page * this)
{
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	GtkTreeIter iter;
	this->job_list_store = gtk_list_store_new(JOB_LIST_N, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
	
	for(int i; i < controler_get_job_queue_size(); i++)
	{
		gtk_list_store_append(this->job_list_store, &iter);
		const char * job_status = NULL;		

		if((strcmp(controler_get_printed_job_name(), controler_get_job_name(i)) == 0))
		{
			if(controler_machine_status_val() == MACHINE_STATE_NEXT)
				job_status = multi_lang->print_state_wait_for_data;
			else if(controler_machine_status_val() == MACHINE_STATE_ERROR)
				job_status = multi_lang->print_state_job_error;
			else
				job_status = multi_lang->print_state_print;
		}
		else
		{
			job_status = multi_lang->print_state_wait;
		}

               	gtk_list_store_set (this->job_list_store, &iter,
				JOB_STATE, job_status,
				JOB_NAME, controler_get_job_name(i),
				JOB_ORDER, controler_get_job_order(i),
				JOB_SHEET_NUMBER, controler_get_job_sheet_number(i), 
				JOB_STAMP_NUMBER, controler_get_job_stamp_number(i),
				JOB_DATE_TIME, controler_get_job_date_time(i), 
				-1);
	}

	gtk_tree_view_set_model (GTK_TREE_VIEW(this->job_list), GTK_TREE_MODEL(this->job_list_store));
        g_object_unref(G_OBJECT(this->job_list_store));
}


void gui_control_page_load_report_csv_list(gui_control_page * this)
{
	DIR * dir_ref = opendir(controler_get_job_report_hotfolder_path());
	
	bool list_changed = false;
	array_list * temp_csv_list = array_list_new();

	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;

		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				//if(util_str_ends_with(dir_cont->d_name, ".csv", 0) == 0)
				{
					int report_csv_list_index = 0;
					c_string* csv_file_name = c_string_new_with_init(dir_cont->d_name);
					array_list_add(temp_csv_list, csv_file_name);
					bool presence = false;

					while(report_csv_list_index < array_list_size(this->report_csv_list))
					{
						if(strcmp(c_string_get_char_array(array_list_get(this->report_csv_list, report_csv_list_index)), dir_cont->d_name) == 0)
						{
							presence = true;
							break;
						}
						else
						{
							presence = false;
						}

						report_csv_list_index++;
					}
					
					if(presence == false)
					{
						list_changed = true;
						array_list_add(this->report_csv_list, c_string_new_with_init(dir_cont->d_name));
					}
				}
				//else
				//{
			//		printf("removed file: %s\n", dir_cont->d_name);
					//util_delete_file(core_get_job_report_hotfolder_path(this->gui_base_ref->core_ref), dir_cont->d_name);
			//	}
			}
		}

		closedir(dir_ref);
	}


	if(list_changed == true)
	{
		int report_csv_list_index = 0;
		int local_csv_list_index = 0;
	

		while(report_csv_list_index < array_list_size(this->report_csv_list))
		{
			bool presence = false;
			while(local_csv_list_index < array_list_size(temp_csv_list))
			{
				printf(" %s - %s", c_string_get_char_array(array_list_get(this->report_csv_list, report_csv_list_index)), 
					c_string_get_char_array(array_list_get(temp_csv_list, local_csv_list_index)));

				if(strcmp(c_string_get_char_array(array_list_get(this->report_csv_list, report_csv_list_index)), 
					c_string_get_char_array(array_list_get(temp_csv_list, local_csv_list_index))) == 0)
				{
					presence = true;
					break;
				}

				local_csv_list_index ++;
			}

			if(presence == false)
				array_list_remove_with_release(this->report_csv_list, report_csv_list_index, c_string_finalize_v2);
			else
				report_csv_list_index++;
		}

		gui_control_page_update_report_csv_list(this);
	}

	array_list_destructor_with_release(&temp_csv_list, c_string_finalize_v2);
}

void gui_control_page_update_report_csv_list(gui_control_page * this)
{
	GtkTreeIter iter;
	this->job_report_list_store = gtk_list_store_new(REPO_ITEM_N, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
	int index = 0;

	while(index < array_list_size(this->report_csv_list))
	{
		rep_struct * job_report = hot_load_report_information(array_list_get(this->report_csv_list, index));
		if(job_report != NULL)
		{
			gtk_list_store_append(this->job_report_list_store, &iter);

               		gtk_list_store_set (this->job_report_list_store, &iter, REP_NAME, job_report->job_name, -1);
               		gtk_list_store_set (this->job_report_list_store, &iter, REP_FINISH_STATE, job_report->finish_state, -1);
               		gtk_list_store_set (this->job_report_list_store, &iter, REP_REJECTED_SHEET_NUMBER, job_report->rejected_sheet_number, -1);
               		gtk_list_store_set (this->job_report_list_store, &iter, REP_SHEET_NUMBER, job_report->sheet_number, -1);
        		gtk_list_store_set (this->job_report_list_store, &iter, REP_STAMP_NUMBER, job_report->stamp_number, -1);
			gtk_list_store_set (this->job_report_list_store, &iter, REP_DATE_TIME, job_report->date_time, -1);

			free(job_report);
		}
		index++;
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(this->job_report_list), GTK_TREE_MODEL(this->job_report_list_store));
	g_object_unref(G_OBJECT(this->job_report_list_store));
}


void gui_control_page_set_machine_mode_callback (GtkComboBox *widget, gpointer param)
{
	gui_control_page * this = (gui_control_page *) param;
	controler_set_machine_mode(gtk_combo_box_get_active(GTK_COMBO_BOX(this->print_mode_combo)));
}


void gui_control_page_btn_feed_sheet_callback(GtkWidget * widget, gpointer param)
{
	if(controler_machine_status_val() == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
		controler_sheet_source_confirmation();
}


void gui_control_page_open_report_csv (GtkTreeView *tree_view, gpointer param)
{
	GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));
	GtkTreeIter iter;
        GtkTreeModel *model;
        gchar *csv_name;

        if (gtk_tree_selection_get_selected (selection, &model, &iter))
        {
		char cmd[1024];
                gtk_tree_model_get (model, &iter, REP_NAME, &csv_name, -1);
		sprintf(cmd, "nohup $(cat %s/%s.csv | %s) > /dev/null 2>&1&", controler_get_job_report_hotfolder_path(), csv_name, DEFAULT_TEXT_EDITOR);
		system(cmd);
                g_free (csv_name);
        }	
}

gui_settings_page * gui_settings_page_new(gui_base * gui_base_ref)
{
	gui_settings_page * this = (gui_settings_page*) malloc(sizeof(gui_settings_page));
	
	this->gui_base_ref = gui_base_ref;
	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);

	int i;
	for(i = 0; i < S_BTN_N; i++)
	{
		this->btn[i] = settings_button_new(fg,settings_btn_bg, settings_btn_fg_s, fg, gui_base_ref->work_area_geometry.width/2, 50);	
		settings_button_set_font_size(this->btn[i], 18);
		gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn[i]), 
					gui_base_ref->work_area_geometry.width/4, 250+((gui_base_ref->work_area_geometry.height-400)/S_BTN_N)*i);	
		if(i == S_BTN_BACK)
			settings_button_set_selected(this->btn[i], 2);
	}

	return this;
}


void gui_settings_page_language(gui_settings_page * this, lang * multi_lang)
{
	for(int i = 0; i< S_BTN_N; i++)
	{
		if(i == S_BTN_NETWORK)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_network);
		else if(i == S_BTN_LANG)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_lang_settings);
		else if(i == S_BTN_PRINT_PARAMETERS)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_print_param);
		else if(i == S_BTN_HOTFOLDERS)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_hotfolder);
		else if(i == S_BTN_IO_VISION)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_io_vision);
		else if(i == S_BTN_BACK)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_back);
	}
}

void gui_settings_page_btn_go_to_io_vision_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{	
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "io_vision_page");
}


void gui_settings_page_btn_go_to_lang_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "lang_page");
}

void gui_settings_page_btn_go_to_network_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "network_page");
}

void gui_settings_page_btn_go_to_print_params_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "print_params_page");
}

void gui_settings_page_btn_go_to_hotfolder_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "hotfolder_page");
}


void gui_settings_page_btn_back_callback(GtkWidget* widget, GdkEventButton* event, gpointer param)
{	
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "control_page");
}


gui_io_vision_page * gui_io_vision_page_new(gui_base * gui_base_ref)
{
	gui_io_vision_page * this = (gui_io_vision_page*) malloc(sizeof(gui_io_vision_page));

	this->gui_base_ref = gui_base_ref;
	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), gui_base_ref->work_area_geometry.width, gui_base_ref->work_area_geometry.height);
	
	this->io_vision = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->io_vision), gui_base_ref->work_area_geometry.width, gui_base_ref->work_area_geometry.height);


	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, gui_base_ref->work_area_geometry.width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);

	gtk_fixed_put(GTK_FIXED(this->page), this->io_vision, 0, 0);
	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
		gui_base_ref->work_area_geometry.width/4, 250);

	return this;
}
void gui_settings_page_return_callback(GtkButton *button, GdkEventButton * event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "settings_page");
}


void gui_io_vision_draw_socket(cairo_t* cr, double x, double y, double w, double h, char * label)
{
	cairo_save(cr);
	
	cairo_set_font_size(cr, 25);
	cairo_text_extents_t extents;
	cairo_text_extents(cr, label, &extents);

	cairo_move_to(cr, x+w/2-extents.width/2, y-30);
	cairo_show_text(cr, label);

	cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
	cairo_set_line_width(cr, 3);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 

	cairo_move_to(cr, x, y+10);
        cairo_line_to(cr, x, y+h-10);
        cairo_line_to(cr, x+10, y+h);
        cairo_line_to(cr, x+w, y+h-50);
        cairo_line_to(cr, x+w, y+50);
        cairo_line_to(cr, x+10, y);
	cairo_line_to(cr, x, y+10);

	cairo_stroke(cr);
	cairo_restore(cr);
}


void gui_io_vision_draw_points(gui * this, cairo_t * cr, double x, double y, double w, double h, char ** i_labels, char ** o_labels, int8_t card)
{
	int i;
	int io_index = 0;
	int io_in = 0;
	int io_out = 0;

	for(i = 50; i < (h - 50); i = i+((h-100)/18))
	{
		if(io_index != 8 && io_index != 9 && io_index != 18)
		{
			if(io_in < 16)
			{
				cairo_set_source_rgb(cr, 0.2,0.2,0.2);
				cairo_set_font_size(cr, 15);
				cairo_text_extents_t extents;
				cairo_text_extents(cr, i_labels[io_in], &extents);
				cairo_move_to(cr,(x)-extents.width-20, y+i+w/20+extents.height/2);
				cairo_show_text(cr, i_labels[io_in]);

				if(controler_get_card_input(card, io_in) > 0)
					cairo_set_source_rgb(cr, 0.4,0.8,0);
				else
					cairo_set_source_rgb(cr, 1,0.2,0);

				io_in = io_in + 2;
			}
			else
			{
				cairo_set_source_rgb(cr, 0.2,0.2,0.2);
				cairo_set_font_size(cr, 15);
				cairo_text_extents_t extents;
				cairo_text_extents(cr, o_labels[io_out], &extents);
				cairo_move_to(cr,(x)-extents.width-20, y+i+w/20+extents.height/2);
				cairo_show_text(cr, o_labels[io_out]);

				if(controler_get_card_output(card, io_out) > 0)
					cairo_set_source_rgb(cr, 0.4,0.8,0);
				else
					cairo_set_source_rgb(cr, 1,0.2,0);
			
				io_out = io_out +2;	
			}	
			
			cairo_arc(cr, (x+w/2)-20, y + i+w/20, w/10, 0, 2 * M_PI);
			cairo_fill(cr);
		}
		io_index++;
	}
	
	io_in = 1;
	io_out = 1;
	for(i = 50;i < (h - 50-((h-100)/18)); i = i+((h-100)/18))
	{
		if(io_index != 27 && io_index != 28)
		{
			if(io_in < 16)
			{
				cairo_set_source_rgb(cr, 0.2,0.2,0.2);
				cairo_set_font_size(cr, 15);
				cairo_text_extents_t extents;
				cairo_text_extents(cr, i_labels[io_in], &extents);
				cairo_move_to(cr,(x)+w+20, y+i+w/20+extents.height/2);
				cairo_show_text(cr, i_labels[io_in]);

				if(controler_get_card_input(card, io_in) > 0)
					cairo_set_source_rgb(cr, 0.4,0.8,0);
				else
					cairo_set_source_rgb(cr, 1,0.2,0);

				io_in = io_in + 2;
			}
			else
			{
				cairo_set_source_rgb(cr, 0.2,0.2,0.2);
				cairo_set_font_size(cr, 15);
				cairo_text_extents_t extents;
				cairo_text_extents(cr, o_labels[io_out], &extents);
				cairo_move_to(cr,(x)+w+20, y+i+w/20+extents.height/2);
				cairo_show_text(cr, o_labels[io_out]);

				if(controler_get_card_output(card, io_out) > 0)
					cairo_set_source_rgb(cr, 0.4,0.8,0);
				else
					cairo_set_source_rgb(cr, 1,0.2,0);

				io_out = io_out +2;	
			}	
			
			cairo_arc(cr, (x+w/2)+20, y + i+w/20, w/10, 0, 2 * M_PI);
			cairo_fill(cr);
		}
		io_index++;
	}
}

void gui_io_vision_state_explanation(gui * this, cairo_t * cr, double width, double height, double y_pos)
{
	cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	cairo_set_font_size(cr, 15);
	cairo_move_to(cr,this->gui_base_ref->work_area_geometry.width/2-40, y_pos+height+5);
	cairo_show_text(cr, " = true");
	cairo_set_source_rgb(cr, 0.4,0.8,0);
	cairo_arc(cr, this->gui_base_ref->work_area_geometry.width/2-50, y_pos+height, width/10, 0, 2 * M_PI);
	cairo_fill(cr);

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	cairo_set_font_size(cr, 15);
	cairo_move_to(cr,this->gui_base_ref->work_area_geometry.width/2+60, y_pos+height+5);
	cairo_show_text(cr, " = false");
	cairo_set_source_rgb(cr, 1,0.2,0);
	cairo_arc(cr, this->gui_base_ref->work_area_geometry.width/2+50, y_pos+height, width/10, 0, 2 * M_PI);
	cairo_fill(cr);
}

gboolean gui_io_vision_draw_event(GtkWidget * widget, cairo_t * cr, gpointer param)
{
	gui * this = (gui*) param;

	double width = this->gui_base_ref->work_area_geometry.width/20.0;
	double height = this->gui_base_ref->work_area_geometry.height/1.7;

	double a1_x_pos = this->gui_base_ref->work_area_geometry.width/4.0;
	double y_pos = this->gui_base_ref->work_area_geometry.height/3.0+10;

	double a2_x_pos = this->gui_base_ref->work_area_geometry.width/4.0*3 - this->gui_base_ref->work_area_geometry.width/20.0;

	gui_io_vision_draw_socket(cr, a1_x_pos, y_pos, width, height, "A1");
	gui_io_vision_draw_socket(cr, a2_x_pos, y_pos, width, height, "A2");

	gui_io_vision_draw_points(this, cr, a1_x_pos, y_pos, width, height, controler_get_a1_input_labels(), controler_get_a1_output_labels(), IO_CARD_A1);
	gui_io_vision_draw_points(this, cr, a2_x_pos, y_pos, width, height, controler_get_a2_input_labels(), controler_get_a1_output_labels(), IO_CARD_A2);

	gui_io_vision_state_explanation(this, cr, width, height, y_pos);

	return TRUE;
}



gui_network_page * gui_network_page_new(gui_base * gui_base_ref)
{
	gui_network_page * this = (gui_network_page*) malloc(sizeof(gui_network_page));

	this->gui_base_ref = gui_base_ref;
	int width = gui_base_ref->work_area_geometry.width;
	int height = gui_base_ref->work_area_geometry.height;

	this->iij_tcp_port_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_tcp_port_entry), 300, 35);	
	char tcp_port_str[12];
	sprintf(tcp_port_str, "%d", controler_iij_get_tcp_port());
	gtk_entry_set_text(GTK_ENTRY(this->iij_tcp_port_entry), tcp_port_str);

	this->iij_ip_address_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_ip_address_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->iij_ip_address_entry), controler_iij_get_ip_addr());

	this->iij_tcp_port_label = gtk_label_new(NULL);

	this->iij_ip_address_label = gtk_label_new(NULL);
	this->iij_conn_label = gtk_label_new(NULL);

	this->iij_network_switch = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_network_switch), 100, 35);

	this->iij_ip_addr_corect_label = gtk_label_new(NULL);
	this->iij_tcp_port_corect_label = gtk_label_new(NULL);

	this->pci_connection_label = gtk_label_new(NULL);
	this->quadient_connection_label = gtk_label_new(NULL);
	this->iij_connection_label = gtk_label_new(NULL);

	this->pci_connection_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->pci_connection_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->pci_connection_entry), controler_pci_network_get_ip_address());	

	this->quadient_connection_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->quadient_connection_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->quadient_connection_entry), controler_quadient_network_get_ip_address());

	this->iij_connection_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_connection_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->iij_connection_entry), controler_iij_network_get_ip_address());

	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), width, height);

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);


	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), width/4, 250);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_tcp_port_label, width/4, 350);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_tcp_port_entry, width/4+500, 350);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_tcp_port_corect_label, width/4+820, 350);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_ip_address_label, width/4, 420);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_ip_address_entry, width/4+500, 420);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_ip_addr_corect_label, width/4+820, 420);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_conn_label, width/4, 490);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_network_switch, width/4+500, 490);
	gtk_fixed_put(GTK_FIXED(this->page), this->pci_connection_label, width/4, 560);
	gtk_fixed_put(GTK_FIXED(this->page), this->pci_connection_entry, width/4+500, 560);
	gtk_fixed_put(GTK_FIXED(this->page), this->quadient_connection_label, width/4, 630);
	gtk_fixed_put(GTK_FIXED(this->page), this->quadient_connection_entry, width/4+500, 630);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_connection_label, width/4, 700);
	gtk_fixed_put(GTK_FIXED(this->page), this->iij_connection_entry, width/4+500, 700);

	return this; 
}


void gui_network_page_language(gui_network_page * this, lang * multi_lang)
{
	gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_label), multi_lang->set_net_iij_tcp_port);
	gtk_label_set_text(GTK_LABEL(this->iij_conn_label), multi_lang->set_net_iij_connection);
	gtk_label_set_text(GTK_LABEL(this->iij_ip_address_label), multi_lang->set_net_iij_ip_addr);
	gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), NULL);
	gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), NULL);

	gtk_label_set_text(GTK_LABEL(this->iij_connection_label), multi_lang->set_net_iij_connection_test_label);
	gtk_label_set_text(GTK_LABEL(this->quadient_connection_label), multi_lang->set_net_quadient_connection_test_label);
	gtk_label_set_text(GTK_LABEL(this->pci_connection_label), multi_lang->set_net_pci_connection_test_label);
}

gboolean gui_setting_page_iij_network_control_callback(GtkSwitch *widget, gboolean state, gpointer param)
{
	gui_network_page * this = (gui_network_page*) param;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	if(state)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->iij_network_switch), FALSE);
		
		if(controler_iij_is_connected() != STATUS_CLIENT_CONNECTED)
		{
			uint8_t tcp_port_res = controler_iij_set_tcp_port(atoi(gtk_entry_get_text(GTK_ENTRY(this->iij_tcp_port_entry))));
			uint8_t ip_address_res = controler_iij_set_ip_addr((char*) gtk_entry_get_text(GTK_ENTRY(this->iij_ip_address_entry)));
	
			if((tcp_port_res == STATUS_SUCCESS) && (ip_address_res == STATUS_SUCCESS))
			{
				controler_iij_connect();
				gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), NULL);
				gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), NULL);
			}
			else
			{
				if(ip_address_res != STATUS_SUCCESS)
					gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), multi_lang->set_net_iij_wrong_tcp_port_label);
		
				if(tcp_port_res != STATUS_SUCCESS)
					gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), multi_lang->set_net_iij_wrong_ip_address_label);
			}
		}

		gtk_widget_set_sensitive(GTK_WIDGET(this->iij_network_switch), TRUE);
	}
	else
	{
		if(controler_iij_is_connected() == STATUS_CLIENT_CONNECTED)
			controler_iij_disconnect();
	}
	
	return TRUE;
}


void gui_network_page_set_pci_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	controler_pci_network_set_ip_address((char*) gtk_entry_get_text(GTK_ENTRY(widget)));
}

void gui_network_page_set_quadient_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	controler_quadient_network_set_ip_address((char*) gtk_entry_get_text(GTK_ENTRY(widget)));
}

void gui_network_page_set_iij_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	controler_iij_network_set_ip_address((char*) gtk_entry_get_text(GTK_ENTRY(widget)));
}



gui_hotfolder_page * gui_hotfolder_page_new(gui_base * gui_base_ref)
{
	gui_hotfolder_page * this = (gui_hotfolder_page*) malloc(sizeof(gui_hotfolder_page));

	this->gui_base_ref = gui_base_ref;

	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), gui_base_ref->work_area_geometry.width, gui_base_ref->work_area_geometry.height);

	this->hot_entry = (GtkWidget **) malloc(sizeof(GtkWidget*)*HOT_LIST_N);
	this->hot_btn = (GtkWidget **) malloc(sizeof(GtkWidget*)*HOT_LIST_N);	
	this->hot_lbl = (GtkWidget **) malloc(sizeof(GtkWidget*)*HOT_LIST_N);

	for(int i = 0; i < HOT_LIST_N; i++)
	{
		int y_pos = 350+(50*i);
		this->hot_entry[i] = gtk_entry_new();
		gtk_widget_set_size_request(GTK_WIDGET(this->hot_entry[i]), 300, 35);
		
		switch(i)
		{
			case Q_HOT_MAIN:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_q_main_hotfolder_path());
			break;
			case Q_HOT_BACKUP:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_q_backup_hotfolder_path());
			break;
			case Q_HOT_FEEDBACK:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_q_feedback_hotfolder_path());
			break;
			case PCI_HOT_IN:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_pci_hotfolder_in_path());
			break;
			case PCI_HOT_OUT:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_pci_hotfolder_out_path());
			break;
			case GIS_HOT:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_gis_hotfolder_path());
			break;
			case JOB_LOG_DIR:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), controler_get_job_report_hotfolder_path());
			break;
		}
	
		gtk_fixed_put(GTK_FIXED(this->page), this->hot_entry[i], gui_base_ref->work_area_geometry.width/4*3-410, y_pos);

		this->hot_btn[i] = gtk_button_new();
		gtk_widget_set_size_request(GTK_WIDGET(this->hot_btn[i]), 100, 35);
		gtk_fixed_put(GTK_FIXED(this->page), this->hot_btn[i], gui_base_ref->work_area_geometry.width/4*3-100, y_pos);

		this->hot_lbl[i] = gtk_label_new(NULL);
		gtk_fixed_put(GTK_FIXED(this->page), this->hot_lbl[i], gui_base_ref->work_area_geometry.width/4, y_pos);
	}

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, gui_base_ref->work_area_geometry.width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);

	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
		gui_base_ref->work_area_geometry.width/4, 250);


	return this;
}


void gui_hotfolder_page_language(gui_hotfolder_page * this, lang * multi_lang)
{
	for(int i = 0; i < HOT_LIST_N; i++)
	{
		gtk_button_set_label(GTK_BUTTON(this->hot_btn[i]), multi_lang->file_chooser_btn_browse);
		
		switch(i)
		{
			case Q_HOT_MAIN:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_q_main_fs_label);
			break;
			case Q_HOT_FEEDBACK:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_q_feedback_fs_label);
			break;
			case Q_HOT_BACKUP:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_q_backup_fs_label);
			break;
			case PCI_HOT_IN:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_pci_in_fs_label);
			break;
			case PCI_HOT_OUT:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_pci_out_fs_label);
			break;
			case GIS_HOT:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_gis_fs_label);
			break;
			case JOB_LOG_DIR:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang->hot_report_csv_fs_label);
			break;
		}
	}
}


void gui_hotfolder_page_select_q_main_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_main_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_MAIN]), path);
		controler_set_q_main_hotfolder_path(path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_q_feedback_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_feedback_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_FEEDBACK]), path);
		controler_set_q_feedback_hotfolder_path(path);
		g_free(path);
	}
}


void gui_hotfolder_page_select_q_backup_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_backup_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_BACKUP]), path);
		controler_set_q_backup_hotfolder_path(path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_pci_in_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_pci_in_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[PCI_HOT_IN]), path);
		controler_set_pci_hotfolder_in_path(path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_pci_out_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_pci_out_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[PCI_HOT_OUT]), path);
		controler_set_pci_hotfolder_out_path(path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_gis_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_gis_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[GIS_HOT]), path);
		controler_set_gis_hotfolder_path(path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_report_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_report_csv_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[JOB_LOG_DIR]), path);
		controler_set_job_report_hotfolder_path(path);
		g_free(path);
	}
}

gui_print_params_page * gui_print_params_page_new(gui_base * gui_base_ref)
{
	gui_print_params_page * this = (gui_print_params_page*) malloc(sizeof(gui_print_params_page));
	
	this->gui_base_ref = gui_base_ref;

	int width = gui_base_ref->work_area_geometry.width;
	int height = gui_base_ref->work_area_geometry.height;

	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), width, height);

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg,width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);


	this->par_lbl = (GtkWidget**) malloc(sizeof(GtkWidget *)* PAR_LIST_N);
	this->par_entry = (GtkWidget **) malloc(sizeof(GtkWidget *)*PAR_LIST_N);

	char buf[32];
	int i;

	for(i = 0; i < PAR_LIST_N; i++)
	{
		this->par_lbl[i] = gtk_label_new(NULL);

		this->par_entry[i] = gtk_entry_new();
		gtk_widget_set_size_request(GTK_WIDGET(this->par_entry[i]), 300, 35);

		switch(i)
		{
			case PAR_MAX_STACKED_SHEETS:
				sprintf(buf, "%d", controler_get_max_stacked_sheet());
			break;
			case PAR_MAX_REJECTED_SHEET_SEQ:
				sprintf(buf, "%d", controler_get_max_rejected_sheet_seq());
			break;
		}

		gtk_entry_set_text(GTK_ENTRY(this->par_entry[i]), buf);	

		gtk_fixed_put(GTK_FIXED(this->page), this->par_lbl[i], width/4, 250+80+(50*i));
		gtk_fixed_put(GTK_FIXED(this->page), this->par_entry[i], (width/4*3)-300, 250+80+(50*i));
	}	

	this->sheet_source_lbl = gtk_label_new(NULL);

	this->print_confirm_lbl = gtk_label_new(NULL);

	this->sheet_source_combo = gtk_combo_box_text_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->sheet_source_combo), 300, 35);

	this->print_confirm_switch = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->print_confirm_switch), 100,35);
	gtk_switch_set_active(GTK_SWITCH(this->print_confirm_switch), controler_get_sheet_source_confirmation());



	gtk_fixed_put(GTK_FIXED(this->page), this->sheet_source_lbl, width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->sheet_source_combo, width/4*3-300, 250+80+(50*i));
	i++;
	gtk_fixed_put(GTK_FIXED(this->page), this->print_confirm_lbl, width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->print_confirm_switch, width/4*3-300, 250+80+(50*i));
	i++;

	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
	gui_base_ref->work_area_geometry.width/4, 250);

	return this;
}

void gui_print_params_page_language(gui_print_params_page * this, lang * multi_lang)
{
	for(int i=0; i < PAR_LIST_N; i++)
	{
		switch(i)
		{
			case PAR_MAX_STACKED_SHEETS:
				gtk_label_set_text(GTK_LABEL(this->par_lbl[i]), multi_lang->par_max_stacked_sheet_lbl);
			break;
			case PAR_MAX_REJECTED_SHEET_SEQ:
				gtk_label_set_text(GTK_LABEL(this->par_lbl[i]), multi_lang->par_rejected_sheet_seq_lbl);
			break;
		}
	}

	
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(this->sheet_source_combo));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang->par_sheet_source_main);
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang->par_sheet_source_companion);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(this->sheet_source_combo), controler_get_sheet_source());
	
	gtk_label_set_text(GTK_LABEL(this->sheet_source_lbl), multi_lang->par_sheet_source_lbl);
	gtk_label_set_text(GTK_LABEL(this->print_confirm_lbl), multi_lang->par_print_confirm_lbl);

}


void gui_print_params_set_sheet_source_callback (GtkComboBox *widget, gpointer param)
{
	controler_set_companion_sheet_source(gtk_combo_box_get_active (widget));
}

gboolean gui_print_params_set_print_confirmation_state_callback (GtkSwitch *widget, gboolean state, gpointer param)
{
	controler_set_sheet_source_confirmation(state);

	return TRUE;
}

void gui_print_params_set_max_rejected_sheet_seq(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	int sheets_val = 0;
	char num_str[32];

	sprintf(num_str, "%s", gtk_entry_get_text(GTK_ENTRY(widget)));

	int exp = 0;
	uint8_t err = 0;

	while(num_str[exp] != 0)
	{
		if((isdigit(num_str[exp])))
		{
			sheets_val = sheets_val * 10 + (num_str[exp] - '0');
			if((sheets_val > 100) )
			{
				err++;
				num_str[exp] = 0;
				break;
			}
		}
		else
		{
			err++;
			num_str[exp] = 0;
			break;
		}

		exp ++;
	}

	if(err > 0)
		gtk_entry_set_text(GTK_ENTRY(widget), num_str);
	else
		controler_set_max_rejected_sheet_seq(sheets_val);
}

void gui_print_params_page_max_stacked_sheet_callback (GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	int sheets_val = 0;
	char num_str[32];

	sprintf(num_str, "%s", gtk_entry_get_text(GTK_ENTRY(widget)));

	int exp = 0;
	uint8_t err = 0;

	while(num_str[exp] != 0)
	{
		if((isdigit(num_str[exp])))
		{
			sheets_val = sheets_val * 10 + (num_str[exp] - '0');
			if((sheets_val > 10000))
			{
				err++;
				num_str[exp] = 0;
				break;
			}
		}
		else
		{
			err++;
			num_str[exp] = 0;
			break;
		}

		exp ++;
	}

	if(err > 0)
		gtk_entry_set_text(GTK_ENTRY(widget), num_str);
	else
		controler_set_max_stacked_sheets(sheets_val);
}


gui_lang_page * gui_lang_page_new(gui_base * gui_base_ref)
{
	gui_lang_page * this = (gui_lang_page*) malloc(sizeof(gui_lang_page));

	this->gui_base_ref = gui_base_ref;
	this->lang_list = gtk_combo_box_text_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->lang_list), 400, 35);
	this->list_label = gtk_label_new(NULL);

	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), gui_base_ref->work_area_geometry.width, gui_base_ref->work_area_geometry.height);

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, gui_base_ref->work_area_geometry.width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);

	for(int i = 0; i<lang_num; i++)
	{
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->lang_list), ((lang*) multi_lang_get(i))->lang_name);
	}

	gtk_combo_box_set_active (GTK_COMBO_BOX(this->lang_list), controler_get_interface_language());

	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
		gui_base_ref->work_area_geometry.width/4, 250);

	gtk_fixed_put(GTK_FIXED(this->page), this->list_label, gui_base_ref->work_area_geometry.width/4, 405);
	gtk_fixed_put(GTK_FIXED(this->page), this->lang_list, gui_base_ref->work_area_geometry.width/4*3-400, 400);

	return this;
}


void gui_lang_page_language(gui_lang_page * this, lang * multi_lang)
{
	gtk_label_set_text(GTK_LABEL(this->list_label), multi_lang->set_lan_label);
}

void gui_lang_page_set_interface_language_callback(GtkComboBox * widget, gpointer * param)
{
	gui * this = (gui*) param;
	controler_set_interface_language(gtk_combo_box_get_active (GTK_COMBO_BOX(widget)));
	gui_set_language(this);
}

/**
** @ingroup Gui
** Constructor for gui_base class. Here are defined all shared data needed in graphic interface.
*/
gui_base * gui_base_new()
{
	gui_base * this = (gui_base*) malloc(sizeof(gui_base));

	GdkDisplay* display = gdk_display_get_default(); 
	GdkMonitor* monitor = gdk_display_get_monitor(display, 0); 
	gdk_monitor_get_workarea(monitor, &(this->work_area_geometry));

	return this; 
}

/**
** @ingroup Gui 
** function for loading GdkPixbuf icon struct, preparing for manipulation in GTK widgets
*/
GdkPixbuf * gui_base_load_icon(char * addr)
{
	GError * error = NULL;
	GdkPixbuf * icon = gdk_pixbuf_new_from_file(addr, &error);

	return icon;
}

gui_info_window * gui_info_window_new(char * title, char * info_label, void (*callback) (void*), void * param)
{
	gui_info_window * this = (gui_info_window*) malloc(sizeof(gui_info_window));

	this->callback = callback;
	this->param = param;
	
	this->dialog = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_resizable(GTK_WINDOW(this->dialog), FALSE);
	gtk_window_set_position(GTK_WINDOW(this->dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(this->dialog), 400, 150);
	gtk_window_set_title(GTK_WINDOW(this->dialog), title);

	this->info_label = gtk_label_new(info_label);
	gtk_widget_set_size_request(GTK_WIDGET(this->info_label), 400, 30);
	gtk_label_set_justify(GTK_LABEL(this->info_label), GTK_JUSTIFY_CENTER);

	this->button = gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(GTK_WIDGET(this->button), 100, 30);

	this->container = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(this->container), this->info_label, 0, 30);
	gtk_fixed_put(GTK_FIXED(this->container), this->button, 150, 80);
		
	gtk_container_add(GTK_CONTAINER(this->dialog), this->container);
	
	g_signal_connect(G_OBJECT(this->button), "clicked", G_CALLBACK(gui_info_window_callback), this);

	gtk_widget_show_all(GTK_WIDGET(this->dialog));
	
	return this;
}

void gui_info_window_callback(GtkWidget * widget, gpointer param)
{
	gui_info_window * this = (gui_info_window*) param;

	if(this->callback != NULL)
		this->callback(this->param);
	
	gtk_info_window_finalize();

	gtk_main_quit();
}

void gtk_info_window_finalize()
{
	
}

