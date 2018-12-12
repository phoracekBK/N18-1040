/**
* @file template.c
* 
* Template file 
*/

/******************* End of Header *********************************************************
******************** Includes **************************************************************/

/* Library include */

/* Header files include */
#include "config.h"
#include "io_mapping.h"


/* C library  include */
#include <features.h>
#include <libconfig.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <comedilib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>

#include <settings_button.h>
#include <c_log.h>
#include <c_string.h>
#include <array_list.h>
#include <c_freq.h>


/**
** @todo
** create documentation of new code
** do handling of pci camera system - not defined
*/


/******************* End of Includes *********************************************************
******************** Constants definitions ***************************************************/


#define PCI_COMM FALSE


/**
** @ingroup InternalStates
** Macro contains information when was this software build.
*/
#define LAST_BUILD_DATE __TIMESTAMP__

/**
** @ingroup InternalStates
** Macro contains information about version of the software.
*/
#define VERSION "0.7"


#define CFG_GROUP_NETWORK "network"
#define CFG_GROUP_LANGUAGE "language"	
#define CFG_GROUP_PRINT_PARAMS "print_params"
#define CFG_GROUP_HOTFOLDER "hotfolder"

#define CFG_HOT_QUADIENT_MAIN "q_hot_main"
#define CFG_HOT_QUADIENT_FEEDBACK "q_hot_feedback"
#define CFG_HOT_QUADIENT_BACKUP "q_hot_backup"
#define CFG_HOT_GIS "gis_hot"
#define CFG_HOT_PCI_IN "pci_hot_in"
#define CFG_HOT_PCI_OUT "pci_hot_out"
#define CFG_HOT_REP_CSV "rep_hot"

#define CFG_PP_SHEET_FOR_STOP "max_rj_sheet_sq"
#define CFG_PP_MAX_SHEET_IN_STACKER "max_st_sheet"
#define CFG_PP_COMPAION_SHEET_SOURCE "companion_sheet_source"
#define CFG_PP_SHEET_SOURCE_CONFIRMATION "sheet_source_confirmation"
#define CFG_PP_NON_STACKED_SHEET_LIMIT "non_stacked_sheet_limit"
#define CFG_PP_NON_REVIDED_SHEET_LIMIT "non_revided_sheet_limit"
#define CFG_PP_GR_MACHINE_MODE "gr_machine_mode"

#define CFG_LANG_INDEX "lang_index"

#define CFG_NETWORK_GIS_TCP_PORT "gis_tcp_port"
#define CFG_NETWORK_GIS_IP_ADDRESS "gis_ip_address"
#define CFG_NETWORK_QUADIENT_PC_TCP_PORT "quadient_tcp_port"
#define CFG_NETWORK_QUADIENT_PC_IP_ADDRESS "quadient_ip_address"
#define CFG_NETWORK_PCI_PC_TCP_PORT "pci_pc_tcp_port"
#define CFG_NETWORK_PCI_PC_IP_ADDRESS "pci_pc_ip_address"
#define CFG_NETWORK_IIJ_PC_TCP_PORT "iij_pc_tcp_port"
#define CFG_NETWORK_IIJ_PC_IP_ADDRESS "iij_pc_ip_address"

/**
** @ingroup Cli
** Macro defines the size of input buffer created at the start of the application, used for 
** loading of input string from command line.
*/
#define CLI_IO_BUFFER_SIZE 1024


/**
** @ingroup Cli
** Macro defines the command for exit application from command line interface
*/
#define CLI_CMD_EXIT "exit"

/**
** @ingroup Cli
** Macro defines the command for clear the screen of command line interface
*/
#define CLI_CMD_CLEAR "clear"

#define CLI_CMD_JOB "job"

#define CLI_CMD_JOBLIST "list"

#define CLI_CMD_JOBFILE "file"

#define CLI_CMD_JOBFLAG "flag"

#define CLI_CMD_JOBORDER "order"

#define CLI_CMD_PRINT "print"

#define CLI_CMD_PRINT_START "start"

#define CLI_CMD_PRINT_CANCEL "cancel"

#define CLI_CMD_PRINT_STATUS "status"

#define CLI_CMD_PRINT_PAUSE "pause"

#define CLI_CMD_PRINT_CONTINUE "continue"

#define CLI_CMD_PRINT_RESET "reset"

#define CLI_CMD_PRINTER "printer"

#define CLI_CMD_PRINTER_STATUS "status"

#define CLI_CMD_CAMERA "camera"

#define CLI_CMD_NET_SET "set"

#define CLI_CMD_NET_IP "ip"

#define CLI_CMD_NET_PORT "port"

#define CLI_CMD_NET_GET "get"

#define CLI_CMD_NET_CONNECT "connect"

#define CLI_CMD_NET_DISCONNECT "disconnect"

#define CLI_CMD_NET_CONNECTED "connected"

#define CLI_CMD_HOTFOLDER "hotfolder"

#define CLI_CMD_HOTFOLDER_SET "set"

#define CLI_CMD_HOTFOLDER_GET "get"

#define CLI_CMD_HOTFOLDER_JOB "job"

#define CLI_CMD_HOTFOLDER_BACKUP "backup"

#define CLI_CMD_HOTFOLDER_FEEDBACK "feedback"

#define CLI_CMD_HOTFOLDER_GIS "gis"

#define CLI_CMD_HOTFOLDER_PCI "pci"

#define CLI_CMD_HOTFOLDER_PCI_IN "in"

#define CLI_CMD_HOTFOLDER_PCI_OUT "out"


/**
** @ingroup Cli
** Macro defines the command for enter the pin for unlock core layer functions
*/
#define CLI_CMD_LOGIN "login"

/**
** @ingroup Cli
** Macro defines the command for lock the core layer functons
*/
#define CLI_CMD_LOGOUT "logout"

/**
** @ingroup Communication
** Macro defines the size of input and output buffer created at the start of the application, used for
** loading of output string sent to the server and string receaved as the respond. 
**/
#define COMM_TCP_IO_BUFFER_SIZE 1024

/**
** @ingroup InternalStates
** Macro defines success status of connection try.
*/
#define STATUS_SUCCESS 0

/**
** @ingroup InternalStates
** Macro defines socket creation error status of connection try.
*/
#define STATUS_SOCKET_ERROR 1

/**
** @ingroup InternalStates
** Macro defines ip address conversion error status of connection try
*/
#define STATUS_IP_ADDRESS_ERROR 2

/**
** @ingroup InternalStates
** Macro defines socket connection error status of connection try
*/
#define STATUS_CONNECTION_ERROR 3

/**
** @ingroup InternalStates
** Macro defines error, try set network parametr when client is stil connected.
*/
#define STATUS_CLIENT_CONNECTED 4

/**
** @ingroup InternalStates
** Macro defines error tcp port value out of range: <0:65535>
*/
#define STATUS_PORT_OUT_OF_RANGE 5

#define STATUS_CLIENT_DISCONNECTED 6

#define STATUS_COMMUNICATION_ERROR 7


/**
** @ingroup InternalStates
** Macro defines state of the called operation from core layer interface, access denied to the interface function
*/
#define STATUS_GENERAL_ERROR 255








#define MACHINE_FN_OFF 0

#define MACHINE_FN_E_STOP 1

#define MACHINE_FN_FEEDER_MULTIFUNCTION 2

#define MACHINE_FN_FEEDER_READY 3

#define MACHINE_FN_READY_TO_FEED 4

#define MACHINE_FN_DOUBLE_SHEET 5

#define MACHINE_FN_MISSING_SHEET 6

#define MACHINE_FN_FEEDING 7


#define MACHINE_SN_OFF 0

#define MACHINE_SN_STACKER_MULTIFUNCTION 1

#define MACHINE_SN_BOTTOM 2

#define MACHINE_SN_STACKER_READY 3 

#define MACHINE_SN_STACKER_READY_TO_STACK 4

#define MACHINE_SN_JAM_CONVEYOR 5

#define MACHINE_SN_JAM_PILE 6

#define MACHINE_SN_STACKING 7




#define MACHINE_STATE_WAIT 0

#define MACHINE_STATE_PREPARE 1

#define MACHINE_STATE_PRINT_MAIN 2

#define MACHINE_STATE_ERROR 3

#define MACHINE_STATE_SAVE_Q_CSV 4

#define MACHINE_STATE_CLEAR_HOT_FOLDER 5
 
#define MACHINE_STATE_CLEAR_TO_FINISH 6

#define MACHINE_STATE_NEXT 7

#define MACHINE_STATE_PRINT_FINISH 8

#define MACHINE_STATE_JOB_FINISH 9

#define MACHINE_STATE_PAUSE 10

#define MACHINE_STATE_READ_CSV_LINE 11

#define MACHINE_STATE_PRINT_COMPANION 12

#define MACHINE_STATE_PRINT_BREAK 13

#define MACHINE_STATE_READY_TO_START 14

#define MACHINE_STATE_WAIT_FOR_PRINT_FINISH 15

#define MACHINE_STATE_WAIT_FOR_CONFIRMATION 16



#define MACHINE_ERR_NO_ERROR 0
#define MACHINE_ERR_CANT_SAVE_F_CSV 1 
#define MACHINE_ERR_ANALYZE_CAMERA_CSV 2
#define MACHINE_ERR_CANT_LOAD_BKCORE_CSV 3
#define MACHINE_ERR_CANT_COPY_PDF_TO_GIS 4
#define MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA 5
#define MACHINE_ERR_JOB_ORDER_MISMATCH 6
#define MACHINE_ERR_STACKER_JAM_PILE 7
#define MACHINE_ERR_STACKER_JAM_CONVEYOR 8
#define MACHINE_ERR_STACKER_BOTTOM 9
#define MACHINE_ERR_STACKER_MULTIFUNCTION 10
#define MACHINE_ERR_STACKER_OFF 11
#define MACHINE_ERR_FEEDER_E_STOP 12
#define MACHINE_ERR_FEEDER_OFF 13
#define MACHINE_ERR_FEEDER_MULTIFUNCTION 14
#define MACHINE_ERR_FEEDER_SHEET_MISSING 15
#define MACHINE_ERR_FEEDER_DOUBLE_SHEET 16
#define MACHINE_ERR_E_STOP 17
#define MACHINE_ERR_REJECT_BIN_JAM 18
#define MACHINE_ERR_FEEDER_JAM 19
#define MACHINE_ERR_TI 20
#define MACHINE_ERR_TA 21
#define MACHINE_ERR_COUNTERS_MISMATCH 22
#define MACHINE_ERR_LOW_PRINT_QUALITY 23
#define MACHINE_ERR_GIS_DISCONNECTED 24
#define MACHINE_ERR_CSV_SHEET_NUM 25
#define MACHINE_ERR_SHEET_FEEDER_REDIRECTION 26
#define MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING 27
#define MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING 28
#define MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING 29
#define MACHINE_ERR_UNKNOWN_ERROR 255


enum _gremser_print_mode_
{
	GR_SETUP=0,
	GR_PRINT,
	GR_INSPECTION,
	GR_PRINT_INSPECTION,

	GR_MODE_N
};


enum _gui_job_list_items_
{
	JOB_STATE= 0,
	JOB_NAME,
	JOB_PDF_NAME,
	JOB_CAMERA_CSV_NAME,

	JOB_BKCORE_CSV_NAME,

	JOB_LIST_N
};

enum _gui_job_report_list_item_
{
	REP_CSV_NAME = 0,

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

enum _sheet_source_
{
	SSOURCE_MAIN=0,
	SSOURCE_COMPANION,

	SSOURCE_N
};


/******************* End of sections with constants definitions ******************************
******************** Structures declaration **************************************************/
struct _q_job_;
typedef struct _q_job_ q_job;

struct _comm_tcp_;
typedef struct _comm_tcp_ comm_tcp;

struct _con_monitor_;
typedef struct _con_monitor_ con_monitor;

struct _core_;
typedef struct _core_ core;

struct _job_info_;
typedef struct _job_info_ job_info;

struct _info_struct_;
typedef struct _info_struct_ info_struct;

struct _io_card_;
typedef struct _io_card_ io_card;

struct _gui_;
typedef struct _gui_ gui;

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

struct _gui_info_window_;
typedef struct _gui_info_window_ gui_info_window;

struct _gui_io_vision_page_;
typedef struct _gui_io_vision_page_ gui_io_vision_page;



/******************* End of structures declaration *******************************************
******************** Structures definitions **************************************************/

struct _core_
{
	config_t cfg_ref;

	c_log * log;

	pthread_t machine_handler_thread;
	pthread_t iij_gis_state_reading_thread;

	comm_tcp * iij_tcp_ref;
	comm_tcp * pci_tcp_ref;
	
	/* connection testing, communication with network_responder utility */
	comm_tcp * pci_connection;
	comm_tcp * iij_connection;
	comm_tcp * quadient_connection;
	
	pthread_t pci_conn_therad;
	pthread_t iij_conn_thread;
	pthread_t quadient_conn_thread;

	io_card * io_card_ref;	

	job_info * info;
	
	c_freq * ta_freq;
	c_freq * ti_freq;

	/* machine handler internal state */
	int machine_state;
	int machine_state_pre;
	c_string * printed_job_name;
	int32_t printed_job_index;
	int error_code;

	uint8_t feed_sheet;

	/* control signals to machine handler */
	bool machine_pause_req;
	bool machine_print_req;
	bool machine_error_reset_req;
	bool machine_cancel_req;


	uint32_t rejected_sheet_for_stop;
	uint32_t max_stacked_sheets;

	uint8_t companion_sheet_source;
	bool sheet_source_confirmation;
	bool print_confirmation_req;

	/* printing counters */
	uint32_t feeded_main_sheet_counter;
	uint32_t feeded_companion_sheet_counter;
	uint32_t stacked_sheet_counter;
	uint32_t rejected_sheet_counter;
	uint32_t rejected_sheet_seq_counter;
	uint32_t tab_insert_counter;
	uint32_t inspected_sheet_counter;

	int32_t non_stacked_upper_limit;
	int32_t non_revided_upper_limit;
	
	uint8_t machine_mode;
	uint8_t machine_mode_pre;

	c_string * print_controller_status;

	uint8_t sn_trig;					/* stacker trigger */
	uint8_t camera_trig;					/* camera trigger */
	uint8_t rj_trig;					/* reject counter trigger */
	uint8_t ti_trig;					/* tab insert trigger */

	/* hot folders address */
	c_string * q_hotfolder_main_path;
	c_string * q_hotfolder_backup_path;
	c_string * q_hotfolder_feedback_path;
	c_string * pci_hotfolder_in_path;
	c_string * pci_hotfolder_out_path;
	c_string * gis_hotfolder_path;
	c_string * job_log_path;

	array_list * job_list;
	array_list * job_list_pre;
	uint8_t job_list_changed;
	pthread_mutex_t mutex;
	c_string * feedback_csv;

	char * bkcore_csv_content;
	int32_t bkcore_csv_size;

	int32_t bkcore_csv_pos;

	uint8_t lang_index;
	uint64_t timer;

	c_pulse * ena_pulse;
};


struct _job_info_
{
	char * csv_addr;
	c_string * order_name;
	c_string* csv_content;
	c_string * csv_name;

	uint32_t total_sheet_number;
	uint32_t total_stemps_number;

	uint32_t printed_sheet_number;
	uint32_t rejected_sheet_number;

	bool end_status;

	/* 
	** contents list of array_lists contents list of array_list variables contains arrays of info_struct variables
	*/
	array_list * job_list;
};

struct _info_struct_
{
	char * sheet_order;
	char * result;
};

struct _comm_tcp_
{
	int sockfd;
	struct sockaddr_in serv_addr;
	pthread_mutex_t mutex;

	char* io_buffer;
};

struct _q_job_
{
	char * job_name;
	char * bkcore_name;
	char * camera_name;
	char * pdf_name;
	char flag;
	int order;
};


struct _io_card_
{
	comedi_t *ref_a1;
	comedi_t *ref_a2;

	uint8_t *** io_ref;

	unsigned int A1_in;
	unsigned int A1_out;
	unsigned int A2_in;
	unsigned int A2_out;

#if IO_CARD_SIMAULATION_EN == TRUE
	pthread_t simulation_thread;
#endif
	int64_t timer;
};



/**
** @ingroup Gui
*/
struct _gui_
{
	core * core_ref;

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
	core * core_ref;
	GdkRectangle work_area_geometry;
};



/******************* End of structures definitions *******************************************
******************** Static and local functions declarations *********************************/

	/******************* Static function declarations section - core internal functions *****************************************************************/
uint8_t core_iij_try_connect(core * this);
uint8_t core_pci_try_connect(core * this);


void * core_connection_testing_thread(void * param);
uint8_t core_iij_network_connected(core * this);
uint8_t core_pci_network_connected(core * this);
uint8_t core_quadient_network_connected(core * this);

void core_default_config(core * this);
uint8_t core_load_config(core* this);
void core_update_config(core * this, char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail);
void core_initialize_variables(core * this);

void core_safety_system_in(core * this);
void core_safety_system_out(core * this);
int8_t core_printer_abbort_print(core * this, uint8_t step);
void core_set_machine_error(core * this, uint8_t error_code);

int8_t core_csv_analyze(core * this);
void core_csv_compare(core * this, char * q_csv, char * c_csv);
int32_t core_save_response_csv(core * this, char * name);
void core_copy_job_list(core * this);
uint8_t core_compare_job_lists(core * this);
void core_read_dir_content(core * this);
void  core_clear_hotfolder(core * this);
void core_total_clear_hotfolder(core * this);
uint8_t core_check_job_consistency(core * this, char * name,  uint8_t mod);
void core_read_csv_line(core * this);
uint8_t core_load_bkcore_csv(core * this);
int8_t core_check_filename_param(char* filename, const char** params, int param_number, int offset);
int8_t core_parse_file_name(core * this, char * file_name);
q_job * core_find_job(core * this, char * name, int32_t * index);

void * core_machine_handler(void * param);
void core_machine_state_read_hotfolder(core * this);
void core_machine_state_print_main(core * this);
void core_machine_state_error(core * this);
void core_machine_state_print_companion(core * this);
void core_machine_state_read_csv_line(core * this);
void core_machine_pause(core * this);
void core_machine_state_clear_hotfolder(core * this);
void core_machine_state_prepare(core * this);
void core_machine_state_ready_to_start(core * this);
void core_machine_state_save_q_csv(core * this);
void core_machine_wait_for_print_finish(core * this);
void core_machine_state_finish(core * this);
void core_machine_state_print_break(core * this);
void core_stacker_error_handler(core * this);
void core_feeder_error_handler(core * this, uint8_t feeder_status);
void core_machine_mode_control(core * this);
void core_machine_stacker_counter(core * this);
void core_machine_tab_insert_counter(core * this);
void core_counter_check_sum(core * this);
void core_machine_reject_counter(core * this);
void core_machine_camera_counter(core * this);
void core_machine_log_monitor(core * this);
int8_t core_machine_slow_down(core * this);

void * core_gis_runtime_state_reading(void * param);
int core_gis_load_status_id(char ** msg);
void core_parse_gis_status(core * this, char * status_str);

	/******************* Local function declarations section - core punlic interface *****************************************************************/

core * core_new();
void core_finalize(core * this);

uint8_t core_set_interface_language(core * this, int lang_index);
void core_refresh_dir_list(core * this);
void core_set_machine_mode(core * this, int mode);

uint8_t core_iij_set_ip_addr(core * this, char * ip_addr);
uint8_t core_iij_set_tcp_port(core * this, int port);
char * core_iij_get_ip_addr(core * this);
int core_iij_get_tcp_port(core * this);
uint8_t core_iij_connect(core * this);
uint8_t core_iij_is_connected(core * this);
uint8_t core_iij_disconnect(core * this);

uint8_t core_pci_set_ip_addr(core * this, char * ip_addr);
uint8_t core_pci_set_tcp_port(core * this, int port);
char * core_pci_get_ip_addr(core * this);
int core_pci_get_tcp_port(core * this);
uint8_t core_pci_connect(core * this);
uint8_t core_pci_is_connected(core * this);
uint8_t core_pci_disconnect(core * this);

uint8_t core_quadient_network_set_ip_address(core * this, char * ip_address);
uint8_t core_iij_network_set_ip_address(core * this, char * ip_address);
uint8_t core_pci_network_set_ip_address(core * this, char * ip_address);


void core_set_max_stacked_sheets(core * this, int sheet_val);
void core_set_max_rejected_sheet_seq(core * this, int sheet_val);
void core_set_companion_sheet_source(core * this, int source);
void core_set_sheet_source_confirmation(core * this, bool confirm);

uint8_t core_set_q_main_hotfolder_path(core * this, const char * path);
uint8_t core_set_q_feedback_hotfolder_path(core * this, const char * path);
uint8_t core_set_q_backup_hotfolder_path(core * this, const char * path);
uint8_t core_set_pci_hotfolder_in_path(core * this, const char * path);
uint8_t core_set_pci_hotfolder_out_path(core * this, const char * path);
uint8_t core_set_gis_hotfolder_path(core * this, const char * path);
uint8_t core_set_job_report_hotfolder_path(core * this, const char * path);


char * core_get_q_main_hotfolder_path(core * this);
char * core_get_q_feedback_hotfolder_path(core * this);
char * core_get_q_backup_hotfolder_path(core * this);
char * core_get_pci_hotfolder_in_path(core * this);
char * core_get_pci_hotfolder_out_path(core * this);
char * core_get_gis_hotfolder_path(core * this);
char * core_get_job_report_hotfolder_path(core * this);


uint8_t core_print_start(core * this, const char * job_name);
uint8_t core_print_pause(core * this);
uint8_t core_print_continue(core * this);
uint8_t core_print_cancel(core * this);
uint8_t core_print_reset_error(core * this);
void core_sheet_source_confirmation(core * this);

/* error messages return and translations  */
char* core_machine_get_state_str(core * this);
uint8_t core_machine_status_val(core * this);

int core_get_error_val(core * this);
const char* core_get_error_str(core * this);
char * core_get_return_val_string(uint8_t status);
uint8_t core_iij_status(core * this);
uint8_t core_pci_status(core * this);



job_info * job_info_new(char * csv_address);
int job_info_get_job_index(job_info * this);
int job_info_get_printed_sheet_number(job_info * this);
void job_info_set_order_name(job_info * this, char* order_name);
void job_info_clear(job_info * this);
void info_struct_finalize(void * this);
void job_info_add_job_record(job_info * this);
void job_info_generate_missing_sheet_records(job_info * this);
void job_info_add_sheet_record(job_info * this, char * sheet_order);
void job_info_set_sheet_record_result(job_info * this, char * result, int index);
int8_t job_info_generate_csv(job_info * this);
void job_info_generate_csv_name(job_info * this);
void job_info_finalize();

