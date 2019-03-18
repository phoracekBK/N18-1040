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
	JOB_ORDER_NAME,
	JOB_NOMINAL,
	JOB_SHEET_NUMBER,
	JOB_STAMP_NUMBER,
	JOB_Q_NAME,
	JOB_DATE_TIME,

	JOB_LIST_N
};

enum _gui_job_report_list_item_
{
	REP_FINISH_STATE = 0,
	REP_ORDER_NAME,
	REP_REJECTED_SHEET_NUMBER,
	REP_SHEET_NUMBER,
	REP_STAMP_NUMBER,
	REP_JOB_NAME,
	REP_DATE_TIME,

	REPO_ITEM_N
};

enum _gui_control_btn_
{
	GC_BTN_PRINT = 0,
	GC_BTN_PAUSE,
	GC_BTN_CONTINUE,
	GC_BTN_CANCEL,
	GC_BTN_PRINT_ONE,
	GC_BTN_ERROR_RESET,
	GC_BTN_FEED,
	GC_BTN_FAKE,

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
	S_BTN_MACHINE_OVERVIEW,
	S_BTN_IO_VISION,
	S_BTN_BACK,

	S_BTN_N
};

enum _gui_params_list_
{
	PAR_MAX_STACKED_SHEETS = 0,
	PAR_MAX_REJECTED_SHEET_SEQ,
	PAR_FEED_DELAY,

	PAR_LIST_N
};



struct _gui_base_;
typedef struct _gui_base_ gui_base;

struct _gui_control_page_;
typedef struct _gui_control_page_ gui_control_page;


struct _gui_csv_manage_;
typedef struct _gui_csv_manage_ gui_csv_manage;

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

struct _gui_machine_overview_;
typedef struct _gui_machine_overview_ gui_machine_overview;



/**
** @ingroup Gui
*/
struct _gui_
{
	GtkWidget * main_win;
	GtkWidget * window_container;
	GtkWidget * page_stack;
	GtkWidget * drawing_area;

	GtkWidget * status_bar;
	c_string * error_string;
	c_string * info_label;

	GtkWidget * bk_logo;
	GtkWidget * km_logo;
	GtkWidget * stc_logo;

	gui_base * gui_base_ref;
	gui_control_page * control_page;
	gui_csv_manage * csv_manage_page;
	gui_settings_page * settings_page;
	gui_io_vision_page * io_vision_page;
	gui_network_page * network_page;
	gui_lang_page * lang_page;
	gui_hotfolder_page * hotfolder_page;
	gui_print_params_page * print_params_page;
	gui_machine_overview * machine_overview;

	uint8_t error_blink;
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
	
	GtkWidget * img_machine;


	gui_base * gui_base_ref;
};

struct _gui_csv_manage_
{
	GtkWidget * page;

	GtkWidget * btn_return;
	GtkWidget * spin_year_to;
	GtkWidget * spin_month_to;
	GtkWidget * spin_day_to;
	GtkWidget * list_report_csv;
	GtkListStore * job_list_store;
	GtkWidget * scroll_report_list_csv;
	GtkWidget * btn_export;
	GtkWidget * lbl_year_to;
	GtkWidget * lbl_month_to;
	GtkWidget * lbl_day_to;
	
	GtkWidget * lbl_year_from;
	GtkWidget * lbl_month_from;
	GtkWidget * lbl_day_from;

	GtkWidget * spin_year_from;
	GtkWidget * spin_month_from;
	GtkWidget * spin_day_from;

	GtkWidget * separator_from;
	GtkWidget * separator_to;
	GtkWidget * lbl_from;
	GtkWidget * lbl_to;

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
	GtkWidget ** btn_out;
	GtkWidget * btn_manual;
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

	GtkWidget * fan_intensity_spin;
	GtkWidget * fan_control_switch;

	GtkWidget * fan_intensity_lbl;
	GtkWidget * fan_control_lbl;

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

struct _gui_machine_overview_
{
	GtkWidget * page;
	GtkWidget * machine_img;
	GtkWidget * info_box;	


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
	array_list * report_csv_list;
	bool blink;
};






void gui_create_main_window(gui * this);
gboolean gui_on_draw_event(GtkWidget * widget, cairo_t *cr, gpointer param);
gboolean gui_cyclic_interupt(gpointer param);
void gui_set_language(gui * this);
void gui_lang_page_set_interface_language_callback(GtkComboBox * widget, gpointer * param);
void gui_pack(gui * this);
char * gui_hotfolder_page_def_file_chooser(char * title);


char * gui_def_file_chooser(gui* this, char * title, GtkFileChooserAction choose_action, GtkFileFilter *filter, char * default_file_name);

gui_control_page * gui_control_page_new(gui_base * gui_base_ref);
void gui_control_page_language(gui_control_page * this);
gboolean gui_control_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param);
void gui_control_page_load_jobs(gui_control_page * this);
GtkTreeViewColumn * gui_control_page_new_tree_column(char * label, int index);
void gui_control_page_delete_columns(GtkWidget * list);
void gui_control_page_load_report_csv_list(gui_control_page * this);
void gui_control_page_open_report_csv (GtkTreeView *tree_view, gpointer param);


void gui_control_page_btn_print_req(gui_control_page * this);
void gui_control_page_btn_feed_sheet_callback(GtkWidget * widget, gpointer param);
void gui_control_page_btn_fake_companion_sheet(GtkWidget * widget, gpointer param);
void gui_control_page_btn_print_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_reset_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_pause_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_go_to_settings_callback(GtkButton *button, gpointer param);
void gui_control_page_set_machine_mode_callback (GtkComboBox *widget, gpointer param);
void gui_control_page_set_ena_callback(GtkSwitch *widget, gboolean state, gpointer param);
void gui_control_page_manual_sheet_feed(GtkWidget * widget, gpointer param);
void gui_control_page_go_to_csv_manage_page(GtkWidget * widget, gpointer param);
void gui_control_page_print_one_callback(GtkWidget * widget, gpointer param);

gui_settings_page * gui_settings_page_new(gui_base * gui_base_ref);
void gui_settings_page_language(gui_settings_page * this, lang * multi_lang);
void gui_settings_page_btn_go_to_io_vision_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_lang_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_print_params_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_hotfolder_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_go_to_network_settings_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_back_callback(GtkWidget* widget, GdkEventButton* event, gpointer param);
void gui_settings_page_btn_got_to_machine_overview_callback(GtkWidget * widget, GdkEventButton* event, gpointer param);
void gui_settings_page_return_callback(GtkButton *button, GdkEventButton * event, gpointer param);


