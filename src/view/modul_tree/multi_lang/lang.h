#ifndef _LANG_H_
#define _LANG_H_

#include <stdio.h>
#include <stdlib.h>

struct _lang_;
typedef struct _lang_ lang;


/* multilanguage labels settings */
enum _lang_index_
{
	lang_cz = 0,
	lang_en,
	
	lang_num,
};

struct _lang_
{
	const char * lang_name;
 	const char * win_title;
	const char * gui_job_state;
	const char * gui_job_order_name;
	const char * gui_job_q_name;
	const char * gui_job_nominal;
	const char * gui_job_sheet_number;
	const char * gui_job_stamp_number;
	const char * gui_job_date_time;
	const char * print_state_print;
	const char * print_state_pause;
	const char * print_state_job_error;
	const char * print_state_wait;
	const char * print_state_wait_for_data;
	const char * set_btn_network;
	const char * set_btn_lang_settings;
	const char * set_btn_print_param;
	const char * set_btn_hotfolder;
	const char * set_btn_machine_overview;
	const char * set_btn_io_vision;
	const char * set_btn_back;
	const char * set_btn_back_to_settngs;

	const char * info_label_preparing;
	const char * info_label_finishing;
	const char * info_label_feed_sheet_confirm;
	const char * info_label_fix_feeder_issue;
	const char * info_label_companion_fake;
	const char * info_label_companion_fake_instruction;
	const char * info_label_data_generation;
	const char * info_label_approx_max_stacked_sheet;
	const char * info_label_max_stacked_sheet;
	const char * info_label_feeder_stacker_not_prepared;


	const char * err_no_error;
	const char * err_paper_jam_conveyor;
	const char * err_cant_save_f_csv;
	const char * err_analyze_camera_csv;
	const char * err_cant_load_bkcore_csv;
	const char * err_cant_copy_pdf_to_gis;
	const char * err_cant_copy_csv_to_camera;
	const char * err_job_order_mismatch;
	const char * err_stacker_jam_pile;
	const char * err_stacker_jam_conveyor;
	const char * err_stacker_bottom;
	const char * err_stacker_multifunction;
	const char * err_stacker_off;
	const char * err_feeder_e_stop;
	const char * err_feeder_off;
	const char * err_feeder_multifunction;
	const char * err_feeder_sheet_missing;
	const char * err_feeder_double_sheet;
	const char * err_machine_e_stop;
	const char * err_reject_bin_jam;
	const char * err_feeder_jam;
	const char * err_ti;
	const char * err_ta;
	const char * err_counters_mismatch;
	const char * err_low_print_quality;
	const char * err_gis_disconnected;
	const char * err_csv_sheet_num;
	const char * err_sheet_feeder_redirection;
	const char * err_pci_computer_not_responding;
	const char * err_iij_computer_not_responding;
	const char * err_quadient_computer_not_responding;
	const char * err_print_main_freeze;
	const char * err_print_companion_freeze;
	const char * err_print_initialization_freeze;
	const char * err_print_finalizing_freeze;
	const char * err_stacker_full;
	const char * err_reject_bin_full;
	const char * err_unknown_error;

	const char * set_lan_label;
	const char * set_net_iij_tcp_port;
	const char * set_net_iij_ip_addr;
	const char * set_net_iij_connection;
	const char * set_net_iij_wrong_tcp_port_label;
	const char * set_net_iij_wrong_ip_address_label;
	const char * set_net_iij_connection_test_label;
	const char * set_net_pci_connection_test_label;
	const char * set_net_quadient_connection_test_label;

	const char * file_chooser_btn_browse;
	const char * file_chooser_cancel_btn;
	const char * file_chooser_select_btn;
	const char * hot_q_main_fs_title;
	const char * hot_q_feedback_fs_title;
	const char * hot_q_backup_fs_title;
	const char * hot_pci_in_fs_title;
	const char * hot_pci_out_fs_title;
	const char * hot_gis_fs_title;
	const char * hot_report_csv_fs_title;

	const char * hot_q_main_fs_label;
	const char * hot_q_feedback_fs_label;
	const char * hot_q_backup_fs_label;
	const char * hot_pci_in_fs_label;
	const char * hot_pci_out_fs_label;
	const char * hot_gis_fs_label;
	const char * hot_report_csv_fs_label;

	const char * rep_csv_state;
	const char * rep_csv_order_name;
	const char * rep_csv_rejected_sheets;
	const char * rep_csv_sheet_number;
	const char * rep_csv_stamp_number;	
	const char * rep_csv_job_name;
	const char * rep_csv_date_time;
	

	const char * g_status_gis_lbl;
	const char * g_counters_label;
	const char * g_feeder_lbl;
	const char * g_status_machine_lbl;
	const char * g_reject_bin_lbl;
	const char * g_cnt_main_feed_lbl;
	const char * g_stacker_lbl;
	const char * g_cnt_companion_feed_lbl;
	const char * g_cnt_rejected_lbl;
	const char * g_cnt_stakced_lbl;
	const char * g_cnt_compenzation_lbl;
	const char * g_cnt_rejected_seq_lbl;
	const char * g_cnt_tab_insert_blb;
	const char * g_job_list_lbl;
	const char * g_report_csv_list_lbl;
	const char * g_complet_feeded_sheets;
	const char * g_complet_stacked_sheets;
	const char * g_complet_rejected_sheets;
	const char * g_error_rate;

	const char * par_max_stacked_sheet_lbl;
	const char * par_rejected_sheet_seq_lbl;
	const char * par_sheet_source_lbl;
	const char * par_print_confirm_lbl;
	const char * par_sheet_source_main;
	const char * par_sheet_source_companion;
	const char * par_feed_delay_lbl;
	const char * par_machine_mode_lbl;
	const char * par_mm_setup_comb;
	const char * par_mm_inspection_comb;
	const char * par_mm_print_comb;
	const char * par_mm_print_inspection_comb;
	const char * par_stacker_status;
	const char * par_feeder_status;
	const char * feeder_status_ready;
	const char * feeder_status_ready_to_feed;
	const char * feeder_status_feeding;
	const char * stacker_status_ready;
	const char * stacker_status_ready_to_stack;
	const char * stacker_status_stacking;	
	const char * gui_go_to_csv_manage_page_label;
	
	const char * filter_day;
	const char * filter_year;
	const char * filter_month;
	const char * csv_manage_return;
	const char * csv_manage_export;
	const char * to;
	const char * from;
	const char * print_active;
};


void multi_lang_init();
lang * multi_lang_get(int language);

#endif