	/******************* Static and local function declarations section - abcd ****************************************************************/
comm_tcp * comm_tcp_new();
uint8_t comm_tcp_set_ip_addr(comm_tcp * client, char * ip_addr);
uint8_t comm_tcp_set_tcp_port(comm_tcp * client, int port);
char * comm_tcp_get_ip_addr(comm_tcp * client);
int comm_tcp_get_tcp_port(comm_tcp * client);
uint8_t comm_tcp_connect(comm_tcp * client);
char * comm_tcp_transaction(comm_tcp * client, char* msg, int max_rcv);
uint8_t comm_tcp_is_connected(comm_tcp * client);
void comm_tcp_disconnect(comm_tcp * client);
void comm_tcp_finalize(comm_tcp * client);
int comm_tcp_send(comm_tcp * client, char* str);
char * comm_tcp_recv(comm_tcp * client);



io_card * io_card_new();
void io_card_sync_inputs(io_card * this);
void io_card_sync_outputs(io_card * this);
uint8_t io_card_get_input(io_card * this, int card, int addr);
uint8_t io_card_get_output(io_card * this, int card, int addr);
void io_card_set_output(io_card * this, int card, int addr, uint8_t val);
uint8_t io_card_get_bit_value(io_card * this, int card, int bit1, int bit2, int bit3);
void io_card_finalize(io_card * this);

q_job * q_job_new(char* name, uint32_t order, char flag);
q_job * q_job_copy(q_job * job);
uint8_t q_job_compare(q_job * job_1, q_job * job_2);
void q_job_finalize(void* job);


int32_t util_file_size(char * path, char * file_name);
int32_t util_save_csv(char* addr, char* name, char* buff);
char * util_load_csv(char * addr, char* name, int32_t* size);
uint8_t util_move_file(char * src, char* dest, char* name);
uint8_t util_copy_file(char * src, char *dest, char *name);
void util_delete_file(char *addr, char* name);
int32_t util_str_ends_with(char* str, const char * suffix, int offset);
char * util_get_time_string();



gui * gui_new(core * core_ref);
void gui_signals(gui * this);
void gui_finalize(GtkWidget * widget, gpointer param);
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
void gui_control_page_open_report_csv (GtkTreeView *tree_view, gpointer param);

void gui_control_page_btn_feed_sheet_callback(GtkWidget * widget, gpointer param);
void gui_control_page_btn_print_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_reset_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_pause_callback(GtkButton *button, gpointer param);
void gui_control_page_btn_go_to_settings_callback(GtkButton *button, gpointer param);
void gui_control_page_set_machine_mode_callback (GtkComboBox *widget, gpointer param);


gui_settings_page * gui_settings_page_new(gui_base * gui_base_ref);
void gui_settings_page_language(gui_settings_page * this);
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
void gui_network_page_language(gui_network_page * this);
gboolean gui_setting_page_iij_network_control_callback(GtkSwitch *widget, gboolean state, gpointer param);
void gui_network_page_set_pci_address(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_network_page_set_quadient_address(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_network_page_set_iij_address(GtkWidget *widget, GdkEvent  *event, gpointer param);


gui_hotfolder_page * gui_hotfolder_page_new(gui_base * gui_base_ref);
void gui_hotfolder_page_language(gui_hotfolder_page * this);
void gui_hotfolder_page_select_q_main_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_q_feedback_path_callback(GtkWidget * widget, gpointer parama);
void gui_hotfolder_page_select_q_backup_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_pci_in_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_pci_out_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_gis_path_callback(GtkWidget * widget, gpointer param);
void gui_hotfolder_page_select_report_path_callback(GtkWidget * widget, gpointer param);

gui_print_params_page * gui_print_params_page_new(gui_base * gui_base_ref);
void gui_print_params_page_language(gui_print_params_page * this);
void gui_print_params_page_max_stacked_sheet_callback (GtkWidget *widget, GdkEvent  *event, gpointer   param);
void gui_print_params_set_max_rejected_sheet_seq(GtkWidget *widget, GdkEvent  *event, gpointer param);
void gui_print_params_set_sheet_source_callback (GtkComboBox *widget, gpointer param);
gboolean gui_print_params_set_print_confirmation_state_callback (GtkSwitch *widget, gboolean state, gpointer param);


gui_lang_page * gui_lang_page_new(gui_base * gui_base_ref);
void gui_lang_page_language(gui_lang_page * this);

gui_info_window * gui_info_window_new(char * title, char * info_label, void (*callback)(void*), void * param);
void gtk_info_window_finalize();
void gui_info_window_callback(GtkWidget * widget, gpointer param);

gui_base * gui_base_new(core * core_ref);
GdkPixbuf * gui_base_load_icon(char * addr);




/******************* End of static and local functions declarations ***************************
******************** Functions definitions ****************************************************/

void * core_gis_runtime_state_reading(void * param)
{
	core * this = (core*) param;
	comm_tcp_send(this->iij_tcp_ref, "P,L\n");
	int id = -1;
	uint64_t timer = 0;

	while(core_iij_is_connected(this) == STATUS_CLIENT_CONNECTED)
	{
		char * state_msg = comm_tcp_recv(this->iij_tcp_ref);
		
		if(state_msg != NULL)
		{
			timer = c_freq_millis();

			while(*state_msg != 0)
			{
				if(*state_msg == 'I')
				{
					int msg_id = core_gis_load_status_id(&state_msg);
					state_msg++;
		
					if(*state_msg == 'S')
					{
						if(id == -1)
							id = msg_id;

						if(msg_id == id)
						{
							core_parse_gis_status(this, state_msg);
							break;
						}
					}			
				}
				else
				{	
					while((*state_msg != '\n') && (*state_msg != 0)){state_msg++;}

					if(*state_msg != 0)
						state_msg++;
				}
			}
		}
#if 0
		if((timer+60000) <= c_freq_millis())
			core_iij_disconnect(this);
#endif
	}
	
	c_string_set_string(this->print_controller_status, "Unknown");
	core_iij_disconnect(this);
	
	return NULL;
}

int core_gis_load_status_id(char ** msg)
{
	int id = 0;
	
	*msg = (*msg)+(sizeof(char)*2);

	while(isdigit(**msg))
	{
		id = id * 10 + (**msg - '0');
		(*msg)++;
	}

	return id;
}

void core_parse_gis_status(core * this, char * status_str)
{
	status_str = status_str+2;
	c_string_clear(this->print_controller_status);

	while((*status_str != ',') && (*status_str != 0) && (*status_str != '\n'))
	{
		c_string_add_char(this->print_controller_status, *status_str);
		status_str++;
	}
}

uint8_t core_iij_try_connect(core * this)
{
	uint8_t res = comm_tcp_connect(this->iij_tcp_ref);

	if(res == STATUS_CLIENT_CONNECTED)
	{
		if(strcmp(comm_tcp_recv(this->iij_tcp_ref), "N,C\n") == 0)
		{
			pthread_create(&(this->iij_gis_state_reading_thread), 0, &core_gis_runtime_state_reading, (void*) this);
		}
		else
		{
			res = STATUS_CONNECTION_ERROR;
		}
	}

	return res;
}

uint8_t core_pci_try_connect(core * this)
{
	return comm_tcp_connect(this->pci_tcp_ref);
}


void core_update_config(core * this, char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail)
{
	config_setting_t * root, *group, *settings;

	root = config_root_setting(&(this->cfg_ref));
	group = config_setting_get_member(root, group_name);

	if(config_setting_remove(group, var_name) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, var_name, type);

		if(type == CONFIG_TYPE_INT)
			config_setting_set_int(settings, *((int*) data));
		else if(type == CONFIG_TYPE_STRING)
			config_setting_set_string(settings, (char*) data);
		else if(type == CONFIG_TYPE_BOOL)
			config_setting_set_bool(settings, *((int*) data));

		config_write_file(&(this->cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(this->log, log_msg_ok);
	}
	else
	{
		c_log_add_record_with_cmd(this->log, "%s: %s",log_msg_fail, config_error_text(&(this->cfg_ref)));
	}
}

uint8_t core_load_config(core* this)
{
	config_setting_t * settings;
	const char * setting_val_str;
	int setting_val_int;
	
	/* load hotfolder settings */
	settings = config_lookup(&(this->cfg_ref), CFG_GROUP_HOTFOLDER);
	if(settings == NULL)
		return 1;
	
	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_MAIN, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->q_hotfolder_main_path, (char*) setting_val_str);
	else
		return 2;

	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_BACKUP, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->q_hotfolder_backup_path, (char*) setting_val_str);
	else
		return 3;
	
	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_FEEDBACK, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->q_hotfolder_feedback_path, (char*) setting_val_str);
	else
		return 4;

	if(config_setting_lookup_string(settings, CFG_HOT_PCI_IN, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->pci_hotfolder_in_path, (char*) setting_val_str);
	else
		return 5;

	if(config_setting_lookup_string(settings, CFG_HOT_PCI_OUT, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->pci_hotfolder_out_path, (char*) setting_val_str);
	else
		return 6;

	if(config_setting_lookup_string(settings, CFG_HOT_GIS, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->gis_hotfolder_path, (char*) setting_val_str);
	else
		return 7;
	
	if(config_setting_lookup_string(settings, CFG_HOT_REP_CSV, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(this->job_log_path, (char*) setting_val_str);
	else
		return 8;


	/* load network settings */
	settings = config_lookup(&(this->cfg_ref), CFG_GROUP_NETWORK);

	if(config_setting_lookup_string(settings, CFG_NETWORK_GIS_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_tcp_set_ip_addr(this->iij_tcp_ref, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_GIS_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_tcp_set_tcp_port(this->iij_tcp_ref, setting_val_int);
	else
		return 10;


	if(config_setting_lookup_string(settings, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_tcp_set_ip_addr(this->quadient_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_tcp_set_tcp_port(this->quadient_connection, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_PCI_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_tcp_set_ip_addr(this->pci_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_PCI_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_tcp_set_tcp_port(this->pci_connection, setting_val_int);
	else
		return 10;
	
	if(config_setting_lookup_string(settings, CFG_NETWORK_IIJ_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_tcp_set_ip_addr(this->iij_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_IIJ_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_tcp_set_tcp_port(this->iij_connection, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_tcp_set_ip_addr(this->quadient_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_tcp_set_tcp_port(this->quadient_connection, setting_val_int);
	else
		return 10;



	/* load print params settings */
	settings = config_lookup(&(this->cfg_ref), CFG_GROUP_PRINT_PARAMS);

	if(config_setting_lookup_int(settings, CFG_PP_SHEET_FOR_STOP, &setting_val_int) == CONFIG_TRUE)
		this->rejected_sheet_for_stop = setting_val_int;
	else
		return 11;

	if(config_setting_lookup_int(settings, CFG_PP_MAX_SHEET_IN_STACKER, &setting_val_int) == CONFIG_TRUE)
		this->max_stacked_sheets = setting_val_int;
	else
		return 12;



	if(config_setting_lookup_int(settings, CFG_PP_COMPAION_SHEET_SOURCE, &setting_val_int) == CONFIG_TRUE)
	{
		if((setting_val_int) >= 0 && (setting_val_int < SSOURCE_N))
			this->companion_sheet_source = setting_val_int;
		else
			return 13;
	}
	else
		return 14;


	if(config_setting_lookup_bool(settings, CFG_PP_SHEET_SOURCE_CONFIRMATION, &setting_val_int) == CONFIG_TRUE)
		this->sheet_source_confirmation = setting_val_int;
	else
		return 15;
	

	if(config_setting_lookup_int(settings, CFG_PP_NON_STACKED_SHEET_LIMIT, &setting_val_int) == CONFIG_TRUE)
		this->non_stacked_upper_limit = setting_val_int;
	else
		return 16;
	

	if(config_setting_lookup_int(settings, CFG_PP_NON_REVIDED_SHEET_LIMIT, &setting_val_int) == CONFIG_TRUE)
		this->non_revided_upper_limit = setting_val_int;
	else
		return 17;

	if(config_setting_lookup_int(settings, CFG_PP_GR_MACHINE_MODE, &setting_val_int) == CONFIG_TRUE)
		this->machine_mode = setting_val_int;
	else
		return 18;

	/* load language settings */
	settings = config_lookup(&(this->cfg_ref), CFG_GROUP_LANGUAGE);

	if(config_setting_lookup_int(settings, CFG_LANG_INDEX, &setting_val_int) == CONFIG_TRUE)
		this->lang_index = setting_val_int;
	else
		return 19;


	return 0;
}


void core_initialize_variables(core * this)
{

		/* machine handler status and control variables */
		this->machine_state = MACHINE_STATE_PRINT_FINISH;
		this->machine_state_pre = MACHINE_STATE_WAIT;
		this->printed_job_name = c_string_new();
		this->machine_pause_req = false;
		this->machine_print_req = false;
		this->machine_error_reset_req = false;
		this->machine_cancel_req = false;

		this->error_code = 0;

		this->job_list = array_list_new();
		this->job_list_pre = array_list_new();
		this->job_list_changed = 0;

		this->ena_pulse = c_pulse_new();

		this->printed_job_index = -1;
		this->bkcore_csv_pos = 0;

		this->q_hotfolder_main_path = c_string_new();
		this->q_hotfolder_feedback_path = c_string_new();
		this->q_hotfolder_backup_path = c_string_new();
		this->pci_hotfolder_in_path = c_string_new();
		this->pci_hotfolder_out_path = c_string_new();
		this->gis_hotfolder_path = c_string_new();
		this->job_log_path = c_string_new();

		this->timer = c_freq_millis();

		this->feeded_main_sheet_counter = 0;
		this->feeded_companion_sheet_counter = 0;
		this->stacked_sheet_counter = 0;
		this->rejected_sheet_counter = 0;
		this->rejected_sheet_seq_counter = 0;
		this->tab_insert_counter = 0;
		this->inspected_sheet_counter = 0;

	
		this->machine_mode_pre = 0;	
		this->print_confirmation_req = false;

		/* GIS printer status */
		this->print_controller_status = c_string_new_with_init("Unknown");
}


void core_default_config(core * this)
{
	config_setting_t *root, *hotfolder, *network, *print_params, *language, *settings;

	/* create root settings */
	root = config_root_setting(&(this->cfg_ref));

	/* create groups */
	hotfolder = config_setting_add(root, CFG_GROUP_HOTFOLDER, CONFIG_TYPE_GROUP);
	network = config_setting_add(root, CFG_GROUP_NETWORK, CONFIG_TYPE_GROUP);
	print_params = config_setting_add(root, CFG_GROUP_PRINT_PARAMS, CONFIG_TYPE_GROUP);
	language = config_setting_add(root, CFG_GROUP_LANGUAGE, CONFIG_TYPE_GROUP);

	
	/* save hotfolder settings */
	c_string_set_string(this->q_hotfolder_main_path, Q_HOT_FOLDER_ADDR);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_MAIN, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_HOT_FOLDER_ADDR);

	c_string_set_string(this->q_hotfolder_feedback_path, Q_FEEDBACK_ADDR);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_FEEDBACK, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_FEEDBACK_ADDR);

	c_string_set_string(this->q_hotfolder_backup_path, Q_JOB_BACKUP);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_BACKUP, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_JOB_BACKUP);	

	c_string_set_string(this->pci_hotfolder_in_path, PCI_HOT_FOLDER_ADDR_IN);
	settings = config_setting_add(hotfolder, CFG_HOT_PCI_IN, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, PCI_HOT_FOLDER_ADDR_IN);

	c_string_set_string(this->pci_hotfolder_out_path, PCI_HOT_FOLDER_ADDR_OUT);
	settings = config_setting_add(hotfolder, CFG_HOT_PCI_OUT, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, PCI_HOT_FOLDER_ADDR_OUT);

	c_string_set_string(this->gis_hotfolder_path, GIS_HOT_FOLDER);
	settings = config_setting_add(hotfolder, CFG_HOT_GIS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, GIS_HOT_FOLDER);

	c_string_set_string(this->job_log_path, JOB_INFO_CSV_PATH);
	settings = config_setting_add(hotfolder, CFG_HOT_REP_CSV, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, JOB_INFO_CSV_PATH);


	/* save print parameters settings */
	this->rejected_sheet_for_stop = 10;
	settings = config_setting_add(print_params, CFG_PP_SHEET_FOR_STOP, CONFIG_TYPE_INT);
	config_setting_set_int(settings, this->rejected_sheet_for_stop);

	this->max_stacked_sheets = 2500;
	settings = config_setting_add(print_params, CFG_PP_MAX_SHEET_IN_STACKER, CONFIG_TYPE_INT);
	config_setting_set_int(settings, this->max_stacked_sheets);

	core_set_companion_sheet_source(this,  SSOURCE_COMPANION);
	settings = config_setting_add(print_params, CFG_PP_COMPAION_SHEET_SOURCE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, SSOURCE_COMPANION);

	core_set_sheet_source_confirmation(this, false);
	settings = config_setting_add(print_params, CFG_PP_SHEET_SOURCE_CONFIRMATION, CONFIG_TYPE_BOOL);
	config_setting_set_bool(settings, false);

	this->non_stacked_upper_limit = 2;
	settings = config_setting_add(print_params, CFG_PP_NON_STACKED_SHEET_LIMIT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 2);

	this->non_revided_upper_limit = 3;
	settings = config_setting_add(print_params, CFG_PP_NON_REVIDED_SHEET_LIMIT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 3);	

	this->machine_mode = GR_PRINT_INSPECTION;
	settings = config_setting_add(print_params, CFG_PP_GR_MACHINE_MODE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, GR_PRINT_INSPECTION);	

	/* save network settings */
	comm_tcp_set_ip_addr(this->iij_tcp_ref,  DEFAULT_IP_ADDRESS_GIS);
	settings = config_setting_add(network, CFG_NETWORK_GIS_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_GIS);

	comm_tcp_set_tcp_port(this->iij_tcp_ref, DEFAULT_TCP_PORT_GIS);
	settings = config_setting_add(network, CFG_NETWORK_GIS_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_TCP_PORT_GIS);

	comm_tcp_set_tcp_port(this->quadient_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_tcp_set_ip_addr(this->quadient_connection, DEFAULT_IP_ADDRESS_QUADIENT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_QUADIENT);

	comm_tcp_set_tcp_port(this->pci_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_tcp_set_ip_addr(this->pci_connection, DEFAULT_IP_ADDRESS_PCI);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_PCI);

	comm_tcp_set_tcp_port(this->iij_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_tcp_set_ip_addr(this->iij_connection, DEFAULT_IP_ADDRESS_GIS);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_GIS);


	/* save language settings */
	this->lang_index = lang_cz;
	settings = config_setting_add(language, CFG_LANG_INDEX, CONFIG_TYPE_INT);
	config_setting_set_int(settings, lang_cz);
	


	config_write_file(&(this->cfg_ref), CONFIGURATION_FILE_PATH);
}

void core_set_machine_error(core * this, uint8_t error_code)
{
	this->machine_state = MACHINE_STATE_ERROR;
	this->error_code = error_code;
}

/**
** @ingroup Core
*/
void core_safety_system_in(core * this)
{

	 /* switch on/off gremser machine as needed */
	uint8_t feeder_status = io_card_get_bit_value(this->io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2); 

	if((this->machine_state == MACHINE_STATE_WAIT) || (this->machine_state == MACHINE_STATE_ERROR))
	{
		if(feeder_status == MACHINE_FN_READY_TO_FEED)
		{
			if(c_pulse_high(this->ena_pulse, 500) > 0)
			{
				io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 1);
			}
			else
			{
				io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 0);
				c_pulse_reset(this->ena_pulse);
			}
		}
	}
	else
	{
		if(feeder_status != MACHINE_FN_READY_TO_FEED)
		{
			if(c_pulse_high(this->ena_pulse, 500) > 0)
			{
				io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 1);
			}
			else
			{
				io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 0);
				c_pulse_reset(this->ena_pulse);
			}
		}
	}	





#if 1
	/* safety protection of input states */
	if(io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_6_LNA_E_stop) == 0)
	{
		core_set_machine_error(this, MACHINE_ERR_E_STOP);
	}

	if(this->machine_state == MACHINE_STATE_READ_CSV_LINE)
	{
		if((io_card_get_input(this->io_card_ref,IO_CARD_A2, A2_IN_1_RJ_full) > 0) || (io_card_get_input(this->io_card_ref,IO_CARD_A2, A2_IN_2_SN_full) > 0))
		{
			this->machine_pause_req = true;
		}
	}

	if(this->rejected_sheet_seq_counter >= this->rejected_sheet_for_stop)
	{
		core_set_machine_error(this, MACHINE_ERR_LOW_PRINT_QUALITY);
	}

	/* if some got error then the counters on the end are ignored */
	if(io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_4_RJ_jam) > 0)
	{
		core_set_machine_error(this, MACHINE_ERR_REJECT_BIN_JAM);
	}

	if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_9_FN_jam) > 0)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_JAM);
	}
	
	if((roundf(c_freq_measure_current(this->ti_freq, io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc)))) >= 2.0)
	{
		core_set_machine_error(this, MACHINE_ERR_TI);
	}

	if((roundf(c_freq_measure_current(this->ta_freq, io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF)))) >= 2.0)
	{
		core_set_machine_error(this, MACHINE_ERR_TA);
	}

	/* network connection checking */
	if(core_quadient_network_connected(this) == STATUS_CLIENT_CONNECTED)
	{
		if(this->error_code == MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING)
			this->error_code = MACHINE_ERR_NO_ERROR;
	}
	else
	{
		if(this->error_code == MACHINE_ERR_NO_ERROR)
			core_set_machine_error(this, MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING);
	}

	if(core_pci_network_connected(this) == STATUS_CLIENT_CONNECTED)
	{
		if(this->error_code == MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING)
			this->error_code = MACHINE_ERR_NO_ERROR;
	}
	else
	{	
		if(this->error_code == MACHINE_ERR_NO_ERROR)
			core_set_machine_error(this, MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING);
	}

	if(core_iij_network_connected(this) == STATUS_CLIENT_CONNECTED)
	{
		if(this->error_code == MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING)
			this->error_code = MACHINE_ERR_NO_ERROR;
	}
	else
	{	
		if(this->error_code == MACHINE_ERR_NO_ERROR)
			core_set_machine_error(this, MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING);
	}

	if(core_iij_is_connected(this) == STATUS_CLIENT_CONNECTED)
	{
		if(this->error_code == MACHINE_ERR_GIS_DISCONNECTED)
			this->error_code = MACHINE_ERR_NO_ERROR;
	}
	else
	{
		if(this->error_code == MACHINE_ERR_NO_ERROR)
			core_set_machine_error(this, MACHINE_ERR_GIS_DISCONNECTED);
	}
#endif
}

void core_safety_system_out(core * this)
{

	io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, 0);

#if 1
	core_machine_mode_control(this);
	
	/* activation of gremser machine */
	if((this->machine_state != MACHINE_STATE_WAIT) && (this->machine_state != MACHINE_STATE_ERROR) && (this->machine_state != MACHINE_STATE_NEXT))
	{
		if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_3_PRN_rdy, 1);
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_5_PRN_active, 1);
		}
	}
	else
	{
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_3_PRN_rdy, 0);
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_5_PRN_active, 0);
	}



	/* switch off outputs if the error ocures */
	if(this->machine_state == MACHINE_STATE_ERROR)
	{
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 0);
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_12_INHIBIT_NEXT_SHEET, 0);
	}
	else
	{
		if((this->machine_state != MACHINE_STATE_NEXT) && (this->machine_state != MACHINE_STATE_WAIT) && (this->machine_state != MACHINE_STATE_PREPARE))
		{
			/* counters */
			core_machine_stacker_counter(this);
			core_machine_tab_insert_counter(this);
			core_machine_reject_counter(this);
			core_machine_camera_counter(this);

			if((this->machine_state != MACHINE_STATE_PRINT_BREAK) && (this->machine_state != MACHINE_STATE_CLEAR_HOT_FOLDER) && 
				(this->machine_state != MACHINE_STATE_JOB_FINISH) && (this->machine_state != MACHINE_STATE_CLEAR_TO_FINISH) && 
				(this->machine_state != MACHINE_STATE_PRINT_FINISH))
			{
				//core_counter_check_sum(this);		
				//core_stacker_error_handler(this);

			}
		}
	}
