#ifndef _CONTROLER_INTERFACE_H_
#define _CONTROLER_INTERFACE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <platform.h>
#include <aclib.h>


#ifdef PLATFORM_STC_PC
#include "io_card/io_card_const.h"
#include "io_card/io_mapping.h"
#else
#include "io_card_sim/io_card_const.h"
#include "io_card_sim/io_mapping.h"
#endif

#include "com_tcp/com_states.h"
#include "multi_lang/lang.h"



#define CONTROLER_TEST_MODE false


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

#define MACHINE_STATE_FEEDER_ERROR 17

#define MACHINE_STATE_WAIT_FOR_ENA_START 18

#define MACHINE_STATE_JAM_EVENT 19	

#define MACHINE_STATE_CLEAR_BELT_AFTER_JAM 20

#define MACHINE_STATE_STACKER_ERROR 21

#define MACHINE_STATE_FAKE_COMPANION 22















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
#define MACHINE_ERR_PRINT_MAIN_FREEZE 30
#define MACHINE_ERR_PRINT_COMPANION_FREEZE 31
#define MCAHINE_ERR_PRINT_INITIALIZATION_FREEZE 32
#define MACHINE_ERR_PRINT_FINALIZING_FREEZE 33
#define MACHINE_ERR_REJECT_BIN_FULL 34
#define MACHINE_ERR_STACKER_FULL 35
#define MACHINE_ERR_PAPER_JAM_CONVAYOR 36
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




struct _rep_struct_;
typedef struct _rep_struct_ rep_struct;


struct _rep_struct_
{
	char * finish_state;
	char * order_name;
	char * job_name;
	int wrong_sheet_number;
	int rejected_sheet_number;
	int feeded_sheet_number;
	int stacked_sheet_number;
	int sheet_number;
	int stamp_number;
	char * date_time;
};



/*********************************************** interface functions declarations ***********************************/








int8_t controler_init();
void controler_finalize();


uint8_t controler_iij_network_connected();
uint8_t controler_pci_network_connected();
uint8_t controler_quadient_network_connected();

void controler_set_interface_language(int lang_index);
int controler_get_interface_language();
void controler_refresh_dir_list();
void controler_set_machine_mode(int mode);
int controler_get_machine_mode();

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
char * controler_quadient_network_get_ip_address();
char * controler_iij_network_get_ip_address();
char * controler_pci_network_get_ip_address();

uint8_t controler_job_list_changed();
void controler_total_clear_hotfolder();

void controler_set_fan_intensity(int intensity);
int controler_get_fan_intensity();
uint8_t controler_get_fan_activity();
void controler_set_fan_activity(uint8_t activity);

void controler_tab_insert_set_sequence(int32_t sequence);
int32_t controler_tab_insert_get_sequence();
void controler_tab_insert_set_automat(bool automat);
bool controler_tab_insert_get_automat();
void controler_tab_inset_set_length(int32_t length);
int32_t controler_tab_insert_get_length();
void controler_tab_insert_manual_insert();


void controler_free_report_information_struct(rep_struct * this);

void controler_lock_thread();
void controler_unlock_thread();

array_list * controler_csv_manage_filter_report_csv(array_list * report_csv_list, 
					int day_from, int month_from, int year_from, 
					int day_to, int month_to, int year_to);
uint8_t controler_split_csv(array_list * filtered_csv_list, char * address);

void controler_set_max_stacked_sheets(int sheet_val);
void controler_set_max_rejected_sheet_seq(int sheet_val);
void controler_set_feed_delay(uint32_t delay);
uint32_t controler_get_get_feed_delay();
int controler_get_max_rejected_sheet_seq();
void controler_set_companion_sheet_source(int source);
extern void controler_set_sheet_source_confirmation(bool confirm);
bool controler_get_sheet_source_confirmation();
int controler_get_sheet_source();
int controler_get_max_stacked_sheet();
int controler_get_stacked();

double controler_get_statistics_on_day_error_rate(int8_t day);
int64_t controler_get_statistics_on_day_total_feeded_sheets(int8_t day);
int64_t controler_get_statistics_on_day_total_stacked_sheets(int8_t day);
int64_t controler_get_statistics_on_day_total_rejected_sheets(int8_t day);

double controler_get_statistics_error_rate();
int64_t controler_get_statistics_total_feeded_sheets();
int64_t controler_get_statistics_total_stacked_sheets();
int64_t controler_get_statistics_total_rejected_sheets();


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


uint8_t controler_get_feeder_status();
uint8_t controler_get_stacker_status();
const char * controler_get_feeder_status_string(uint8_t status);
const char * controler_get_stacker_status_string(uint8_t status);

int32_t controler_get_stacked_sheets();
int32_t controler_get_total_sheet_number();
int32_t controler_get_feeded_sheets();
int32_t controler_get_feeded_companion_sheets();
int32_t controler_get_rejected_sheets();
int32_t controler_get_tab_inserts();
int controler_get_sheet_number_in_job();
int controler_get_rejected_sheet_in_job();

char * controler_get_gis_status();

uint8_t controler_print_start(const char * job_name);
uint8_t controler_print_pause();
uint8_t controler_print_continue();
uint8_t controler_print_cancel();
uint8_t controler_print_reset_error();
void controler_manual_feed_sheet();
void controler_manual_set_ena_state(bool state);
void controler_print_one_sheet_req();
void controler_finish_job_and_print_companion();

void controler_sheet_source_confirmation();

/* error messages return and translations  */
char* controler_machine_get_state_str();
uint8_t controler_machine_status_val();

bool controler_get_print_one_req_val();
bool controler_get_fake_companion_req_val();

int controler_get_error_val();
const char* controler_get_error_str();
char * controler_get_return_val_string(uint8_t status);
uint8_t controler_iij_status();
uint8_t controler_pci_status();

array_list * controler_get_report_csv_list();


/* io_card public interface */
char ** controler_get_a1_input_labels();
char ** controler_get_a2_input_labels();
char ** controler_get_a1_output_labels();
char ** controler_get_a2_output_labels();
void controler_set_a1_output(uint8_t index, uint8_t state);
void controler_set_a2_output(uint8_t index, uint8_t state);
uint8_t controler_get_card_input(int card, int addr);
uint8_t controler_get_card_output(int card, int addr);
const char * controler_get_printed_job_name();

rep_struct * controler_load_report_information(char * report_csv_name);



char * controler_get_job_name(int job_index);
char * controler_get_job_order_name(int job_index);
char * controler_get_job_pdf_name(int job_index);
char * controler_get_job_bkcore_csv_name(int job_index);
char * controler_get_job_camera_csv_name(int job_index);
char * controler_get_job_date_time(int job_index);
char * controler_get_job_stamp_type(int job_index);
char * controler_get_job_nominal_value(int job_index);
int controler_get_job_stamp_number(int job_index);
int controler_get_job_sheet_number(int job_index);
int controler_get_job_order(int job_index);
char controler_get_job_flag(int job_index);
int controler_get_job_queue_size();
void controler_set_manual_mode(bool state);
bool controler_get_manual_mode_state();

uint64_t controler_get_time_for_one_sheet();

#if CONTROLER_TEST_MODE == true
void controler_unit_test();
#endif

#endif