gui_csv_manage * gui_csv_manage_new(gui_base * gui_base_ref);
void gui_csv_manage_return_to_control_page(GtkWidget* widget, gpointer param);
void gui_csv_manage_language(gui_csv_manage * this, lang * multi_lang);
int gui_csv_manage_check_day(int day, int year, int month);
void gui_csv_manage_set_day_to (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_set_year_to (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_set_month_to (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_set_day_from (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_set_year_from (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_set_month_from (GtkSpinButton *spin_button, gpointer param);
void gui_csv_manage_split_csv_callback(GtkWidget * widget, gpointer param);

gui_io_vision_page * gui_io_vision_page_new(gui_base * gui_base_ref);
void gui_io_vision_draw_socket(cairo_t* cr, double x, double y, double w, double h, char* label);
gboolean gui_io_vision_outputs_control(GtkWidget *widget, GdkEventButton *event, gpointer param);
void gui_io_vision_set_manual(GtkSwitch *widget, gboolean state, gpointer param);
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
void gui_print_params_set_feed_delay(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_print_params_set_fan_intensity(GtkSpinButton * spin_button, gpointer param);
gboolean gui_print_params_set_fan_activity_callback (GtkSwitch *widget, gboolean state, gpointer param);



gui_machine_overview * gui_machine_overview_new(gui_base * gui_base_ref);
void gui_machine_overview_language(gui_machine_overview * this, lang * multi_lang);
gboolean gui_machine_overview_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param);

gui_lang_page * gui_lang_page_new(gui_base * gui_base_ref);
void gui_lang_page_language(gui_lang_page * this, lang * multi_lang);

gui_info_window * gui_info_window_new(char * title, char * info_label, void (*callback)(void*), void * param);
void gtk_info_window_finalize();
void gui_info_window_callback(GtkWidget * widget, gpointer param);

gui_base * gui_base_new();
GdkPixbuf * gui_base_load_icon(char * addr);
GdkPixbuf * gui_base_scale_icon(GdkPixbuf * icon, uint8_t dim, double new_size);
void gui_base_update_report_csv_list(GtkWidget * tree_view, array_list * report_csv_list, GtkListStore * job_report_list_store);








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
	gtk_widget_set_size_request(GTK_WIDGET(this->status_bar), 
					this->gui_base_ref->work_area_geometry.width, 
					this->gui_base_ref->work_area_geometry.height/12);
	this->error_string = c_string_new();
	this->info_label = c_string_new();
	this->error_blink = 0;
	
	/* create control page */
	this->control_page = gui_control_page_new(this->gui_base_ref);

	/* create csv manage page */
	this->csv_manage_page = gui_csv_manage_new(this->gui_base_ref);

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

	this->machine_overview = gui_machine_overview_new(this->gui_base_ref);

	/* set language */
	gui_set_language(this);

	/* create container and put everything together */
	gui_pack(this);	

	/* cyclic interupt for continues reading of core status */
	g_timeout_add(50, gui_cyclic_interupt, this);

	gtk_widget_show_all(GTK_WIDGET(this->main_win));

	return this;
}

/* todo loading job list */
gboolean gui_cyclic_interupt(gpointer param)
{
	gui * this = (gui*) param;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	if(this->error_blink > 20)
	{
		if(controler_machine_status_val() != MACHINE_STATE_WAIT)
		{
			if((gtk_button_get_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL])) == NULL) || 
				(strcmp(gtk_button_get_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL])), "Storno") != 0))
				gtk_button_set_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL]), "Storno");
		}
		else
		{
			if((gtk_button_get_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL])) == NULL) || 
				(strcmp(gtk_button_get_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL])), "Vyčistit") != 0))
				gtk_button_set_label(GTK_BUTTON(this->control_page->btn[GC_BTN_CANCEL]), "Vyčistit");
		}


		gtk_widget_queue_draw(GTK_WIDGET(this->machine_overview->info_box));

		/* set status of network control widget depend on network connection state */
		gboolean conn = ((controler_iij_is_connected() == STATUS_CLIENT_CONNECTED) ? FALSE : TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_tcp_port_entry), conn);
		gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_ip_address_entry), conn);

		gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->pci_connection_entry), controler_pci_network_connected() != STATUS_CLIENT_CONNECTED);
		gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->quadient_connection_entry), controler_quadient_network_connected() != STATUS_CLIENT_CONNECTED);
		gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_connection_entry), controler_iij_network_connected() != STATUS_CLIENT_CONNECTED);

		gtk_switch_set_active(GTK_SWITCH(this->network_page->iij_network_switch), !conn);

		gtk_switch_set_active(GTK_SWITCH(this->print_params_page->fan_control_switch), ((controler_get_fan_activity() == 0) ? FALSE : TRUE));

		this->gui_base_ref->blink = !(this->gui_base_ref->blink);
		this->error_blink = 0;

		//if(strcmp(visible_page, "control_page") == 0)
		{
			if(((controler_machine_status_val() != MACHINE_STATE_WAIT) &&
				(controler_machine_status_val() != MACHINE_STATE_ERROR)) || 
				((controler_job_list_changed() > 0) && (controler_machine_status_val() == MACHINE_STATE_WAIT)))
			{
				gui_control_page_load_jobs(this->control_page);
			}
		}
	}
	else
	{
		this->error_blink++;
	}

	gtk_widget_set_visible(GTK_WIDGET(this->io_vision_page->btn_manual), controler_machine_status_val() == MACHINE_STATE_WAIT);
	gtk_widget_set_visible(GTK_WIDGET(this->control_page->ena_switch), controler_get_manual_mode_state());
	gtk_widget_set_visible(GTK_WIDGET(this->control_page->xbf_pulse), controler_get_manual_mode_state());

	//const char * visible_page = gtk_stack_get_visible_child_name (GTK_STACK(this->page_stack));

	gtk_combo_box_set_active(GTK_COMBO_BOX(this->control_page->print_mode_combo), controler_get_machine_mode());
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->print_mode_combo),((controler_machine_status_val() == MACHINE_STATE_ERROR) || 
				(controler_machine_status_val() == MACHINE_STATE_WAIT)) && 
				controler_get_manual_mode_state() == false);

	bool pause_en = (controler_machine_status_val() != MACHINE_STATE_WAIT) && (controler_machine_status_val() != MACHINE_STATE_NEXT) && 
						(controler_machine_status_val() != MACHINE_STATE_ERROR) && (controler_machine_status_val() != MACHINE_STATE_PAUSE) && 
						(controler_machine_status_val() != MACHINE_STATE_READY_TO_START) && (controler_machine_status_val() != MACHINE_STATE_WAIT_FOR_CONFIRMATION &&
						controler_get_manual_mode_state() == false);

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PAUSE]), pause_en);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CONTINUE]), controler_machine_status_val() == MACHINE_STATE_PAUSE && 
						(controler_get_manual_mode_state() == false));
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PRINT]), (controler_machine_status_val() == MACHINE_STATE_WAIT) && (controler_get_job_queue_size() > 0) && 
						(controler_get_manual_mode_state() == false));


	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PRINT_ONE]),
					((controler_machine_status_val() == MACHINE_STATE_WAIT) && (controler_get_job_queue_size() > 0)) ||
					(controler_machine_status_val() == MACHINE_STATE_NEXT) || 
					(controler_machine_status_val() == MACHINE_STATE_PRINT_MAIN) || 
					(controler_machine_status_val() == MACHINE_STATE_READY_TO_START) || 
					(controler_machine_status_val() == MACHINE_STATE_READ_CSV_LINE) || 
					(controler_machine_status_val() == MACHINE_STATE_PAUSE));

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_ERROR_RESET]), (controler_get_manual_mode_state() == false));

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CANCEL]), ((controler_machine_status_val() != MACHINE_STATE_ERROR) && 
							(controler_get_manual_mode_state() == false)));

	gtk_widget_set_visible(GTK_WIDGET(this->control_page->btn[GC_BTN_FEED]), controler_machine_status_val() == MACHINE_STATE_WAIT_FOR_CONFIRMATION && 
					(controler_get_manual_mode_state() == false));


	gtk_widget_set_visible(GTK_WIDGET(this->control_page->btn[GC_BTN_FAKE]),((controler_machine_status_val() == MACHINE_STATE_WAIT) && (controler_get_job_queue_size() > 0)) ||
					(((controler_machine_status_val() == MACHINE_STATE_PRINT_MAIN) ||
					(controler_machine_status_val() == MACHINE_STATE_READ_CSV_LINE) ||
					(controler_machine_status_val() == MACHINE_STATE_PAUSE) ||
					(controler_machine_status_val() == MACHINE_STATE_FAKE_COMPANION) || 
					(controler_machine_status_val() == MACHINE_STATE_ERROR)) && (strlen(controler_get_printed_job_name()) > 0)));

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

 	if(((controler_machine_status_val() == MACHINE_STATE_READY_TO_START) && (controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK) && 
		(controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED)) || (controler_machine_status_val() == MACHINE_STATE_PREPARE))
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_preparing);
	}
	else if(controler_machine_status_val() == MACHINE_STATE_WAIT_FOR_PRINT_FINISH)
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_finishing);
	}
	else if(controler_machine_status_val() == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_feed_sheet_confirm);
	}
	else if(controler_machine_status_val() == MACHINE_STATE_FEEDER_ERROR)
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_fix_feeder_issue);
	}

	else if((controler_machine_status_val() == MACHINE_STATE_READY_TO_START) && 
		((controler_get_stacker_status() != MACHINE_SN_STACKER_READY_TO_STACK) || (controler_get_feeder_status() != MACHINE_FN_READY_TO_FEED)))
		c_string_set_string(this->info_label,(char*) multi_lang->info_label_feeder_stacker_not_prepared);
	else if(controler_machine_status_val() == MACHINE_STATE_PAUSE)
	{
		if(controler_get_stacked() >= controler_get_max_stacked_sheet())
			c_string_set_string(this->info_label, (char *) multi_lang->info_label_max_stacked_sheet);
		else
			c_string_set_string(this->info_label, (char *) multi_lang->print_state_pause);
	}
	else if((controler_machine_status_val() == MACHINE_STATE_READ_CSV_LINE) && ((controler_get_stacked()+10) >= controler_get_max_stacked_sheet()))
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_approx_max_stacked_sheet);
	}
	else if(controler_machine_status_val() == MACHINE_STATE_FAKE_COMPANION)
	{
		c_string_set_string(this->info_label, (char *) multi_lang->info_label_companion_fake_instruction);
	}
	else
	{
		if(c_string_len(this->info_label) > 0)
			c_string_clear(this->info_label);
	}	

	gtk_widget_set_sensitive(GTK_WIDGET(this->print_params_page->fan_control_switch), controler_machine_status_val() == MACHINE_STATE_WAIT);
	
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
		settings_button_set_text(this->machine_overview->btn_return, multi_lang->set_btn_back_to_settngs);
		settings_button_set_text(this->lang_page->btn_return, multi_lang->set_btn_back_to_settngs);

		gui_settings_page_language(this->settings_page, multi_lang);

		gui_print_params_page_language(this->print_params_page, multi_lang);

		gui_hotfolder_page_language(this->hotfolder_page, multi_lang);

		gui_network_page_language(this->network_page, multi_lang);

		gui_csv_manage_language(this->csv_manage_page, multi_lang);

		gui_machine_overview_language(this->machine_overview, multi_lang);

		gui_lang_page_language(this->lang_page, multi_lang);
	}
}