#endif
}

int8_t core_printer_abbort_print(core * this, uint8_t step)
{
	int8_t ret_val = -1;
	char * res = NULL;

	if(step == 0)
	{
		res = comm_tcp_transaction(this->iij_tcp_ref, "P,A\n", CLI_IO_BUFFER_SIZE);
		if(res != NULL)
			ret_val = 1;
	}
	else
	{
		res = comm_tcp_transaction(this->iij_tcp_ref, "R,A\n", CLI_IO_BUFFER_SIZE);
		if(res != NULL)
			ret_val = 2;
	}

	return ret_val;
}

void core_clear_hotfolder_base(core * this, q_job * job)
{
	if(job != NULL)
	{
		if(job->pdf_name != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), job->pdf_name);

		if(job->bkcore_name != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), job->bkcore_name);

		if(job->camera_name != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), job->camera_name);
	}
}

void core_clear_hotfolder(core * this)
{
	if(this->printed_job_index >= 0)
	{
		q_job* job = (q_job*) array_list_get(this->job_list, this->printed_job_index);
		core_clear_hotfolder_base(this, job);
	}
}

void core_total_clear_hotfolder(core * this)
{
	int i;
	for(i = 0; i < array_list_size(this->job_list); i++)
	{
		q_job * job = (q_job*) array_list_get(this->job_list, i);
		core_clear_hotfolder_base(this, job);
	}
}

int32_t core_get_order(char* str, int offset, int len)
{
	int str_len = strlen(str);

	if(str_len-offset >= len)
	{
		char str_num[32];
		strncpy(str_num, str+str_len-offset-len, len);
		str_num[len] = 0;
		int32_t res = 0;
		str = (str+str_len-offset-len);
	
		int i;
		for(i = 0; i < len; i++)
		{
			if(isdigit(*(str+i)) == 0)
			{	
				return -1;
			}

			res = res * 10 + (str_num[i] - '0');
		}	
				
		return res;
	}
	else
	{
		return -1;
	}
}

q_job * core_find_job(core * this, char * name, int32_t *index)
{
	int i;
	for(i = 0; i < array_list_size(this->job_list); i++)
	{
		q_job * job = array_list_get(this->job_list, i);
		if(strcmp(job->job_name, name) == 0)
		{	
			if(index != NULL)
				*index = i;

			return job;
		}
	}

	return NULL;
}

int8_t core_parse_file_name(core * this, char * file_name)
{
	int8_t offset = 0;
	int8_t file_format = core_check_filename_param(file_name, available_formats, 2, offset);

	/* pdf or csv */			
	if(file_format == 0 || file_format == 1)
	{
		int8_t file_type = -1; 
 		offset += strlen(available_formats[file_format]); //length of substring .pdf or .csv

		if(file_format == 0)
		{
			file_type = core_check_filename_param(file_name, available_types_pdf, 1, offset);
			if(file_type == 0)
				offset += strlen(available_types_pdf[file_type]);
		}
		else
		{
			file_type = core_check_filename_param(file_name, available_types_csv, 2, offset);
			if((file_type == 0) || (file_type == 1))
				offset += strlen(available_types_csv[file_type]);
		}

		if(((file_format == 0) && (file_type == 0)) || ((file_format == 1) && ((file_type == 0) || (file_type == 1))))
		{
			int8_t flag_param = core_check_filename_param(file_name, available_flags, 3, offset);
				
			if(flag_param >= 0 && flag_param < 3)
			{
				offset += strlen(available_flags[flag_param]);
				int16_t job_order = core_get_order(file_name, offset, 4);
						
				if(job_order > 0)
				{
					offset += 5;
					int8_t job_name_len = strlen(file_name)-offset;
								
					if(job_name_len > 0)
					{
						char * job_name = (char*) malloc(sizeof(char)*(job_name_len+1));
						memset(job_name, 0, job_name_len+1);
						strncpy(job_name, file_name, job_name_len);
									
						q_job * job =  core_find_job(this, job_name, NULL);

						if(job == NULL)
						{
							char flag = available_flags[flag_param][1];
								
							job = q_job_new(job_name, job_order, flag);
							array_list_add(this->job_list, job);
						}
						else
						{
							free(job_name);
						}

						if(file_format == 0)
						{
							job->pdf_name = (char*) malloc(sizeof(char)* (strlen(file_name)+1));
							strcpy(job->pdf_name, file_name);
						}
						else
						{
							if(file_type == 0)
							{
								job->camera_name = (char*) malloc(sizeof(char)* (strlen(file_name)+1));
								strcpy(job->camera_name, file_name);
							}
							else
							{
								
								job->bkcore_name = (char*) malloc(sizeof(char)* (strlen(file_name)+1));
								strcpy(job->bkcore_name, file_name);
							}
						}

						return 1;
					}
				}
			}
		}
	}

	return 0;	
}


void core_read_dir_content(core* this)
{
	pthread_mutex_lock(&(this->mutex));
	DIR * dir_ref = opendir(c_string_get_char_array(this->q_hotfolder_main_path));
	

	/* clear job list form previous reading iteration */
	array_list_clear_with_release(this->job_list, q_job_finalize);
	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;
		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				if (core_parse_file_name(this, dir_cont->d_name) == 0)
				{
					util_delete_file(c_string_get_char_array(this->q_hotfolder_main_path), dir_cont->d_name);					
				}
			}
		}

		closedir(dir_ref);
	}

	this->job_list_changed = core_compare_job_lists(this);

	core_copy_job_list(this);
	
	pthread_mutex_unlock(&(this->mutex));
}


uint8_t core_compare_job_lists(core * this)
{
	if(array_list_size(this->job_list) != array_list_size(this->job_list_pre))
	{
		return 1;
	}
	else
	{
		int i;
		for(i = 0;i < array_list_size(this->job_list); i++)
		{
			if(q_job_compare(array_list_get(this->job_list,i), array_list_get(this->job_list_pre,i)) == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

void core_copy_job_list(core * this)
{
	array_list_clear_with_release(this->job_list_pre, q_job_finalize);
	
	int i;
	for(i = 0;i < array_list_size(this->job_list); i++)
	{
		array_list_add(this->job_list_pre, q_job_copy(array_list_get(this->job_list, i)));
	}
}

uint8_t core_load_bkcore_csv(core * this)
{
	q_job * job = (q_job*) array_list_get(this->job_list, this->printed_job_index);

	if(job != NULL)
	{
		this->bkcore_csv_content = util_load_csv(c_string_get_char_array(this->q_hotfolder_main_path), job->bkcore_name, &(this->bkcore_csv_size));
		if(this->bkcore_csv_content != NULL)
		{
			c_log_add_record_with_cmd(this->log, "Načten obsah bkcore csv souboru.");
			return STATUS_SUCCESS;
		}
	}

	c_log_add_record_with_cmd(this->log, "obsah souboru bkcore csv nelze načíst!");
	return STATUS_GENERAL_ERROR;
}


/*
** utility function for avoid duplicity code
** writes the index of wrong recognized sheet to output csv file for quadient
*/
void core_write_to_csv(c_string * feedback_csv, array_list * sheet_index_array, uint32_t index)
{
	info_struct* sheet_info = array_list_get(sheet_index_array, index);
	c_string_add(feedback_csv, (char*) sheet_info->sheet_order);
			
	if(index+1 < array_list_size(sheet_index_array))
	{
		c_string_concat(feedback_csv, "\n");
	}
}


int8_t core_check_filename_param(char* filename, const char** params, int param_number, int offset)
{
	int i = 0;

	while(i < param_number)
	{
		if(util_str_ends_with(filename, params[i], offset) == 0)
		{	
			return i;
		}
		i++;
	}

	return -1;
}

int32_t core_get_next_csv_line(char * csv_content, int32_t pos, char* buffer, int max)
{
	if(max > 0)
	{
		int32_t offset = pos;

		while((csv_content[offset] != '\n') && (csv_content[offset] != '\0') && ((offset - pos) < max))
		{
			buffer[offset - pos] = csv_content[offset];
			offset ++;
		}
		
		buffer[offset - pos] = 0;

		return offset;
	}
	else
		return 0;
}


/*
** utility function for avoid duplicity code
** if some part of the csv file from camera missing then fill the output csv for quadient remaining 
** sheet indexes.
*/
void core_csv_fill_missing(core * this, c_string * feedback_csv, array_list * sheet_index_array, uint32_t * index)
{
	while(*index < array_list_size(sheet_index_array))
	{
		core_write_to_csv(feedback_csv, sheet_index_array, *index);
		job_info_set_sheet_record_result(this->info, "FAIL", *index);
		(*index)++;
	}
}


void core_csv_compare(core * this, char * q_csv, char * c_csv)
{
	uint32_t q_pos = 0, c_pos = 0, index = 0;
	array_list * sheet_index_array = array_list_get(this->info->job_list, job_info_get_job_index(this->info));

	while((q_csv[q_pos] != 0))
	{		
		/* new line finding */
		if((q_csv[q_pos] == '\n'))
		{
			job_info_set_sheet_record_result(this->info, "PASS", index);

			while(c_csv[c_pos] != q_csv[q_pos])
			{
				if(c_csv[c_pos] != 0)
				{
					c_pos++;
				}
					else
				{
					index++;
					core_csv_fill_missing(this, this->feedback_csv, sheet_index_array, &index);
					return;
				}
			}
			index++;
		}
		
		/* characters testing */
		if(c_csv[c_pos] != q_csv[q_pos])
		{
			core_write_to_csv(this->feedback_csv, sheet_index_array, index);
			job_info_set_sheet_record_result(this->info, "FAIL", index);

			/* jump to end of line */
			while((q_csv[q_pos] != '\n') || (c_csv[c_pos] != '\n'))
			{
				/* if the part on the end of the camera csv file missing */
				if(c_csv[c_pos] == 0)
				{
					index++;
					core_csv_fill_missing(this, this->feedback_csv, sheet_index_array, &index);
					return;
				}

				if(q_csv[q_pos] != '\n')
					q_pos++;

				if(c_csv[c_pos] != '\n')
					c_pos++;

				if(q_csv[q_pos] == 0)
					return;

			}	

			index++;
		}
			
		q_pos++;
		c_pos++;
	}
}

/**
** rutine for evaluting camera inspection and generate ouput csv ontent for quadient
**
** when in quadient csv find end of line, increment the line index and find end of line 
** in camera csv file.
**
** when one (or more but it doesn't matter) character in line of camera csv file not equal with
** character in line of quadient csv file, then add record to feedback csv file and 
** find end of line in both csv files (quadient and camera), increment line index and then 
** continue with inspection.  
*/
int8_t core_csv_analyze(core* this)
{
	q_job * job = (q_job*) array_list_get(this->job_list, this->printed_job_index);
	
	if(job != NULL)
	{
		char * q_csv = util_load_csv(c_string_get_char_array(this->q_hotfolder_main_path), job->camera_name, NULL);
		char * c_csv = util_load_csv(c_string_get_char_array(this->pci_hotfolder_out_path), job->camera_name, NULL);
	
		this->feedback_csv = c_string_new();

		if((q_csv != NULL) && (c_csv != NULL))
		{
			core_csv_compare(this, q_csv, c_csv);
	
			free(q_csv);
			free(c_csv);

			return STATUS_SUCCESS;
		}
		else
		{
			if(q_csv != NULL)
				free(q_csv);
		
			if(c_csv != NULL)
				free(c_csv);
		}
	}

	return STATUS_GENERAL_ERROR;
}

int32_t core_save_response_csv(core * this, char * name)
{
	if(this->feedback_csv != NULL)
	{	
		int32_t res = util_save_csv(c_string_get_char_array(this->q_hotfolder_feedback_path), name, c_string_get_char_array(this->feedback_csv));

		util_save_csv("/home/stc", name, c_string_get_char_array(this->feedback_csv));
		c_string_finalize(&(this->feedback_csv));

		return res;
	}

	return -1;
}


void core_feeder_error_handler(core * this, uint8_t feeder_status)
{
	if(feeder_status == MACHINE_FN_DOUBLE_SHEET)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_DOUBLE_SHEET);
	}

	if(feeder_status == MACHINE_FN_MISSING_SHEET)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_SHEET_MISSING);
	}

	if(feeder_status == MACHINE_FN_FEEDER_MULTIFUNCTION)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_MULTIFUNCTION);
	}
	
	if(feeder_status == MACHINE_FN_OFF)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_OFF);
	}
	
	if(feeder_status == MACHINE_FN_E_STOP)
	{
		core_set_machine_error(this, MACHINE_ERR_FEEDER_E_STOP);
	}
}

void core_stacker_error_handler(core * this)
{
	uint8_t stacker_status = io_card_get_bit_value(this->io_card_ref, IO_CARD_A1, A1_IN_6_SN0, A1_IN_7_SN1, A1_IN_8_SN2); 


	if((this->machine_state == MACHINE_STATE_PRINT_MAIN) || (this->machine_state == MACHINE_STATE_PRINT_COMPANION))
	{
		if(stacker_status == MACHINE_SN_OFF)
		{
			core_set_machine_error(this, MACHINE_ERR_STACKER_OFF);
		}	
	}

	if(stacker_status == MACHINE_SN_STACKER_MULTIFUNCTION)
	{
		core_set_machine_error(this, MACHINE_ERR_STACKER_MULTIFUNCTION);
	}

	if(stacker_status == MACHINE_SN_BOTTOM)
	{
		core_set_machine_error(this, MACHINE_ERR_STACKER_BOTTOM);
	}

	if(stacker_status == MACHINE_SN_JAM_CONVEYOR)
	{
		core_set_machine_error(this, MACHINE_ERR_STACKER_JAM_CONVEYOR);
	}
		
	if(stacker_status == MACHINE_SN_JAM_PILE)
	{
		core_set_machine_error(this, MACHINE_ERR_STACKER_JAM_PILE);
	}
}

/*
** if the cur_machine_mode (status from inputs) is equal to local configured machine_mode, then
** the output reflects inputs
** if the cur_machine_mode not equals to local configured machine_mode, then could occur two situations
** firest situation is cur_machine_mode is equal to machine_mode_pre, this means that machine mode was changed
** from graphic interface, then must be changed states of outputs coresponds to given machine mode
** secound situation is cur_machine_mode is not equal to machine_mode_pre, this means that machine mode was changed
** from gremser machine, then must be changed configuration of machine mode in graphic interface
*/
void core_machine_mode_control(core* this)
{
	int cur_machine_mode = io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) + 2*io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1);

	if(cur_machine_mode != this->machine_mode)
	{
		if(cur_machine_mode == this->machine_mode_pre)
		{
			/* machine mode changed from gui interface, need change output values */
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, this->machine_mode%2);
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_1_BM1, (this->machine_mode/2)%2);	
		}
		else
		{
			/* machine mode changed from gremser machine, need adapt internal state */
			this->machine_mode = cur_machine_mode;
		}	
	}
	else
	{
		/* reflection input state to output */
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0));
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_1_BM1, io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1));
	}

	this->machine_mode_pre = cur_machine_mode;
}

void core_machine_state_read_hotfolder(core * this)
{
	if(this->machine_print_req == false)
		core_read_dir_content(this);	
	else if((this->machine_cancel_req) == true && (this->machine_state == MACHINE_STATE_NEXT))
		this->machine_state = MACHINE_STATE_PRINT_BREAK;

	if(c_string_len(this->printed_job_name) > 0)
	{
		if(((this->machine_print_req == true) && (this->machine_state == MACHINE_STATE_WAIT)) || (this->machine_state == MACHINE_STATE_NEXT))
		{
			int job_index = -1;
			q_job * job = core_find_job(this, c_string_get_char_array(this->printed_job_name), &job_index);

			if((job != NULL) && (job->bkcore_name != NULL))
			{	
				if(job->order == (job_info_get_job_index(this->info)+2))
				{
					if(job->flag == 'e')
					{
						this->machine_state = MACHINE_STATE_PRINT_BREAK;
						this->printed_job_index = job_index;
					}
					else
					{
						if((job->pdf_name != NULL) && (job->camera_name != NULL))
						{	
							if(this->machine_state == MACHINE_STATE_WAIT)
							{
								job_info_set_order_name(this->info, job->job_name);
								job_info_generate_csv_name(this->info);
							}
	
							this->printed_job_index = job_index;
							job_info_add_job_record(this->info);
							this->machine_state = MACHINE_STATE_PREPARE;
						}
					}
				}
				else
				{
					core_set_machine_error(this, MACHINE_ERR_JOB_ORDER_MISMATCH);
				}
			}
		}
	}
}

int8_t core_machine_slow_down(core * this)
{
	int x = 10;
	int8_t slmx = (this->stacked_sheet_counter < (this->max_stacked_sheets - x));
	int8_t sgemx = (this->stacked_sheet_counter >= (this->max_stacked_sheets - x));
	int8_t sgem = !(this->stacked_sheet_counter >= this->max_stacked_sheets);
	int8_t fes = ((this->feeded_main_sheet_counter+this->feeded_companion_sheet_counter) == this->stacked_sheet_counter+this->rejected_sheet_counter);

	return ((!slmx) && sgemx && sgem && fes) || (slmx && (!sgemx) && sgem);
}

void core_machine_state_read_csv_line(core * this)
{
	/* if is set pause request, move to pause state after feed current sheet */
	if((this->machine_pause_req == true))
	{
		this->machine_state = MACHINE_STATE_PAUSE;
	}
	else if(this->machine_cancel_req == true)
	{
		this->machine_state = MACHINE_STATE_PRINT_BREAK;
	}
	else
	{
		if(core_machine_slow_down(this) > 0)
		{
			/* load line from csv */
			q_job * job = array_list_get(this->job_list, this->printed_job_index);
		
			/* process csv line */
			if(this->bkcore_csv_pos < array_list_size(array_list_get(this->info->job_list, job_info_get_job_index(this->info))))
			{
				if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) == 0)
					job_info_set_sheet_record_result(this->info, "PASS", this->bkcore_csv_pos);


				if((job->flag == 'k') || ((job->flag == 'p') && (this->sheet_source_confirmation == false) && (this->companion_sheet_source == SSOURCE_MAIN))) 
				{
					this->machine_state = MACHINE_STATE_PRINT_MAIN;
					printf("go to print main\n");
				}
				else if((job->flag == 'p') && (this->companion_sheet_source == SSOURCE_COMPANION))
				{
					printf("go to print compation\n");
					this->machine_state = MACHINE_STATE_PRINT_COMPANION;
				}
				else if((job->flag == 'p') && (this->companion_sheet_source == SSOURCE_MAIN) && (this->sheet_source_confirmation == true))
				{
					printf("go to wait for confirmation\n");
					this->machine_state = MACHINE_STATE_WAIT_FOR_CONFIRMATION;
				}
				else
				{
					core_set_machine_error(this, MACHINE_ERR_SHEET_FEEDER_REDIRECTION);
				}

				this->bkcore_csv_pos++;
			}
			else
			{
				this->machine_state = MACHINE_STATE_WAIT_FOR_PRINT_FINISH;
			}
		}
		else if(this->stacked_sheet_counter >= this->max_stacked_sheets)
		{
			this->machine_state = MACHINE_STATE_PAUSE;
		}
	}
}

void core_machine_state_wait_for_confirmation(core * this)
{
	if(this->print_confirmation_req == true)
	{
		this->machine_state = MACHINE_STATE_PRINT_MAIN;
		this->print_confirmation_req = false;
	}
}


