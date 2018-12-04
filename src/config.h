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
#define TCP_NETWORK_TTL_U 500000 

/** @ingroup configuration Time interva for correct receave the respond from TCP socket in seconds. */
#define TCP_NETWORK_TTL_S 1

/** @ingroup configuration Default TCP port value for tcp communication with pritner subsystem, needed when configuration file is broken. */
#define DEFAULT_TCP_PORT_GIS 2000

/** @ingroup configuration Default TCP port value for tcp communication with camera revision subsystem, needed when configuration file is broken. */
#define DEFAULT_TCP_PORT_PCI 500

/** @ingroup configuration Default IP address value for tcp communication with pritner subsystem, needed when configuration file is broken. */
#define DEFAULT_IP_ADDRESS_GIS "192.168.1.127"

/** @ingroup configuration Default IP address value for tcp communication with camera revision subsystem, needed when configuration file is broken. */
#define DEFAULT_IP_ADDRESS_PCI "192.168.0.2"

/** @ingroup configuration Path to the file with system configuration for the libconfig. */
#define CONFIGURATION_FILE_PATH "./sys_cfg"

/** @ingroup configuration Main window title string. */
#define MAIN_WINDOW_TITLE "Host-bk"

/** @ingroup configuration Application system icon path (icon shown in system application list panel widget). */
#define PROGRAM_ICON_PATH "./icons/icon.png"

/** @ingroup configuration Interval for content reading of quadient hot folder */
#define HOT_FOLDER_READING_INTERVAL 1000000 

#define MACHINE_CYCLE_TIMING 100000//5000

#define MACHINE_XBF_INTERVAL 100 //ms


#define Q_FEEDBACK_ADDR "./feedback/" //"/media/petr/DATA/q_hotfolder/data_in_feedback/" //

#define Q_HOT_FOLDER_ADDR "./q_hot/" //"/media/petr/DATA/q_hotfolder/data_out/" //"

#define Q_JOB_BACKUP "./q_hot/backup/" //"/media/petr/DATA/q_hotfolder/data_out/backup/" //

#define PCI_HOT_FOLDER_ADDR_IN "./c_hot_in/"

#define PCI_HOT_FOLDER_ADDR_OUT "./c_hot_out/"

#define GIS_HOT_FOLDER "./gis_hot/"

#define JOB_LOG "./job_log/"

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
	const char * err_unknown_error;

	const char * set_lan_labgel;
	const char * set_net_iij_tcp_port;
	const char * set_net_iij_ip_addr;
	const char * set_net_iij_connection;
	const char * set_net_iij_wrong_tcp_port_label;
	const char * set_net_iij_wrong_ip_address_label;

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
};
typedef struct _lang_ lang;

				/* czech language labels */
const lang multi_lang[lang_num] = {{"Čeština",  							/* lang_name */
				"Host-BK CZ",							/* win_title */	
				"Jméno jobu",
				"Soubor pdf",
				"BKcore csv",
				"Camera csv",
				"Tisk",
				"Job v chybě",
				"Čeká se na start",
				"Čeká se na data",
				"Nastavení sítě",
				"Nastavení jazyka",
				"Tiskové parametry",
				"Nastavení hotfolderů",
				"Zobrazení IO",
				"Zpět na controlní panel",
				"Zpět",

				"Připraven",
				"Nelze uložit zpětnovazební csv!",
				"Chyba analýzy camerového csv!",
				"Nelze načíst řídící csv!",
				"Nelze odeslat pdf soubor do GISu!",
				"Nelze odeslat csv soubor do kamery!",
				"Nesoulad pořadových indexů jobů!",
				"Vykladač zahlcen!",
				"Vykladač ucpán!",
				"Vykladač Bottom!",
				"Vykladač multifunkce!",
				"Vykladač není aktivní!",
				"Podavač hlásí E-stop!",
				"Podavač není aktivní!",
				"Podavač multifunkce!",
				"Podavač nepodává archy!",
				"Podavač podal více archů současně!",
				"Stroj hlásí E-stop!",
				"Výhybka ucpána!",
				"Podavač ucpán!",
				"Podavač nastřelovacích proužků hlásí chybu!",
				"Podavač prokladových archů hlásí chybu!",
				"Nesoulad počítadel!",
				"Nízká kvalita tisku! Zkontrolujte tiskové hlavy.",
				"Počítač GISu odpojen!",
				"Neznámá chyba!",

				"Jazyk rozhraní:",
				"Číslo TCP portu GISu:",
				"IP adresa GISu:",
				"Ovládání síťového spojení:",
				"Chybný formát TCP portu!",
				"Chybný formát IP adresy!",

				"Procházet",
				"Zrušit",
				"Vybrat",
				"Datový hotfolder.",
				"Zpětnovazební hotfolder.",
				"Hotfolder pro zálohování.",
				"Vstupní kamerový hotfolder.",
				"Výstupní kamerový hotfolder.",
				"Hotfolder pro GIS.",
				"Reportovací adresář.",
				"Datový hotfolder Quadient:",
				"Zpětnovazební hotfolder Quadient:",
				"Hotfolder pro zálohování Quadient:",
				"Vstupní kamerový hotfolder:",
				"Výstupní kamerový hotfolder:",
				"Hotfolder pro GIS:",
				"Adresář pro reporty:",
				"Reportové csv"},

				/* english language label */
				{"English", 							/* lang_name */
				"Host-BK EN",							/* win_title */
				"Job name",
				"pdf file",
				"BKcore csv",
				"Camera csv",
				"Printing",
				"Job in error",
				"Waiting for start",
				"Waiting for data",
				"Network settings",
				"Language settings",
				"Print parameters",
				"Hotfolder settings",
				"IO vision",
				"Back to control page",
				"Back",

				"Ready",
				"Can't save feedback csv!",
				"Camera csv analyzing error!",
				"Can't load control csv!",
				"Can't send pdf file to GIS!",
				"Can't send csv file to camera!",
				"Job index order mismatch!",
				"Stacker jam pile!",
				"Stacker jam conveyer!",
				"Stakcer bottom!",
				"Stacker multifunction!",
				"Stacker in off state!",
				"Feeder E-stop!",
				"Feeder in off state!",
				"Feeder multifunction!",
				"Feeder sheet missing!",
				"Feeder double sheet!",
				"Machine E-stop!",
				"Reject bin jam!",
				"Feeder jam!",
				"Tab insert error!",
				"Companion sheet feeder error!",
				"Counters mismatch!",
				"Low print quality! Check print heads.",
				"GIS computer disconnected!",
				"Unknown error!",

				"Interface language:",
				"TCP port nuber for GIS:",
				"IP address for GIS:",
				"Network connection control:",
				"Wrong TCP port format!",
				"Wrong IP address format",

				"Browse",
				"Cancel",
				"Select",
				"Data hotfolder",
				"Feedback hotfolder",
				"Backup hotfolder",
				"Camera input hotfolder",
				"Camera output hotfolder",
				"GIS hotfolder",
				"Report directory",
				"Data hotfolder Quadient:",
				"Feedback hotfolder Quadient:",
				"Backup hotfolder Quadient:",
				"Camera input hotfolder:",
				"Camera output hotfolder:",
				"GIS hotfolder:",
				"Report directory:",
				"Report csv"}};							








#endif