gboolean gui_status_bar_draw(GtkWidget * widget, cairo_t *cr, gpointer param)
{
	gui * this = (gui*) param;	

	if(c_string_len(this->error_string) > 0)
	{
		if(this->gui_base_ref->blink == true)
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

		if(this->gui_base_ref->blink == true)
		{
			cairo_text_extents(cr, c_string_get_char_array(this->error_string), &extents);
		}
		else
		{		
			if(c_string_len(this->info_label) > 0)
				cairo_text_extents(cr, c_string_get_char_array(this->info_label), &extents);
			else
				cairo_text_extents(cr, c_string_get_char_array(this->error_string), &extents);
		}

        	cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_move_to(cr, 30, this->gui_base_ref->work_area_geometry.height/24.0 + ((double)extents.height/2.0)-2);
		
		if(this->gui_base_ref->blink == true)
		{
			cairo_show_text(cr, c_string_get_char_array(this->error_string));
		}
		else
		{
			if(c_string_len(this->info_label) > 0)
				cairo_show_text(cr, c_string_get_char_array(this->info_label));
			else
				cairo_show_text(cr, c_string_get_char_array(this->error_string));
		}

        	cairo_fill(cr);
	}
	else
	{
		if(c_string_len(this->info_label) > 0)
		{
			cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
			cairo_set_font_size(cr, 19);
			cairo_text_extents_t extents;
			cairo_text_extents(cr, c_string_get_char_array(this->info_label), &extents);

        		cairo_set_source_rgb(cr, 0, 0, 0);
			cairo_move_to(cr, 30, this->gui_base_ref->work_area_geometry.height/24.0 + ((double)extents.height/2.0)-2);
			cairo_show_text(cr, c_string_get_char_array(this->info_label));

        		cairo_fill(cr);
		}
	}

	if((strlen(controler_get_printed_job_name()) > 0) && (controler_machine_status_val() != MACHINE_STATE_ERROR))
	{
		cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(cr, 19);
		cairo_text_extents_t extents;
        	cairo_set_source_rgb(cr, 0, 0, 0);

		char time_str[128];

		if(controler_get_time_for_one_sheet() == 0)
		{	
			sprintf(time_str, "Odhadovaný čas tisku: --:--:--");
			cairo_text_extents(cr, time_str, &extents);
		}
		else
		{
			int total_sheet_number = controler_get_total_sheet_number();
			int stacked_sheet_number = controler_get_stacked_sheets();
			int companion_sheet_number = controler_get_feeded_companion_sheets();

			uint32_t total_time = (total_sheet_number - (stacked_sheet_number - companion_sheet_number)) * (controler_get_time_for_one_sheet());

			uint8_t hours = total_time/3600000;
			uint8_t minutes = total_time/60000%60;
			uint8_t seconds = (total_time/1000)%60;
			
			sprintf(time_str, "Odhadovaný čas tisku: %02d:%02d:%02d", hours, minutes, seconds);
			cairo_text_extents(cr, time_str, &extents);
		}

		cairo_move_to(cr, this->gui_base_ref->work_area_geometry.width-extents.width-30, 
				this->gui_base_ref->work_area_geometry.height/24.0 + ((double)extents.height/2.0)-2);
		cairo_show_text(cr, time_str);


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
	
	g_signal_connect(G_OBJECT(this->io_vision_page->io_vision), 
				"button_press_event", 
				G_CALLBACK(gui_io_vision_outputs_control), 
				this->io_vision_page);

	g_signal_connect(G_OBJECT(this->io_vision_page->btn_manual), 
				"state-set", 
				G_CALLBACK(gui_io_vision_set_manual), 
				NULL);

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

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->machine_overview->btn_return)), 
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
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_FAKE]), 
				"clicked", 
				G_CALLBACK(gui_control_page_btn_fake_companion_sheet),
				this->control_page);
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
	g_signal_connect(G_OBJECT(this->control_page->btn_export), 
				"clicked", 
				G_CALLBACK(gui_control_page_go_to_csv_manage_page), 
				this);

	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_PRINT_ONE]), 
				"clicked", 
				G_CALLBACK(gui_control_page_print_one_callback), 
				this->control_page);

	/* machine overview page signals*/
	g_signal_connect(G_OBJECT(this->machine_overview->info_box), 
				"draw", 
				G_CALLBACK(gui_machine_overview_info_box_draw_callback), 
				this->machine_overview);



	/* csv manage page signals */
	g_signal_connect(G_OBJECT(this->csv_manage_page->btn_return), 
				"clicked", 
				G_CALLBACK(gui_csv_manage_return_to_control_page), 
				this);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_day_to),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_day_to), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_month_to),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_month_to), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_year_to),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_year_to), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_day_from),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_day_from), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_month_from),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_month_from), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->spin_year_from),
			 	"value_changed",
				G_CALLBACK(gui_csv_manage_set_year_from), this->csv_manage_page);
	g_signal_connect(G_OBJECT(this->csv_manage_page->btn_export), 
				"clicked", 
				G_CALLBACK(gui_csv_manage_split_csv_callback), 
				this);

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

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_MACHINE_OVERVIEW])), 
				"button_press_event", 
				G_CALLBACK(gui_settings_page_btn_got_to_machine_overview_callback),
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
	g_signal_connect(G_OBJECT(this->print_params_page->par_entry[PAR_FEED_DELAY]), 
				"key-release-event", 
				G_CALLBACK(gui_print_params_set_feed_delay), 
				this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->sheet_source_combo), 
				"changed", 
				G_CALLBACK(gui_print_params_set_sheet_source_callback), 
				this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->print_confirm_switch), 
				"state_set", 
				G_CALLBACK(gui_print_params_set_print_confirmation_state_callback), 
				this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->fan_control_switch), 
				"state_set", 
				G_CALLBACK(gui_print_params_set_fan_activity_callback), 
				NULL);
	g_signal_connect(G_OBJECT(this->print_params_page->fan_intensity_spin),
			 	"value_changed",
				G_CALLBACK(gui_print_params_set_fan_intensity), 
				NULL);
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
	//gtk_window_set_decorated (GTK_WINDOW(this->main_win), FALSE);

	double icon_heigh = 75;
	this->bk_logo = gtk_image_new_from_pixbuf(gui_base_scale_icon(gui_base_load_icon(BK_ICON_PATH), 0, icon_heigh));
	this->km_logo = gtk_image_new_from_pixbuf(gui_base_scale_icon(gui_base_load_icon(KM_ICON_PATH), 0, icon_heigh));
	this->stc_logo = gtk_image_new_from_pixbuf(gui_base_scale_icon(gui_base_load_icon(STC_ICON_PATH), 0, icon_heigh));


	this->drawing_area = gtk_drawing_area_new();
        gtk_widget_set_size_request(GTK_WIDGET(this->drawing_area), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);

	this->page_stack = gtk_stack_new();
        gtk_widget_set_size_request(GTK_WIDGET(this->page_stack), this->gui_base_ref->work_area_geometry.width, this->gui_base_ref->work_area_geometry.height);
}


char * gui_def_file_chooser(gui* this, char * title, GtkFileChooserAction choose_action, GtkFileFilter *filter, char * default_file_name)
{
	GtkWidget *dialog;

	GtkFileChooserAction action = choose_action;
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

	if(filter != NULL)
		gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog), filter);
	
	if(default_file_name != NULL)
		 gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), default_file_name);

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
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->csv_manage_page->page, "csv_manage_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->settings_page->page, "settings_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->io_vision_page->page, "io_vision_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->lang_page->page, "lang_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->network_page->page, "network_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->hotfolder_page->page, "hotfolder_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->print_params_page->page, "print_params_page");
	gtk_stack_add_named(GTK_STACK(this->page_stack), this->machine_overview->page, "machine_overview");

	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "control_page");

	this->window_container = gtk_fixed_new();

	gtk_fixed_put(GTK_FIXED(this->window_container), this->drawing_area, 0, 0);
	gtk_fixed_put(GTK_FIXED(this->window_container), this->status_bar, 0, this->gui_base_ref->work_area_geometry.height/12);
	gtk_fixed_put(GTK_FIXED(this->window_container), this->page_stack, 0, 0);

	gtk_fixed_put(GTK_FIXED(this->window_container), 
			this->bk_logo, 
			this->gui_base_ref->work_area_geometry.width-100-gdk_pixbuf_get_width(gtk_image_get_pixbuf(GTK_IMAGE(this->bk_logo))), 
			5);

	gtk_fixed_put(GTK_FIXED(this->window_container), 
			this->stc_logo, 
			this->gui_base_ref->work_area_geometry.width/2-gdk_pixbuf_get_width(gtk_image_get_pixbuf(GTK_IMAGE(this->stc_logo))), 
			5);

	gtk_fixed_put(GTK_FIXED(this->window_container), 
			this->km_logo, 
			100, 
			5);

	gtk_container_add(GTK_CONTAINER(this->main_win), this->window_container);
}