void core_machine_state_print_main(core * this)
{
	uint8_t feeder_status = 0; 

	/* gremser machine control */
	feeder_status = io_card_get_bit_value(this->io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2); 
	
	/* wait for feeder ready */	
	if((this->feed_sheet == 0) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		this->feed_sheet = 1;
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 1);
		this->io_card_ref->timer = c_freq_millis();
	}
	else if(this->feed_sheet == 1)
	{
		/* feed one sheet */
		if(((this->io_card_ref->timer+MACHINE_XBF_INTERVAL) <= c_freq_millis()))
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			//this->feed_sheet = 2;
			this->feed_sheet = 3;	//only for testing skip the point 2
		}

		if((feeder_status == MACHINE_FN_FEEDING))
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			this->feed_sheet = 3;
		}
	}
	else if(this->feed_sheet == 2)
	{
		if(feeder_status == MACHINE_FN_FEEDING)
		{
			this->feed_sheet = 3;
		}
	}
	else if((this->feed_sheet == 3) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		this->feed_sheet = 0;
		this->feeded_main_sheet_counter++;
		this->machine_state = MACHINE_STATE_READ_CSV_LINE;
	}

	/* feedeng error handling */
	core_feeder_error_handler(this, feeder_status);
}

void core_machine_state_print_companion(core * this)
{
	/* check the state of companion feeder, TI_incyc must be in false state */
	if((this->feed_sheet == 0) && (io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) == 0))
	{
		/* set output TI_ins to true state */
		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 1);
		this->timer = c_freq_millis();
		this->feed_sheet = 1;
	}
	else if(this->feed_sheet == 1)
	{	
		/* wait for TI_incyc true state or given time */
		if(((this->io_card_ref->timer+MACHINE_XBF_INTERVAL) <= c_freq_millis()))
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 0);
			//this->feed_sheet = 2;
			this->feed_sheet = 3; // only for testing
		}

		if(io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) > 0)
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 0);
			this->feed_sheet = 3;
		}
	}
	else if(this->feed_sheet == 2)
	{	
		/* wait for TI_incyc true state */
		if(io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) > 0)
		{
			this->feed_sheet = 3;
		}
	}
	else if((this->feed_sheet == 3) && (io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) == 0))
	{
		/* wait for TI_incyc true state, incremet the counter state and go to READ_CSV_LINE state */	
		this->feed_sheet = 0;
		this->feeded_companion_sheet_counter++;
		this->machine_state = MACHINE_STATE_READ_CSV_LINE;
	}
}

/* in some error state could be restored the print process */
void core_machine_state_error(core * this)
{
	if(this->machine_error_reset_req == true)
	{
		this->error_code = MACHINE_ERR_NO_ERROR;
		this->machine_state = MACHINE_STATE_PRINT_BREAK;
	}	
}

void core_machine_pause(core * this)
{
	/* from pause state can return to print state only if the conditions are met */
	if(this->machine_pause_req == false)
	{
		if((this->stacked_sheet_counter < 2500) && 
		(io_card_get_input(this->io_card_ref,IO_CARD_A2, A2_IN_1_RJ_full) == 0) && 
		(io_card_get_input(this->io_card_ref,IO_CARD_A2, A2_IN_2_SN_full) == 0))
		{
			this->machine_state = MACHINE_STATE_READ_CSV_LINE;
		}
	}
}

void core_machine_state_clear_hotfolder(core * this)
{
	/* delete csv from camera hotfolder */
	if((this->printed_job_index >= 0) && (io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0))
	{
		q_job * job = array_list_get(this->job_list, this->printed_job_index);
		if(job != NULL)
			util_delete_file(c_string_get_char_array(this->pci_hotfolder_out_path), job->camera_name);
	}

	if((c_string_get_char_array(this->printed_job_name) != NULL))
		core_clear_hotfolder(this);

	if(this->machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER)
	{
		this->machine_state = MACHINE_STATE_JOB_FINISH;
	}
	else
	{
		this->machine_state = MACHINE_STATE_PRINT_FINISH;
	}
}

void core_machine_state_prepare(core * this)
{
	/* load bkcore csv content */
	if(core_load_bkcore_csv(this) == STATUS_SUCCESS)
	{	
		q_job * job = array_list_get(this->job_list, this->printed_job_index);
		
		uint8_t status = 0;

		/* copy pdf to gis hotfolder or based on MB0 input status */
		if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
			status = util_copy_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->gis_hotfolder_path), job->pdf_name);
		
		if(status == 0)
		{
			if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
				status = util_copy_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->pci_hotfolder_in_path), job->camera_name);

			if(status == 0)
			{
				this->machine_state = MACHINE_STATE_READY_TO_START;
				this->timer = c_freq_millis();

				/* prepare job info structure for current job */
				int rows = 0, csv_line_index = 0, csv_pos = 0;
				char sheet_index[255];

				while(this->bkcore_csv_content[csv_pos] != 0)
				{
					if(isdigit(this->bkcore_csv_content[csv_pos]))	
					{
						if((rows == 0) && (job->flag == 'k'))
						{
							this->info->total_stemps_number = ((this->info->total_stemps_number*10) + (this->bkcore_csv_content[csv_pos]-'0'));
						}	
						else if((rows == 1) && (job->flag == 'k'))
						{
							if(job->order == 1)
								this->info->total_sheet_number = ((this->info->total_sheet_number*10) + 
												(this->bkcore_csv_content[csv_pos]-'0'));					
						}
						else
						{
							printf("%c", this->bkcore_csv_content[csv_pos]);
							sheet_index[csv_line_index] = this->bkcore_csv_content[csv_pos];
							csv_line_index++;
						}
					}

					if((this->bkcore_csv_content[csv_pos]) == '\n')
					{
						if((rows > 1) || (job->flag != 'k'))
						{	
							sheet_index[csv_line_index] = 0;
							job_info_add_sheet_record(this->info, sheet_index);
							csv_line_index = 0;
						}
						
						rows ++;
					}

					csv_pos++;
				}

				if(csv_line_index > 0)
				{
					printf("add sheet record\n");
					sheet_index[csv_line_index] = 0;
					job_info_add_sheet_record(this->info, sheet_index);
				}
			}
			else
				core_set_machine_error(this, MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA);
		}			
		else
			core_set_machine_error(this, MACHINE_ERR_CANT_COPY_PDF_TO_GIS);
	}
	else
	{
		core_set_machine_error(this, MACHINE_ERR_CANT_LOAD_BKCORE_CSV);
	}
}

void core_machine_state_ready_to_start(core * this)
{
	if((strcmp(c_string_get_char_array(this->print_controller_status), "Printing") == 0) || (io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) == 0))
	{
		c_pulse_reset(this->ena_pulse);
		this->machine_state = MACHINE_STATE_READ_CSV_LINE;
	}
}


void core_machine_wait_for_print_finish(core * this)
{	
	//if((this->feeded_main_sheet_counter+this->feeded_companion_sheet_counter)  == (this->stacked_sheet_counter + this->rejected_sheet_counter))
	{
		this->timer = c_freq_millis();
		this->machine_state = MACHINE_STATE_SAVE_Q_CSV;
	}
}

void core_machine_state_save_q_csv(core * this)
{
	if((this->timer+TIME_DELAY_ON_JOB_END) < c_freq_millis())
	{
		q_job * job = array_list_get(this->job_list, this->printed_job_index);
		
		if(job != NULL)
		{
			/* if the camera is disabled don't do the camera and quadient csv analyzing */
			if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
			{
				/* end job in Job */
				if(core_csv_analyze(this) == STATUS_SUCCESS)
				{	 
					if(core_save_response_csv(this, job->bkcore_name) >= 0)
						this->machine_state = MACHINE_STATE_CLEAR_HOT_FOLDER;
					else
						core_set_machine_error(this, MACHINE_ERR_CANT_SAVE_F_CSV);
				}
				else
				{
					core_set_machine_error(this, MACHINE_ERR_ANALYZE_CAMERA_CSV);
				}
			}
			else
			{
				/* save empty response csv */
				util_save_csv(c_string_get_char_array(this->q_hotfolder_feedback_path), job->bkcore_name, "");
				this->machine_state = MACHINE_STATE_CLEAR_HOT_FOLDER;
			}
		}
		else
		{
			core_set_machine_error(this, MACHINE_ERR_CANT_SAVE_F_CSV);
		}
	}
}

void core_machine_state_finish(core * this)
{	
	this->printed_job_index = -1;
	this->bkcore_csv_pos = 0;

	this->feed_sheet = 0;

	this->sn_trig = 0;					
	this->camera_trig = 0;					
	this->rj_trig = 0;					
	this->ti_trig = 0;
	
	c_pulse_reset(this->ena_pulse);

	this->machine_cancel_req = false;
	this->machine_print_req = false;
	this->machine_pause_req = false;
	this->print_confirmation_req = false;

	if(this->machine_state == MACHINE_STATE_JOB_FINISH)
	{
		this->machine_state = MACHINE_STATE_NEXT;
	}
	else
	{


		job_info_generate_csv(this->info);
		job_info_clear(this->info);
		
		c_string_clear(this->printed_job_name);

		this->feeded_main_sheet_counter = 0;
		this->feeded_companion_sheet_counter = 0;
		this->stacked_sheet_counter = 0;
		this->rejected_sheet_counter = 0;
		this->rejected_sheet_seq_counter = 0;
		this->tab_insert_counter = 0;
		this->inspected_sheet_counter = 0;

		this->machine_state = MACHINE_STATE_WAIT;		
	}
}

void core_machine_state_print_break(core * this)
{
	if(this->printed_job_index >= 0)
	{
		q_job * job = array_list_get(this->job_list, this->printed_job_index);

		if(job != NULL)
		{
			/* wait for machine done the printing */
			//if((this->feeded_main_sheet_counter+this->feeded_companion_sheet_counter)  == (this->stacked_sheet_counter + this->rejected_sheet_counter))
			{
				/* analyze the output csv */
				if(job->flag != 'e')
				{
					this->info->end_status = false;
					if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
					{
						core_csv_analyze(this);
						job_info_generate_missing_sheet_records(this->info);
					}
				}

				/* save feedback csv file with 'e' flag */
				char * const_char = "_xxxx_e_bkcore.csv";
				char * csv_name = (char*) malloc(sizeof(char)*(strlen(job->job_name)+strlen(const_char)+1));

				sprintf(csv_name, "%s_%d_e_bkcore_.csv", job->job_name, job->order);
				util_save_csv(c_string_get_char_array(this->q_hotfolder_feedback_path), csv_name, "");

				free(csv_name);
		
				if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
					core_printer_abbort_print(this, 0);

				/* end Job */
				this->machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
			}
		}
		else
		{
			this->machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
		}
	}
	else
	{
		/* end Job */
		this->machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
	}
}

void core_machine_tab_insert_counter(core * this)
{
	uint8_t ti_trig_val = io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc);

	if((this->ti_trig != ti_trig_val) && (ti_trig_val > 0))
	{
		this->tab_insert_counter ++;
	}

	this->ti_trig = ti_trig_val;
}

void core_machine_reject_counter(core * this)
{
	uint8_t rj_trig_val = io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_3_RJ_cnt);

	if(this->rj_trig != rj_trig_val && rj_trig_val > 0)
	{
		this->rejected_sheet_counter ++;
		this->rejected_sheet_seq_counter++;
	}

	this->rj_trig = rj_trig_val;
}

void core_machine_camera_counter(core * this)
{
	uint8_t camera_trig_val = io_card_get_input(this->io_card_ref, IO_CARD_A2, A2_IN_5_camera_trigger);

	if((this->camera_trig != camera_trig_val) && (camera_trig_val > 0))
	{
		this->inspected_sheet_counter = 0;
	}

	/* trigger previous state record */
	this->camera_trig = camera_trig_val;					
}

void core_machine_stacker_counter(core * this)
{
	uint8_t stacker_status = io_card_get_bit_value(this->io_card_ref, IO_CARD_A1, A1_IN_6_SN0, A1_IN_7_SN1, A1_IN_8_SN2); 

	/* sheets in stacker counter */
	if((this->sn_trig != stacker_status) && (stacker_status == MACHINE_SN_STACKING))
	{
		this->stacked_sheet_counter ++;
		this->rejected_sheet_seq_counter = 0;
	}

	this->sn_trig = stacker_status;					
}


void core_counter_check_sum(core * this)
{
	int32_t non_stacked = this->inspected_sheet_counter - this->stacked_sheet_counter - this->stacked_sheet_counter;
	int32_t rest = (this->feeded_main_sheet_counter + this->feeded_companion_sheet_counter) - (this->stacked_sheet_counter + this->rejected_sheet_counter);

	/* if the non stacked sheets are int the specific interval check next else set error */
	if((non_stacked >= 0) && (non_stacked <= this->non_stacked_upper_limit))
	{
		int32_t non_revided = (rest - non_stacked);
		if((non_revided < 0) || (non_revided > this->non_revided_upper_limit))
		{
			core_set_machine_error(this, MACHINE_ERR_COUNTERS_MISMATCH);
		}
	}
	else
	{
		core_set_machine_error(this, MACHINE_ERR_COUNTERS_MISMATCH);
	}
}

void core_machine_log_monitor(core * this)
{
	if(this->machine_state != this->machine_state_pre)
		c_log_add_record_with_cmd(this->log, "%d - %s", this->machine_state, core_machine_get_state_str(this));

	this->machine_state_pre = this->machine_state;
}


void * core_machine_handler(void * param)
{
	core * this = (core*) param;

	while(true)
	{
		/* read all inputs */
#if IO_SIMULATION == FALSE
		io_card_sync_inputs(this->io_card_ref);
		core_safety_system_in(this);
#endif

		core_machine_log_monitor(this);

		if((this->machine_state == MACHINE_STATE_WAIT) || (this->machine_state == MACHINE_STATE_NEXT))
		{
			core_machine_state_read_hotfolder(this);
		}

		if(this->machine_state == MACHINE_STATE_PREPARE)
		{
			core_machine_state_prepare(this);
		}
		
		if(this->machine_state == MACHINE_STATE_READY_TO_START)
		{
			core_machine_state_ready_to_start(this);
		}

		if(this->machine_state == MACHINE_STATE_READ_CSV_LINE)
		{
			core_machine_state_read_csv_line(this);
		}

		if(this->machine_state == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
		{
			core_machine_state_wait_for_confirmation(this);
		}

		if(this->machine_state == MACHINE_STATE_PRINT_COMPANION)
		{
			core_machine_state_print_companion(this);
		}

		if(this->machine_state == MACHINE_STATE_PRINT_MAIN)
		{
			core_machine_state_print_main(this);
		}
		
		if(this->machine_state == MACHINE_STATE_PAUSE)
		{
			core_machine_pause(this);
		}

		if(this->machine_state == MACHINE_STATE_WAIT_FOR_PRINT_FINISH)
		{
			core_machine_wait_for_print_finish(this);
		}

		if(this->machine_state == MACHINE_STATE_SAVE_Q_CSV)
		{
			core_machine_state_save_q_csv(this);
		}

		if(this->machine_state == MACHINE_STATE_PRINT_BREAK)
		{
			core_machine_state_print_break(this);
		}

		if((this->machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER) || (this->machine_state == MACHINE_STATE_CLEAR_TO_FINISH))
		{
			core_machine_state_clear_hotfolder(this);
		}

		if((this->machine_state == MACHINE_STATE_PRINT_FINISH) || (this->machine_state == MACHINE_STATE_JOB_FINISH))
		{
			core_machine_state_finish(this);
		}

		/* machine error state handling */
		if(this->machine_state == MACHINE_STATE_ERROR)
		{
			core_machine_state_error(this);
		}

#if IO_SIMULATION == FALSE
		/* write all outputs */
		core_safety_system_out(this);
		io_card_sync_outputs(this->io_card_ref);
#endif
		if((this->machine_state == MACHINE_STATE_WAIT) || (this->machine_state == MACHINE_STATE_ERROR) || 
			(this->machine_state == MACHINE_STATE_NEXT) || (this->machine_state == MACHINE_STATE_PAUSE))
		{
			usleep(HOT_FOLDER_READING_INTERVAL);
		}
		else
		{
			usleep(MACHINE_CYCLE_TIMING); //sleep 3ms
		}
	}

	return NULL;
}









void * core_connection_testing_thread(void * param)
{
	comm_tcp * this = (comm_tcp*) param;

	while(TRUE)
	{
		if(comm_tcp_is_connected(this) == STATUS_CLIENT_CONNECTED)
		{
			char * recv = comm_tcp_transaction(this, "ping", 255);

			if(recv != NULL)
			{
				printf("%s - %s\n", comm_tcp_get_ip_addr(this), recv);
				if(strcmp(recv, "ok") != 0)
					comm_tcp_disconnect(this);
			}
			else
			{
				comm_tcp_disconnect(this);	
			}

			sleep(1);
		}
		else
		{
			comm_tcp_connect(this);
			sleep(5);
		}
	}

	return NULL;
}


uint8_t core_iij_network_connected(core * this)
{
	return comm_tcp_is_connected(this->iij_connection);
}

uint8_t core_pci_network_connected(core * this)
{
	return comm_tcp_is_connected(this->pci_connection);
}

uint8_t core_quadient_network_connected(core * this)
{
	return comm_tcp_is_connected(this->quadient_connection);
}



	/******************* Function definitions section - xyz *****************************************************************/
/**
** @ingroup Core
** Constructor for core layer, define main functionality of the program.
** @return Pointer to new core class object.
*/
core * core_new()
{
	uint64_t s_time = c_freq_millis();

	core * this = (core*) malloc(sizeof(core));

	this->log = c_log_new(LOG_PATH, LOG_FILE_NAME, "Inicializace jádra...");
	c_log_set_log_direction(this->log, 0);
	
	if(this->log != NULL)
	{
		pthread_mutex_init(&(this->mutex), NULL);

		this->io_card_ref = io_card_new();

		this->iij_tcp_ref = comm_tcp_new();
		//this->pci_tcp_ref = comm_tcp_new();
		this->iij_connection = comm_tcp_new();
		this->pci_connection = comm_tcp_new();
		this->quadient_connection = comm_tcp_new();

	

		this->ti_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);
		this->ta_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);

		this->info = job_info_new(JOB_INFO_CSV_PATH);


		/* initialize all core variables */
		core_initialize_variables(this);

		/* load configuration from configuration file */
		config_init(&(this->cfg_ref));

		if(config_read_file(&(this->cfg_ref), CONFIGURATION_FILE_PATH) != CONFIG_TRUE)
		{
			c_log_add_record_with_cmd(this->log, "Konfigurace systému nebyla úspěšně načtena: %s", config_error_text(&(this->cfg_ref)));
			c_log_add_record_with_cmd(this->log, "Vytvářím výchozí konfiguraci.");
			core_default_config(this);
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Načítám konfigurační soubor: %s", CONFIGURATION_FILE_PATH);
			int conf_ret_val = core_load_config(this);

			if(conf_ret_val == 0)
				c_log_add_record_with_cmd(this->log, "Konfigurace načtena");
			else	
				c_log_add_record_with_cmd(this->log, "Konfigurace nebyla úspěšně načtena: %d", conf_ret_val);
		}


		if(pthread_create(&(this->pci_conn_therad), 0, core_connection_testing_thread, this->pci_connection) == 0)
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači PCI vytvořeno.");
		else
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači PCI se nepodařilo vytvořit!");

		if(pthread_create(&(this->iij_conn_thread), 0, core_connection_testing_thread, this->iij_connection) == 0)
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači IIJ vytvořeno.");
		else
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači IIJ se nepodařilo vytvořit!");

		if(pthread_create(&(this->quadient_conn_thread), 0, core_connection_testing_thread, this->quadient_connection) == 0)
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači Qudient vytvořeno.");
		else
			c_log_add_record_with_cmd(this->log, "Vlákno pro sledování připojení k počítači Quadient se nepodařilo vytvořit!");



		/* initialize and parametrizing the network layer */
#if PCI_COMM == TRUE
		/* establish tcp connection to subsystems */
		if (core_pci_try_connect(this) != STATUS_CLIENT_CONNECTED)
		{
			fputs("Camera connection error\n", stderr);
		}
#endif

		if(core_iij_try_connect(this) != STATUS_CLIENT_CONNECTED)
		{
			c_log_add_record_with_cmd(this->log, "Chyba připojení k systému GIS!");
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Systém GIS úspěšně připojen.");
		}

		/* initialize and parametrizing machine handler thread */	
		if(this->io_card_ref != NULL)
		{
			c_log_add_record_with_cmd(this->log, "IO karty úspěšně připojeny.");
			
			if (pthread_create(&(this->machine_handler_thread), 0, &core_machine_handler, (void*) this) == 0)
			{
				c_log_add_record_with_cmd(this->log, "Vlákno obsluhy stroje vytvořeno.");
			}
			else
			{
				c_log_add_record_with_cmd(this->log, "Nelze spustit vlákno obsluhy stroje!");
			}
			uint64_t e_time = c_freq_millis();
	
			c_log_add_record_with_cmd(this->log, "Jádro inicializováno. Doba trvání inicializace: %ldms", e_time-s_time);
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Nelze se připojit k IO kartám!");
			core_finalize(this);
			
			this = NULL;
		}
	}
	else
	{
		fprintf(stderr, "%s", "Nelze vytvořit log, inicializace jádra neúspěšná!!\n");

		free(this);
		this = NULL;
	}

	return this;
}


void core_finalize(core * this)
{
	comm_tcp_finalize(this->iij_tcp_ref);
	//comm_tcp_finalize(this->pci_tcp_ref);
	
	config_write_file(&(this->cfg_ref), CONFIGURATION_FILE_PATH);
	config_destroy(&(this->cfg_ref));


	array_list_destructor(&(this->job_list));
	array_list_destructor(&(this->job_list_pre));

	c_string_finalize(&(this->q_hotfolder_main_path));
	c_string_finalize(&(this->q_hotfolder_feedback_path));
	c_string_finalize(&(this->q_hotfolder_backup_path));
	c_string_finalize(&(this->pci_hotfolder_in_path));
	c_string_finalize(&(this->pci_hotfolder_out_path));
	c_string_finalize(&(this->gis_hotfolder_path));
	c_string_finalize(&(this->job_log_path));

	c_string_finalize(&(this->printed_job_name));

	c_string_finalize(&(this->print_controller_status));

	c_freq_finalize(this->ti_freq);
	c_freq_finalize(this->ta_freq);

	job_info_finalize(this->info);

	io_card_finalize(this->io_card_ref);
		
	c_log_add_record_with_cmd(this->log, "Jádro úspěšně ukončeno.");
	c_log_finalize(this->log);

	free(this);
}

