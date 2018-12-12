/**
** @file config.h
** @defgroup configuration
** File with software configuration (for apply changes application build is necessary!)
**
** @author Petr Horacek
** @version 1
*/


#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
** by defining of this macro is enabled many features from system library (libc) (features.h enables recognize type 
** of directory content by macros)
*/

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif



#define IO_SIMULATION TRUE

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

#define DEFAULT_TEXT_EDITOR "zenity --text-info --width 800 --height 600 2>/dev/null"



/* quadient hotfolder file parameters */
const char* available_formats[2] = {".pdf", ".csv"};
const char* available_types_pdf[1] = {"_pdf"};
const char* available_types_csv[2] = {"_camera", "_bkcore"};
const char* available_flags[3] = {"_p", "_k", "_e"};


/* color definition to main window */
const double bg[3] = {0.97, 0.89, 0};
const double fg[3] = {0.18, 0.18, 0.18};

const double settings_btn_bg[3] = {0.7, 0.7, 0.7};
const double settings_btn_fg_s[3] = {0.7, 0.7, 0.7};


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
	const char * gui_job_name;
	const char * gui_job_pdf_name;
	const char * gui_job_bkcore_name;
	const char * gui_job_camera_name;
	const char * print_state_print;
	const char * print_state_job_error;
	const char * print_state_wait;
	const char * print_state_wait_for_data;
	const char * set_btn_network;
	const char * set_btn_lang_settings;
	const char * set_btn_print_param;
	const char * set_btn_hotfolder;
	const char * set_btn_io_vision;
	const char * set_btn_back;
	const char * set_btn_back_to_settngs;

	const char * err_no_error;
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

	const char * rep_csv_log_column_file_name;

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
	const char * g_cnt_rejected_seq_lbl;
	const char * g_cnt_tab_insert_blb;
	const char * g_job_list_lbl;
	const char * g_report_csv_list_lbl;

	const char * par_max_stacked_sheet_lbl;
	const char * par_rejected_sheet_seq_lbl;
	const char * par_sheet_source_lbl;
	const char * par_print_confirm_lbl;
	const char * par_sheet_source_main;
	const char * par_sheet_source_companion;
	const char * par_machine_mode_lbl;
	const char * par_mm_setup_comb;
	const char * par_mm_inspection_comb;
	const char * par_mm_print_comb;
	const char * par_mm_print_inspection_comb;
};
typedef struct _lang_ lang;


lang multi_lang[lang_num];