gui_control_page * gui_control_page_new(gui_base * gui_base_ref)
{
	gui_control_page * this = (gui_control_page *) malloc(sizeof(gui_control_page));

	this->gui_base_ref = gui_base_ref;

	/* window dimensions */
	double width = gui_base_ref->work_area_geometry.width;
	double height = gui_base_ref->work_area_geometry.height;

	
	this->img_machine = gtk_image_new_from_pixbuf(gui_base_scale_icon(gui_base_load_icon(MACHINE_ICON_PATH), 1, width/4-100));

	this->ena_switch = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->ena_switch), 100, 35);

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
	gtk_fixed_put(GTK_FIXED(this->page), this->ena_switch, width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+470);
	gtk_fixed_put(GTK_FIXED(this->page), this->xbf_pulse, width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+510);
	gtk_fixed_put(GTK_FIXED(this->page), this->img_machine, 50, height/4*3-50);



	for(int i = 0; i < GC_BTN_N; i++)
	{
		if(i == GC_BTN_PRINT)
			this->btn[i] = gtk_button_new_with_label ("Start");
		else if(i == GC_BTN_PAUSE)
			this->btn[i] = gtk_button_new_with_label ("Pauza");
		else if(i == GC_BTN_CONTINUE)
			this->btn[i] = gtk_button_new_with_label ("Pokračovat");
		else if(i == GC_BTN_CANCEL)
			this->btn[i] = gtk_button_new();
		else if(i == GC_BTN_PRINT_ONE)
			this->btn[i] = gtk_button_new_with_label("1 arch + pauza");
		else if(i == GC_BTN_ERROR_RESET)
			this->btn[i] = gtk_button_new_with_label("Reset");
		else if(i == GC_BTN_FEED)
			this->btn[i] = gtk_button_new_with_label ("Naložit");
		else if(i == GC_BTN_FAKE)
			this->btn[i] = gtk_button_new_with_label ("Proklad");

		gtk_widget_set_size_request(GTK_WIDGET(this->btn[i]), 90,70);
		gtk_fixed_put(GTK_FIXED(this->page), this->btn[i], width/4+(125*i), (5*height/24)+(height/8)+90);
	}

	return this;
}


void gui_control_page_print_one_callback(GtkWidget * widget, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;

	if(controler_machine_status_val() == MACHINE_STATE_WAIT)
	{
		gui_control_page_btn_print_req(this);
		controler_print_one_sheet_req();
	}
	else
	{
		controler_print_one_sheet_req();
	}
}


void gui_control_page_go_to_csv_manage_page(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		printf("report list size: %d\n", array_list_size(filtered_job_list));
		//*
		for(int i = 0; i < array_list_size(filtered_job_list); i++)
		{
			c_string * str = array_list_get(filtered_job_list, i);
			if(str!=NULL)
				printf("%s\n", c_string_get_char_array(str));
			else
				printf("EMPTY\n");
		}
		//*/
		gui_base_update_report_csv_list(this->csv_manage_page->list_report_csv, 
						filtered_job_list, 
						this->csv_manage_page->job_list_store);
	
		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}


	gtk_stack_set_visible_child_name(GTK_STACK(this->page_stack), "csv_manage_page");
}

void gui_control_page_manual_sheet_feed(GtkWidget * widget, gpointer param)
{
	controler_manual_feed_sheet();
}

void gui_control_page_set_ena_callback(GtkSwitch *widget, gboolean state, gpointer param)
{
	controler_manual_set_ena_state(state);
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

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+470);
	cairo_show_text(cr, multi_lang->g_cnt_compenzation_lbl);


	cairo_set_font_size(cr, 19);

	sprintf(temp_buff, "%d", controler_get_feeded_sheets());
	cairo_text_extents_t ext_main_counter;
	cairo_text_extents(cr, temp_buff, &ext_main_counter);
	cairo_move_to(cr, width/4-left_horizontal_offset - ext_main_counter.width, 5*height/24+160);
	cairo_show_text(cr, temp_buff);
	cairo_move_to(cr, width/4-left_horizontal_offset, height/4*3+130);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_feeded_companion_sheets());
	cairo_text_extents_t ext_companion_counter;
	cairo_text_extents(cr, temp_buff, &ext_companion_counter);
	cairo_move_to(cr, width/4- left_horizontal_offset - ext_companion_counter.width, 5*height/24+200);
	cairo_show_text(cr, temp_buff);
	cairo_move_to(cr, width/4-140, height/4*3+130);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_rejected_sheets());
	cairo_text_extents_t ext_reject_counter;
	cairo_text_extents(cr, temp_buff, &ext_reject_counter);
	cairo_move_to(cr, width/4-left_horizontal_offset - ext_reject_counter.width, 5*height/24+290);
	cairo_show_text(cr, temp_buff);
	cairo_move_to(cr, 130, height/4*3+130);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_tab_inserts());
	cairo_text_extents_t ext_ti_counter;
	cairo_text_extents(cr, temp_buff, &ext_ti_counter);
	cairo_move_to(cr, width/4-left_horizontal_offset - ext_ti_counter.width, 5*height/24+390);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d/%d",controler_get_total_sheet_number(), controler_get_stacked_sheets()-controler_get_feeded_companion_sheets());
	cairo_text_extents_t ext_stacked_counter;
	cairo_text_extents(cr, temp_buff, &ext_stacked_counter);
	cairo_move_to(cr, width/4-left_horizontal_offset - ext_stacked_counter.width, 5*height/24+430);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", controler_get_rejected_sheet_in_job());
	cairo_text_extents_t ext_compenzation_counter;
	cairo_text_extents(cr, temp_buff, &ext_compenzation_counter);
	cairo_move_to(cr, width/4-left_horizontal_offset - ext_compenzation_counter.width, 5*height/24+470);
	cairo_show_text(cr, temp_buff);


	sprintf(temp_buff, "%d", controler_get_stacked_sheets());
	cairo_move_to(cr, 50, height/4*3+130);
	cairo_show_text(cr, temp_buff);

	/* right side */

	cairo_set_font_size(cr, 15);
	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+200);
	cairo_show_text(cr, multi_lang->g_status_machine_lbl);

	cairo_text_extents_t ext_machine_status;
	cairo_text_extents(cr, controler_machine_get_state_str(), &ext_machine_status);
	cairo_move_to(cr, width-(width/4-350)/2-ext_machine_status.width, 5*height/24+200);
	cairo_show_text(cr, controler_machine_get_state_str());

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
	
	cairo_move_to(cr,width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+360);
	cairo_show_text(cr, multi_lang->print_active);

	cairo_save(cr);
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


	if(controler_get_card_output(IO_CARD_A1, A1_OUT_3_PRN_rdy) == 0)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);

	cairo_arc(cr, width-(width/4-350)/2-15, (5*height/24)+(height/8)+360, 10, 0, 2*M_PI);

	cairo_fill(cr);


	if(((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && 
		(controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK || controler_get_stacker_status() == MACHINE_SN_STACKING))) 
	{
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && (controler_get_stacker_status() == MACHINE_SN_STACKER_READY))
	{
		if(this->gui_base_ref->blink == true)
			cairo_set_source_rgb(cr,1,0.5,0);
		else
			cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) == 0) && 
		(controler_get_stacker_status() == MACHINE_SN_STACKER_READY || controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK))
	{
		cairo_set_source_rgb(cr,1,0.5,0);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}


	cairo_arc(cr, 50, height/4*3+10, 10, 0, 2*M_PI);
	cairo_fill(cr);


	if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && ((controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED) || (controler_get_feeder_status() == MACHINE_FN_FEEDING))) 
	{
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && (controler_get_feeder_status() == MACHINE_FN_FEEDER_READY))
	{
		if(this->gui_base_ref->blink == true)
			cairo_set_source_rgb(cr,1,0.5,0);
		else
			cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) == 0) && (controler_get_feeder_status() == MACHINE_FN_FEEDER_READY || (controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED)))
	{
		cairo_set_source_rgb(cr,1,0.5,0);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}

	cairo_arc(cr, width/4-70, height/4*3+25, 10, 0, 2*M_PI);
	cairo_fill(cr);

	cairo_restore(cr);

	cairo_move_to(cr,width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+410);
	cairo_show_text(cr, multi_lang->par_stacker_status);
	cairo_stroke(cr);

	cairo_save(cr);
	cairo_text_extents_t ext_stacker_status;
	cairo_text_extents(cr, controler_get_stacker_status_string(controler_get_stacker_status()), &ext_stacker_status);

	if((controler_get_stacker_status() == MACHINE_SN_STACKER_READY) || 
		(controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK) ||
		(controler_get_stacker_status() == MACHINE_SN_STACKING))
	{
		cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}

	cairo_move_to(cr, width-(width/4-350)/2-ext_stacker_status.width, (5*height/24)+(height/8)+410);
	cairo_show_text(cr, controler_get_stacker_status_string(controler_get_stacker_status()));
	cairo_stroke(cr);
	cairo_restore(cr);

	cairo_move_to(cr,width/4*3+(width/4-350)/2, (5*height/24)+(height/8)+450);
	cairo_show_text(cr, multi_lang->par_feeder_status);
	cairo_stroke(cr);

	cairo_text_extents_t ext_feeder_status;
	cairo_text_extents(cr, controler_get_feeder_status_string(controler_get_feeder_status()), &ext_feeder_status);
	
	if((controler_get_feeder_status() == MACHINE_FN_FEEDER_READY) || 
		(controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED) ||
		(controler_get_feeder_status() == MACHINE_FN_FEEDING))
	{
		cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}

	cairo_move_to(cr,width-(width/4-350)/2-ext_feeder_status.width, (5*height/24)+(height/8)+450);
	cairo_show_text(cr, controler_get_feeder_status_string(controler_get_feeder_status()));

	return FALSE;
}


void gui_control_page_language(gui_control_page * this)
{
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	gtk_button_set_label(GTK_BUTTON(this->btn_export), multi_lang->gui_go_to_csv_manage_page_label);

	/* columns for the job list tree widget */
	gui_control_page_delete_columns(this->job_list);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_state, JOB_STATE)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_order_name, JOB_ORDER_NAME)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_nominal, JOB_NOMINAL)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_sheet_number, JOB_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_stamp_number, JOB_STAMP_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->gui_job_q_name, JOB_Q_NAME)));

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
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_order_name, REP_ORDER_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_rejected_sheets, REP_REJECTED_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_sheet_number, REP_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_stamp_number, REP_STAMP_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_job_name, REP_JOB_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_date_time, REP_DATE_TIME)));

}