uint8_t core_quadient_network_set_ip_address(core * this, char * ip_address)
{
	if(core_quadient_network_connected(this) != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = comm_tcp_set_ip_addr(this->quadient_connection, ip_address);
		
		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(this->log, "IP adresa k serveru Quadient nastavena.");

			core_update_config(this, 
					CFG_GROUP_NETWORK, 
					CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, 
					CONFIG_TYPE_STRING, 
					ip_address, 
					"Nastavení síťového ip adresy pro kontrolu připojení počítače Quadient aktualizováno.", 
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače Quadient");
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru Quadient! Chybný formát IP adresy.");
		}

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru Quadient! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}

uint8_t core_iij_network_set_ip_address(core * this, char * ip_address)
{
	if(core_iij_network_connected(this) != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = comm_tcp_set_ip_addr(this->iij_connection, ip_address);
		
		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(this->log, "IP adresa k serveru IIJ nastavena.");

			core_update_config(this, 
					CFG_GROUP_NETWORK, 
					CFG_NETWORK_IIJ_PC_IP_ADDRESS, 
					CONFIG_TYPE_STRING, 
					ip_address, 
					"Nastavení síťového ip adresy pro kontrolu připojení počítače IIJ aktualizováno.", 
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače IIJ");
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru IIJ! Chybný formát IP adresy.");
		}

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru IIJ! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}

uint8_t core_pci_network_set_ip_address(core * this, char * ip_address)
{
	if(core_pci_network_connected(this) != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = comm_tcp_set_ip_addr(this->pci_connection, ip_address);
		
		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(this->log, "IP adresa k serveru PCI nastavena.");

			core_update_config(this, 
					CFG_GROUP_NETWORK, 
					CFG_NETWORK_PCI_PC_IP_ADDRESS, 
					CONFIG_TYPE_STRING, 
					ip_address, 
					"Nastavení síťového ip adresy pro kontrolu připojení počítače PCI aktualizováno.", 
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače PCI");
		}
		else
			c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru PCI! Chybný formát IP adresy.");

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(this->log, "Nelze nastavit novou IP adresu k serveru Quadient! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}


uint8_t core_set_interface_language(core * this, int lang_index)
{
	if((lang_index >=0) && (lang_index < lang_num))
	{
		this->lang_index = lang_index;
		c_log_add_record_with_cmd(this->log, "Jazyk rozhraní změněn na: %s", multi_lang[this->lang_index].lang_name);

		core_update_config(this, 
				CFG_GROUP_LANGUAGE, 
				CFG_LANG_INDEX, 
				CONFIG_TYPE_INT, 
				&lang_index, 
				"Nastavení jazyka bylo aktualizováno.", 
				"Nepodařilo se aktualizovat nastavení jazyka!");

		return STATUS_SUCCESS;
	}

	return STATUS_GENERAL_ERROR;
}


void core_set_machine_mode(core * this, int mode)
{
	if((mode >= 0) && (mode < GR_MODE_N))
	{
		this->machine_mode = mode;
		char * mode_str = NULL;

		switch(mode)
		{
			case GR_SETUP:
				mode_str = "Nastavení";
			break;

			case GR_PRINT:
				mode_str = "Tisk";
			break;

			case GR_INSPECTION:
				mode_str = "Inspekce";
			break;

			case GR_PRINT_INSPECTION:
				mode_str = "Tisk s inspekcí";
			break;

			default:
				mode_str = "Neznámá hodnota";
			break;
		}
	
		c_log_add_record_with_cmd(this->log, "Nastaven režim Gremser stroje: %s", mode_str);

		core_update_config(this, 
				CFG_GROUP_PRINT_PARAMS, 
				CFG_PP_GR_MACHINE_MODE, 
				CONFIG_TYPE_INT, 
				&mode, 
				"Nastavení režimu Gremser dopravníku aktualizováno.", 
				"Nepodařilo se aktualizovat režim Gremser dopravníku!");
	}
}

void core_refresh_dir_list(core * this)
{
	pthread_mutex_lock(&(this->mutex));

	this->job_list_changed = 1;
	
	pthread_mutex_unlock(&(this->mutex));
}

uint8_t core_iij_set_ip_addr(core * this, char * ip_addr)
{
	uint8_t res = comm_tcp_set_ip_addr(this->iij_tcp_ref, ip_addr);	

	if(res == STATUS_SUCCESS)
		c_log_add_record_with_cmd(this->log, "Nastavena nová IP adresa pro připojení GISu: %s", ip_addr);
	else
		c_log_add_record_with_cmd(this->log, "Neúspěšné nastavení nové IP adresy pro připojení GISu: %s", ip_addr);

	core_update_config(this, 
			CFG_GROUP_NETWORK, 
			CFG_NETWORK_GIS_IP_ADDRESS, 
			CONFIG_TYPE_STRING, 
			ip_addr, 
			"Nastavení síťového ip adresy pro GIS aktualizováno.", 
			"Nepodařilo se aktualizovat ip adresu pro GIS");
	return res;
}

uint8_t core_iij_set_tcp_port(core * this, int port)
{
	uint8_t res = comm_tcp_set_tcp_port(this->iij_tcp_ref, port);

	if(res == STATUS_SUCCESS)
		c_log_add_record_with_cmd(this->log, "Nastavena nové hodnoty TCP portu pro připojení GISu: %d", port);
	else
		c_log_add_record_with_cmd(this->log, "Neúspěšné nastavení nové hodnoty TCP portu pro připojení GISu: %d", port);

	core_update_config(this, 
			CFG_GROUP_NETWORK, 
			CFG_NETWORK_GIS_TCP_PORT, 
			CONFIG_TYPE_INT, 
			&port, 
			"Nastavení síťového portu pro GIS aktualizováno.", 
			"Nepodařilo se aktualizovat síťového portu pro GIS");

	return res;
}

char * core_iij_get_ip_addr(core * this)
{
	return comm_tcp_get_ip_addr(this->iij_tcp_ref);
}

int core_iij_get_tcp_port(core * this)
{
	return comm_tcp_get_tcp_port(this->iij_tcp_ref);
}

uint8_t core_iij_connect(core * this)
{
	uint8_t res = core_iij_try_connect(this);

	if(res == STATUS_CLIENT_CONNECTED)
		c_log_add_record_with_cmd(this->log, "Spojení s GISem navázáno.");
	else
		c_log_add_record_with_cmd(this->log, "Spojení s GISem nebylo navázáno!");

	return res;
}

uint8_t core_iij_is_connected(core * this)
{
	return comm_tcp_is_connected(this->iij_tcp_ref);
}

uint8_t core_iij_disconnect(core * this)
{
	comm_tcp_disconnect(this->iij_tcp_ref);
	c_log_add_record_with_cmd(this->log, "Spojení s GISem ukončeno.");

	return STATUS_SUCCESS;
}

uint8_t core_iij_status(core * this)
{
	return 0;
}

uint8_t core_pci_set_ip_addr(core * this, char * ip_addr)
{
	return comm_tcp_set_ip_addr(this->pci_tcp_ref, ip_addr);
}

uint8_t core_pci_set_tcp_port(core * this, int port)
{
	return comm_tcp_set_tcp_port(this->pci_tcp_ref, port);
}

char * core_pci_get_ip_addr(core * this)
{
	return comm_tcp_get_ip_addr(this->pci_tcp_ref);
}

int core_pci_get_tcp_port(core * this)
{
	return comm_tcp_get_tcp_port(this->pci_tcp_ref);
}

uint8_t core_pci_connect(core * this)
{
	return core_pci_try_connect(this);
}

uint8_t core_pci_is_connected(core * this)
{
	return comm_tcp_is_connected(this->pci_tcp_ref);
}

uint8_t core_pci_disconnect(core * this)
{
	comm_tcp_disconnect(this->pci_tcp_ref);
	return STATUS_SUCCESS;
}

uint8_t core_pci_status(core * this)
{
	return 0;
}


void core_set_max_stacked_sheets(core * this, int sheet_val)
{
	this->max_stacked_sheets = sheet_val;
	c_log_add_record_with_cmd(this->log, "Nastaven maximální počet archů ve vykladači: %d", sheet_val);

	core_update_config(this, 
			CFG_GROUP_PRINT_PARAMS, 
			CFG_PP_MAX_SHEET_IN_STACKER, 
			CONFIG_TYPE_INT, 
			&sheet_val, 
			"Nastavení maximálního počtu archů ve vykladači aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení maximálního počtu archů ve vykladači!");

}

void core_set_max_rejected_sheet_seq(core * this, int sheet_val)
{
	this->rejected_sheet_for_stop = sheet_val;
	c_log_add_record_with_cmd(this->log, "Nastaven počet vadně vyhodnocených archů pro zastavení tisku : %d", sheet_val);

	core_update_config(this, 
			CFG_GROUP_PRINT_PARAMS, 
			CFG_PP_SHEET_FOR_STOP, 
			CONFIG_TYPE_INT, 
			&sheet_val, 
			"Nastavení maximální sekvence vadně vyhodnocených archů aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení maximální sekvence vadn vyhodnocených archů!");
}

void core_set_companion_sheet_source(core * this, int source)
{
	if(source >= 0 && source < SSOURCE_N)
	{
		this->companion_sheet_source = source;

		char * sheet_src_str = NULL;

		switch(source)
		{
			case SSOURCE_MAIN:
				sheet_src_str = "Hlavní nakladač";
			break;
			case SSOURCE_COMPANION:
				sheet_src_str = "Nakladač prokladových archů";
			break;
		}

		c_log_add_record_with_cmd(this->log, "Nastaven zdroj pro prokladový arch: %s", sheet_src_str);

		core_update_config(this, 
				CFG_GROUP_PRINT_PARAMS, 
				CFG_PP_COMPAION_SHEET_SOURCE, 
				CONFIG_TYPE_INT, 
				&source, 
				"Nastavení zdroje pro prokladový arch aktualizováno.", 
				"Nepodařilo se aktualizovat nastavení zdroje pro prokladový arch!");
	}
	else
	{
		c_log_add_record_with_cmd(this->log, "Neúspěšný pokus o nastavení nového zdroje pro prokladové archy: %d", source);
	}
}

void core_set_sheet_source_confirmation(core * this, bool confirm)
{
	this->sheet_source_confirmation = confirm;
	c_log_add_record_with_cmd(this->log, "Potvrzování naložení prokladového archu z hlavního nakladače: %s", confirm == true ? "Aktivováno" : "Deaktivováno");

	core_update_config(this, 
			CFG_GROUP_PRINT_PARAMS, 
			CFG_PP_SHEET_SOURCE_CONFIRMATION, 
			CONFIG_TYPE_BOOL, 
			&confirm, 
			"Nastavení potvrzování naložení prokladového archu z hlavního nakladače aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení potvrzování naložení prokladového archu z hlavního nakladače!");
}


uint8_t core_set_q_main_hotfolder_path(core * this, const char * path)
{
	c_string_set_string(this->q_hotfolder_main_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k výstupnímu hotfolderu pro Quadient: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_QUADIENT_MAIN, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy výstupního hotfolderu pro Quadient aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro Quadient!");

	return STATUS_SUCCESS;
}

uint8_t core_set_q_feedback_hotfolder_path(core * this, const char * path)
{	
	c_string_set_string(this->q_hotfolder_feedback_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k hotfolderu pro ukládání zpětnovazebných csv pro Quadient: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_QUADIENT_FEEDBACK, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy zpětnovazebního hotfolderu pro Quadient aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy zpětnovazebního hotfolderu pro Quadient!");


	return STATUS_SUCCESS;
}

uint8_t core_set_q_backup_hotfolder_path(core * this, const char * path)
{
	c_string_set_string(this->q_hotfolder_backup_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k hotfolderu pro zálohování tiskových dat z Quadientu: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_QUADIENT_BACKUP, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy hotfolderu pro zálohování dat z Quadient aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy hotfolderu zálohování pro Quadient!");

	return STATUS_SUCCESS;
}

uint8_t core_set_pci_hotfolder_in_path(core * this, const char * path)
{
	c_string_set_string(this->pci_hotfolder_in_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty ke vstupnímu hotfolderu kamery: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_PCI_IN, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy vstupního hotfolderu pro kameru aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro kameru!");

	return STATUS_SUCCESS;
}

uint8_t core_set_pci_hotfolder_out_path(core * this, const char * path)
{
	c_string_set_string(this->pci_hotfolder_out_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k výstupnímu hotfolderu kamery: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_PCI_OUT, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy výstupního hotfolderu pro kameru aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro kameru!");

	return STATUS_SUCCESS;
}

uint8_t core_set_gis_hotfolder_path(core * this, const char * path)
{
	c_string_set_string(this->gis_hotfolder_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k hotfolderu gisu: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_QUADIENT_MAIN, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy vstupního hotfolderu pro GIS aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy vstupního hotfolderu pro GIS!");

	return STATUS_SUCCESS;
}

uint8_t core_set_job_report_hotfolder_path(core * this, const char * path)
{
	c_string_set_string(this->job_log_path, (char*) path);
	c_log_add_record_with_cmd(this->log, "Nastavení nové cesty k reportovacímu hotfolderu: %s", path);

	core_update_config(this, 
			CFG_GROUP_HOTFOLDER, 
			CFG_HOT_QUADIENT_MAIN, 
			CONFIG_TYPE_STRING, 
			(void*) path, 
			"Nastavení adresy hotfolderu pro reportování csv z tisku aktualizováno.", 
			"Nepodařilo se aktualizovat nastavení adresy hotfolderu pro reportování csv z tisku!");

	return STATUS_SUCCESS;
}


char * core_get_q_main_hotfolder_path(core * this)
{
	return c_string_get_char_array(this->q_hotfolder_main_path);
}

char * core_get_q_feedback_hotfolder_path(core * this)
{
	return c_string_get_char_array(this->q_hotfolder_feedback_path);
}

char* core_get_q_backup_hotfolder_path(core * this)
{
	return c_string_get_char_array(this->q_hotfolder_backup_path);
}

char * core_get_pci_hotfolder_in_path(core * this)
{
	return c_string_get_char_array(this->pci_hotfolder_in_path);
}

char * core_get_pci_hotfolder_out_path(core * this)
{
	return c_string_get_char_array(this->pci_hotfolder_out_path);
}

char * core_get_gis_hotfolder_path(core * this)
{
	return c_string_get_char_array(this->gis_hotfolder_path);
}

char * core_get_job_report_hotfolder_path(core * this)
{
	return c_string_get_char_array(this->job_log_path);
}


uint8_t core_machine_status_val(core * this)
{
	return this->machine_state;
}

uint8_t core_print_start(core * this, const char * job_name)
{
	uint8_t status = STATUS_SUCCESS;
	int job_name_len = strlen(job_name);

	if(c_string_len(this->printed_job_name) == 0)
	{
		if(job_name_len > 0)
		{
			c_string_set_string(this->printed_job_name, (char*) job_name);
			
			int ttl = 0;
			while(((this->machine_state == MACHINE_STATE_WAIT)) && ttl < 5)
			{
				this->machine_print_req = true;
				usleep(HOT_FOLDER_READING_INTERVAL);
				ttl++;
			}

			this->machine_print_req = false;
	
			if(ttl >= 5)
			{
				status = STATUS_GENERAL_ERROR;
				c_log_add_record_with_cmd(this->log, "Pokus o spuštění tisku neúspěný!");
			}
			else
			{
				if(this->machine_state == MACHINE_STATE_ERROR)
				{
					status = STATUS_GENERAL_ERROR;
					c_log_add_record_with_cmd(this->log, "Pokus o spuštění tisku zkončil chybou: %d - %s", this->error_code,(char*) core_get_error_str(this));
				}
				else
				{
					c_log_add_record_with_cmd(this->log, "Spuštěn tisk jobu %s.", c_string_get_char_array(this->printed_job_name));
				}
			}	
		}
		else
		{
			status = STATUS_GENERAL_ERROR;
			c_log_add_record_with_cmd(this->log, "Pokus o spuštění tisku neúspěšný! Nebylo specifikováno jméno jobu.");
		}
	}
	else
	{
		status = STATUS_GENERAL_ERROR;
		c_log_add_record_with_cmd(this->log, "Pokus o spuštění tisku neúspěšný! Tisk jiného jobu již běží (%s).", c_string_get_char_array(this->printed_job_name));
	}

	return status;
}

uint8_t core_print_pause(core * this)
{
	uint8_t res = STATUS_SUCCESS;
	
	if((this->machine_state != MACHINE_STATE_ERROR) && (this->machine_state != MACHINE_STATE_WAIT))
	{
		this->machine_pause_req = true;
		c_log_add_record_with_cmd(this->log, "Tisk jobu %s pozastaven.", c_string_get_char_array(this->printed_job_name));
	}
	else
	{
		res = STATUS_GENERAL_ERROR;
		if(this->machine_state == MACHINE_STATE_ERROR)
		{
			c_log_add_record_with_cmd(this->log, "Pokus o pozastavení jobu %s neúspěšný! Obslužné vlákno se nachází v chybovém stavu: %d - %s",
					 c_string_get_char_array(this->printed_job_name), this->machine_state, (char*) core_get_error_str(this));

		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Pokus o pozastavení jobu %s neúspěšný! Obslužné vlákno se nachází ve stavu: %d - %s",
					 c_string_get_char_array(this->printed_job_name), this->machine_state, core_machine_get_state_str(this));

		}
	}

	return res;
}

uint8_t core_print_continue(core * this)
{
	int8_t res = STATUS_SUCCESS;

	if(this->machine_state == MACHINE_STATE_PAUSE)
	{
		int ttl = 0;
		this->machine_pause_req = false;
	
		while((ttl < 3) || (this->machine_state != MACHINE_STATE_PAUSE))
		{
			usleep(HOT_FOLDER_READING_INTERVAL/1000);
			ttl++;
		}

		if(this->machine_state == MACHINE_STATE_PAUSE)
		{
			c_log_add_record_with_cmd(this->log, "Nelze obnovit job: %s", c_string_get_char_array(this->printed_job_name));
			res = STATUS_GENERAL_ERROR;
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Obnovení jobu: %s", c_string_get_char_array(this->printed_job_name));
		}
	}
	else
	{	
		res = STATUS_GENERAL_ERROR;
		if(this->machine_state == MACHINE_STATE_ERROR)
		{
			c_log_add_record_with_cmd(this->log, "Obnovení jobu neúspěšné. Obslužné vlákno se nachízí v chybovém stavu: %d - %s", this->machine_state, core_get_error_str(this));
		}
		else
		{
			c_log_add_record_with_cmd(this->log, "Obnovení jobu neúspěšné. Obslužné vlákno se nachízí ve stavu: %d - %s", 
						this->machine_state, core_machine_get_state_str(this));
		}
	}

	return res;
}

uint8_t core_print_cancel(core * this)
{
	uint8_t res = STATUS_SUCCESS;

	if(c_string_len(this->printed_job_name) > 0)
	{
		c_log_add_record_with_cmd(this->log, "Požadavek na ukončení jobu %s.", c_string_get_char_array(this->printed_job_name));
		this->machine_cancel_req = true;
	}
	else
	{
		res = STATUS_GENERAL_ERROR;
		c_log_add_record_with_cmd(this->log, "Pokus o přerušení jobu neúspšný! Neprobíhá tisk žádného jobu.");
	}

	return res;
}


void core_sheet_source_confirmation(core * this)
{
	this->print_confirmation_req = true;
}

uint8_t core_print_reset_error(core * this)
{
	uint8_t res = STATUS_SUCCESS;
	int ttl = 0;

	this->machine_cancel_req = false;
	this->machine_print_req = false;
	this->machine_pause_req = false;

	while((this->machine_state == MACHINE_STATE_ERROR) && (ttl < 2))
	{
		this->machine_error_reset_req = true;
		usleep(HOT_FOLDER_READING_INTERVAL);
		ttl++;
	}
						
	this->machine_error_reset_req = false;

	if(this->machine_state == MACHINE_STATE_ERROR)
	{
		c_log_add_record_with_cmd(this->log, "Pokus o reset chybového stavu neúspěšný. Obslužné vlákno se nachází v chybě: %d - %s", this->error_code, core_get_error_str(this));
		res = STATUS_GENERAL_ERROR;
	}
	else
	{	
		c_log_add_record_with_cmd(this->log, "Reset chybového stavu.");
	}
	
	return res;
}

int core_get_error_val(core * this)
{
	return this->error_code;
}


const char* core_get_error_str(core * this)
{
	const char * error_str = NULL;

	switch(this->error_code)
	{
		case MACHINE_ERR_NO_ERROR:
			error_str = multi_lang[this->lang_index].err_no_error;
			break;
		
		case MACHINE_ERR_CANT_SAVE_F_CSV:
			error_str = multi_lang[this->lang_index].err_cant_save_f_csv;
			break;
 
		case MACHINE_ERR_ANALYZE_CAMERA_CSV:
			error_str = multi_lang[this->lang_index].err_analyze_camera_csv;
			break;
	
		case MACHINE_ERR_CANT_LOAD_BKCORE_CSV:
			error_str = multi_lang[this->lang_index].err_cant_load_bkcore_csv;
			break;

		case MACHINE_ERR_CANT_COPY_PDF_TO_GIS:
			error_str = multi_lang[this->lang_index].err_cant_copy_pdf_to_gis;
			break;

		case MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA:
			error_str = multi_lang[this->lang_index].err_cant_copy_csv_to_camera;
			break;

		case MACHINE_ERR_JOB_ORDER_MISMATCH:
			error_str = multi_lang[this->lang_index].err_job_order_mismatch;
			break;

		case MACHINE_ERR_STACKER_JAM_PILE:
			error_str = multi_lang[this->lang_index].err_stacker_jam_pile;
			break;

		case MACHINE_ERR_STACKER_JAM_CONVEYOR:
			error_str = multi_lang[this->lang_index].err_stacker_jam_conveyor;
			break;

		case MACHINE_ERR_STACKER_BOTTOM: 
			error_str = multi_lang[this->lang_index].err_stacker_bottom;
			break;

		case MACHINE_ERR_STACKER_MULTIFUNCTION:
			error_str = multi_lang[this->lang_index].err_stacker_multifunction;
			break;

		case MACHINE_ERR_STACKER_OFF:
			error_str = multi_lang[this->lang_index].err_stacker_off;
			break;

		case MACHINE_ERR_FEEDER_E_STOP:
			error_str = multi_lang[this->lang_index].err_feeder_e_stop;
			break;	

		case MACHINE_ERR_FEEDER_OFF:
			error_str = multi_lang[this->lang_index].err_feeder_off;
			break;

		case MACHINE_ERR_FEEDER_MULTIFUNCTION:
			error_str = multi_lang[this->lang_index].err_feeder_multifunction;
			break;

		case MACHINE_ERR_FEEDER_SHEET_MISSING:
			error_str = multi_lang[this->lang_index].err_feeder_sheet_missing;
			break;

		case MACHINE_ERR_FEEDER_DOUBLE_SHEET:
			error_str = multi_lang[this->lang_index].err_feeder_double_sheet;
			break;

		case MACHINE_ERR_E_STOP:
			error_str = multi_lang[this->lang_index].err_machine_e_stop;
			break;

		case MACHINE_ERR_REJECT_BIN_JAM:
			error_str = multi_lang[this->lang_index].err_reject_bin_jam;
			break;

		case MACHINE_ERR_FEEDER_JAM:
			error_str = multi_lang[this->lang_index].err_feeder_jam;
			break;

		case MACHINE_ERR_TI:
			error_str = multi_lang[this->lang_index].err_ti;
			break;

		case MACHINE_ERR_TA:
			error_str = multi_lang[this->lang_index].err_ta;
			break;

		case MACHINE_ERR_COUNTERS_MISMATCH:
			error_str = multi_lang[this->lang_index].err_counters_mismatch;
			break;

		case MACHINE_ERR_LOW_PRINT_QUALITY:
			error_str = multi_lang[this->lang_index].err_low_print_quality;
			break;
	
		case MACHINE_ERR_GIS_DISCONNECTED:
			error_str = multi_lang[this->lang_index].err_gis_disconnected;
			break;

		case MACHINE_ERR_CSV_SHEET_NUM:
			error_str = multi_lang[this->lang_index].err_csv_sheet_num;
			break;

		case MACHINE_ERR_SHEET_FEEDER_REDIRECTION:
			error_str = multi_lang[this->lang_index].err_sheet_feeder_redirection;
			break;

		case MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang[this->lang_index].err_pci_computer_not_responding;
			break;

		case MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang[this->lang_index].err_iij_computer_not_responding;
			break;

		case MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang[this->lang_index].err_quadient_computer_not_responding;
			break;

		default:
			error_str = multi_lang[this->lang_index].err_unknown_error;
	}

	return error_str;
}

char* core_machine_get_state_str(core * this)
{
	char * status_str = NULL;

	switch(this->machine_state)
	{
		case MACHINE_STATE_WAIT:
			status_str = "Čekání spuštění jobu";
		break;

		case MACHINE_STATE_PREPARE:
			status_str = "Příprava dat pro tisk";
		break;

		case MACHINE_STATE_PRINT_MAIN:
			status_str = "Tisk";
		break;
			
		case MACHINE_STATE_SAVE_Q_CSV:
			status_str = "Generování zpětnovazebního csv pro Quadient.";
		break;

		case MACHINE_STATE_CLEAR_HOT_FOLDER:
			status_str = "Čištění hotfolderů.";
		break;

		case MACHINE_STATE_NEXT:
			status_str = "Čekání a další soubory jobu.";
		break;

		case MACHINE_STATE_PRINT_FINISH:
			status_str = "Dokončování procesu tisku.";
		break;

		case MACHINE_STATE_CLEAR_TO_FINISH:
			status_str = "Čištění hotfolderu pro ukončení tisku.";
		break;

		case MACHINE_STATE_JOB_FINISH:
			status_str = "Dokončování tiskového jobu";
		break;

		case MACHINE_STATE_ERROR:
			status_str = "Stroj je v chybovém stavu!";
		break;
		
		case MACHINE_STATE_PAUSE:
			status_str = "Tisk pozastaven.";
		break;
		
		case MACHINE_STATE_READ_CSV_LINE:
			status_str = "Čtění řádku z datového csv souboru.";
		break;

		case MACHINE_STATE_PRINT_COMPANION:
			status_str = "Tisk prokladového archu.";
		break;

		case MACHINE_STATE_PRINT_BREAK:
			status_str = "Přerušení procesu tisku.";
		break;	

		case MACHINE_STATE_READY_TO_START:
			status_str = "Čekání na inicializaci stroje.";
		break;

		default:
			status_str = "Stroj je v neznámém stavu!!!";
	} 

	return status_str; 
}

char * core_get_return_val_string(uint8_t status)
{
	char * msg = NULL;

	switch(status)
	{
		case STATUS_SUCCESS:
			msg = "Operace byla úspšná";
		break;

		case STATUS_CLIENT_CONNECTED:
			msg = "Klient přiojen.";
		break;

		case STATUS_SOCKET_ERROR:
			msg="Chyba síťového soketu!";
		break;

		case STATUS_CONNECTION_ERROR:
			msg = "Chyba síťového spojení!";
		break;

		case STATUS_PORT_OUT_OF_RANGE:
			msg = "Hodnota tcp portu mimo rozsah!";
		break;

		case STATUS_IP_ADDRESS_ERROR:
			msg = "Chyba překladu IP adresy!";
		break;

		case STATUS_CLIENT_DISCONNECTED:
			msg = "Klient odpojen.";
		break;
		
		case STATUS_COMMUNICATION_ERROR:
			msg = "Chyba komunikace!";
		break;
		
		default:
			msg = "Unknown result!";
	}

	return msg;
}

job_info * job_info_new(char * csv_address)
{
	job_info * this = (job_info*) malloc(sizeof(job_info));

	this->csv_addr = (char *) malloc(sizeof(char) * (strlen(csv_address)+1));
	strcpy(this->csv_addr, csv_address);

	this->order_name = c_string_new();
	this->csv_content = c_string_new();
	this->job_list = array_list_new();
	this->csv_name = c_string_new();


	this->total_sheet_number = 0;
	this->total_stemps_number = 0;
	this->printed_sheet_number = 0;
	this->rejected_sheet_number = 0;

	this->end_status = true;

	return this;
}


int job_info_get_printed_sheet_number(job_info * this)
{
	return this->printed_sheet_number;
}

int job_info_get_job_index(job_info * this)
{
	return (array_list_size(this->job_list) - 1);
}

void job_info_set_order_name(job_info * this, char* order_name)
{
	c_string_set_string(this->order_name, order_name);
}

void job_info_clear(job_info * this)
{
	c_string_clear(this->order_name);
	c_string_clear(this->csv_content);
	c_string_clear(this->csv_name);

	this->total_sheet_number = 0;
	this->total_stemps_number = 0;


	this->printed_sheet_number = 0;
	this->rejected_sheet_number = 0;

	this->end_status = true;

	int i, j;
	for(i = 0; i < array_list_size(this->job_list); i++)
	{	
		array_list * job = array_list_get(this->job_list, i);

		if(job != NULL)
		{
			for(j = 0; j < array_list_size(job); j++)
			{
				info_struct * sheet_info = array_list_get(job, j);

				if(sheet_info != NULL)
				{
					if(sheet_info->sheet_order != NULL)
						free(sheet_info->sheet_order);
				
					free(sheet_info);
				}
			}

			array_list_destructor_v2(job);
		}
	}

	array_list_clear(this->job_list);
}	


void job_info_add_job_record(job_info * this)
{
	array_list_add(this->job_list, array_list_new());
}

void job_info_generate_missing_sheet_records(job_info * this)
{
	job_info_add_job_record(this);
	
	int k,i,j;

	for(k = 0; k < this->total_sheet_number; k++)
	{
		char sheet_order_str[12];
		sprintf(sheet_order_str, "%d", k+1);		
		array_list * job = NULL;
		bool sheet_printed = false;

		for(i = 0; i < array_list_size(this->job_list)-1; i++)
		{
			job = array_list_get(this->job_list, i);

			if(job != NULL)
			{
				for(j = 0; j < array_list_size(job); j++)
				{
					info_struct * sheet_info = array_list_get(job, j);
					if(sheet_info == NULL)
						printf("sheet info NULL - %d\n", j);

					if(strcmp(sheet_order_str, sheet_info->sheet_order) == 0)
					{
						sheet_printed = true;
						break;		
					}
				}

				if(sheet_printed == true)
					break;				
			}
			else
			{
				break;
			}
		}

		if(sheet_printed == false)
		{
			job_info_add_sheet_record(this, sheet_order_str);
			job_info_set_sheet_record_result(this, "FAIL", k);
		}
	}
}

void job_info_add_sheet_record(job_info * this, char * sheet_order)
{
	info_struct * sheet_info = (info_struct*) malloc(sizeof(info_struct));
	sheet_info->sheet_order = (char*) malloc(sizeof(char)*(strlen(sheet_order)+1));
	strcpy(sheet_info->sheet_order, sheet_order);
	sheet_info->result = NULL;
	array_list_add(array_list_get(this->job_list, job_info_get_job_index(this)), sheet_info);
}

void job_info_set_sheet_record_result(job_info * this, char * result, int index)
{
	info_struct * sheet_info = array_list_get(array_list_get(this->job_list, job_info_get_job_index(this)), index);

	if(sheet_info != NULL)
	{
		if(strcmp(result, "PASS") == 0)
			this->printed_sheet_number ++;
		else
			this->rejected_sheet_number ++;

		sheet_info->result = result;
	}
}	

int8_t job_info_generate_csv(job_info * this)
{
	if(array_list_size(this->job_list) > 0)
	{
		c_string_add(this->csv_content, "Job: ");
		c_string_add(this->csv_content, c_string_get_char_array(this->order_name));

		if(this->end_status == true)
			c_string_add(this->csv_content, "Corectly finished\n");
		else
			c_string_add(this->csv_content, "Prematurely finished\n");

		char str_total_number[10];
		sprintf(str_total_number, "%d\n", this->rejected_sheet_number);
		c_string_add(this->csv_content,  "Total rejected sheet number: ");
		c_string_add(this->csv_content, str_total_number);		

		sprintf(str_total_number, "%d\n",  this->total_sheet_number);	
		c_string_add(this->csv_content, "Total sheet number: ");
		c_string_add(this->csv_content, str_total_number);

		sprintf(str_total_number,"%d\n", this->total_stemps_number);
		c_string_add(this->csv_content, "Total stemps number: ");
		c_string_add(this->csv_content, str_total_number);

		c_string_add(this->csv_content, "Finish time: ");
		char * time_date_str = util_get_time_string();
		c_string_add(this->csv_content, time_date_str);
		free(time_date_str);

		int i,j;
		for(i = 0; i < array_list_size(this->job_list); i++)
		{
			array_list * job = array_list_get(this->job_list, i);
			char order_string[5];
			sprintf(order_string, "%.4d", i+1);
			c_string_add(this->csv_content, "\nOrder: ");
			c_string_add(this->csv_content, order_string);	

			for(j = 0; j < array_list_size(job); j++)
			{
				info_struct * sheet_info = array_list_get(job, j);
				c_string_add(this->csv_content, "\n");
				c_string_add(this->csv_content, sheet_info->sheet_order);
				c_string_add(this->csv_content, "	");

				if(sheet_info->result != NULL)
					c_string_add(this->csv_content, sheet_info->result);
				else
					c_string_add(this->csv_content, "FAIL");
			}
		}

		return util_save_csv(this->csv_addr, c_string_get_char_array(this->csv_name), c_string_get_char_array(this->csv_content));
	}
	
	return -1;
}

void job_info_generate_csv_name(job_info * this)
{
	c_string_set_string(this->csv_name, c_string_get_char_array(this->order_name));
	c_string_concat(this->csv_name, ".csv");
}

void job_info_finalize(job_info * this)
{
	job_info_clear(this);
	c_string_finalize(&(this->order_name));
	c_string_finalize(&(this->csv_content));
	c_string_finalize(&(this->csv_name));

	if(this->csv_addr != NULL)
		free(this->csv_addr);

	free(this);
}


	/******************* Function definitions section - abcd ****************************************************************/

/**
** @ingroup Communication
** Constructor for comm_tcp_client class, initialization of socket structure.
** @return Pointer to comm_tcp_client class object.
*/
comm_tcp * comm_tcp_new()
{
	comm_tcp * this = (comm_tcp*) malloc(sizeof(comm_tcp));

	pthread_mutex_init(&(this->mutex), NULL);

	/* initialization of the socket data */
	this->io_buffer = (char*) malloc(sizeof(char)*COMM_TCP_IO_BUFFER_SIZE);
	memset(this->io_buffer, 0, COMM_TCP_IO_BUFFER_SIZE);
	memset(&(this->serv_addr), '0', sizeof(this->serv_addr));
	this->sockfd = 0;

	return this;
}


/**
** @ingroup Communication
** Function for connect the client (open POSIX tcp socket).
** @param client Pointer to comm_tcp_client class object
** @return STATUS_SOCKET_ERROR if socket could not be created 
** @return STATUS_CONNECTION_ERROR if connection could not be created or 
** @return STATUS_CLIENT_CONNECTED if the connection is established.
*/
uint8_t comm_tcp_connect(comm_tcp * client)
{
	/* create tcp socket */
	if((client->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		close(client->sockfd);
		client->sockfd = 0;
		return STATUS_SOCKET_ERROR;
	}

	client->serv_addr.sin_family = AF_INET;
	
	/* set socket operations timeout */
	struct timeval timeout;      
	timeout.tv_sec = TCP_NETWORK_TTL_S;
   	timeout.tv_usec = TCP_NETWORK_TTL_U;

	if (setsockopt (client->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        	fputs("setsockopt failed\n", stderr);

	if (setsockopt (client->sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
        	fputs("setsockopt failed\n", stderr);

	/* tcp socket connect */
	if(connect(client->sockfd, (struct sockaddr *) &(client->serv_addr), sizeof(client->serv_addr)) < 0)
	{
		close(client->sockfd);
		client->sockfd = 0;
		return STATUS_CONNECTION_ERROR;
	}

	return STATUS_CLIENT_CONNECTED;
}


uint8_t comm_tcp_is_connected(comm_tcp * client)
{
	int error_code = -1;
	int error_code_size = sizeof(error_code);
	getsockopt(client->sockfd, SOL_SOCKET, SO_ERROR, &error_code, (socklen_t*) &error_code_size);

	if(error_code == 0)
	{
		return STATUS_CLIENT_CONNECTED;
	}
	else
	{
		return STATUS_CLIENT_DISCONNECTED;
	}
}

void comm_tcp_disconnect(comm_tcp * client)
{
	if(client->sockfd != 0)
	{
		close(client->sockfd);
		client->sockfd = 0;
	}	
}

/**
** @ingroup Communication
** Function for setting new ip address to TCP socket.
** @param client Pointer to comm_tcp_client class object
** @param ip_addr New IP address
** @return STATUS_IP_ADDRESS_ERROR if the ip address has wrong format
** @return STATUS_SUCCESS if the ip address is successfully changed
** @return STATUS_CLIENT_CONNECTED if the client is connected and the ip address could not be changed
*/
uint8_t comm_tcp_set_ip_addr(comm_tcp * client, char * ip_addr)
{
	if(client->sockfd == 0)
	{
		if(inet_pton(AF_INET, ip_addr, &(client->serv_addr.sin_addr)) <= 0)
		{
			return STATUS_IP_ADDRESS_ERROR;
		}
		else
		{
			return STATUS_SUCCESS;
		}
	}
	else
	{
		return STATUS_CLIENT_CONNECTED;
	}	
}


char * comm_tcp_get_ip_addr(comm_tcp * client)
{
	return inet_ntoa(client->serv_addr.sin_addr);
}

int comm_tcp_get_tcp_port(comm_tcp * client)
{
	return htons(client->serv_addr.sin_port);
}

/**
** @ingroup Communication
** Function for settings new tcp port value to the TCP socket.
** @param client Pointer to comm_tcp_client class object
** @param tcp_port New tcp port value
** @return STATUS_SUCCESS if the tcp port is successfully changed
** @return STATUS_PORT_OUT_OF_RANGE if the port has value lower then 0 or higher then 65535
** @return STATUS_CLIENT_CONNECTED if the client is connected and the tcp port could not be changed
*/
uint8_t comm_tcp_set_tcp_port(comm_tcp * client, int tcp_port)
{
	if (client->sockfd == 0)
	{
		if(tcp_port >= 0 && tcp_port <= 65535)
		{
    			client->serv_addr.sin_port = htons(tcp_port); 
			return STATUS_SUCCESS;
		}
		else
		{
			/* tcp port value out of range */
			return STATUS_PORT_OUT_OF_RANGE;
		}
	}
	else
	{
		return STATUS_CLIENT_CONNECTED;
	}
}

/**
** @ingroup Communication
** This function does tcp transaction (send message to the server and wait for some respond).
** Tcp socket communication is blocked, but it's defined ttl interval for receave the respond without error,
** this ttl interval is defined in config.h file.
** @param client Pointer to comm_tcp_client class object.
** @param out_buffer Pointer to buffer with respond message.
** @param max_out Maximum characters to receave.
** @param msg Pointer to message buffer.
** @return Number of receaved characters.
*/
char * comm_tcp_transaction(comm_tcp * client, char* msg, int max_rcv)
{
	pthread_mutex_lock(&(client->mutex));

	/* critical section */
	int size_out = send(client->sockfd, msg, strlen(msg), 0);
	char * str_in = NULL;

	if(size_out > 0)
	{
		int size_in = recv(client->sockfd, client->io_buffer, max_rcv-1, 0);
		if(size_in > 0)
		{
			str_in = client->io_buffer;
		}		
	}
	
	pthread_mutex_unlock(&(client->mutex));
	return str_in;
}


int comm_tcp_send(comm_tcp * client, char* str)
{
	pthread_mutex_lock(&(client->mutex));
	/* critical section */
	int size_out = send(client->sockfd, str, strlen(str), 0);

	pthread_mutex_unlock(&(client->mutex));

	if(size_out > 0)
	{
		return size_out;
	}
	else
	{
		return -1;
	}
}


/**
** criticaly for timing in real time operations 
*/
char * comm_tcp_recv(comm_tcp * client)
{
	pthread_mutex_lock(&(client->mutex));

	/* critical section */
	int size_in = recv(client->sockfd, client->io_buffer, COMM_TCP_IO_BUFFER_SIZE-1, 0);

	pthread_mutex_unlock(&(client->mutex));

	if(size_in > 0)
	{
		client->io_buffer[size_in] = 0;
		return client->io_buffer;
	}
	else
	{
		return NULL;
	}
}

/**
** @ingroup Communication
** Safety closing the tcp socket and release the alocated memory of the instance.
** @param client Pointer to comm_tcp_client class object
*/
void comm_tcp_finalize(comm_tcp * client)
{
	/* close socket */
	if(client != NULL)
	{
		close(client->sockfd);	
		free(client->io_buffer);

		/* release memory */
		free(client);
	}
}


io_card * io_card_new()
{
	io_card * this = (io_card*) malloc(sizeof(io_card));
	
	this->timer = c_freq_millis();

	/* connect cards */
	this->ref_a1 = comedi_open(IO_CART_A1_REF);
	this->ref_a2 = comedi_open(IO_CART_A2_REF);

	if(((this->ref_a1 != NULL) && (this->ref_a2 != NULL)) || (IO_SIMULATION == TRUE))
	{
		this->A1_in = 0;
		this->A1_out = 0;
		this->A2_in = 0;
		this->A2_out = 0;	

		return this;
	}
	else
	{
		fputs("Can't connect IO cards\n", stderr);
		free(this);
		return NULL;
	}
}	

void io_card_sync_inputs(io_card * this)
{
	if(comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_INPUT, 0x0, &this->A1_in, 0) < 0)
	{
		fputs("Can't read inputs from card A1!\n", stderr);
	}

	if(comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_INPUT, 0x0, &this->A2_in, 0) < 0)
	{
		fputs("Can't read inputs from card A2!\n", stderr);
	} 
}

void io_card_sync_outputs(io_card * this)
{
	if(comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A1_out, 0) < 0)
	{
		fputs("Can't write utputs to card A1!\n", stderr);
	}

	if(comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A2_out, 0) < 0)
	{
		fputs("Can't write utputs to card A2!\n", stderr);
	}
}

uint8_t io_card_get_output(io_card * this, int card, int addr)
{
	if(card == IO_CARD_A1)
	{
		return ((this->A1_out & SO(addr)) > 0 ? 1 : 0);
	}
	else
	{
		return ((this->A2_out & SO(addr)) > 0 ? 1 : 0);
	}
}


uint8_t io_card_get_input(io_card * this, int card, int addr)
{
	/* reverse logic 0 = 1 */
	if(card == IO_CARD_A1)
	{
		return ((this->A1_in & SO(addr)) > 0 ? 0 : 1);
	}
	else
	{
		return ((this->A2_in & SO(addr)) > 0 ? 0 : 1);
	}
}

void io_card_set_output(io_card * this, int card, int addr, uint8_t val)
{
	if(card == IO_CARD_A1)
	{
		if(val > 0)
			this->A1_out |= SO(addr);
		else
			this->A1_out &= SZ(addr);
	}
	else
	{
		if(val > 0)
			this->A2_out |= SO(addr);
		else
			this->A2_out &= SZ(addr);
	}
}


uint8_t io_card_get_bit_value(io_card * this, int card, int bit1, int bit2, int bit3)
{
	int bit_val = 0;

	if(io_card_get_input(this, card, bit1) > 0)
		bit_val += 1;

	if(io_card_get_input(this, card, bit2) > 0)
		bit_val += 2;

	if(io_card_get_input(this, card, bit3) > 0)
		bit_val += 4;

	return bit_val;
}

void io_card_finalize(io_card * this)
{
	comedi_close(this->ref_a1);
	comedi_close(this->ref_a2);

	free(this);
}

/************************************************************** end of section with io_card functions definitions *****************
*************************************************************** section with q_job functions definitiosn *************************/

q_job * q_job_new(char* name, uint32_t order, char flag)
{
	q_job * job = (q_job*) malloc(sizeof(q_job));
	job->job_name = name;
	job->camera_name = NULL;
	job->pdf_name = NULL;
	job->bkcore_name = NULL;
	job->order = order;
	job->flag = flag;

	return job;
}


uint8_t q_job_compare(q_job * job_1, q_job * job_2)
{
	if((job_1 != NULL) && (job_2 != NULL))
	{
		if(strcmp(job_1->job_name, job_2->job_name) == 0)
		{
			if(job_1->order == job_2->order && job_1->flag == job_2->flag)
			{
				return 1;
			}
		}
	}

	return 0;
}

q_job * q_job_copy(q_job * job)
{
	uint8_t str_len = strlen(job->job_name)+1;

	char * job_name  = (char*) malloc(sizeof(char)*str_len);
	memcpy(job_name, job->job_name, str_len);

	q_job * new_job = q_job_new(job_name, job->order, job->flag);

	if(job->camera_name != NULL)
	{
		str_len = strlen(job->camera_name)+1;
		new_job->camera_name = (char*) malloc(sizeof(char) * str_len);
		memcpy(new_job->camera_name, job->camera_name, str_len);
	}

	if(job->pdf_name != NULL)
	{
		str_len = strlen(job->pdf_name)+1;
		new_job->pdf_name = (char*) malloc(sizeof(char) * str_len);
		memcpy(new_job->pdf_name, job->pdf_name, str_len);
	}

	if(job->bkcore_name != NULL)
	{
		str_len = strlen(job->bkcore_name)+1;
		new_job->bkcore_name = (char*) malloc(sizeof(char)*str_len);
		memcpy(new_job->bkcore_name, job->bkcore_name, str_len);
	}

	return new_job;
}


void q_job_finalize(void * job)
{	
	q_job * job_ref = (q_job*) job;

	if(job_ref->job_name != NULL)
		free(job_ref->job_name);
	
	if(job_ref->camera_name != NULL)
		free(job_ref->camera_name);

	if(job_ref->pdf_name != NULL)
		free(job_ref->pdf_name);
	
	if(job_ref->bkcore_name != NULL)
		free(job_ref->bkcore_name);
	
	free(job);
}

/************************************************************* end of section with q_job functions definitions **************************
************************************************************** section with utility functions definitions *******************************/

uint8_t util_move_file(char * src, char* dest, char* name)
{
	char dest_addr[255];
	char src_addr[255];

	sprintf(dest_addr, "%s/%s", dest, name);
	sprintf(src_addr, "%s/%s", src, name);

	printf("%s -> %s\n", src_addr, dest_addr);

	return rename(src_addr, dest_addr);
}

uint8_t util_copy_file(char * src, char * dest, char * name)
{
	FILE  *ptr_src, *ptr_dest;
	int  in;
	char src_addr[255];
	char dest_addr[255];

	sprintf(src_addr, "%s/%s", src, name);
	sprintf(dest_addr, "%s/%s", dest, name);

	printf("%s -> %s\n", src_addr, dest_addr);

	ptr_src = fopen(src_addr, "rb");
	ptr_dest = fopen(dest_addr, "wb");

	if(ptr_src == NULL)
		return  1;

	if(ptr_dest == NULL)
		return  2;

	while((in = fgetc(ptr_src)) != EOF)
	{
		fputc(in, ptr_dest);
	}

	fclose(ptr_src);
	fclose(ptr_dest);

	return  0;
}

void util_delete_file(char* path, char * name)
{
	char file_addr[255];
	sprintf(file_addr, "%s/%s", path, name);				
	remove(file_addr);
}

/**
** @ingroup Communication
**
*/
int32_t util_save_csv(char* addr, char * name, char* buff)
{
	char csv_addr[255];
	sprintf(csv_addr, "%s/%s", addr, name);

	FILE * csv_out = fopen(csv_addr, "w");

	if(csv_out != NULL)
	{
		//printf("csv content - %s\n", buff);
		int32_t size_out = fwrite(buff, sizeof(char), strlen(buff), csv_out);
		fclose(csv_out);
		return size_out;
	}

	return -1;
}

int32_t util_file_size(char * path, char * file_name)
{
        char file_path[255];
        sprintf(file_path,"%s/%s", path, file_name);

        struct stat buffer;
        int status;
        status = stat(file_path, &buffer);

        if(status == 0)
        {
                return buffer.st_size;
        }
        else
        {
                return 0;
        }
}

/**
** @ingroup Communication
** Function load all content of given csv file and stored it in memory. Memory with csv file content is returned as
** result of the function. Returned memory is nececery released with free function after finish working.
** @param addr address to csv file.
** @param size pointer to size of csv file in bytes as the output parameter
** @return pointer to csv file content or NULL if any problem.
*/
char * util_load_csv(char * addr, char* name, int32_t * size)
{
	char csv_addr[255];
	sprintf(csv_addr, "%s/%s", addr,name);
    	int file_size = util_file_size(addr, name);

	FILE * csv_in = fopen(csv_addr, "r");

	if (csv_in != NULL)
	{
		char * csv_content = (char *) malloc(sizeof(char)*(file_size+1));

		fread(csv_content, sizeof(char), file_size, csv_in);
		csv_content[file_size] = 0;

		if(size != NULL)
			*size = file_size;
		
		fclose(csv_in);

		return csv_content;
	}
		
	return NULL;
}


int32_t util_str_ends_with(char* str, const char * suffix, int offset)
{
	if(!str || !suffix)
		return -1;

	int32_t str_len = strlen(str);
	int32_t suffix_len = strlen(suffix);

	if(str_len-offset >= suffix_len)
	{
		return (int32_t) strncmp(str+str_len-suffix_len-offset, suffix, suffix_len);
	}
	else
	{
		return -1;
	}
}	

char * util_get_time_string()
{
	//time formating
	time_t my_time;
	struct tm* time_info;
	char * time_str = (char*) malloc(sizeof(char)*23);	

	time(&my_time);
	time_info = localtime(&my_time);
	
	strftime(time_str, 22, "%d/%m/%y - %H:%M:%S", time_info);

	return time_str;
}

/*********************************************************** end of section with util functions definitiosn ***************************
************************************************************ section with cli functions definitions ***********************************/
/*0
void cli_clear_screen()
{
	fputs("\033[H\033[J", stdout);
}
*/

/******************************************************** end of section with cli functions definitions ************************
********************************************************* section with gui functions definitions ******************************/

/**
** @ingroup Gui
** Contructor for gui layer.
** Definition of main window and window pages and connection with core layer
*/
gui * gui_new(core * core_ref)
{
	gui * this = (gui*) malloc(sizeof(gui));


	this->core_ref = core_ref;
	this->gui_base_ref = gui_base_new(core_ref);	

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
	g_timeout_add(HOT_FOLDER_READING_INTERVAL/1000, gui_cyclic_interupt, this);

	gtk_widget_show_all(GTK_WIDGET(this->main_win));

	return this;
}


gboolean gui_cyclic_interupt(gpointer param)
{
	gui * this = (gui*) param;

	this->blink = !this->blink;

	//const char * visible_page = gtk_stack_get_visible_child_name (GTK_STACK(this->page_stack));

	gtk_combo_box_set_active(GTK_COMBO_BOX(this->control_page->print_mode_combo), this->core_ref->machine_mode);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->print_mode_combo), this->core_ref->machine_state == MACHINE_STATE_WAIT);

	bool pause_en = (this->core_ref->machine_state != MACHINE_STATE_WAIT) && (this->core_ref->machine_state != MACHINE_STATE_NEXT) && 
						(this->core_ref->machine_state != MACHINE_STATE_ERROR) && (this->core_ref->machine_state != MACHINE_STATE_PAUSE) && 
						(this->core_ref->machine_state != MACHINE_STATE_READY_TO_START) && (this->core_ref->machine_state != MACHINE_STATE_WAIT_FOR_CONFIRMATION);

	

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PAUSE]), pause_en);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CONTINUE]), this->core_ref->machine_state == MACHINE_STATE_PAUSE);
	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_PRINT]), this->core_ref->machine_state == MACHINE_STATE_WAIT);

	gtk_widget_set_sensitive(GTK_WIDGET(this->control_page->btn[GC_BTN_CANCEL]), (this->core_ref->machine_state != MACHINE_STATE_WAIT) && (this->core_ref->machine_state != MACHINE_STATE_ERROR));
	gtk_widget_set_visible(GTK_WIDGET(this->control_page->btn[GC_BTN_FEED]), this->core_ref->machine_state == MACHINE_STATE_WAIT_FOR_CONFIRMATION);

	/* set status of network control widget depend on network connection state */
	gboolean conn = ((core_iij_is_connected(this->core_ref) == STATUS_CLIENT_CONNECTED) ? FALSE : TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_tcp_port_entry), conn);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_ip_address_entry), conn);

	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->pci_connection_entry), core_pci_network_connected(this->core_ref) != STATUS_CLIENT_CONNECTED);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->quadient_connection_entry), core_quadient_network_connected(this->core_ref) != STATUS_CLIENT_CONNECTED);
	gtk_widget_set_sensitive(GTK_WIDGET(this->network_page->iij_connection_entry), core_iij_network_connected(this->core_ref) != STATUS_CLIENT_CONNECTED);

	gtk_switch_set_active(GTK_SWITCH(this->network_page->iij_network_switch), !conn);

	/* can set only if the machine state is in wait state */
	for(int i = 0; i < HOT_LIST_N; i++)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->hotfolder_page->hot_btn[i]), (this->core_ref->machine_state == MACHINE_STATE_WAIT));
		gtk_widget_set_sensitive(GTK_WIDGET(this->hotfolder_page->hot_entry[i]), (this->core_ref->machine_state == MACHINE_STATE_WAIT));
	}

	/* load report csv list */
	gui_control_page_load_report_csv_list(this->control_page);

	/* check state of core */