void multi_lang_init_czech()
{
/* Czech labels */
    multi_lang[lang_cz].lang_name = "Čeština";
    multi_lang[lang_cz].win_title = "Host-BK CZ";
    multi_lang[lang_cz].gui_job_name = "Jméno jobu";
    multi_lang[lang_cz].gui_job_pdf_name = "Soubor pdf";
    multi_lang[lang_cz].gui_job_bkcore_name = "BKcore csv";
    multi_lang[lang_cz].gui_job_camera_name = "Camera csv";
    multi_lang[lang_cz].print_state_print = "Tisk";
    multi_lang[lang_cz].print_state_job_error = "Job v chybě";
    multi_lang[lang_cz].print_state_wait = "Čeká se na start";
    multi_lang[lang_cz].print_state_wait_for_data = "Čeká se na data";
    multi_lang[lang_cz].set_btn_network = "Nastavení sítě";
    multi_lang[lang_cz].set_btn_lang_settings = "Nastavení jazyka";
    multi_lang[lang_cz].set_btn_print_param = "Tiskové parametry";
    multi_lang[lang_cz].set_btn_hotfolder = "Nastavení hotfolderů";
    multi_lang[lang_cz].set_btn_io_vision = "Zobrazení IO";
    multi_lang[lang_cz].set_btn_back = "Zpět na controlní panel";
    multi_lang[lang_cz].set_btn_back_to_settngs = "Zpět";

    multi_lang[lang_cz].err_no_error = "Připraven";
    multi_lang[lang_cz].err_cant_save_f_csv = "Nelze uložit zpětnovazební csv!";
    multi_lang[lang_cz].err_analyze_camera_csv = "Chyba analýzy camerového csv!";
    multi_lang[lang_cz].err_cant_load_bkcore_csv = "Nelze načíst řídící csv!";
    multi_lang[lang_cz].err_cant_copy_pdf_to_gis = "Nelze odeslat pdf soubor do GISu!";
    multi_lang[lang_cz].err_cant_copy_csv_to_camera = "Nelze odeslat csv soubor do kamery!";
    multi_lang[lang_cz].err_job_order_mismatch = "Nesoulad pořadových indexů jobů!";
    multi_lang[lang_cz].err_stacker_jam_pile = "Vykladač zahlcen!";
    multi_lang[lang_cz].err_stacker_jam_conveyor = "Vykladač ucpán!";
    multi_lang[lang_cz].err_stacker_bottom = "Vykladač Bottom!";
    multi_lang[lang_cz].err_stacker_multifunction = "Vykladač multifunkce!";
    multi_lang[lang_cz].err_stacker_off = "Vykladač není aktivní!";
    multi_lang[lang_cz].err_feeder_e_stop = "Podavač hlásí E-stop!";
    multi_lang[lang_cz].err_feeder_off = "Podavač není aktivní!";
    multi_lang[lang_cz].err_feeder_multifunction = "Podavač multifunkce!";
    multi_lang[lang_cz].err_feeder_sheet_missing = "Podavač nepodává archy!";
    multi_lang[lang_cz].err_feeder_double_sheet = "Podavač podal více archů současně!";
    multi_lang[lang_cz].err_machine_e_stop = "Stroj hlásí E-stop!";
    multi_lang[lang_cz].err_reject_bin_jam = "Výhybka ucpána!";
    multi_lang[lang_cz].err_feeder_jam = "Podavač ucpán!";
    multi_lang[lang_cz].err_ti = "Podavač nastřelovacích proužků hlásí chybu!";
    multi_lang[lang_cz].err_ta = "Podavač prokladových archů hlásí chybu!";
    multi_lang[lang_cz].err_counters_mismatch = "Nesoulad počítadel!";
    multi_lang[lang_cz].err_low_print_quality = "Nízká kvalita tisku! Zkontrolujte tiskové hlavy.";
    multi_lang[lang_cz].err_gis_disconnected = "Počítač GISu neodpovídá!";
    multi_lang[lang_cz].err_csv_sheet_num = "Chybné pořadí csv archů!"; 
    multi_lang[lang_cz].err_sheet_feeder_redirection = "Chyba při přesměrování nakladače pro prokladový arch!";
    multi_lang[lang_cz].err_pci_computer_not_responding = "Počítač PCI neodpovídá!";
    multi_lang[lang_cz].err_iij_computer_not_responding = "Počítač IIJ neodpovídá!";
    multi_lang[lang_cz].err_quadient_computer_not_responding = "Počítač Quadient neodpovídá!";
    multi_lang[lang_cz].err_unknown_error = "Neznámá chyba!";

    multi_lang[lang_cz].set_lan_label = "Jazyk rozhraní:";
    multi_lang[lang_cz].set_net_iij_tcp_port = "Číslo TCP portu GISu:";
    multi_lang[lang_cz].set_net_iij_ip_addr = "IP adresa GISu:";
    multi_lang[lang_cz].set_net_iij_connection = "Ovládání síťového spojení:";
    multi_lang[lang_cz].set_net_iij_wrong_tcp_port_label = "Chybný formát TCP portu!";
    multi_lang[lang_cz].set_net_iij_wrong_ip_address_label = "Chybný formát IP adresy!";
    multi_lang[lang_cz].set_net_iij_connection_test_label = "IP adresa počítače IIJ:";
    multi_lang[lang_cz].set_net_pci_connection_test_label = "IP adresa počítače PCI:";
    multi_lang[lang_cz].set_net_quadient_connection_test_label = "IP adresa počítače Quadient:";

    multi_lang[lang_cz].file_chooser_btn_browse = "Procházet";
    multi_lang[lang_cz].file_chooser_cancel_btn = "Zrušit";
    multi_lang[lang_cz].file_chooser_select_btn = "Vybrat";
    multi_lang[lang_cz].hot_q_main_fs_title = "Datový hotfolder.";
    multi_lang[lang_cz].hot_q_feedback_fs_title = "Zpětnovazební hotfolder.";
    multi_lang[lang_cz].hot_q_backup_fs_title = "Hotfolder pro zálohování.";
    multi_lang[lang_cz].hot_pci_in_fs_title = "Vstupní kamerový hotfolder.";
    multi_lang[lang_cz].hot_pci_out_fs_title = "Výstupní kamerový hotfolder.";
    multi_lang[lang_cz].hot_gis_fs_title = "Hotfolder pro GIS.";
    multi_lang[lang_cz].hot_report_csv_fs_title = "Reportovací adresář.";

    multi_lang[lang_cz].hot_q_main_fs_label = "Datový hotfolder Quadient:";
    multi_lang[lang_cz].hot_q_feedback_fs_label = "Zpětnovazební hotfolder Quadient:";
    multi_lang[lang_cz].hot_q_backup_fs_label = "Hotfolder pro zálohování Quadient:";
    multi_lang[lang_cz].hot_pci_in_fs_label = "Vstupní kamerový hotfolder:";
    multi_lang[lang_cz].hot_pci_out_fs_label = "Výstupní kamerový hotfolder:";
    multi_lang[lang_cz].hot_gis_fs_label = "Hotfolder pro GIS:";
    multi_lang[lang_cz].hot_report_csv_fs_label = "Adresář pro reporty:";

    multi_lang[lang_cz].rep_csv_log_column_file_name = "Reportové csv";

    multi_lang[lang_cz].g_counters_label = "POČÍTADLA STROJE:";
    multi_lang[lang_cz].g_feeder_lbl = "NAKLADAČE:";
    multi_lang[lang_cz].g_status_gis_lbl = "GIS tiskový řadič status:";
    multi_lang[lang_cz].g_status_machine_lbl = "Status stroje:";
    multi_lang[lang_cz].g_cnt_main_feed_lbl = "Naložené archy hlavní nakladač:";
    multi_lang[lang_cz].g_stacker_lbl = "VYKLADAČ:";
    multi_lang[lang_cz].g_reject_bin_lbl = "VÝHYBKA:";
    multi_lang[lang_cz].g_cnt_companion_feed_lbl = "Naložené prokladové archy:";
    multi_lang[lang_cz].g_cnt_rejected_lbl = "Vadné archy:";
    multi_lang[lang_cz].g_cnt_stakced_lbl = "Vyložené archy očekávané/aktuální:";
    multi_lang[lang_cz].g_cnt_rejected_seq_lbl = "Sekvence vadných archů:";
    multi_lang[lang_cz].g_cnt_tab_insert_blb = "Nastřelovací proužky:";
    multi_lang[lang_cz].g_job_list_lbl = "Aktuální tisková úloha:";
    multi_lang[lang_cz].g_report_csv_list_lbl = "Historie vytištěných úloh:";

    multi_lang[lang_cz].par_max_stacked_sheet_lbl = "Maximum archů ve vykladači:";
    multi_lang[lang_cz].par_rejected_sheet_seq_lbl = "Maximální sekvence vadných archů:";
    multi_lang[lang_cz].par_sheet_source_lbl = "Nakladač pro prokladový arch:";
    multi_lang[lang_cz].par_print_confirm_lbl = "Potvrzení naložení archu z hlavního nakladače:";
    multi_lang[lang_cz].par_sheet_source_main = "Hlavní nakladač";
    multi_lang[lang_cz].par_sheet_source_companion = "Prokladový nakladač";
    multi_lang[lang_cz].par_machine_mode_lbl = "Pracovní režim dopravníku: ";
    multi_lang[lang_cz].par_mm_setup_comb = "NASTAVENÍ";
    multi_lang[lang_cz].par_mm_inspection_comb = "INSPEKCE";
    multi_lang[lang_cz].par_mm_print_comb = "TISK";
    multi_lang[lang_cz].par_mm_print_inspection_comb = "TISK S INSPEKCÍ";
}