void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param)
{
	if(controler_machine_status_val() != MACHINE_STATE_WAIT)
		controler_print_cancel();
	else
		controler_total_clear_hotfolder();
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
	/* set pause */
	if(controler_machine_status_val() != MACHINE_STATE_ERROR)
		controler_print_pause();
}

void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param)
{
	if(controler_machine_status_val() == MACHINE_STATE_PAUSE)
		controler_print_continue();
}


void gui_control_page_btn_print_req(gui_control_page * this)
{
	/* start new print */
	GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (this->job_list));
	GtkTreeIter iter;
       	GtkTreeModel *model;
       	gchar *job_name;

       	if (gtk_tree_selection_get_selected (selection, &model, &iter))
       	{
       	        gtk_tree_model_get (model, &iter, JOB_Q_NAME, &job_name, -1);

		controler_print_start((const char*) job_name);
       	        g_free(job_name);
       	}
	else
	{
		controler_print_start(NULL);
	}
}

void gui_control_page_btn_print_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;

	if(controler_machine_status_val() == MACHINE_STATE_WAIT)
	{
		gui_control_page_btn_print_req(this);
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
	this->job_list_store = gtk_list_store_new(JOB_LIST_N, 
						G_TYPE_STRING, 
						G_TYPE_STRING, 
						G_TYPE_STRING, 
						G_TYPE_INT, 
						G_TYPE_INT, 
						G_TYPE_STRING, 
						G_TYPE_STRING);

	if(this->job_list_store != NULL)
	{
		controler_lock_thread();

		for(int i = 0; i < controler_get_job_queue_size(); i++)
		{
			gtk_list_store_append(this->job_list_store, &iter);
			const char * job_status = NULL;		

			const char * printed_job_name = controler_get_printed_job_name();
			char * job_name = controler_get_job_name(i);
			
			if(((printed_job_name != NULL) && (job_name != NULL)) && (strcmp(printed_job_name, job_name) == 0))
			{
				if(controler_machine_status_val() == MACHINE_STATE_NEXT)
					job_status = multi_lang->print_state_wait_for_data;
				else if(controler_machine_status_val() == MACHINE_STATE_ERROR)
					job_status = multi_lang->print_state_job_error;
				else if(controler_machine_status_val() == MACHINE_STATE_PAUSE)
					job_status = multi_lang->print_state_pause;
				else
					job_status = multi_lang->print_state_print;
			}
			else
			{
				job_status = multi_lang->print_state_wait;
			}

        	       	gtk_list_store_set (this->job_list_store, &iter,
					JOB_STATE, ((job_status == NULL) ? " " : job_status),
					JOB_ORDER_NAME, ((controler_get_job_order_name(i) == NULL) ? " " : controler_get_job_order_name(i)),
					JOB_NOMINAL, ((controler_get_job_nominal_value(i) == NULL) ? " " : controler_get_job_nominal_value(i)),
					JOB_SHEET_NUMBER, controler_get_job_sheet_number(i), 
					JOB_STAMP_NUMBER, controler_get_job_stamp_number(i),
					JOB_Q_NAME, ((controler_get_job_name(i) == NULL) ? " " : controler_get_job_name(i)),
					JOB_DATE_TIME, ((controler_get_job_date_time(i) == NULL) ? " " : controler_get_job_date_time(i)), 
					-1);
		}

		controler_unlock_thread();
		gtk_tree_view_set_model (GTK_TREE_VIEW(this->job_list), GTK_TREE_MODEL(this->job_list_store));
        	g_object_unref(G_OBJECT(this->job_list_store));
		this->job_list_store = NULL;
	}
}


void gui_control_page_load_report_csv_list(gui_control_page * this)
{
	array_list * current_csv_list = controler_get_report_csv_list();

	if(current_csv_list != NULL)
	{
		if(this->gui_base_ref->report_csv_list == NULL)
		{
			this->gui_base_ref->report_csv_list = current_csv_list;
			gui_base_update_report_csv_list(this->job_report_list, this->gui_base_ref->report_csv_list, this->job_report_list_store);
		}
		else
		{
			if(array_list_size(current_csv_list) != array_list_size(this->gui_base_ref->report_csv_list))	
			{			
				array_list_destructor_with_release(&(this->gui_base_ref->report_csv_list), c_string_finalize_v2);
				this->gui_base_ref->report_csv_list = current_csv_list;
				gui_base_update_report_csv_list(this->job_report_list, this->gui_base_ref->report_csv_list, this->job_report_list_store);
			}
			else
			{
				bool list_changed = false;
	
				for(int i = 0; i < array_list_size(current_csv_list); i++)
				{
					for(int j = 0; j < array_list_size(this->gui_base_ref->report_csv_list); j++)
					{
						if(strcmp(c_string_get_char_array(array_list_get(current_csv_list, i)), 
							c_string_get_char_array(array_list_get(this->gui_base_ref->report_csv_list, j))) == 0)
						{
							list_changed = true;
							break;
						}
					}

					if(list_changed == false)
						break;
				}

				if((list_changed == false) && (array_list_size(this->gui_base_ref->report_csv_list) > 0))
				{	
					array_list_destructor_with_release(&this->gui_base_ref->report_csv_list, c_string_finalize_v2);
					this->gui_base_ref->report_csv_list = current_csv_list;
					gui_base_update_report_csv_list(this->job_report_list, this->gui_base_ref->report_csv_list, this->job_report_list_store);
				}
				else
				{
					array_list_destructor_with_release(&current_csv_list, c_string_finalize_v2);
				}
			}
		}
	}
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


void gui_control_page_btn_fake_companion_sheet(GtkWidget * widget, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;

	if(controler_machine_status_val() == MACHINE_STATE_WAIT)
		gui_control_page_btn_print_req(this);
	
	controler_finish_job_and_print_companion();
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
                gtk_tree_model_get (model, &iter, REP_ORDER_NAME, &csv_name, -1);
		for(int i=0; i< strlen(csv_name); i++)
		{
			if(csv_name[i]=='/')
				csv_name[i] = '_';
		}

		sprintf(cmd, "nohup $(cat %s/%s.csv | %s) > /dev/null 2>&1&", controler_get_job_report_hotfolder_path(), csv_name, DEFAULT_TEXT_EDITOR);
		system(cmd);
                g_free (csv_name);
        }	
}

gui_csv_manage * gui_csv_manage_new(gui_base * gui_base_ref)
{
	gui_csv_manage * this = (gui_csv_manage*) malloc(sizeof(gui_csv_manage));

	this->gui_base_ref = gui_base_ref;
	double width = gui_base_ref->work_area_geometry.width;
	double height = gui_base_ref->work_area_geometry.height;

	this->btn_return = gtk_button_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_return), 150, 35);

	this->spin_year_to = gtk_spin_button_new_with_range(2018,2100,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_year_to), 100, 35);

	this->spin_month_to = gtk_spin_button_new_with_range(1,12,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_month_to), 100, 35);

	this->spin_day_to = gtk_spin_button_new_with_range(1,31,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_day_to), 100, 35);

	this->spin_year_from = gtk_spin_button_new_with_range(2018,2100,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_year_to), 100, 35);

	this->spin_month_from = gtk_spin_button_new_with_range(1,12,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_month_to), 100, 35);

	this->spin_day_from = gtk_spin_button_new_with_range(1,31,1);
	gtk_widget_set_size_request(GTK_WIDGET(this->spin_day_to), 100, 35);


	this->separator_from = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_size_request(GTK_WIDGET(this->separator_from), 400, 2);

	this->separator_to = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_size_request(GTK_WIDGET(this->separator_to), 400, 2);

	this->lbl_from = gtk_label_new(NULL);
	this->lbl_to = gtk_label_new(NULL);


	this->list_report_csv = gtk_tree_view_new();
	this->job_list_store = NULL;

	this->scroll_report_list_csv = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(this->scroll_report_list_csv), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(GTK_WIDGET(this->scroll_report_list_csv), width/2, height/2-185);
	gtk_container_add(GTK_CONTAINER(this->scroll_report_list_csv), this->list_report_csv);

	this->btn_export = gtk_button_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_export), 150, 35);

	this->lbl_year_to = gtk_label_new(NULL);
	this->lbl_month_to = gtk_label_new(NULL);
	this->lbl_day_to = gtk_label_new(NULL);

	this->lbl_year_from = gtk_label_new(NULL);
	this->lbl_month_from = gtk_label_new(NULL);
	this->lbl_day_from = gtk_label_new(NULL);

	this->page = gtk_fixed_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->page), width, height);

	gtk_fixed_put(GTK_FIXED(this->page), this->btn_return, width/4, height/2+(height/2-185)/2+30);
	gtk_fixed_put(GTK_FIXED(this->page), this->scroll_report_list_csv, width/4, height/2-(height/2-185)/2);
	gtk_fixed_put(GTK_FIXED(this->page), this->btn_export, (width*3/4)-150, height/2+(height/2-185)/2+30);
	gtk_fixed_put(GTK_FIXED(this->page), this->spin_year_to, (width*3/4)-115, height/2-(height/2-185)/2-50);
	gtk_fixed_put(GTK_FIXED(this->page), this->spin_month_to, (width*3/4)-265, height/2-(height/2-185)/2-50);
	gtk_fixed_put(GTK_FIXED(this->page), this->spin_day_to, (width*3/4)-415, height/2-(height/2-185)/2-50);

	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_year_to, (width*3/4)-115, height/2-(height/2-185)/2-70);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_month_to, (width*3/4)-265, height/2-(height/2-185)/2-70);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_day_to, (width*3/4)-415, height/2-(height/2-185)/2-70);


	gtk_fixed_put(GTK_FIXED(this->page), this->spin_year_from, (width/4)+300, height/2-(height/2-185)/2-50);
	gtk_fixed_put(GTK_FIXED(this->page), this->spin_month_from, (width/4)+150, height/2-(height/2-185)/2-50);
	gtk_fixed_put(GTK_FIXED(this->page), this->spin_day_from, (width/4), height/2-(height/2-185)/2-50);

	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_year_from, (width/4)+300, height/2-(height/2-185)/2-70);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_month_from, (width/4)+150, height/2-(height/2-185)/2-70);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_day_from, (width/4), height/2-(height/2-185)/2-70);

	gtk_fixed_put(GTK_FIXED(this->page), this->separator_from, width/4, height/2-(height/2-185)/2-80);
	gtk_fixed_put(GTK_FIXED(this->page), this->separator_to, (width*3/4)-400, height/2-(height/2-185)/2-80);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_to, (width*3/4)-400, height/2-(height/2-185)/2-100);
	gtk_fixed_put(GTK_FIXED(this->page), this->lbl_from, (width/4), height/2-(height/2-185)/2-100);

	return this;
}