/*
	if(core_get_error_val(this->core_ref) > 0)
	{
		c_string_set_string(this->error_string, (char*) core_get_error_str(this->core_ref));
	}
	else
	{
		if(c_string_len(this->error_string) > 0)
			c_string_clear(this->error_string);
	}
*/

	
	c_string_set_string(this->error_string, "Test chyby");
	//if(strcmp(visible_page, "control_page") == 0)
	{
		if(this->core_ref->job_list_changed > 0)
			gui_control_page_load_jobs(this->control_page);
	}
 	
	gtk_widget_queue_draw(GTK_WIDGET(this->drawing_area));

	return TRUE;
}

void gui_set_language(gui * this)
{
	gtk_window_set_title(GTK_WINDOW(this->main_win), multi_lang[this->core_ref->lang_index].win_title);

	gui_control_page_language(this->control_page);

	settings_button_set_text(this->io_vision_page->btn_return, multi_lang[this->core_ref->lang_index].set_btn_back_to_settngs);
	settings_button_set_text(this->hotfolder_page->btn_return, multi_lang[this->core_ref->lang_index].set_btn_back_to_settngs);
	settings_button_set_text(this->print_params_page->btn_return, multi_lang[this->core_ref->lang_index].set_btn_back_to_settngs);
	settings_button_set_text(this->network_page->btn_return, multi_lang[this->core_ref->lang_index].set_btn_back_to_settngs);
	settings_button_set_text(this->lang_page->btn_return, multi_lang[this->core_ref->lang_index].set_btn_back_to_settngs);

	gui_settings_page_language(this->settings_page);

	gui_print_params_page_language(this->print_params_page);

	gui_hotfolder_page_language(this->hotfolder_page);


	gui_network_page_language(this->network_page);

	gui_lang_page_language(this->lang_page);

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
	g_signal_connect(G_OBJECT(this->main_win), "destroy", G_CALLBACK(gui_finalize), this);
	g_signal_connect(G_OBJECT(this->drawing_area), "draw", G_CALLBACK(gui_on_draw_event), this);
	g_signal_connect(G_OBJECT(this->status_bar), "draw", G_CALLBACK(gui_status_bar_draw), this);

	g_signal_connect(G_OBJECT(this->io_vision_page->io_vision), "draw", G_CALLBACK(gui_io_vision_draw_event), this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->io_vision_page->btn_return)), "button_press_event", G_CALLBACK(gui_settings_page_return_callback), this);


	g_signal_connect(G_OBJECT(settings_button_get_instance(this->network_page->btn_return)), "button_press_event", G_CALLBACK(gui_settings_page_return_callback), this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->lang_page->btn_return)), "button_press_event", G_CALLBACK(gui_settings_page_return_callback), this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->hotfolder_page->btn_return)), "button_press_event", G_CALLBACK(gui_settings_page_return_callback), this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->print_params_page->btn_return)), "button_press_event", G_CALLBACK(gui_settings_page_return_callback), this);



	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_PRINT]), "clicked", G_CALLBACK(gui_control_page_btn_print_callback), this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_PAUSE]), "clicked", G_CALLBACK(gui_control_page_btn_pause_callback), this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_CONTINUE]), "clicked", G_CALLBACK(gui_control_page_btn_continue_callback), this->control_page);

	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_CANCEL]), "clicked", G_CALLBACK(gui_control_page_btn_cancel_callback), this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_ERROR_RESET]), "clicked", G_CALLBACK(gui_control_page_btn_reset_callback), this->control_page);
	g_signal_connect(G_OBJECT(this->control_page->btn_settings), "clicked", G_CALLBACK(gui_control_page_btn_go_to_settings_callback), this);
	g_signal_connect(G_OBJECT(this->control_page->job_report_list), "cursor-changed", G_CALLBACK(gui_control_page_open_report_csv), this);
	g_signal_connect(G_OBJECT(this->control_page->btn[GC_BTN_FEED]), "clicked", G_CALLBACK(gui_control_page_btn_feed_sheet_callback), this->core_ref);

	g_signal_connect(G_OBJECT(this->control_page->info_box), "draw", G_CALLBACK(gui_control_info_box_draw_callback), this->control_page);

	g_signal_connect(G_OBJECT(this->control_page->print_mode_combo), "changed", G_CALLBACK(gui_control_page_set_machine_mode_callback), this->control_page);



	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_IO_VISION])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_go_to_io_vision_callback), this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_LANG])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_go_to_lang_settings_callback), this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_NETWORK])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_go_to_network_settings_callback), this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_PRINT_PARAMETERS])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_go_to_print_params_settings_callback), this);
	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_HOTFOLDERS])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_go_to_hotfolder_settings_callback), this);

	g_signal_connect(G_OBJECT(settings_button_get_instance(this->settings_page->btn[S_BTN_BACK])), 
				"button_press_event", G_CALLBACK(gui_settings_page_btn_back_callback), this);

	g_signal_connect(G_OBJECT(this->lang_page->lang_list), "changed", G_CALLBACK(gui_lang_page_set_interface_language_callback), this);
	
	g_signal_connect(G_OBJECT(this->network_page->iij_network_switch), "state-set", G_CALLBACK(gui_setting_page_iij_network_control_callback), this->network_page);
	g_signal_connect(G_OBJECT(this->network_page->iij_connection_entry), "key-release-event", G_CALLBACK(gui_network_page_set_iij_address), this->core_ref);
	g_signal_connect(G_OBJECT(this->network_page->pci_connection_entry), "key-release-event", G_CALLBACK(gui_network_page_set_pci_address), this->core_ref);
	g_signal_connect(G_OBJECT(this->network_page->quadient_connection_entry), "key-release-event", G_CALLBACK(gui_network_page_set_quadient_address), this->core_ref);



	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_MAIN]), "clicked", G_CALLBACK(gui_hotfolder_page_select_q_main_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_FEEDBACK]), "clicked", G_CALLBACK(gui_hotfolder_page_select_q_feedback_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[Q_HOT_BACKUP]), "clicked", G_CALLBACK(gui_hotfolder_page_select_q_backup_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[PCI_HOT_IN]), "clicked", G_CALLBACK(gui_hotfolder_page_select_pci_in_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[PCI_HOT_OUT]), "clicked", G_CALLBACK(gui_hotfolder_page_select_pci_out_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[GIS_HOT]), "clicked", G_CALLBACK(gui_hotfolder_page_select_gis_path_callback), this);
	g_signal_connect(G_OBJECT(this->hotfolder_page->hot_btn[JOB_LOG_DIR]), "clicked", G_CALLBACK(gui_hotfolder_page_select_report_path_callback), this);

	g_signal_connect(G_OBJECT(this->print_params_page->par_entry[PAR_MAX_STACKED_SHEETS]), "key-release-event", G_CALLBACK(gui_print_params_page_max_stacked_sheet_callback), this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->par_entry[PAR_MAX_REJECTED_SHEET_SEQ]), "key-release-event", G_CALLBACK(gui_print_params_set_max_rejected_sheet_seq), this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->sheet_source_combo), "changed", G_CALLBACK(gui_print_params_set_sheet_source_callback), this->print_params_page);
	g_signal_connect(G_OBJECT(this->print_params_page->print_confirm_switch), "state_set", G_CALLBACK(gui_print_params_set_print_confirmation_state_callback), this->print_params_page);


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

	dialog = gtk_file_chooser_dialog_new (title,
                                      GTK_WINDOW(this->main_win),
                                      action,
                                      multi_lang[this->core_ref->lang_index].file_chooser_cancel_btn,
                                      GTK_RESPONSE_CANCEL,
                                      multi_lang[this->core_ref->lang_index].file_chooser_select_btn,
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

gui_control_page * gui_control_page_new(gui_base * gui_base_ref)
{
	gui_control_page * this = (gui_control_page *) malloc(sizeof(gui_control_page));

	this->gui_base_ref = gui_base_ref;

	/* window dimensions */
	double width = gui_base_ref->work_area_geometry.width;
	double height = gui_base_ref->work_area_geometry.height;


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

gboolean gui_control_info_box_draw_callback(GtkWidget * widget, cairo_t * cr, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	int width = this->gui_base_ref->work_area_geometry.width;
	int height = this->gui_base_ref->work_area_geometry.height;
	core * core_ref = this->gui_base_ref->core_ref;
	int lang_index = core_ref->lang_index;
	int left_horizontal_offset = 40;	

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15);

	/* widget labels */
	cairo_move_to(cr, width/4, (5*height/24)+20);
	cairo_show_text(cr, multi_lang[lang_index].g_job_list_lbl);


	cairo_move_to(cr, width/4*3+(width/4-350)/2, (5*height/24)+80);
	cairo_show_text(cr, multi_lang[lang_index].par_machine_mode_lbl);

	cairo_move_to(cr, width/4, (5*height/24)+(height/8)+210);
	cairo_show_text(cr, multi_lang[lang_index].g_report_csv_list_lbl);

	cairo_set_font_size(cr, 18);
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);


	/* left side */
	char temp_buff[16];

	cairo_move_to(cr,left_horizontal_offset , 5*height/24+80);
	cairo_show_text(cr, multi_lang[lang_index].g_counters_label);
	
	cairo_select_font_face(cr, "Arial",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15);
	cairo_move_to(cr, left_horizontal_offset, 5*height/24+130);
	cairo_show_text(cr, multi_lang[lang_index].g_feeder_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+160);
	cairo_show_text(cr, multi_lang[lang_index].g_cnt_main_feed_lbl);



	cairo_move_to(cr, left_horizontal_offset, 5*height/24+200);
	cairo_show_text(cr, multi_lang[lang_index].g_cnt_companion_feed_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+260);
	cairo_show_text(cr, multi_lang[lang_index].g_reject_bin_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+290);
	cairo_show_text(cr, multi_lang[lang_index].g_cnt_rejected_lbl);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+360);
	cairo_show_text(cr, multi_lang[lang_index].g_stacker_lbl);


	cairo_move_to(cr, left_horizontal_offset, 5*height/24+390);
	cairo_show_text(cr, multi_lang[lang_index].g_cnt_tab_insert_blb);

	cairo_move_to(cr, left_horizontal_offset, 5*height/24+430);
	cairo_show_text(cr, multi_lang[lang_index].g_cnt_stakced_lbl);


	cairo_set_font_size(cr, 19);

	sprintf(temp_buff, "%d", core_ref->feeded_main_sheet_counter);
	cairo_text_extents_t ext_main_counter;
	cairo_text_extents(cr, temp_buff, &ext_main_counter);
	cairo_move_to(cr, width/4-80 - ext_main_counter.width, 5*height/24+160);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", core_ref->feeded_companion_sheet_counter);
	cairo_text_extents_t ext_companion_counter;
	cairo_text_extents(cr, temp_buff, &ext_companion_counter);
	cairo_move_to(cr, width/4-80 - ext_companion_counter.width, 5*height/24+200);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", core_ref->rejected_sheet_counter);
	cairo_text_extents_t ext_reject_counter;
	cairo_text_extents(cr, temp_buff, &ext_reject_counter);
	cairo_move_to(cr, width/4-80 - ext_reject_counter.width, 5*height/24+290);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d", core_ref->tab_insert_counter);
	cairo_text_extents_t ext_ti_counter;
	cairo_text_extents(cr, temp_buff, &ext_ti_counter);
	cairo_move_to(cr, width/4-80 - ext_ti_counter.width, 5*height/24+390);
	cairo_show_text(cr, temp_buff);

	sprintf(temp_buff, "%d/%d",core_ref->info->total_stemps_number, core_ref->stacked_sheet_counter);
	cairo_text_extents_t ext_stacked_counter;
	cairo_text_extents(cr, temp_buff, &ext_stacked_counter);
	cairo_move_to(cr, width/4-80 - ext_stacked_counter.width, 5*height/24+430);
	cairo_show_text(cr, temp_buff);

	/* right side */

	cairo_set_font_size(cr, 15);
	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+200);
	cairo_show_text(cr, multi_lang[lang_index].g_status_machine_lbl);

	cairo_text_extents_t ext_machine_status;
	cairo_text_extents(cr, core_get_error_str(core_ref), &ext_machine_status);
	cairo_move_to(cr, width-(width/4-350)/2-ext_machine_status.width, 5*height/24+200);
	cairo_show_text(cr, core_get_error_str(core_ref));

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+260);
	cairo_show_text(cr, multi_lang[lang_index].g_status_gis_lbl);

	cairo_text_extents_t ext_gis_status;
	cairo_text_extents(cr, c_string_get_char_array(core_ref->print_controller_status), &ext_gis_status);
	cairo_move_to(cr, width-(width/4-350)/2 - ext_gis_status.width, 5*height/24+260);
	cairo_show_text(cr, c_string_get_char_array(core_ref->print_controller_status));

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+320);
	cairo_show_text(cr, "TCP/IP status PCI:");
	
	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+380);
	cairo_show_text(cr, "TCP/IP status IIJ:");

	cairo_move_to(cr, width/4*3+(width/4-350)/2, 5*height/24+440);
	cairo_show_text(cr, "TCP/IP status QUADIENT:");
	
	/* network connection status points */
	if(core_pci_network_connected(core_ref) != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);

	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+320, 10, 0, 2*M_PI);
	cairo_fill(cr);

	if(core_iij_network_connected(core_ref) != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);

	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+380, 10, 0, 2*M_PI);
	cairo_fill(cr);


	if(core_quadient_network_connected(core_ref) != STATUS_CLIENT_CONNECTED)
		cairo_set_source_rgb(cr,1,0,0);
	else
		cairo_set_source_rgb(cr, 0.2,0.8,0.1);
	cairo_arc(cr, width-(width/4-350)/2-15, 5*height/24+440, 10, 0, 2*M_PI);
	cairo_fill(cr);

	return FALSE;
}