void multi_lang_init_english()
{
 /* English labels */

    multi_lang[lang_en].lang_name = "English";
    multi_lang[lang_en].win_title = "Host-BK EN";
    multi_lang[lang_en].gui_job_name = "Job name";
    multi_lang[lang_en].gui_job_pdf_name = "pdf file";
    multi_lang[lang_en].gui_job_bkcore_name = "BKcore csv";
    multi_lang[lang_en].gui_job_camera_name = "Camera csv";
    multi_lang[lang_en].print_state_print = "Printing";
    multi_lang[lang_en].print_state_job_error = "Job in error";
    multi_lang[lang_en].print_state_wait = "Waiting for start";
    multi_lang[lang_en].print_state_wait_for_data = "Waiting for data";
    multi_lang[lang_en].set_btn_network = "Network settings";
    multi_lang[lang_en].set_btn_lang_settings = "Language settings";
    multi_lang[lang_en].set_btn_print_param = "Print parameters";
    multi_lang[lang_en].set_btn_hotfolder = "Hotfolder settings";
    multi_lang[lang_en].set_btn_io_vision = "IO vision";
    multi_lang[lang_en].set_btn_back = "Back to control page";
    multi_lang[lang_en].set_btn_back_to_settngs = "Back";

    multi_lang[lang_en].err_no_error = "Ready";
    multi_lang[lang_en].err_cant_save_f_csv = "Can't save feedback csv!";
    multi_lang[lang_en].err_analyze_camera_csv = "Camera csv analyzing error!";
    multi_lang[lang_en].err_cant_load_bkcore_csv = "Can't load control csv!";
    multi_lang[lang_en].err_cant_copy_pdf_to_gis = "Can't send pdf file to GIS!";
    multi_lang[lang_en].err_cant_copy_csv_to_camera = "Can't send csv file to camera!";
    multi_lang[lang_en].err_job_order_mismatch = "Job index order mismatch!";
    multi_lang[lang_en].err_stacker_jam_pile = "Stacker jam pile!";
    multi_lang[lang_en].err_stacker_jam_conveyor = "Stacker jam conveyer!";
    multi_lang[lang_en].err_stacker_bottom = "Stakcer bottom!";
    multi_lang[lang_en].err_stacker_multifunction = "Stacker multifunction!";
    multi_lang[lang_en].err_stacker_off = "Stacker in off state!";
    multi_lang[lang_en].err_feeder_e_stop = "Feeder E-stop!";
    multi_lang[lang_en].err_feeder_off = "Feeder in off state!";
    multi_lang[lang_en].err_feeder_multifunction = "Feeder multifunction!";
    multi_lang[lang_en].err_feeder_sheet_missing = "Feeder sheet missing!";
    multi_lang[lang_en].err_feeder_double_sheet = "Feeder double sheet!";
    multi_lang[lang_en].err_machine_e_stop = "Machine E-stop!";
    multi_lang[lang_en].err_reject_bin_jam = "Reject bin jam!";
    multi_lang[lang_en].err_feeder_jam = "Feeder jam!";
    multi_lang[lang_en].err_ti = "Tab insert error!";
    multi_lang[lang_en].err_ta = "Companion sheet feeder error!";
    multi_lang[lang_en].err_counters_mismatch = "Counters mismatch!";
    multi_lang[lang_en].err_low_print_quality = "Low print quality! Check print heads.";
    multi_lang[lang_en].err_gis_disconnected = "GIS computer not responding!";
    multi_lang[lang_en].err_csv_sheet_num = "Wrong sheet order in csv file!"; 
    multi_lang[lang_en].err_sheet_feeder_redirection = "Error with feeder redirection for companion sheet!";
    multi_lang[lang_en].err_pci_computer_not_responding = "PCI computer not responding!";
    multi_lang[lang_en].err_iij_computer_not_responding = "IIJ computer not responding!";
    multi_lang[lang_en].err_quadient_computer_not_responding = "Quadient computer not responding!";
    multi_lang[lang_en].err_unknown_error = "Unknown error!";

    multi_lang[lang_en].set_lan_label = "Interface language:";
    multi_lang[lang_en].set_net_iij_tcp_port = "TCP port nuber for GIS:";
    multi_lang[lang_en].set_net_iij_ip_addr = "IP address for GIS:";
    multi_lang[lang_en].set_net_iij_connection = "Network connection control:";
    multi_lang[lang_en].set_net_iij_wrong_tcp_port_label = "Wrong TCP port format!";
    multi_lang[lang_en].set_net_iij_wrong_ip_address_label = "Wrong IP address format";
    multi_lang[lang_en].set_net_iij_connection_test_label = "IP address of IIJ computer:";
    multi_lang[lang_en].set_net_pci_connection_test_label = "IP address of PCI computer:";
    multi_lang[lang_en].set_net_quadient_connection_test_label = "IP address of Quadient computer:";

    multi_lang[lang_en].file_chooser_btn_browse = "Browse";
    multi_lang[lang_en].file_chooser_cancel_btn = "Cancel";
    multi_lang[lang_en].file_chooser_select_btn = "Select";
    multi_lang[lang_en].hot_q_main_fs_title = "Data hotfolder";
    multi_lang[lang_en].hot_q_feedback_fs_title = "Feedback hotfolder";
    multi_lang[lang_en].hot_q_backup_fs_title = "Backup hotfolder";
    multi_lang[lang_en].hot_pci_in_fs_title = "Camera input hotfolder";
    multi_lang[lang_en].hot_pci_out_fs_title = "Camera output hotfolder";
    multi_lang[lang_en].hot_gis_fs_title = "GIS hotfolder";
    multi_lang[lang_en].hot_report_csv_fs_title = "Report directory";

    multi_lang[lang_en].hot_q_main_fs_label = "Data hotfolder Quadient:";
    multi_lang[lang_en].hot_q_feedback_fs_label = "Feedback hotfolder Quadient:";
    multi_lang[lang_en].hot_q_backup_fs_label = "Backup hotfolder Quadient:";
    multi_lang[lang_en].hot_pci_in_fs_label = "Camera input hotfolder:";
    multi_lang[lang_en].hot_pci_out_fs_label = "Camera output hotfolder:";
    multi_lang[lang_en].hot_gis_fs_label = "GIS hotfolder:";
    multi_lang[lang_en].hot_report_csv_fs_label = "Report directory:";

    multi_lang[lang_en].rep_csv_log_column_file_name = "Report csv";

    multi_lang[lang_en].g_counters_label = "MACHINE COUNTERS:";
    multi_lang[lang_en].g_feeder_lbl = "FEEDERS:";
    multi_lang[lang_en].g_status_gis_lbl = "GIS print controler status:";
    multi_lang[lang_en].g_status_machine_lbl = "Machine status";
    multi_lang[lang_en].g_cnt_main_feed_lbl = "Feeded sheets main feeder:";
    multi_lang[lang_en].g_stacker_lbl = "STAKCER:";
    multi_lang[lang_en].g_reject_bin_lbl = "Reject bin:";
    multi_lang[lang_en].g_cnt_companion_feed_lbl = "Feeded companion sheets:";
    multi_lang[lang_en].g_cnt_rejected_lbl = "Rejected sheets:";
    multi_lang[lang_en].g_cnt_stakced_lbl = "Stacked sheets expected/current:";
    multi_lang[lang_en].g_cnt_rejected_seq_lbl = "Rejected sheets sequence:";
    multi_lang[lang_en].g_cnt_tab_insert_blb = "Tab inserts:";
    multi_lang[lang_en].g_job_list_lbl = "Current print job:";
    multi_lang[lang_en].g_report_csv_list_lbl = "Printed job history:";

    multi_lang[lang_en].par_max_stacked_sheet_lbl = "Maximum number of sheets in the stecker:";
    multi_lang[lang_en].par_rejected_sheet_seq_lbl = "Maximum number of rejected sheets in sequence:";
    multi_lang[lang_en].par_sheet_source_lbl = "Feeder for companion sheet:";
    multi_lang[lang_en].par_print_confirm_lbl = "Confirmation feeding from main feeder:";
    multi_lang[lang_en].par_sheet_source_main = "Main feeder";
    multi_lang[lang_en].par_sheet_source_companion = "Companion feeder";
    multi_lang[lang_en].par_machine_mode_lbl = "Conveyor work mode:";
    multi_lang[lang_en].par_mm_setup_comb = "SETUP";
    multi_lang[lang_en].par_mm_inspection_comb = "INSPECTION";
    multi_lang[lang_en].par_mm_print_comb = "PRINT";
    multi_lang[lang_en].par_mm_print_inspection_comb = "PRINT INSPECTION";
}

void multi_lang_init()
{
    multi_lang_init_czech();
    multi_lang_init_english();
}


#endif