int gui_csv_manage_check_day(int day, int year, int month)
{
	if(month == 2)
	{
		if((year % 4) == 0)
		{
			if(day > 29)
				day = 29;
		}
		else
		{
			if(day > 28)
				day = 28;
		}
	
	}
	else if(month == 4 || month == 6 || month == 9 || month == 10 || month == 11)
	{
		if(day > 30)
			day = 30;
	}	

	return day;
}


void gui_csv_manage_split_csv_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_set_name (GTK_FILE_FILTER(filter), "Report");
	gtk_file_filter_add_pattern(GTK_FILE_FILTER(filter), "*.csv");
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_main_fs_title, GTK_FILE_CHOOSER_ACTION_SAVE, filter, "split_report.csv");

	if(path != NULL)
	{
		printf("%s\n", path);
		
		array_list * filtered_csv_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->csv_manage_page->spin_year_to)));
		controler_split_csv(filtered_csv_list, path);

		array_list_destructor_with_release(&filtered_csv_list, c_string_finalize_v2);
		g_free(path);
	}
}

void gui_csv_manage_set_day_from (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int day = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (spin_button, 
				(gdouble) gui_csv_manage_check_day(day, 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)), 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from))));
	
	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										day,
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);

		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}


void gui_csv_manage_set_year_from (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int year = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(this->spin_day_from), 
				(gdouble) gui_csv_manage_check_day(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)), 
					year, 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from))));

	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from)),
										year,
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);
	
		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}

void gui_csv_manage_set_month_from (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int month = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(this->spin_day_from), 
				(gdouble) gui_csv_manage_check_day(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)), 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)), 
					month));


	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)),
										month,
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);
	
		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}



void gui_csv_manage_set_day_to (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int day = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (spin_button, 
				(gdouble) gui_csv_manage_check_day(day, 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)), 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to))));

	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)),
										day,
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);

		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}


void gui_csv_manage_set_year_to (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int year = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(this->spin_day_to), 
				(gdouble) gui_csv_manage_check_day(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)), 
					year, 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to))));



	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_to)),
										year);
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);
	
		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}

void gui_csv_manage_set_month_to (GtkSpinButton *spin_button, gpointer param)
{
	gui_csv_manage * this = (gui_csv_manage*) param;
	int month = gtk_spin_button_get_value_as_int (spin_button);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(this->spin_day_to), 
				(gdouble) gui_csv_manage_check_day(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)), 
					gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)), 
					month));

	array_list * filtered_job_list =  controler_csv_manage_filter_report_csv(this->gui_base_ref->report_csv_list, 
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_month_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_from)),
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_day_to)),
										month,
										gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->spin_year_to)));
	if(filtered_job_list != NULL)
	{
		gui_base_update_report_csv_list(this->list_report_csv, 
						filtered_job_list, 
						this->job_list_store);
	
		array_list_destructor_with_release(&filtered_job_list, c_string_finalize_v2);
	}
}


void gui_csv_manage_language(gui_csv_manage * this, lang * multi_lang)
{

	gtk_label_set_text(GTK_LABEL(this->lbl_to), multi_lang->to);
	gtk_label_set_text(GTK_LABEL(this->lbl_from), multi_lang->from);

	gtk_label_set_text(GTK_LABEL(this->lbl_year_to), multi_lang->filter_year);
	gtk_label_set_text(GTK_LABEL(this->lbl_month_to), multi_lang->filter_month);
	gtk_label_set_text(GTK_LABEL(this->lbl_day_to), multi_lang->filter_day);

	gtk_button_set_label(GTK_BUTTON(this->btn_export), multi_lang->csv_manage_export);
	gtk_button_set_label(GTK_BUTTON(this->btn_return), multi_lang->csv_manage_return);


	gtk_label_set_text(GTK_LABEL(this->lbl_year_from), multi_lang->filter_year);
	gtk_label_set_text(GTK_LABEL(this->lbl_month_from), multi_lang->filter_month);
	gtk_label_set_text(GTK_LABEL(this->lbl_day_from), multi_lang->filter_day);

	gui_control_page_delete_columns(this->list_report_csv);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_state, REP_FINISH_STATE)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_order_name, REP_ORDER_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_rejected_sheets, REP_REJECTED_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_sheet_number, REP_SHEET_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_stamp_number, REP_STAMP_NUMBER)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->list_report_csv), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang->rep_csv_date_time, REP_DATE_TIME)));
}

void gui_csv_manage_return_to_control_page(GtkWidget* widget, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name(GTK_STACK(this->page_stack), "control_page");

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
		else if(i == S_BTN_MACHINE_OVERVIEW)
			settings_button_set_text(this->btn[i], multi_lang->set_btn_machine_overview);
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


void gui_settings_page_btn_got_to_machine_overview_callback(GtkWidget * widget, GdkEventButton* event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name(GTK_STACK(this->page_stack), "machine_overview");
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
	gtk_widget_add_events (this->io_vision, GDK_BUTTON_PRESS_MASK);

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, gui_base_ref->work_area_geometry.width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);

	this->btn_manual = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->btn_manual), 100, 35);

	gtk_fixed_put(GTK_FIXED(this->page), this->io_vision, 0, 0);
	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
		gui_base_ref->work_area_geometry.width/4, 250);
	gtk_fixed_put(GTK_FIXED(this->page), this->btn_manual, gui_base_ref->work_area_geometry.width/2-50, gui_base_ref->work_area_geometry.height-150);

	return this;
}

void gui_settings_page_return_callback(GtkButton *button, GdkEventButton * event, gpointer param)
{
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "settings_page");
}

uint8_t gui_io_vision_in_pos(double x, double y, double x_click, double y_click, double radius)
{
	return ((((y_click-y) <= radius) && ((y_click - y) >=0) && ((x_click -x) >= 0) && ((x_click-x) <= radius)) ? 1 : 0);
}


void gui_io_vision_set_manual(GtkSwitch *widget, gboolean state, gpointer param)
{
	controler_set_manual_mode(state);
}