void gui_control_page_language(gui_control_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;

	/* columns for the job list tree widget */
	gui_control_page_delete_columns(this->job_list);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].gui_job_name, JOB_STATE)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].gui_job_name, JOB_NAME)));
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].gui_job_pdf_name, JOB_PDF_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].gui_job_bkcore_name, JOB_BKCORE_CSV_NAME)));

	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].gui_job_camera_name, JOB_CAMERA_CSV_NAME)));


	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(this->print_mode_combo));
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang[lang_index].par_mm_setup_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang[lang_index].par_mm_print_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang[lang_index].par_mm_inspection_comb);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(this->print_mode_combo), multi_lang[lang_index].par_mm_print_inspection_comb);
	/* columns for job report csv list tree widget */

	gui_control_page_delete_columns(this->job_report_list);
	gtk_tree_view_append_column (GTK_TREE_VIEW(this->job_report_list), 
					GTK_TREE_VIEW_COLUMN(gui_control_page_new_tree_column((gchar*) multi_lang[lang_index].rep_csv_log_column_file_name, REP_CSV_NAME)));

}

void gui_control_page_btn_cancel_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	core_print_cancel(this->gui_base_ref->core_ref);
}

void gui_control_page_btn_reset_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	core_print_reset_error(this->gui_base_ref->core_ref);

	core_refresh_dir_list(this->gui_base_ref->core_ref);
}

