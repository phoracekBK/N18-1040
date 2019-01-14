#ifndef _CORE_H_
#define _CORE_H_

/* Library include */

/* Header files include */
//#include "config.h"
#include "model.h"

/* C library  include */

#include <features.h>
#include <libconfig.h>
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
#include <c_unit_lib.h>


#define IO_SIMULATION TRUE

#define CORE_TEST_MODE TRUE

/** @ingroup configuration Time interva for correct receave the respond from TCP socket in microseconds. */
#define TCP_NETWORK_TTL_U 100000 

/** @ingroup configuration Time interva for correct receave the respond from TCP socket in seconds. */
#define TCP_NETWORK_TTL_S 0

/** @ingroup configuration Default TCP port value for tcp communication with pritner subsystem, needed when configuration file is broken. */
#define DEFAULT_TCP_PORT_GIS 2000

/** @ingroup configuration Default TCP port value for tcp communication with camera revision subsystem, needed when configuration file is broken. */
#define DEFAULT_TCP_PORT_PCI 500

/** @ingroup configuration Default IP address value for tcp communication with pritner subsystem, needed when configuration file is broken. */
#define DEFAULT_IP_ADDRESS_GIS "192.168.25.16"

/** @ingroup configuration Default IP address value for tcp communication with camera revision subsystem, needed when configuration file is broken. */
#define DEFAULT_IP_ADDRESS_PCI "192.168.50.14"

#define DEFAULT_IP_ADDRESS_QUADIENT "192.168.50.15"

#define DEFAULT_NETWORK_RESPONDER_TCP_PORT 1500

/** @ingroup configuration Path to the file with system configuration for the libconfig. */
#define CONFIGURATION_FILE_PATH "./sys_cfg"

/** @ingroup configuration Main window title string. */
#define MAIN_WINDOW_TITLE "Host-bk"

/** @ingroup configuration Application system icon path (icon shown in system application list panel widget). */
#define PROGRAM_ICON_PATH "./icons/icon.png"

/** @ingroup configuration Interval for content reading of quadient hot folder */
#define HOT_FOLDER_READING_INTERVAL 500000 // 500ms

#define MACHINE_CYCLE_TIMING 5000 //5ms

#define MACHINE_XBF_INTERVAL 100 //ms


#define Q_FEEDBACK_ADDR "./feedback/" //"/media/petr/DATA/q_hotfolder/data_in_feedback/" //

#define Q_HOT_FOLDER_ADDR "./q_hot/" //"/media/petr/DATA/q_hotfolder/data_out/" //"

#define Q_JOB_BACKUP "./q_hot/backup/" //"/media/petr/DATA/q_hotfolder/data_out/backup/" //

#define PCI_HOT_FOLDER_ADDR_IN "./c_hot_in/"

#define PCI_HOT_FOLDER_ADDR_OUT "./c_hot_out/"

#define GIS_HOT_FOLDER "./gis_hot/"

#define LOG_PATH "./log/"

#define JOB_INFO_CSV_PATH "./job_info_csv/"

#define LOG_FILE_NAME "host_log"

#define TIME_DELAY_ON_JOB_END 3000 // ms


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

enum _sheet_source_
{
	SSOURCE_MAIN=0,
	SSOURCE_COMPANION,

	SSOURCE_N
};

struct _core_;
typedef struct _core_ core;

/*********************************************** interface functions declarations ***********************************/


controler_init();
void controler_finalize();


uint8_t controler_iij_network_connected();
uint8_t controler_pci_network_connected();
uint8_t controler_quadient_network_connected();

uint8_t controler_set_interface_language(int lang_index);
void controler_refresh_dir_list();
void controler_set_machine_mode(int mode);

uint8_t controler_iij_set_ip_addr(char * ip_addr);
uint8_t controler_iij_set_tcp_port(int port);
char * controler_iij_get_ip_addr();
int controler_iij_get_tcp_port();
uint8_t controler_iij_connect();
uint8_t controler_iij_is_connected();
uint8_t controler_iij_disconnect();

uint8_t controler_pci_set_ip_addr(char * ip_addr);
uint8_t controler_pci_set_tcp_port(int port);
char * controler_pci_get_ip_addr();
int controler_pci_get_tcp_port();
uint8_t controler_pci_connect();
uint8_t controler_pci_is_connected();
uint8_t controler_pci_disconnect();

uint8_t controler_quadient_network_set_ip_address(char * ip_address);
uint8_t controler_iij_network_set_ip_address(char * ip_address);
uint8_t controler_pci_network_set_ip_address(char * ip_address);


void controler_set_max_stacked_sheets(int sheet_val);
void controler_set_max_rejected_sheet_seq(int sheet_val);
void controler_set_companion_sheet_source(int source);
void controler_set_sheet_source_confirmation(bool confirm);

uint8_t controler_set_q_main_hotfolder_path(const char * path);
uint8_t controler_set_q_feedback_hotfolder_path(const char * path);
uint8_t controler_set_q_backup_hotfolder_path(const char * path);
uint8_t controler_set_pci_hotfolder_in_path(const char * path);
uint8_t controler_set_pci_hotfolder_out_path(const char * path);
uint8_t controler_set_gis_hotfolder_path(const char * path);
uint8_t controler_set_job_report_hotfolder_path(const char * path);


char * controler_get_q_main_hotfolder_path();
char * controler_get_q_feedback_hotfolder_path();
char * controler_get_q_backup_hotfolder_path();
char * controler_get_pci_hotfolder_in_path();
char * controler_get_pci_hotfolder_out_path();
char * controler_get_gis_hotfolder_path();
char * controler_get_job_report_hotfolder_path();


uint8_t controler_print_start(const char * job_name);
uint8_t controler_print_pause();
uint8_t controler_print_continue();
uint8_t controler_print_cancel();
uint8_t controler_print_reset_error();
void controler_sheet_source_confirmation();

/* error messages return and translations  */
char* controler_machine_get_state_str();
uint8_t controler_machine_status_val();

int controler_get_error_val();
const char* controler_get_error_str();
char * controler_get_return_val_string(uint8_t status);
uint8_t controler_iij_status();
uint8_t controler_pci_status();

#if CORE_TEST_MODE
void controler_unit_test();
#endif


#endif