gboolean gui_io_vision_outputs_control(GtkWidget *widget, GdkEventButton *event, gpointer param)
{
	gui_io_vision_page * this = (gui_io_vision_page*) param;

	if(controler_get_manual_mode_state() == true)
	{
		double width = this->gui_base_ref->work_area_geometry.width/20.0;
		double height = this->gui_base_ref->work_area_geometry.height/1.7;

		double a1_x_pos = this->gui_base_ref->work_area_geometry.width/4.0;
		double y_pos = this->gui_base_ref->work_area_geometry.height/3.0+10;

		double a2_x_pos = this->gui_base_ref->work_area_geometry.width/4.0*3 - this->gui_base_ref->work_area_geometry.width/20.0;
	
		for(int i = 0; i < A1_OUT_SIZE; i++)
		{
			if(i < 8)
			{
				if(gui_io_vision_in_pos((a1_x_pos+width/2.0)-20.0, y_pos + (i+11)*(((height-100)/18))+width/20, event->x, event->y, width/5) > 0)
					controler_set_a1_output(i*2, ((controler_get_card_output(IO_CARD_A1, i*2) > 0) ? 0 : 1));
			}
			else
			{
				if(gui_io_vision_in_pos((a1_x_pos+width/2.0)+20.0, y_pos + (i+11-8)*(((height-100)/18))+width/20, event->x, event->y, width/5) > 0)
					controler_set_a1_output((i-8)*2+1, ((controler_get_card_output(IO_CARD_A1, (i-8)*2+1) > 0) ? 0 : 1));
			}	
		}

		for(int i = 0; i < A2_OUT_SIZE; i++)
		{
			if(i < 8)
			{
				if(gui_io_vision_in_pos((a2_x_pos+width/2.0)-20.0, y_pos + (i+11)*(((height-100)/18))+width/20, event->x, event->y, width/5) > 0)
					controler_set_a2_output(i*2, ((controler_get_card_output(IO_CARD_A2, i*2) > 0) ? 0 : 1));
			}
			else
			{
				if(gui_io_vision_in_pos((a2_x_pos+width/2.0)+20.0, y_pos + (i+11-8)*(((height-100)/18))+width/20, event->x, event->y, width/5) > 0)
					controler_set_a2_output((i-8)*2+1, ((controler_get_card_output(IO_CARD_A2, (i-8)*2+1) > 0) ? 0 : 1));
			}	
		}
	}

	return FALSE;
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
	gui_io_vision_draw_points(this, cr, a2_x_pos, y_pos, width, height, controler_get_a2_input_labels(), controler_get_a2_output_labels(), IO_CARD_A2);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_main_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_feedback_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);


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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_q_backup_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_pci_in_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_pci_out_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_gis_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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
	char * path = gui_def_file_chooser(this, (char*) multi_lang->hot_report_csv_fs_title, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);

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

			case PAR_FEED_DELAY:
				sprintf(buf, "%d", controler_get_get_feed_delay());
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

	this->fan_intensity_spin = gtk_spin_button_new_with_range(0,100,10);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->fan_intensity_spin), (int) controler_get_fan_intensity());
	gtk_widget_set_size_request(GTK_WIDGET(this->fan_intensity_spin), 150, 35);

	this->fan_control_switch = gtk_switch_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->fan_control_switch), 100, 35);

	this->fan_intensity_lbl = gtk_label_new(NULL);
	this->fan_control_lbl = gtk_label_new(NULL);



	gtk_fixed_put(GTK_FIXED(this->page), this->sheet_source_lbl, width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->sheet_source_combo, width/4*3-300, 250+80+(50*i));
	i++;
	gtk_fixed_put(GTK_FIXED(this->page), this->print_confirm_lbl, width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->print_confirm_switch, width/4*3-300, 250+80+(50*i));
	i++;

	gtk_fixed_put(GTK_FIXED(this->page), this->fan_intensity_lbl ,width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->fan_intensity_spin, width/4*3-300, 250+80+(50*i));
	i++;

	gtk_fixed_put(GTK_FIXED(this->page), this->fan_control_lbl ,width/4, 250+80+(50*i));
	gtk_fixed_put(GTK_FIXED(this->page), this->fan_control_switch, width/4*3-300, 250+80+(50*i));
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
			case PAR_FEED_DELAY:
				gtk_label_set_text(GTK_LABEL(this->par_lbl[i]), multi_lang->par_feed_delay_lbl);
			break;
		}
	}

	gtk_label_set_text(GTK_LABEL(this->fan_control_lbl), multi_lang->fan_control_lbl);
	gtk_label_set_text(GTK_LABEL(this->fan_intensity_lbl), multi_lang->fan_intensity_lbl);

	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(this->sheet_source_combo));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang->par_sheet_source_main);
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang->par_sheet_source_companion);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(this->sheet_source_combo), controler_get_sheet_source());
	
	gtk_label_set_text(GTK_LABEL(this->sheet_source_lbl), multi_lang->par_sheet_source_lbl);
	gtk_label_set_text(GTK_LABEL(this->print_confirm_lbl), multi_lang->par_print_confirm_lbl);

}

void gui_print_params_set_fan_intensity(GtkSpinButton * spin_button, gpointer param)
{
	controler_set_fan_intensity((uint8_t) gtk_spin_button_get_value_as_int(spin_button));
}

gboolean gui_print_params_set_fan_activity_callback (GtkSwitch *widget, gboolean state, gpointer param)
{
	controler_set_fan_activity(((state == true) ? 1 : 0));
	return TRUE;
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


void gui_print_params_set_feed_delay(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	int delay = 0;
	char num_str[32];

	sprintf(num_str, "%s", gtk_entry_get_text(GTK_ENTRY(widget)));

	int exp = 0;
	uint8_t err = 0;

	while(num_str[exp] != 0)
	{
		if((isdigit(num_str[exp])))
		{
			delay = delay * 10 + (num_str[exp] - '0');
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
		controler_set_feed_delay(delay);
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



gui_machine_overview * gui_machine_overview_new(gui_base * gui_base_ref)
{
	gui_machine_overview * this = (gui_machine_overview *) malloc(sizeof(gui_machine_overview));

	this->gui_base_ref = gui_base_ref;

	double height = gui_base_ref->work_area_geometry.height;
	double width = gui_base_ref->work_area_geometry.width;

	this->page = gtk_fixed_new();
	this->machine_img = gtk_image_new_from_pixbuf(gui_base_scale_icon(gui_base_load_icon(MACHINE_ICON_PATH), 1, width/2));

	this->info_box = gtk_drawing_area_new();	
	gtk_widget_set_size_request(GTK_WIDGET(this->info_box), width, height);

	this->btn_return = settings_button_new(settings_btn_fg_s, fg, bg, fg, width/2, 50);
	settings_button_set_font_size(this->btn_return, 18);
	settings_button_set_selected(this->btn_return, 1);

	gtk_fixed_put(GTK_FIXED(this->page), this->machine_img, width/4, 400);
	gtk_fixed_put(GTK_FIXED(this->page), this->info_box, 0,0);
	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), width/4, 250);

	return this;
}


void gui_machine_overview_language(gui_machine_overview * this, lang * multi_lang)
{

}

gboolean gui_machine_overview_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param)
{
	gui_machine_overview * this = (gui_machine_overview*) param;
	lang * multi_lang = multi_lang_get(controler_get_interface_language());

	double height = this->gui_base_ref->work_area_geometry.height;
	double width = this->gui_base_ref->work_area_geometry.width;

	int left_horizontal_offset = 40;	
	char temp_buff[32];
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 19);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+80);
	cairo_show_text(cr, multi_lang->statistics_mon);

	

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+230);
	cairo_show_text(cr, multi_lang->statistics_tue);


	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+380);
	cairo_show_text(cr, multi_lang->statistics_wed);


	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+530);
	cairo_show_text(cr, multi_lang->statistics_thu);


	cairo_move_to(cr,width/4*3+left_horizontal_offset , (5*height/24)+80);
	cairo_show_text(cr, multi_lang->statistics_fri);


	cairo_move_to(cr,width/4*3+left_horizontal_offset , (5*height/24)+230);
	cairo_show_text(cr, multi_lang->statistics_sat);


	cairo_move_to(cr,width/4*3+left_horizontal_offset , (5*height/24)+380);
	cairo_show_text(cr, multi_lang->statistics_sun);


	/* total statistics */
	sprintf(temp_buff, "%ld",  controler_get_statistics_total_rejected_sheets());
	cairo_move_to(cr, width/4+150, height/1.35);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%f%%", controler_get_statistics_error_rate());
	cairo_move_to(cr, width/4+150, height/1.3);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%ld", controler_get_statistics_total_stacked_sheets());
	cairo_move_to(cr, width/4+40, height/1.35);
	cairo_show_text(cr, temp_buff);


	sprintf(temp_buff, "%ld", controler_get_statistics_total_feeded_sheets());
	cairo_move_to(cr, width/4*3-70, height/1.35);
	cairo_show_text(cr, temp_buff);

	for(int i = 0; i<7; i++)
	{
		sprintf(temp_buff, "%ld", controler_get_statistics_on_day_total_feeded_sheets((i+1)%7));
		cairo_text_extents_t ext_feeded_counter;
		cairo_text_extents(cr, temp_buff, &ext_feeded_counter);

		if(i >= 4)
			cairo_move_to(cr,width-40 - ext_feeded_counter.width , (5*height/24)+110 + 150*(i-4));
		else
			cairo_move_to(cr,width/4-40 - ext_feeded_counter.width , (5*height/24)+110 + (150*i));

		cairo_show_text(cr, temp_buff);

		sprintf(temp_buff, "%ld", controler_get_statistics_on_day_total_stacked_sheets((i+1)%7));
		cairo_text_extents_t ext_stacked_counter;
		cairo_text_extents(cr, temp_buff, &ext_stacked_counter);
		
		if(i >= 4)
			cairo_move_to(cr,width-40 - ext_stacked_counter.width , (5*height/24)+140 + 150*(i-4));
		else
			cairo_move_to(cr,width/4-40 - ext_stacked_counter.width , (5*height/24)+140 + (150*i));
	
		cairo_show_text(cr, temp_buff);

		sprintf(temp_buff, "%ld", controler_get_statistics_on_day_total_rejected_sheets((i+1)%7));
		cairo_text_extents_t ext_rejected_counter;
		cairo_text_extents(cr, temp_buff, &ext_rejected_counter);

		if(i >= 4)
			cairo_move_to(cr,width-40 - ext_rejected_counter.width , (5*height/24)+170+150*(i-4));
		else
			cairo_move_to(cr,width/4-40 - ext_rejected_counter.width , (5*height/24)+170+(150*i));

		cairo_show_text(cr, temp_buff);

		sprintf(temp_buff, "%f%%", controler_get_statistics_on_day_error_rate((i+1)%7));
		cairo_text_extents_t ext_error_rate;
		cairo_text_extents(cr, temp_buff, &ext_error_rate);

		if(i >= 4)
			cairo_move_to(cr,width-40 - ext_error_rate.width , (5*height/24)+200 +150*(i-4));
		else
			cairo_move_to(cr,width/4-40 - ext_error_rate.width , (5*height/24)+200 +(150*i));

		cairo_show_text(cr, temp_buff);
	}


	cairo_fill(cr);
	
	cairo_set_font_size(cr, 15);

	/* monday */
	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+110);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+140);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+170);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+200);
	cairo_show_text(cr, multi_lang->statistics_error_rate);


	/* tuesday */
	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+260);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+290);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+320);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+350);
	cairo_show_text(cr, multi_lang->statistics_error_rate);

	/* wednesday */
	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+410);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+440);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+470);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+500);
	cairo_show_text(cr, multi_lang->statistics_error_rate);

	/* thursday */
	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+560);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+590);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+620);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,left_horizontal_offset , (5*height/24)+650);
	cairo_show_text(cr, multi_lang->statistics_error_rate);


	/* friday */
	cairo_move_to(cr, width/4*3 + left_horizontal_offset , (5*height/24)+110);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+140);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+170);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+200);
	cairo_show_text(cr, multi_lang->statistics_error_rate);

	/* saturday */
	cairo_move_to(cr, width/4*3 + left_horizontal_offset , (5*height/24)+260);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+290);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+320);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+350);
	cairo_show_text(cr, multi_lang->statistics_error_rate);

	/* sunday */
	cairo_move_to(cr, width/4*3 + left_horizontal_offset , (5*height/24)+410);
	cairo_show_text(cr, multi_lang->statistics_feeded_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+440);
	cairo_show_text(cr, multi_lang->statistics_stacked_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+470);
	cairo_show_text(cr, multi_lang->statistics_rejected_sheets);

	cairo_move_to(cr,width/4*3 + left_horizontal_offset , (5*height/24)+500);
	cairo_show_text(cr, multi_lang->statistics_error_rate);

	cairo_fill(cr);