void gui_control_page_btn_go_to_settings_callback(GtkButton *button, gpointer param)
{	
	gui * this = (gui*) param;
	gtk_stack_set_visible_child_name (GTK_STACK(this->page_stack), "settings_page");
}



void gui_control_page_btn_pause_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	if(this->gui_base_ref->core_ref->machine_state != MACHINE_STATE_ERROR)
	{
		/* set pause */
		core_print_pause(this->gui_base_ref->core_ref);
	}
}

void gui_control_page_btn_continue_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;
	if(core_print_continue(this->gui_base_ref->core_ref) == 0)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_CONTINUE]), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(this->btn[GC_BTN_PAUSE]), TRUE);
	}
}

void gui_control_page_btn_print_callback(GtkButton *button, gpointer param)
{
	gui_control_page * this = (gui_control_page*) param;

	if(this->gui_base_ref->core_ref->machine_state == MACHINE_STATE_WAIT)
	{
		/* start new print */
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (this->job_list));
		GtkTreeIter iter;
        	GtkTreeModel *model;
        	gchar *job_name;

        	if (gtk_tree_selection_get_selected (selection, &model, &iter))
        	{
        	        gtk_tree_model_get (model, &iter, JOB_NAME, &job_name, -1);

			if(core_print_start(this->gui_base_ref->core_ref, (const char*) job_name) == 0)
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
	core * core_ref = this->gui_base_ref->core_ref;

	pthread_mutex_lock(&(core_ref->mutex));
	pthread_mutex_unlock(&(core_ref->mutex));

	GtkTreeIter iter;
	this->job_list_store = gtk_list_store_new(JOB_LIST_N, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	int i = 0;
	while(i < array_list_size(core_ref->job_list))
	{
		q_job * job = array_list_get(core_ref->job_list, i);
			
		gtk_list_store_append(this->job_list_store, &iter);
		const char * job_status = NULL;		
		if((strcmp(c_string_get_char_array(core_ref->printed_job_name), job->job_name) == 0))
		{
			if(core_ref->machine_state == MACHINE_STATE_NEXT)
			{
				job_status = multi_lang[core_ref->lang_index].print_state_wait_for_data;
			}
			else if(core_ref->machine_state == MACHINE_STATE_ERROR)
			{
				job_status = multi_lang[core_ref->lang_index].print_state_job_error;
			}
			else
			{
				job_status = multi_lang[core_ref->lang_index].print_state_print;
			}
		}
		else
		{
			job_status = multi_lang[core_ref->lang_index].print_state_wait;
		}

               	gtk_list_store_set (this->job_list_store, &iter,
				JOB_STATE, job_status,
				JOB_NAME, job->job_name,
				JOB_PDF_NAME, job->pdf_name,
				JOB_BKCORE_CSV_NAME, job->bkcore_name, 
				JOB_CAMERA_CSV_NAME, job->camera_name, -1);
		i++;
	}

	gtk_tree_view_set_model (GTK_TREE_VIEW(this->job_list), GTK_TREE_MODEL(this->job_list_store));
        g_object_unref(G_OBJECT(this->job_list_store));
}


void gui_control_page_load_report_csv_list(gui_control_page * this)
{
	GtkTreeIter iter;

	this->job_report_list_store = gtk_list_store_new(REPO_ITEM_N, G_TYPE_STRING);
	DIR * dir_ref = opendir(core_get_job_report_hotfolder_path(this->gui_base_ref->core_ref));

	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;

		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				if(util_str_ends_with(dir_cont->d_name, ".csv", 0) == 0)
				{
					gtk_list_store_append(this->job_report_list_store, &iter);
               				gtk_list_store_set (this->job_report_list_store, &iter, REP_CSV_NAME, dir_cont->d_name, -1);
				}
				else
				{
					printf("removed file: %s\n", dir_cont->d_name);
					util_delete_file(core_get_job_report_hotfolder_path(this->gui_base_ref->core_ref), dir_cont->d_name);
				}
			}
		}

		closedir(dir_ref);
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(this->job_report_list), GTK_TREE_MODEL(this->job_report_list_store));
	g_object_unref(G_OBJECT(this->job_report_list_store));
}

void gui_control_page_set_machine_mode_callback (GtkComboBox *widget, gpointer param)
{
	gui_control_page * this = (gui_control_page *) param;
	core_set_machine_mode(this->gui_base_ref->core_ref, gtk_combo_box_get_active(GTK_COMBO_BOX(this->print_mode_combo)));
}


void gui_control_page_btn_feed_sheet_callback(GtkWidget * widget, gpointer param)
{
	core * this = param;
	if(this->machine_state == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
		core_sheet_source_confirmation(this);
}


void gui_control_page_open_report_csv (GtkTreeView *tree_view, gpointer param)
{
	gui * this = (gui*) param;

	GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));
	GtkTreeIter iter;
        GtkTreeModel *model;
        gchar *csv_name;

        if (gtk_tree_selection_get_selected (selection, &model, &iter))
        {
		char cmd[1024];
                gtk_tree_model_get (model, &iter, REP_CSV_NAME, &csv_name, -1);
		sprintf(cmd, "nohup $(cat %s/%s | %s) > /dev/null 2>&1&",core_get_job_report_hotfolder_path(this->core_ref), csv_name, DEFAULT_TEXT_EDITOR);
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


void gui_settings_page_language(gui_settings_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;
	
	for(int i = 0; i< S_BTN_N; i++)
	{
		if(i == S_BTN_NETWORK)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_network);
		else if(i == S_BTN_LANG)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_lang_settings);
		else if(i == S_BTN_PRINT_PARAMETERS)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_print_param);
		else if(i == S_BTN_HOTFOLDERS)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_hotfolder);
		else if(i == S_BTN_IO_VISION)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_io_vision);
		else if(i == S_BTN_BACK)
			settings_button_set_text(this->btn[i], multi_lang[lang_index].set_btn_back);
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


void gui_io_vision_draw_points(gui * this, cairo_t * cr, double x, double y, double w, double h, const char ** i_labels, const char ** o_labels, int8_t card)
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

				if(io_card_get_input(this->core_ref->io_card_ref, card, io_in) > 0)
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

				if(io_card_get_output(this->core_ref->io_card_ref, card , io_out) > 0)
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

				if(io_card_get_input(this->core_ref->io_card_ref,card, io_in) > 0)
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

				if(io_card_get_output(this->core_ref->io_card_ref, card, io_out) > 0)
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

	gui_io_vision_draw_points(this, cr, a1_x_pos, y_pos, width, height, a1_in_labels, a1_out_labels, IO_CARD_A1);
	gui_io_vision_draw_points(this, cr, a2_x_pos, y_pos, width, height, a2_in_labels, a2_out_labels, IO_CARD_A2);

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
	sprintf(tcp_port_str, "%d", core_iij_get_tcp_port(gui_base_ref->core_ref));
	gtk_entry_set_text(GTK_ENTRY(this->iij_tcp_port_entry), tcp_port_str);

	this->iij_ip_address_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_ip_address_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->iij_ip_address_entry), core_iij_get_ip_addr(gui_base_ref->core_ref));

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
	gtk_entry_set_text(GTK_ENTRY(this->pci_connection_entry), (char*) comm_tcp_get_ip_addr(this->gui_base_ref->core_ref->pci_connection));	

	this->quadient_connection_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->quadient_connection_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->quadient_connection_entry), (char*) comm_tcp_get_ip_addr(this->gui_base_ref->core_ref->quadient_connection));

	this->iij_connection_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(this->iij_connection_entry), 300, 35);
	gtk_entry_set_text(GTK_ENTRY(this->iij_connection_entry), (char*) comm_tcp_get_ip_addr(this->gui_base_ref->core_ref->iij_connection));

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


void gui_network_page_language(gui_network_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;

	gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_label), multi_lang[lang_index].set_net_iij_tcp_port);
	gtk_label_set_text(GTK_LABEL(this->iij_conn_label), multi_lang[lang_index].set_net_iij_connection);
	gtk_label_set_text(GTK_LABEL(this->iij_ip_address_label), multi_lang[lang_index].set_net_iij_ip_addr);
	gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), NULL);
	gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), NULL);

	gtk_label_set_text(GTK_LABEL(this->iij_connection_label), multi_lang[lang_index].set_net_iij_connection_test_label);
	gtk_label_set_text(GTK_LABEL(this->quadient_connection_label), multi_lang[lang_index].set_net_quadient_connection_test_label);
	gtk_label_set_text(GTK_LABEL(this->pci_connection_label), multi_lang[lang_index].set_net_pci_connection_test_label);
}

gboolean gui_setting_page_iij_network_control_callback(GtkSwitch *widget, gboolean state, gpointer param)
{
	gui_network_page * this = (gui_network_page*) param;

	if(state)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(this->iij_network_switch), FALSE);
		
		if(core_iij_is_connected(this->gui_base_ref->core_ref) != STATUS_CLIENT_CONNECTED)
		{
			uint8_t tcp_port_res = core_iij_set_tcp_port(this->gui_base_ref->core_ref, atoi(gtk_entry_get_text(GTK_ENTRY(this->iij_tcp_port_entry))));
			uint8_t ip_address_res = core_iij_set_ip_addr(this->gui_base_ref->core_ref, (char*) gtk_entry_get_text(GTK_ENTRY(this->iij_ip_address_entry)));
	
			if((tcp_port_res == STATUS_SUCCESS) && (ip_address_res == STATUS_SUCCESS))
			{
				core_iij_connect(this->gui_base_ref->core_ref);
				gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), NULL);
				gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), NULL);
			}
			else
			{
				if(ip_address_res != STATUS_SUCCESS)
					gtk_label_set_text(GTK_LABEL(this->iij_ip_addr_corect_label), multi_lang[this->gui_base_ref->core_ref->lang_index].set_net_iij_wrong_tcp_port_label);
		
				if(tcp_port_res != STATUS_SUCCESS)
					gtk_label_set_text(GTK_LABEL(this->iij_tcp_port_corect_label), multi_lang[this->gui_base_ref->core_ref->lang_index].set_net_iij_wrong_ip_address_label);
			}
		}

		gtk_widget_set_sensitive(GTK_WIDGET(this->iij_network_switch), TRUE);
	}
	else
	{
		if(core_iij_is_connected(this->gui_base_ref->core_ref) == STATUS_CLIENT_CONNECTED)
			core_iij_disconnect(this->gui_base_ref->core_ref);
	}
	
	return TRUE;
}


void gui_network_page_set_pci_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	core * this = (core*) param;
	core_pci_network_set_ip_address(this, (char*) gtk_entry_get_text(GTK_ENTRY(widget)));
}

void gui_network_page_set_quadient_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	core * this = (core*) param;
	core_quadient_network_set_ip_address(this, (char*) gtk_entry_get_text(GTK_ENTRY(widget)));
}

void gui_network_page_set_iij_address(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	core * this = (core*) param;
	core_iij_network_set_ip_address(this, (char*) gtk_entry_get_text(GTK_ENTRY(widget)));
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
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_q_main_hotfolder_path(this->gui_base_ref->core_ref));
			break;
			case Q_HOT_BACKUP:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_q_backup_hotfolder_path(this->gui_base_ref->core_ref));
			break;
			case Q_HOT_FEEDBACK:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_q_feedback_hotfolder_path(this->gui_base_ref->core_ref));
			break;
			case PCI_HOT_IN:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_pci_hotfolder_in_path(this->gui_base_ref->core_ref));
			break;
			case PCI_HOT_OUT:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_pci_hotfolder_out_path(this->gui_base_ref->core_ref));
			break;
			case GIS_HOT:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_gis_hotfolder_path(this->gui_base_ref->core_ref));
			break;
			case JOB_LOG_DIR:
				gtk_entry_set_text(GTK_ENTRY(this->hot_entry[i]), core_get_job_report_hotfolder_path(this->gui_base_ref->core_ref));
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


void gui_hotfolder_page_language(gui_hotfolder_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;

	for(int i = 0; i < HOT_LIST_N; i++)
	{
		gtk_button_set_label(GTK_BUTTON(this->hot_btn[i]), multi_lang[lang_index].file_chooser_btn_browse);
		
		switch(i)
		{
			case Q_HOT_MAIN:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_q_main_fs_label);
			break;
			case Q_HOT_FEEDBACK:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_q_feedback_fs_label);
			break;
			case Q_HOT_BACKUP:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_q_backup_fs_label);
			break;
			case PCI_HOT_IN:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_pci_in_fs_label);
			break;
			case PCI_HOT_OUT:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_pci_out_fs_label);
			break;
			case GIS_HOT:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_gis_fs_label);
			break;
			case JOB_LOG_DIR:
				gtk_label_set_text(GTK_LABEL(this->hot_lbl[i]), multi_lang[lang_index].hot_report_csv_fs_label);
			break;
		}
	}
}


void gui_hotfolder_page_select_q_main_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;

	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_q_main_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_MAIN]), path);
		core_set_q_main_hotfolder_path(this->core_ref, path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_q_feedback_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_q_feedback_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_FEEDBACK]), path);
		core_set_q_feedback_hotfolder_path(this->core_ref, path);
		g_free(path);
	}
}


void gui_hotfolder_page_select_q_backup_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_q_backup_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[Q_HOT_BACKUP]), path);
		core_set_q_backup_hotfolder_path(this->core_ref, path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_pci_in_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_pci_in_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[PCI_HOT_IN]), path);
		core_set_pci_hotfolder_in_path(this->core_ref, path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_pci_out_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_pci_out_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[PCI_HOT_OUT]), path);
		core_set_pci_hotfolder_out_path(this->core_ref, path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_gis_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_gis_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[GIS_HOT]), path);
		core_set_gis_hotfolder_path(this->core_ref, path);
		g_free(path);
	}
}

void gui_hotfolder_page_select_report_path_callback(GtkWidget * widget, gpointer param)
{
	gui * this = (gui*) param;
	char * path = gui_def_file_chooser(this, (char*) multi_lang[this->core_ref->lang_index].hot_report_csv_fs_title);

	if(path != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(this->hotfolder_page->hot_entry[JOB_LOG_DIR]), path);
		core_set_job_report_hotfolder_path(this->core_ref, path);
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
				sprintf(buf, "%d", gui_base_ref->core_ref->max_stacked_sheets);
			break;
			case PAR_MAX_REJECTED_SHEET_SEQ:
				sprintf(buf, "%d", gui_base_ref->core_ref->rejected_sheet_for_stop);
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
	gtk_switch_set_active(GTK_SWITCH(this->print_confirm_switch), this->gui_base_ref->core_ref->sheet_source_confirmation);



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

void gui_print_params_page_language(gui_print_params_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;

	for(int i=0; i< PAR_LIST_N; i++)
	{
		switch(i)
		{
			case PAR_MAX_STACKED_SHEETS:
				gtk_label_set_text(GTK_LABEL(this->par_lbl[i]), multi_lang[lang_index].par_max_stacked_sheet_lbl);
			break;
			case PAR_MAX_REJECTED_SHEET_SEQ:
				gtk_label_set_text(GTK_LABEL(this->par_lbl[i]), multi_lang[lang_index].par_rejected_sheet_seq_lbl);
			break;
		}
	}

	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(this->sheet_source_combo));
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang[lang_index].par_sheet_source_main);
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->sheet_source_combo), multi_lang[lang_index].par_sheet_source_companion);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(this->sheet_source_combo), this->gui_base_ref->core_ref->companion_sheet_source);
	
	gtk_label_set_text(GTK_LABEL(this->sheet_source_lbl), multi_lang[lang_index].par_sheet_source_lbl);
	gtk_label_set_text(GTK_LABEL(this->print_confirm_lbl), multi_lang[lang_index].par_print_confirm_lbl);

}


void gui_print_params_set_sheet_source_callback (GtkComboBox *widget, gpointer param)
{
	gui_print_params_page * this = (gui_print_params_page*) param;
	core_set_companion_sheet_source(this->gui_base_ref->core_ref, gtk_combo_box_get_active (widget));
}

gboolean gui_print_params_set_print_confirmation_state_callback (GtkSwitch *widget, gboolean state, gpointer param)
{
	gui_print_params_page * this = (gui_print_params_page*) param;
	core_set_sheet_source_confirmation(this->gui_base_ref->core_ref, state);
	return TRUE;
}




void gui_print_params_set_max_rejected_sheet_seq(GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	gui_print_params_page * this = (gui_print_params_page*) param;

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
		core_set_max_rejected_sheet_seq(this->gui_base_ref->core_ref, sheets_val);
}

void gui_print_params_page_max_stacked_sheet_callback (GtkWidget *widget, GdkEvent  *event, gpointer param)
{
	gui_print_params_page * this = (gui_print_params_page*) param;

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
		core_set_max_stacked_sheets(this->gui_base_ref->core_ref, sheets_val);
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

	for(int i = 0;i<lang_num;i++)
	{
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(this->lang_list), multi_lang[i].lang_name);
	}

	gtk_combo_box_set_active (GTK_COMBO_BOX(this->lang_list), gui_base_ref->core_ref->lang_index);

	gtk_fixed_put(GTK_FIXED(this->page), settings_button_get_instance(this->btn_return), 
		gui_base_ref->work_area_geometry.width/4, 250);

	gtk_fixed_put(GTK_FIXED(this->page), this->list_label, gui_base_ref->work_area_geometry.width/4, 405);
	gtk_fixed_put(GTK_FIXED(this->page), this->lang_list, gui_base_ref->work_area_geometry.width/4*3-400, 400);

	return this;
}


void gui_lang_page_language(gui_lang_page * this)
{
	int lang_index = this->gui_base_ref->core_ref->lang_index;
	gtk_label_set_text(GTK_LABEL(this->list_label), multi_lang[lang_index].set_lan_label);
}

void gui_lang_page_set_interface_language_callback(GtkComboBox * widget, gpointer * param)
{
	gui * this = (gui*) param;
	core_set_interface_language(this->core_ref,gtk_combo_box_get_active (GTK_COMBO_BOX(widget)));
	gui_set_language(this);
}

/**
** @ingroup Gui
** Constructor for gui_base class. Here are defined all shared data needed in graphic interface.
*/
gui_base * gui_base_new(core * core_ref)
{
	gui_base * this = (gui_base*) malloc(sizeof(gui_base));
	this->core_ref = core_ref;

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

/******************* End of functions definitions ********************************************
******************** Test cases **************************************************************/

	/******************* Test cases section - xyz *************************************************************************/



	/******************* Test cases section - abcd *************************************************************************/



/******************* End of section with test cases ******************************************/
/******************* Main function section ***************************************************/



int main(int argv, char ** argc)
{
	multi_lang_init();
	
	core * core_instance = core_new();	

	gtk_init(&argv, &argc);
	
	if(core_instance != NULL)
	{
		gui * gui_instance = gui_new(core_instance);
		gui_signals(gui_instance);
	}
	else
	{
		gui_info_window_new("Chyba!", "Inicializace jádra neproběhla v pořádku!\nProgramu bude niní ukončen.", NULL, NULL);
	}

	gtk_main();
	
	/* safety exit the core instance */
	if(core_instance != NULL)
		core_finalize(core_instance);
	

	return 0;
}