/*
    cz_lang->statistics_mon = "Statistika tisku Pondělí:";
    cz_lang->statistics_tue = "Statistika tisku Úterý:";
    cz_lang->statistics_wed = "Statistika tisku Středa:";
    cz_lang->statistics_thu = "Statistika tisku čtvrtek:";	
    cz_lang->statistics_fri = "Statistika tisku Pátek:";
    cz_lang->statistics_sat = "Statistika tisku Sobota:";
    cz_lang->statistics_sun = "Statistika tisku Neděle:";
    cz_lang->statistics_feeded_sheets = "Naložené archy:";
    cz_lang->statistics_stacked_sheets = "Vyložené archy:";
    cz_lang->statistics_rejected_sheets = "Vyhozené archy";
    cz_lang->statistics_error_rate = "Chybovost tisku:";

*/

/*
	sprintf(temp_buff, "%f%%", controler_get_statistics_error_rate(-1));
	cairo_text_extents_t ext_main_counter;
	cairo_text_extents(cr, temp_buff, &ext_main_counter);
	cairo_move_to(cr, width/4-40 - ext_main_counter.width, 400);
	cairo_show_text(cr, temp_buff);

	cairo_fill(cr);
*/
	/*  */
	if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && ((controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED) || (controler_get_feeder_status() == MACHINE_FN_FEEDING))) 
	{
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && (controler_get_feeder_status() == MACHINE_FN_FEEDER_READY))
	{
		if(this->gui_base_ref->blink == true)
			cairo_set_source_rgb(cr,1,0.5,0);
		else
			cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) == 0) && (controler_get_feeder_status() == MACHINE_FN_FEEDER_READY || (controler_get_feeder_status() == MACHINE_FN_READY_TO_FEED)))
	{
		cairo_set_source_rgb(cr,1,0.5,0);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}

	cairo_arc(cr, width/4*3-50, height/1.8, 15, 0, 2*M_PI);
	cairo_fill(cr);


	if(((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && 
		(controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK || controler_get_stacker_status() == MACHINE_SN_STACKING))) 
	{
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) > 0) && (controler_get_stacker_status() == MACHINE_SN_STACKER_READY))
	{
		if(this->gui_base_ref->blink == true)
			cairo_set_source_rgb(cr,1,0.5,0);
		else
			cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}
	else if((controler_get_card_output(IO_CARD_A1, A1_OUT_10_ENA) == 0) && 
		(controler_get_stacker_status() == MACHINE_SN_STACKER_READY || controler_get_stacker_status() == MACHINE_SN_STACKER_READY_TO_STACK))
	{
		cairo_set_source_rgb(cr,1,0.5,0);
	}
	else
	{
		cairo_set_source_rgb(cr,1,0,0);
	}


	cairo_arc(cr, width/4+70, height/1.82, 15,0, 2*M_PI);
	cairo_fill(cr);

	if((controler_get_card_output(IO_CARD_A2, A2_IN_5_camera_trigger) > 0) || 
		(controler_get_card_output(IO_CARD_A1, A1_IN_9_FN_jam) > 0) || 
		(controler_get_card_output(IO_CARD_A2, A2_IN_4_RJ_jam) >0)) 
	{
		cairo_set_source_rgb(cr,1,0,0);
	}
	else
	{
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	}

	cairo_rectangle(cr, width/4+width/16, height/1.9,width/2-width/8,20);
	cairo_fill(cr);

/*
	cairo_text_extents_t ext_gis_status;
	cairo_text_extents(cr, controler_get_gis_status(), &ext_gis_status);
	cairo_move_to(cr, width-(width/4-350)/2 - ext_gis_status.width, 5*height/24+260);
	cairo_show_text(cr, controler_get_gis_status());

    en_lang->g_complet_feeded_sheets = "Total amount of feeded sheets:";
    en_lang->g_complet_stacked_sheets = "Total amount of stacked sheets:";
    en_lang->g_complet_rejected_sheets = "Total amount of rejected sheets:";
    cz_lang->g_error_rate = "Průměrná chybovost tisku:";
*/
	return TRUE;
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

	this->blink = false;
	GdkDisplay* display = gdk_display_get_default(); 
	GdkMonitor* monitor = gdk_display_get_monitor(display, 0); 
	gdk_monitor_get_workarea(monitor, &(this->work_area_geometry));
	this->report_csv_list = NULL;

	return this; 
}


void gui_base_update_report_csv_list(GtkWidget * tree_view, array_list * report_csv_list, GtkListStore * job_report_list_store)
{
	GtkTreeIter iter;
	job_report_list_store = gtk_list_store_new(REPO_ITEM_N, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);

	if(report_csv_list != NULL)
	{
		for(int index = 0; index < array_list_size(report_csv_list); index++)
		{		
			rep_struct * job_report = controler_load_report_information(c_string_get_char_array(array_list_get(report_csv_list, index)));

			if(job_report != NULL)
			{
				gtk_list_store_append(job_report_list_store, &iter);
							
				if(job_report->order_name != NULL)
	               			gtk_list_store_set (job_report_list_store, &iter, REP_ORDER_NAME, job_report->order_name, -1);
				else
	               			gtk_list_store_set (job_report_list_store, &iter, REP_ORDER_NAME, "", -1);

				if(job_report->finish_state != NULL)
		               		gtk_list_store_set (job_report_list_store, &iter, REP_FINISH_STATE, job_report->finish_state, -1);
				else
		               		gtk_list_store_set (job_report_list_store, &iter, REP_FINISH_STATE, "", -1);

	               		gtk_list_store_set (job_report_list_store, &iter, REP_REJECTED_SHEET_NUMBER, job_report->wrong_sheet_number, -1);
	               		gtk_list_store_set (job_report_list_store, &iter, REP_SHEET_NUMBER, job_report->sheet_number, -1);
	        		gtk_list_store_set (job_report_list_store, &iter, REP_STAMP_NUMBER, job_report->stamp_number, -1);

				if(job_report->job_name != NULL)
		               		gtk_list_store_set (job_report_list_store, &iter, REP_JOB_NAME, job_report->job_name, -1);
				else
		               		gtk_list_store_set (job_report_list_store, &iter, REP_JOB_NAME, "", -1);

				if(job_report->date_time != NULL)
					gtk_list_store_set (job_report_list_store, &iter, REP_DATE_TIME, job_report->date_time, -1);
				else
					gtk_list_store_set (job_report_list_store, &iter, REP_DATE_TIME, "", -1);
				
				controler_free_report_information_struct(job_report);
			}
			
		}
	}
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(job_report_list_store));
	g_object_unref(G_OBJECT(job_report_list_store));
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

GdkPixbuf * gui_base_scale_icon(GdkPixbuf * icon, uint8_t dim, double new_size)
{
	GdkPixbuf* scaled_icon = NULL;

	if(icon != NULL)
	{
		if(dim == 0)
		{
			double scale = (((double) new_size)/((double)gdk_pixbuf_get_height(icon))) * ((double)gdk_pixbuf_get_width(icon));
			scaled_icon = gdk_pixbuf_scale_simple (icon, (int) scale, new_size, GDK_INTERP_HYPER);
		}
		else
		{
			double scale = (((double) new_size)/((double)gdk_pixbuf_get_width(icon))) * ((double)gdk_pixbuf_get_height(icon));
			scaled_icon = gdk_pixbuf_scale_simple (icon, (int) new_size, scale, GDK_INTERP_HYPER);
		}
	}

	return scaled_icon;
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

