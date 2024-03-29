
#include "interface.h"
#include "controler_interface.h"



#define CORE_TEST_MODE true

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


/** @ingroup configuration Interval for content reading of quadient hot folder */
#define HOT_FOLDER_READING_INTERVAL 500 // 300ms

#define MACHINE_CYCLE_TIMING 1000 //1ms

#define MACHINE_XBF_INTERVAL 150 //ms


#define Q_FEEDBACK_ADDR "./feedback/" //"/media/petr/DATA/q_hotfolder/data_in_feedback/" //

#define Q_HOT_FOLDER_ADDR "./q_hot/" //"/media/petr/DATA/q_hotfolder/data_out/" //"

#define Q_JOB_BACKUP "./q_hot/backup/" //"/media/petr/DATA/q_hotfolder/data_out/backup/" //

#define PCI_HOT_FOLDER_ADDR_IN "./c_hot_in/"

#define PCI_HOT_FOLDER_ADDR_OUT "./c_hot_out/"

#define GIS_HOT_FOLDER "./gis_hot/"

#define MERGE_DIRECTORY_ADDRESS "/media/stc/q_hot/merge/"
#define MERGED_PDF_FILE "stc-0000_0001_p_pdf.pdf"
#define MERGED_CSV_FILE "stc-0000_0001_p_camera.csv"


#ifdef PLATFORM_STC_PC
	#define LOG_PATH "/home/stc/host-bk/log/"
#else
	#define LOG_PATH "./log/"
#endif

#define JOB_INFO_CSV_PATH "./job_info_csv/"

#define LOG_FILE_NAME "host_log"

#define TIME_DELAY_ON_JOB_END 3000 // ms


#define PCI_COMM FALSE

#define CFG_GROUP_NETWORK "network"
#define CFG_GROUP_LANGUAGE "language"
#define CFG_GROUP_PRINT_PARAMS "print_params"
#define CFG_GROUP_HOTFOLDER "hotfolder"
#define CFG_GROUP_STATISTICS "statistics"
#define CFG_GROUP_STATISTICS_MON "statistics_monday"
#define CFG_GROUP_STATISTICS_TUE "statistics_tuesday"
#define CFG_GROUP_STATISTICS_WED "statistics_wednesday"
#define CFG_GROUP_STATISTICS_THU "statistics_thursday"
#define CFG_GROUP_STATISTICS_FRI "statistics_friday"
#define CFG_GROUP_STATISTICS_SAT "statistics_saturday"
#define CFG_GROUP_STATISTICS_SUN "statistics_sunday"


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
#define CFG_PP_FEED_DELAY "feed_delay"
#define CFG_PP_FAN_INTENSITY "fan_intensity"
#define CFG_PP_TAB_INSERT_LENGTH "tab_insert_length"
#define CFG_PP_TAB_INSERT_CUT_DELAY "tab_cut_delay"
#define CFG_PP_TAB_INSERT_SEQUENCE "tab_insert_sequence"
#define CFG_PP_TAB_INSERT_AUTOMAT "tab_insert_automat"


#define CFG_LANG_INDEX "lang_index"

#define CFG_NETWORK_GIS_TCP_PORT "gis_tcp_port"
#define CFG_NETWORK_GIS_IP_ADDRESS "gis_ip_address"
#define CFG_NETWORK_QUADIENT_PC_TCP_PORT "quadient_tcp_port"
#define CFG_NETWORK_QUADIENT_PC_IP_ADDRESS "quadient_ip_address"
#define CFG_NETWORK_PCI_PC_TCP_PORT "pci_pc_tcp_port"
#define CFG_NETWORK_PCI_PC_IP_ADDRESS "pci_pc_ip_address"
#define CFG_NETWORK_IIJ_PC_TCP_PORT "iij_pc_tcp_port"
#define CFG_NETWORK_IIJ_PC_IP_ADDRESS "iij_pc_ip_address"

#define CFG_STATISTICS_TOTAL_FEEDED_SHEETS "total_feeded_sheets"
#define CFG_STATISTICS_TOTAL_STACKED_SHEETS "total_stacked_sheets"
#define CFG_STATISTICS_TOTAL_REJECTED_SHEETS "total_rejected_sheets"
#define CFG_STATISTICS_ERROR_RATE "error_rate"
#define CFG_STATISTICS_DAY_PRE "previous_day"


struct _comm_monitor_
{
	bool run_monitor;
	com_tcp * comm;
	bool comm_status;
	pthread_t thread;
};

typedef struct _comm_monitor_ comm_monitor;



/**************************************************** global variables **********************************************************/

config_t cfg_ref;

c_log * log_ref;

pthread_t machine_handler_thread;
pthread_t iij_gis_state_reading_thread;

comm_monitor * iij_comm_monitor;
comm_monitor * pci_comm_monitor;
comm_monitor * quadient_comm_monitor;


com_tcp * iij_tcp_ref;
com_tcp * pci_tcp_ref;
bool iij_tcp_connection_req;
c_timer * iij_connection_try_timer;

/* connection testing, communication with network_responder utility */
//com_tcp * pci_connection;
//com_tcp * iij_connection;
//com_tcp * quadient_connection;

pthread_t pci_conn_therad;
pthread_t iij_conn_thread;
pthread_t quadient_conn_thread;

io_card * io_card_ref;

job_info * info;
machine_statistic * statistics_ref;

c_freq * ta_freq;
c_freq * ti_freq;

c_timer * hot_reading_interval;

c_timer * switch_off_ena_in_feeder_error;

/* machine handler internal state */
int machine_state;
int machine_state_trig;
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
bool machine_jam_req;
bool machine_print_one_req;

uint32_t rejected_sheet_for_stop;
uint32_t max_stacked_sheets;

uint8_t companion_sheet_source;
bool sheet_source_confirmation;
bool print_confirmation_req;

/* printing counters */
uint32_t main_sheet_feed_request_counter;
uint32_t companion_sheet_feed_request_counter;
uint32_t stacked_sheet_counter;
uint32_t stacked_sheet_counter_in_job;
uint32_t rejected_sheet_counter_in_job;
uint32_t rejected_sheet_counter;
uint32_t rejected_sheet_seq_counter;
uint32_t tab_insert_counter;
uint32_t camera_sensor_trigger_counter;
uint32_t feeded_sheet_counter;
uint32_t feeded_sheet_counter_in_job;
uint32_t feeded_sheet_counter_pre;
uint32_t stacked_sheet;
uint32_t rejected_sheet;
uint32_t feeded_sheet;
uint64_t complet_feeded;
uint64_t complet_rejected;
uint32_t stacked_for_ti;
uint8_t stacked_sensor_for_ti_pre;

int32_t non_stacked_upper_limit;
int32_t non_revided_upper_limit;

uint8_t machine_mode;
uint8_t machine_mode_pre;
uint64_t machine_mode_timer_for_adaptation;

c_string * print_controler_status;

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


uint64_t manual_feed_xbf;
bool manual_feed;
bool ena_force;

bool io_manual;
bool machine_handler_run;

c_timer * print_timer;

uint8_t feeder_status;
uint8_t stacker_status;

uint8_t clear_on_startup;

uint64_t delay_on_end;
uint8_t sheet_sensor_pre;

uint8_t print_finishing_step;
c_timer * feed_sensor_timer;

c_timer * network_connection_timer;
uint64_t ena_timer;
uint64_t ena_start_timer;
uint64_t file_delay;

c_timer * feeding_rate;
uint64_t feed_delay;

uint32_t wait_for_files_ready;
c_timer * f_s_delay;
uint64_t time_for_nth_sheet[4];
uint64_t last_feeded_sheet_millis;
int nth_sheet_time_index;

bool fake_companion_req;
bool companion_faked;

uint32_t merged_companion_sheets;

soft_pwm * fan_regulation;
int fan_intensity;
int fan_intensity_set;
uint8_t fan_activity;


uint8_t tab_insert_step;
uint64_t timer_tab_insert;
uint8_t tab_insert_req;
int tab_insert_len;
bool tab_insert_automat;
int32_t tab_insert_sequence;
bool tab_insert_busy;
uint32_t last_ti_req;
int32_t ti_delay;
int64_t ti_fail_delay_check;

int stacked_sheet_offset;

int64_t companion_generation_delay;

/**************************************************** functions declarations *****************************************************/

comm_monitor * comm_monitor_new(bool);
bool comm_monitor_status(comm_monitor *);
uint8_t comm_monitor_set_ip_address(comm_monitor *, char *);
uint8_t comm_monitor_set_tcp_port(comm_monitor *, int );
int comm_monitor_get_tcp_port(comm_monitor *);
char * comm_monitor_get_ip_address(comm_monitor *);
void comm_monitor_finalize(comm_monitor *);
void * comm_monitor_testing_thread(void *);

uint8_t controler_iij_try_connect();
uint8_t controler_pci_try_connect();



void controler_default_config();
uint8_t controler_load_config();
void controler_update_config(char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail);
void controler_initialize_variables();

void controler_safety_system_in();
void controler_safety_system_out();
int8_t controler_printer_abbort_print(uint8_t step);
void controler_set_machine_error(uint8_t code);
void controler_set_machine_state(int new_machine_state);

c_csv_struct * controler_analyze_csv(c_csv_struct * q_csv, c_csv_struct * c_csv);
c_string * controler_compare_csv(q_job * job, int * csv_lines);


void  controler_clear_hotfolder();
uint8_t controler_check_job_consistency(char * name,  uint8_t mod);
uint8_t controler_load_bkcore_csv();

void * controler_machine_handler(void * param);
void controler_machine_state_read_hotfolder();
void controler_machine_state_print_main();
void controler_machine_state_error();
void controler_machine_state_print_companion();
void controler_machine_state_wait_for_confirmation();
void controler_machine_state_clear_belt_after_jam();
void controler_machine_state_stacker_error();
void controler_machine_state_feeder_error();
void controler_machine_state_read_csv_line();
void controler_machine_state_wait_for_ena_ready();
void controler_machine_state_pause();
void controler_machine_state_clear_hotfolder();
void controler_machine_state_prepare();
void controler_machine_state_ready_to_start();
void controler_machine_state_save_q_csv();
void controler_machine_state_wait_for_print_finish();
void controler_machine_state_finish();
void controler_machine_state_print_break();
void controler_machine_state_fake_companion();
void controler_machine_state_copy_companion_pdf_file();
void controler_machine_state_delete_stamp_pdf_file();
void controler_machine_state_merge_companion_pdf_files();
void controler_machine_state_send_response_to_ia();
void controler_machine_state_finish_print_companion_pdf();
void controler_machine_state_send_companion_data();
void coontroler_machine_state_companion_generation_delay();


void contorler_sheet_counter();
void controler_print_break_req();
void controler_feed_sheet();
void controler_stacker_error_handler(uint8_t stacker_status);
void controler_feeder_error_handler(uint8_t feeder_status);
void controler_machine_mode_control();
void machine_mode_runtime_switching(int cur_machine_mode, int machine_mode);
void controler_machine_mode_adaptation_protection_time_out(int gremser_machine_mode, int local_machine_mode);


void controler_clear_merge_directory();

q_job * controler_get_printed_job();

void controler_machine_stacker_counter(uint8_t stacker_counter);
void controler_machine_tab_insert_counter();
void controler_counter_check_sum();
void controler_machine_reject_counter();
void controler_machine_camera_counter();
void controler_machine_log_monitor();
int8_t controler_machine_slow_down();
void controler_feed_control();
void controler_record_feeding_time();

void controler_save_statistics(int8_t day);

void controler_fun_control();
void controler_tab_insert_control();

void * controler_gis_runtime_state_reading(void * param);
int controler_gis_load_status_id(char ** msg);
void controler_parse_gis_status(char * status_str);




/******************************************** interface functions definitions *******************************************************/

int8_t controler_init()
{
	uint64_t s_time = c_freq_millis();

	log_ref = c_log_new(LOG_PATH, LOG_FILE_NAME, "Inicializace jádra...");

	if(log_ref != NULL)
	{
		c_log_set_log_direction(log_ref, 0);

		pthread_mutex_init(&(mutex), NULL);

		io_card_ref = io_card_new();

		iij_tcp_ref = com_tcp_new();
		//pci_tcp_ref = com_tcp_new();
		
		/* Pozor!!! Průběžná kontrola připojení sítě je vypnutá. */
		iij_comm_monitor = comm_monitor_new(false);
		pci_comm_monitor = comm_monitor_new(false);
		quadient_comm_monitor = comm_monitor_new(false);

		ti_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);
		ta_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);
		hot_reading_interval = c_timer_new();
		switch_off_ena_in_feeder_error = c_timer_new();

		info = job_info_new(JOB_INFO_CSV_PATH);

		statistics_ref = machine_statistic_new();
		printf("statistika vytvorena\n");

		print_timer = c_timer_new();

		f_s_delay = c_timer_new();

		controler_initialize_variables();

		config_init(&(cfg_ref));

		if(config_read_file(&(cfg_ref), CONFIGURATION_FILE_PATH) != CONFIG_TRUE)
		{
			c_log_add_record_with_cmd(log_ref, "Konfigurace systému nebyla úspěšně načtena: %s", config_error_text(&(cfg_ref)));
			c_log_add_record_with_cmd(log_ref, "Vytvářím výchozí konfiguraci.");
			controler_default_config();
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Načítám konfigurační soubor: %s", CONFIGURATION_FILE_PATH);
			int conf_ret_val = controler_load_config();

			if(conf_ret_val == 0)
				c_log_add_record_with_cmd(log_ref, "Konfigurace načtena");
			else
				c_log_add_record_with_cmd(log_ref, "Konfigurace nebyla úspěšně načtena: %d", conf_ret_val);
		}

		/* run machine_handler thread */
		machine_handler_run = true;
		
		
		if(pthread_create(&(pci_comm_monitor->thread), 0, comm_monitor_testing_thread, pci_comm_monitor) == 0)
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači PCI vytvořeno.");
		else
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači PCI se nepodařilo vytvořit!");

		if(pthread_create(&(iij_comm_monitor->thread), 0, comm_monitor_testing_thread, iij_comm_monitor) == 0)
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači IIJ vytvořeno.");
		else
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači IIJ se nepodařilo vytvořit!");

		if(pthread_create(&(quadient_comm_monitor->thread), 0, comm_monitor_testing_thread, quadient_comm_monitor) == 0)
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači Qudient vytvořeno.");
		else
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači Quadient se nepodařilo vytvořit!");



		/* initialize and parametrizing the network layer */
#if PCI_COMM == true
		/* establish tcp connection to subsystems */
		if (controler_pci_try_connect() != STATUS_CLIENT_CONNECTED)
		{
			fputs("Camera connection error\n", stderr);
		}
#endif

		if(controler_iij_try_connect() != STATUS_CLIENT_CONNECTED)
		{
			c_log_add_record_with_cmd(log_ref, "Chyba připojení k systému GIS!");
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Systém GIS úspěšně připojen.");
		}

		/* initialize and parametrizing machine handler thread */
		if(io_card_ref != NULL)
		{
			c_log_add_record_with_cmd(log_ref, "IO karty úspěšně připojeny.");

			if (pthread_create(&(machine_handler_thread), 0, &controler_machine_handler, NULL) == 0)
			{
				c_log_add_record_with_cmd(log_ref, "Vlákno obsluhy stroje vytvořeno.");
			}
			else
			{
				c_log_add_record_with_cmd(log_ref, "Nelze spustit vlákno obsluhy stroje!");
			}

			uint64_t e_time = c_freq_millis();
			c_log_add_record_with_cmd(log_ref, "Jádro inicializováno. Doba trvání inicializace: %ldms", e_time-s_time);
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Nelze se připojit k IO kartám!");
			controler_finalize();
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "%s", "Nelze vytvořit log, inicializace jádra neúspěšná!!\n");
		return 2;
	}

	return 0;
}


void controler_finalize()
{
	machine_handler_run = false;

	while(machine_handler_run == false){usleep(1000);}

	controler_save_statistics(machine_statistic_get_day());


	com_tcp_finalize(iij_tcp_ref);
	//com_tcp_finalize(pci_tcp_ref);


	if(job_list != NULL)
		array_list_destructor_with_release_v2(job_list, q_job_finalize);

	if(job_list_pre != NULL)
		array_list_destructor_with_release_v2(job_list_pre, q_job_finalize);

	c_string_finalize(&(q_hotfolder_main_path));
	c_string_finalize(&(q_hotfolder_feedback_path));
	c_string_finalize(&(q_hotfolder_backup_path));
	c_string_finalize(&(pci_hotfolder_in_path));
	c_string_finalize(&(pci_hotfolder_out_path));
	c_string_finalize(&(gis_hotfolder_path));
	c_string_finalize(&(job_log_path));

	c_string_finalize(&(printed_job_name));

	c_string_finalize(&(print_controler_status));

	c_freq_finalize(ti_freq);
	c_freq_finalize(ta_freq);

	job_info_finalize(info);

	io_card_finalize(io_card_ref);

	config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
	sleep(1);
	config_destroy(&(cfg_ref));


	c_log_add_record_with_cmd(log_ref, "Jádro úspěšně ukončeno.");
	c_log_finalize(log_ref);
}



int controler_get_interface_language()
{
	return lang_index;
}


void controler_set_interface_language(int lang)
{
	lang_index = lang;
}

bool controler_iij_network_connected()
{
	return comm_monitor_status(iij_comm_monitor);// 	com_tcp_is_connected(iij_connection);
}

bool controler_pci_network_connected()
{
	return 	comm_monitor_status(pci_comm_monitor);	//com_tcp_is_connected(pci_connection);
}

bool controler_quadient_network_connected()
{
	return 	comm_monitor_status(quadient_comm_monitor);	//com_tcp_is_connected(quadient_connection);
}

void controler_sheet_source_confirmation()
{
	print_confirmation_req = true;
}

uint8_t controler_machine_status_val()
{
	return machine_state;
}


uint64_t controler_date_to_int(char * file_name)
{
	uint64_t int_time = 0;

	char * csv_content = util_load_csv(c_string_get_char_array(job_log_path), file_name, NULL);

	uint32_t year = 0;
	uint32_t month = 0;
	uint32_t day = 0;
	uint32_t hour = 0;
	uint32_t minute = 0;
	uint32_t second = 0;
	int i = 0;
	uint8_t row = 0;
	uint8_t item = 0;

	if(csv_content != NULL)
	{
		while((csv_content[i] != 0) && (row < 10) && ((row < 9) || (item < 6) || (isdigit(csv_content[i]) != 0)))
		{
			if(csv_content[i] == '\n')
				row ++;

			if(((csv_content[i] == '/') || (csv_content[i] == '-') || ((item > 2) && (csv_content[i] == ':'))) && (row == 9))
				item ++;

			if((row == 9) && (isdigit(csv_content[i])))
			{
				switch(item)
				{
					case 0:
						day = day*10+(csv_content[i]-48);
					break;

					case 1:
						month = month*10+(csv_content[i]-48);
					break;

					case 2:
						year = year*10+(csv_content[i]-48);
					break;

					case 3:
						hour = hour*10+(csv_content[i]-48);
					break;

					case 4:
						minute = minute*10+(csv_content[i]-48);
					break;

					case 5:
						second = second*10+(csv_content[i]-48);
					break;
				}
			}
			i++;
		}

		int_time += second;
		int_time += minute*60;
		int_time += hour*3600;
		int_time += day*24*3600;
		int_time += ((uint32_t)((double)month)*30.416)*24*3600;
		int_time += year*365*24*3600;

		free(csv_content);

	}



	return int_time;
}

array_list * controler_get_report_csv_list()
{
	DIR * dir_ref = opendir(controler_get_job_report_hotfolder_path());

	array_list * temp_csv_list = NULL;

	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;
		temp_csv_list = array_list_new();
		array_list * date_csv_list = array_list_new();

		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				if(util_str_ends_with(dir_cont->d_name, ".csv", 0) == 0)
				{
					c_string * csv_file_name = c_string_new_with_init(dir_cont->d_name);

					uint64_t * int_date = malloc(sizeof(uint64_t));
					*int_date = controler_date_to_int(dir_cont->d_name);

					if(array_list_size(temp_csv_list) > 0)
					{
						for(int i = 0; i< array_list_size(temp_csv_list); i++)
						{
							uint64_t * pre_int_date = array_list_get(date_csv_list, i);

							if(*pre_int_date < *int_date)
							{
								array_list_add_to_index(date_csv_list, i, int_date);
								array_list_add_to_index(temp_csv_list, i, csv_file_name);
								break;
							}
						}
					}
					else
					{
						array_list_add(date_csv_list, int_date);
						array_list_add(temp_csv_list, csv_file_name);
					}
				}
				else
				{
					printf("removed file: %s\n", dir_cont->d_name);
					util_delete_file(c_string_get_char_array(job_log_path), dir_cont->d_name);


					c_log_add_record_with_cmd(log_ref, "Mazání neznámého souboru z reportovacího adresáře: %s",
								 c_string_get_char_array(job_log_path), dir_cont->d_name);
				}
			}
		}

		array_list_destructor_with_release_v2(date_csv_list, free);
		closedir(dir_ref);
	}

	return temp_csv_list;
}
//86
//35 65 95 120 150


uint8_t controler_print_start(const char * job_name)
{
	uint8_t status = STATUS_SUCCESS;

	if(c_string_len(printed_job_name) == 0)
	{
		if((job_name != NULL) && (strlen(job_name) > 0))
		{
			c_string_set_string(printed_job_name, (char*) job_name);
		}
		else if((job_list != NULL) && (array_list_size(job_list) > 0))
		{
			q_job * job = array_list_get(job_list, 0);

			if(job != NULL)
			{
				c_string_set_string(printed_job_name, q_job_get_job_name(job));
			}
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěšný! Nebylo specifikováno jméno jobu.");
			return STATUS_GENERAL_ERROR;
		}

		int ttl = 0;
		while(((machine_state == MACHINE_STATE_WAIT)) && (ttl < 2000))
		{
			machine_print_req = true;
			usleep(MACHINE_CYCLE_TIMING);
			ttl++;
		}

		machine_print_req = false;

		if(ttl >= 2000)
		{
			status = STATUS_GENERAL_ERROR;
			c_string_clear(printed_job_name);
			c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěný!");
		}
		else
		{
			if(machine_state == MACHINE_STATE_ERROR)
			{
				status = STATUS_GENERAL_ERROR;
				c_string_clear(printed_job_name);
				c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku zkončil chybou: %d - %s", error_code, (char*) controler_get_error_str());
			}
			else
			{
				c_log_add_record_with_cmd(log_ref, "Spuštěn tisk jobu %s.", c_string_get_char_array(printed_job_name));
			}
		}
		status = STATUS_GENERAL_ERROR;
	}
	else
	{
		status = STATUS_GENERAL_ERROR;
		c_log_add_record_with_cmd(log_ref,
			"Pokus o spuštění tisku neúspěšný! Tisk jiného jobu již běží (%s).",
			c_string_get_char_array(printed_job_name));
	}

	return status;
}


uint8_t controler_print_all_companion(const char * job_name)
{
	if(c_string_len(printed_job_name) == 0)
	{
		if((job_list != NULL) && (array_list_size(job_list) > 0))
		{
			if((job_name != NULL) && (strlen(job_name) > 0))
			{
				c_string_set_string(printed_job_name, (char*) job_name);
				controler_set_machine_state(MACHINE_STATE_LOAD_NEXT_JOB_FILE);
				return STATUS_SUCCESS;
			}
			else if((job_list != NULL) && (array_list_size(job_list) > 0))
			{
				q_job * job = array_list_get(job_list, 0);

				if(job != NULL)
				{
					c_string_set_string(printed_job_name, q_job_get_job_name(job));
					controler_set_machine_state(MACHINE_STATE_LOAD_NEXT_JOB_FILE);

					return STATUS_SUCCESS;
				}
			}
			else
			{
				c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěšný! Nebylo specifikováno jméno jobu.");
				return STATUS_GENERAL_ERROR;
			}
		}
	}

	return STATUS_GENERAL_ERROR;
}

uint8_t controler_print_pause()
{
	uint8_t res = STATUS_SUCCESS;

	if((machine_state != MACHINE_STATE_ERROR) && (machine_state != MACHINE_STATE_WAIT))
	{
		machine_pause_req = true;
		c_log_add_record_with_cmd(log_ref, "Tisk jobu %s pozastaven.", c_string_get_char_array(printed_job_name));
	}
	else
	{
		res = STATUS_GENERAL_ERROR;
		if(machine_state == MACHINE_STATE_ERROR)
		{
			c_log_add_record_with_cmd(log_ref, "Pokus o pozastavení jobu %s neúspěšný! Obslužné vlákno se nachází v chybovém stavu: %d - %s",
					 c_string_get_char_array(printed_job_name), machine_state, (char*) controler_get_error_str());

		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Pokus o pozastavení jobu %s neúspěšný! Obslužné vlákno se nachází ve stavu: %d - %s",
					 c_string_get_char_array(printed_job_name), machine_state, controler_machine_get_state_str());

		}
	}

	return res;
}

uint8_t controler_print_continue()
{
	int8_t res = STATUS_SUCCESS;

	if(machine_state == MACHINE_STATE_PAUSE)
	{
		int ttl = 0;
		machine_pause_req = false;
		rejected_sheet_seq_counter = 0;

		while((ttl < 3) && (machine_state == MACHINE_STATE_PAUSE))
		{
			usleep(MACHINE_CYCLE_TIMING);
			ttl++;
		}

		if(machine_state == MACHINE_STATE_PAUSE)
		{
			c_log_add_record_with_cmd(log_ref, "Nelze obnovit job: %s", c_string_get_char_array(printed_job_name));
			res = STATUS_GENERAL_ERROR;
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Obnovení jobu: %s", c_string_get_char_array(printed_job_name));
		}
	}
	else
	{
		res = STATUS_GENERAL_ERROR;
		if(machine_state == MACHINE_STATE_ERROR)
		{
			c_log_add_record_with_cmd(log_ref,
				"Obnovení jobu neúspěšné. Obslužné vlákno se nachízí v chybovém stavu: %d - %s",
				machine_state, controler_get_error_str());
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Obnovení jobu neúspěšné. Obslužné vlákno se nachízí ve stavu: %d - %s",
						machine_state, controler_machine_get_state_str());
		}
	}

	return res;
}

uint8_t controler_print_cancel()
{
	uint8_t res = STATUS_SUCCESS;

	if(c_string_len(printed_job_name) > 0)
	{
		c_log_add_record_with_cmd(log_ref, "Požadavek na ukončení jobu %s.", c_string_get_char_array(printed_job_name));
		machine_cancel_req = true;
	}
	else
	{
		res = STATUS_GENERAL_ERROR;
		c_log_add_record_with_cmd(log_ref, "Pokus o přerušení jobu neúspšný! Neprobíhá tisk žádného jobu.");
	}

	return res;
}


uint8_t controler_print_reset_error()
{
	uint8_t res = STATUS_SUCCESS;
	int ttl = 0;

	machine_cancel_req = false;
	machine_print_req = false;
	machine_pause_req = false;
	machine_jam_req = false;
	machine_print_one_req = false;

	while((machine_state == MACHINE_STATE_ERROR) && (ttl < 2))
	{
		machine_error_reset_req = true;
		usleep(MACHINE_CYCLE_TIMING);
		ttl++;
	}

	machine_error_reset_req = false;

	if(machine_state == MACHINE_STATE_ERROR)
	{
		c_log_add_record_with_cmd(log_ref,
				"Pokus o reset chybového stavu neúspěšný. Obslužné vlákno se nachází v chybě: %d - %s",
				error_code, controler_get_error_str());
		res = STATUS_GENERAL_ERROR;
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Reset chybového stavu.");
	}

	return res;
}

void controler_manual_feed_sheet()
{
	manual_feed = true;
}

/*
** once per 50ms check the state of the feeding sensor
*/
void contorler_sheet_counter()
{
	if(c_timer_delay(feed_sensor_timer, 50) > 0)
	{
		uint8_t sheet_sensor_state = io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_2_FEEDING_SENSOR);

		if(machine_state != MACHINE_STATE_WAIT)
		{
			if((sheet_sensor_state != sheet_sensor_pre) && (sheet_sensor_state > 0))
			{
				feeded_sheet_counter++;

				feeded_sheet++;
				feeded_sheet_counter_in_job++;
				machine_statistic_increment_feeded_sheets(statistics_ref);

				if(machine_state == MACHINE_STATE_PRINT_COMPANION)
				{
					companion_sheet_feed_request_counter++;
				}
				else
				{
					complet_feeded++;
				}

				printf("%d\n", feeded_sheet_counter);
			}
		}

		sheet_sensor_pre = sheet_sensor_state;
	}
}


void controler_print_one_sheet_req()
{
	machine_pause_req = false;
	machine_print_one_req = true;
	rejected_sheet_seq_counter = 0;
}


void controler_finish_job_and_print_companion()
{
	fake_companion_req = true;
	machine_pause_req = false;
}


bool controler_get_print_one_req_val()
{
	return machine_print_one_req;
}

bool controler_get_fake_companion_req_val()
{
	return fake_companion_req;
}

void controler_feed_sheet()
{
	if(manual_feed == true)
	{
		if(manual_feed_xbf == 0)
		{
			manual_feed_xbf = c_freq_millis();
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 1);
		}

		if((manual_feed_xbf+100) <= c_freq_millis())
		{
			manual_feed_xbf = 0;
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			manual_feed = false;
		}
	}
}

void controler_manual_set_ena_state(bool state)
{
	ena_force = state;
	if(state == true)
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 1);
	else
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 0);
}



void controler_set_feed_delay(uint32_t delay)
{
	feed_delay = delay;

	c_log_add_record_with_cmd(log_ref, "Nastavena prodleva pro zpomalení nakládání kolkových archů: %d", delay);

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_FEED_DELAY,
			CONFIG_TYPE_INT,
			&delay,
			"Nastavení prodlevy pro zpomalení nakládání kolkových archů aktualizováno.",
			"Nepodařilo se aktualizovat prodlevu pro zpomalení nakládání kolkových archů!");
}

uint32_t controler_get_get_feed_delay()
{
	return feed_delay;
}

void controler_set_max_rejected_sheet_seq(int sheet_val)
{
	if(sheet_val > 0)
	{
		rejected_sheet_for_stop = sheet_val;
		c_log_add_record_with_cmd(log_ref, "Nastaven počet vadně vyhodnocených archů pro zastavení tisku: %d", sheet_val);

		controler_update_config(CFG_GROUP_PRINT_PARAMS,
				CFG_PP_SHEET_FOR_STOP,
				CONFIG_TYPE_INT,
				&sheet_val,
				"Nastavení maximální sekvence vadně vyhodnocených archů aktualizováno.",
				"Nepodařilo se aktualizovat nastavení maximální sekvence vadně vyhodnocených archů!");
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Počet vadně vyhodnocených archů pro zastavení tisku, nebyl nastaven, hodnota mimo povolený rozsah: %d", sheet_val);
	}
}

int controler_get_max_rejected_sheet_seq()
{
	return rejected_sheet_for_stop;
}

void controler_set_companion_sheet_source(int source)
{
	if(source >= 0 && source < SSOURCE_N)
	{
		companion_sheet_source = source;

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

		c_log_add_record_with_cmd(log_ref, "Nastaven zdroj pro prokladový arch: %s", sheet_src_str);

		controler_update_config(CFG_GROUP_PRINT_PARAMS,
				CFG_PP_COMPAION_SHEET_SOURCE,
				CONFIG_TYPE_INT,
				&source,
				"Nastavení zdroje pro prokladový arch aktualizováno.",
				"Nepodařilo se aktualizovat nastavení zdroje pro prokladový arch!");
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Neúspěšný pokus o nastavení nového zdroje pro prokladové archy: %d", source);
	}
}


void controler_set_max_stacked_sheets(int sheet_val)
{
	if(sheet_val > 0)
	{
		max_stacked_sheets = sheet_val;
		c_log_add_record_with_cmd(log_ref, "Nastaven maximální počet archů ve vykladači: %d", sheet_val);

		controler_update_config(CFG_GROUP_PRINT_PARAMS,
				CFG_PP_MAX_SHEET_IN_STACKER,
				CONFIG_TYPE_INT,
				&sheet_val,
				"Nastavení maximálního počtu archů ve vykladači aktualizováno.",
				"Nepodařilo se aktualizovat nastavení maximálního počtu archů ve vykladači!");
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Nastavení maximálního počtu archů ve vykladači neúspěšné, hodnota je mimo povolený rozsah: %d", sheet_val);
	}
}


void controler_set_manual_mode(bool state)
{
	c_log_add_record_with_cmd(log_ref, "Nastaven řežim řízení IO na %s", state == true ? "true":"fasle");
	io_manual = state;
}

bool controler_get_manual_mode_state()
{
	return io_manual;
}


char ** controler_get_a1_input_labels()
{
	return (char**) io_card_get_a1_in_labels();
}

char ** controler_get_a2_input_labels()
{
	return (char **) io_card_get_a2_in_labels();
}

char ** controler_get_a1_output_labels()
{
	return (char **) io_card_get_a1_out_labels();
}

char ** controler_get_a2_output_labels()
{
	return (char**) io_card_get_a2_out_labels();
}

void controler_set_a1_output(uint8_t index, uint8_t state)
{
	io_card_set_output(io_card_ref, IO_CARD_A1, index, state);
}

void controler_set_a2_output(uint8_t index, uint8_t state)
{
	io_card_set_output(io_card_ref, IO_CARD_A2, index, state);
}

uint8_t controler_get_card_input(int card, int addr)
{
	return io_card_get_input(io_card_ref, card, addr);
}


uint8_t controler_get_card_output(int card, int addr)
{
	return io_card_get_output(io_card_ref, card, addr);
}

int controler_get_stacked()
{
	return stacked_sheet;
}

int controler_get_max_stacked_sheet()
{
	return max_stacked_sheets;
}

int controler_get_sheet_source()
{
	return companion_sheet_source;
}

void controler_set_sheet_source_confirmation(bool confirm)
{
	sheet_source_confirmation = confirm;
	c_log_add_record_with_cmd(log_ref, "Potvrzování naložení prokladového archu z hlavního nakladače: %s", confirm == true ? "Aktivováno" : "Deaktivováno");

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_SHEET_SOURCE_CONFIRMATION,
			CONFIG_TYPE_BOOL,
			&confirm,
			"Nastavení potvrzování naložení prokladového archu z hlavního nakladače aktualizováno.",
			"Nepodařilo se aktualizovat nastavení potvrzování naložení prokladového archu z hlavního nakladače!");
}

bool controler_get_sheet_source_confirmation()
{
	return sheet_source_confirmation;
}



uint8_t controler_set_q_main_hotfolder_path(const char * path)
{
	c_string_set_string(q_hotfolder_main_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k výstupnímu hotfolderu pro Quadient: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_QUADIENT_MAIN,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy výstupního hotfolderu pro Quadient aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro Quadient!");

	return STATUS_SUCCESS;
}

uint8_t controler_set_q_feedback_hotfolder_path(const char * path)
{
	c_string_set_string(q_hotfolder_feedback_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k hotfolderu pro ukládání zpětnovazebných csv pro Quadient: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_QUADIENT_FEEDBACK,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy zpětnovazebního hotfolderu pro Quadient aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy zpětnovazebního hotfolderu pro Quadient!");


	return STATUS_SUCCESS;
}

uint8_t controler_set_q_backup_hotfolder_path(const char * path)
{
	c_string_set_string(q_hotfolder_backup_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k hotfolderu pro zálohování tiskových dat z Quadientu: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_QUADIENT_BACKUP,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy hotfolderu pro zálohování dat z Quadient aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy hotfolderu zálohování pro Quadient!");

	return STATUS_SUCCESS;
}

uint8_t controler_set_pci_hotfolder_in_path(const char * path)
{
	c_string_set_string(pci_hotfolder_in_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty ke vstupnímu hotfolderu kamery: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_PCI_IN,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy vstupního hotfolderu pro kameru aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro kameru!");

	return STATUS_SUCCESS;
}

uint8_t controler_set_pci_hotfolder_out_path(const char * path)
{
	c_string_set_string(pci_hotfolder_out_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k výstupnímu hotfolderu kamery: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_PCI_OUT,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy výstupního hotfolderu pro kameru aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy výstupního hotfolderu pro kameru!");

	return STATUS_SUCCESS;
}

uint8_t controler_set_gis_hotfolder_path(const char * path)
{
	c_string_set_string(gis_hotfolder_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k hotfolderu gisu: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_GIS,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy vstupního hotfolderu pro GIS aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy vstupního hotfolderu pro GIS!");

	return STATUS_SUCCESS;
}

uint8_t controler_set_job_report_hotfolder_path(const char * path)
{
	c_string_set_string(job_log_path, (char*) path);
	c_log_add_record_with_cmd(log_ref, "Nastavení nové cesty k reportovacímu hotfolderu: %s", path);

	controler_update_config(CFG_GROUP_HOTFOLDER,
			CFG_HOT_REP_CSV,
			CONFIG_TYPE_STRING,
			(void*) path,
			"Nastavení adresy hotfolderu pro reportování csv z tisku aktualizováno.",
			"Nepodařilo se aktualizovat nastavení adresy hotfolderu pro reportování csv z tisku!");

	return STATUS_SUCCESS;
}


char * controler_get_q_main_hotfolder_path()
{
	return c_string_get_char_array(q_hotfolder_main_path);
}

char * controler_get_q_feedback_hotfolder_path()
{
	return c_string_get_char_array(q_hotfolder_feedback_path);
}

char* controler_get_q_backup_hotfolder_path()
{
	return c_string_get_char_array(q_hotfolder_backup_path);
}

char * controler_get_pci_hotfolder_in_path()
{
	return c_string_get_char_array(pci_hotfolder_in_path);
}

char * controler_get_pci_hotfolder_out_path()
{
	return c_string_get_char_array(pci_hotfolder_out_path);
}

char * controler_get_gis_hotfolder_path()
{
	return c_string_get_char_array(gis_hotfolder_path);
}

char * controler_get_job_report_hotfolder_path()
{
	return c_string_get_char_array(job_log_path);
}











uint8_t controler_iij_set_ip_addr(char * ip_addr)
{
	uint8_t res = com_tcp_set_ip_addr(iij_tcp_ref, ip_addr);

	if(res == STATUS_SUCCESS)
		c_log_add_record_with_cmd(log_ref, "Nastavena nová IP adresa pro připojení GISu: %s", ip_addr);
	else
		c_log_add_record_with_cmd(log_ref, "Neúspěšné nastavení nové IP adresy pro připojení GISu: %s", ip_addr);

	controler_update_config(CFG_GROUP_NETWORK,
			CFG_NETWORK_GIS_IP_ADDRESS,
			CONFIG_TYPE_STRING,
			ip_addr,
			"Nastavení síťového ip adresy pro GIS aktualizováno.",
			"Nepodařilo se aktualizovat ip adresu pro GIS");
	return res;
}

uint8_t controler_iij_set_tcp_port(int port)
{
	uint8_t res = com_tcp_set_tcp_port(iij_tcp_ref, port);

	if(res == STATUS_SUCCESS)
		c_log_add_record_with_cmd(log_ref, "Nastavena nové hodnoty TCP portu pro připojení GISu: %d", port);
	else
		c_log_add_record_with_cmd(log_ref, "Neúspěšné nastavení nové hodnoty TCP portu pro připojení GISu: %d", port);

	controler_update_config(CFG_GROUP_NETWORK,
			CFG_NETWORK_GIS_TCP_PORT,
			CONFIG_TYPE_INT,
			&port,
			"Nastavení síťového portu pro GIS aktualizováno.",
			"Nepodařilo se aktualizovat síťového portu pro GIS");

	return res;
}



char * controler_quadient_network_get_ip_address()
{
	return 	comm_monitor_get_ip_address(quadient_comm_monitor);	//com_tcp_get_ip_addr(quadient_connection);
}

char * controler_iij_network_get_ip_address()
{
	return 	comm_monitor_get_ip_address(iij_comm_monitor);	//com_tcp_get_ip_addr(iij_connection);
}

char * controler_pci_network_get_ip_address()
{
	return 	comm_monitor_get_ip_address(pci_comm_monitor);	//com_tcp_get_ip_addr(pci_connection);
}


uint8_t controler_quadient_network_set_ip_address(char * ip_address)
{
	if(comm_monitor_status(quadient_comm_monitor) == false)
	{
		uint8_t res = comm_monitor_set_ip_address(quadient_comm_monitor, ip_address);

		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(log_ref, "IP adresa k serveru Quadient nastavena.");

			controler_update_config(CFG_GROUP_NETWORK,
					CFG_NETWORK_QUADIENT_PC_IP_ADDRESS,
					CONFIG_TYPE_STRING,
					ip_address,
					"Nastavení síťového ip adresy pro kontrolu připojení počítače Quadient aktualizováno.",
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače Quadient");
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru Quadient! Chybný formát IP adresy.");
		}

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru Quadient! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}

uint8_t controler_iij_network_set_ip_address(char * ip_address)
{
	if(comm_monitor_status(iij_comm_monitor) == false)
	{
		uint8_t res = comm_monitor_set_ip_address(iij_comm_monitor, ip_address);

		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(log_ref, "IP adresa k serveru IIJ nastavena.");

			controler_update_config(CFG_GROUP_NETWORK,
					CFG_NETWORK_IIJ_PC_IP_ADDRESS,
					CONFIG_TYPE_STRING,
					ip_address,
					"Nastavení síťového ip adresy pro kontrolu připojení počítače IIJ aktualizováno.",
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače IIJ");
		}
		else
		{
			c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru IIJ! Chybný formát IP adresy.");
		}

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru IIJ! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}

uint8_t controler_pci_network_set_ip_address(char * ip_address)
{
	if(comm_monitor_status(pci_comm_monitor) == false)
	{
		uint8_t res = comm_monitor_set_ip_address(pci_comm_monitor, ip_address); //com_tcp_set_ip_addr(pci_connection, ip_address);

		if(res == STATUS_SUCCESS)
		{
			c_log_add_record_with_cmd(log_ref, "IP adresa k serveru PCI nastavena.");

			controler_update_config(CFG_GROUP_NETWORK,
					CFG_NETWORK_PCI_PC_IP_ADDRESS,
					CONFIG_TYPE_STRING,
					ip_address,
					"Nastavení síťového ip adresy pro kontrolu připojení počítače PCI aktualizováno.",
					"Nepodařilo se aktualizovat ip adresu pro kontrolu připojení počítače PCI");
		}
		else
			c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru PCI! Chybný formát IP adresy.");

		return res;
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Nelze nastavit novou IP adresu k serveru Quadient! Síť je stále připojena.");
		return STATUS_IP_ADDRESS_ERROR;
	}
}




char * controler_iij_get_ip_addr()
{
	return com_tcp_get_ip_addr(iij_tcp_ref);
}

int controler_iij_get_tcp_port()
{
	return com_tcp_get_tcp_port(iij_tcp_ref);
}

uint8_t controler_iij_connect()
{
	uint8_t res = controler_iij_try_connect();

	if(res == STATUS_CLIENT_CONNECTED)
	{
		iij_tcp_connection_req = true;
		//c_log_add_record_with_cmd(log_ref, "Spojení s GISem navázáno.");
	}
	else
	{
		//c_log_add_record_with_cmd(log_ref, "Spojení s GISem nebylo navázáno!");
	}

	return res;
}

uint8_t controler_iij_is_connected()
{
	return com_tcp_is_connected(iij_tcp_ref);
}

uint8_t controler_iij_disconnect()
{
	com_tcp_disconnect(iij_tcp_ref);

	iij_tcp_connection_req = false;
	c_log_add_record_with_cmd(log_ref, "Spojení s GISem ukončeno.");

	return STATUS_SUCCESS;
}

uint8_t controler_iij_status()
{
	return 0;
}

uint8_t controler_pci_set_ip_addr(char * ip_addr)
{
	return com_tcp_set_ip_addr(pci_tcp_ref, ip_addr);
}

uint8_t controler_pci_set_tcp_port(int port)
{
	return com_tcp_set_tcp_port(pci_tcp_ref, port);
}

char * controler_pci_get_ip_addr()
{
	return com_tcp_get_ip_addr(pci_tcp_ref);
}

int controler_pci_get_tcp_port()
{
	return com_tcp_get_tcp_port(pci_tcp_ref);
}

uint8_t controler_pci_connect()
{
	return controler_pci_try_connect();
}

uint8_t controler_pci_is_connected()
{
	return com_tcp_is_connected(pci_tcp_ref);
}

uint8_t controler_pci_disconnect()
{
	com_tcp_disconnect(pci_tcp_ref);
	return STATUS_SUCCESS;
}

uint8_t controler_pci_status()
{
	return 0;
}

/************************************************************ functions definitions ********************************************/


void * controler_machine_handler(void * param)
{
	while(true)
	{
		if((machine_handler_run == false))
		{
			if((machine_state == MACHINE_STATE_WAIT) || (machine_state == MACHINE_STATE_ERROR))
			{
				break;
			}
			else
			{
				if((machine_state != MACHINE_STATE_PRINT_MAIN) && (machine_state != MACHINE_STATE_PRINT_COMPANION) &&
					(machine_state != MACHINE_STATE_PRINT_FINISH) && (machine_state != MACHINE_STATE_JOB_FINISH) &&
					(machine_state != MACHINE_STATE_SAVE_Q_CSV) && (machine_state != MACHINE_STATE_WAIT) &&
					(machine_state != MACHINE_STATE_CLEAR_HOT_FOLDER) && (machine_state != MACHINE_STATE_CLEAR_TO_FINISH))
					{
						controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
					}
			}
		}

		/* read all inputs */
		io_card_sync_inputs(io_card_ref);

		/* read feeder and stacker status */
		if(c_timer_delay(f_s_delay, 50) > 0)
		{
			stacker_status = io_card_get_bit_value(io_card_ref, IO_CARD_A1, A1_IN_6_SN0, A1_IN_7_SN1, A1_IN_8_SN2);
			feeder_status = io_card_get_bit_value(io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2);
		}

		if(io_manual == false)
		{
			controler_safety_system_in();

			controler_print_break_req();

			controler_machine_log_monitor();

			if((machine_state == MACHINE_STATE_WAIT) || (machine_state == MACHINE_STATE_NEXT) || machine_state == MACHINE_STATE_LOAD_NEXT_JOB_FILE)
				controler_machine_state_read_hotfolder();

			else if(machine_state == MACHINE_STATE_PREPARE)
				controler_machine_state_prepare();

			else if(machine_state == MACHINE_STATE_READY_TO_START)
				controler_machine_state_ready_to_start();

			else if(machine_state == MACHINE_STATE_READ_CSV_LINE)
				controler_machine_state_read_csv_line();

			else if(machine_state == MACHINE_STATE_FEEDER_ERROR)
				controler_machine_state_feeder_error();

			else if(machine_state == MACHINE_STATE_STACKER_ERROR)
				controler_machine_state_stacker_error();

			else if(machine_state == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
				controler_machine_state_wait_for_confirmation();

			else if(machine_state == MACHINE_STATE_PRINT_COMPANION)
				controler_machine_state_print_companion();

			else if(machine_state == MACHINE_STATE_PRINT_MAIN)
				controler_machine_state_print_main();

			else if(machine_state == MACHINE_STATE_FAKE_COMPANION)
				controler_machine_state_fake_companion();

			else if(machine_state == MACHINE_STATE_CLEAR_BELT_AFTER_JAM)
				controler_machine_state_clear_belt_after_jam();

			else if(machine_state == MACHINE_STATE_WAIT_FOR_ENA_START)
				controler_machine_state_wait_for_ena_ready();

			else if((machine_state == MACHINE_STATE_PAUSE) || (machine_state == MACHINE_STATE_JAM_EVENT))
				controler_machine_state_pause();

			else if(machine_state == MACHINE_STATE_WAIT_FOR_PRINT_FINISH)
				controler_machine_state_wait_for_print_finish();

			else if(machine_state == MACHINE_STATE_SAVE_Q_CSV)
				controler_machine_state_save_q_csv();

			else if(machine_state == MACHINE_STATE_PRINT_BREAK)
				controler_machine_state_print_break();

			else if((machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER) || (machine_state == MACHINE_STATE_CLEAR_TO_FINISH))
				controler_machine_state_clear_hotfolder();

			else if((machine_state == MACHINE_STATE_PRINT_FINISH) || (machine_state == MACHINE_STATE_JOB_FINISH))
				controler_machine_state_finish();

			else if(machine_state == MACHINE_STATE_SEND_RESPONSE_TO_IA)
				controler_machine_state_send_response_to_ia();

			else if(machine_state == MACHINE_STATE_MERGE_COMPANION_PDF_FILES)
				controler_machine_state_merge_companion_pdf_files();

			else if(machine_state == MACHINE_STATE_DELETE_STAMP_PDF)
				controler_machine_state_delete_stamp_pdf_file();

			else if(machine_state == MACHINE_STATE_COPY_COMPANION_PDF)
				controler_machine_state_copy_companion_pdf_file();

			else if(machine_state == MACHINE_STATE_COMPANION_GENERATION_DELAY)
				coontroler_machine_state_companion_generation_delay();

			else if(machine_state == MACHINE_STATE_SEND_COMPANION_DATA)
				controler_machine_state_send_companion_data();

			/* machine error state handling */
			else if(machine_state == MACHINE_STATE_ERROR)
				controler_machine_state_error();

			/* write all outputs */
			controler_safety_system_out();
		}
		else
		{
			controler_feed_sheet();
		}

		io_card_sync_outputs(io_card_ref);

		usleep(MACHINE_CYCLE_TIMING); //sleep 1ms
	}

	/* if the machine handler is finished, must be machine_handler_run in false state, when is set to true agin, it is the flag for successfuly finished thread */
	machine_handler_run = true;

	return NULL;
}

void controler_machine_state_error()
{
	if(machine_error_reset_req == true)
	{
		error_code = MACHINE_ERR_NO_ERROR;
		controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
	}
	else if(fake_companion_req == true)
	{
		error_code = MACHINE_ERR_NO_ERROR;
		controler_set_machine_state(MACHINE_STATE_FAKE_COMPANION);
	}
}

int controler_get_machine_mode()
{
	return machine_mode;
}


int controler_get_error_val()
{
	return error_code;
}

const char* controler_get_error_str()
{
	const char * error_str = NULL;
	const lang * multi_lang = multi_lang_get(controler_get_interface_language());

	switch(error_code)
	{
		case MACHINE_ERR_NO_ERROR:
			error_str = multi_lang->err_no_error;
			break;

		case MACHINE_ERR_CANT_SAVE_F_CSV:
			error_str = multi_lang->err_cant_save_f_csv;
			break;

		case MACHINE_ERR_ANALYZE_CAMERA_CSV:
			error_str = multi_lang->err_analyze_camera_csv;
			break;

		case MACHINE_ERR_CANT_LOAD_BKCORE_CSV:
			error_str = multi_lang->err_cant_load_bkcore_csv;
			break;

		case MACHINE_ERR_CANT_COPY_PDF_TO_GIS:
			error_str = multi_lang->err_cant_copy_pdf_to_gis;
			break;

		case MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA:
			error_str = multi_lang->err_cant_copy_csv_to_camera;
			break;

		case MACHINE_ERR_JOB_ORDER_MISMATCH:
			error_str = multi_lang->err_job_order_mismatch;
			break;

		case MACHINE_ERR_STACKER_JAM_PILE:
			error_str = multi_lang->err_stacker_jam_pile;
			break;

		case MACHINE_ERR_STACKER_JAM_CONVEYOR:
			error_str = multi_lang->err_stacker_jam_conveyor;
			break;

		case MACHINE_ERR_STACKER_BOTTOM:
			error_str = multi_lang->err_stacker_bottom;
			break;

		case MACHINE_ERR_STACKER_MULTIFUNCTION:
			error_str = multi_lang->err_stacker_multifunction;
			break;

		case MACHINE_ERR_STACKER_OFF:
			error_str = multi_lang->err_stacker_off;
			break;

		case MACHINE_ERR_FEEDER_E_STOP:
			error_str = multi_lang->err_feeder_e_stop;
			break;

		case MACHINE_ERR_FEEDER_OFF:
			error_str = multi_lang->err_feeder_off;
			break;

		case MACHINE_ERR_FEEDER_MULTIFUNCTION:
			error_str = multi_lang->err_feeder_multifunction;
			break;

		case MACHINE_ERR_FEEDER_SHEET_MISSING:
			error_str = multi_lang->err_feeder_sheet_missing;
			break;

		case MACHINE_ERR_FEEDER_DOUBLE_SHEET:
			error_str = multi_lang->err_feeder_double_sheet;
			break;

		case MACHINE_ERR_E_STOP:
			error_str = multi_lang->err_machine_e_stop;
			break;

		case MACHINE_ERR_REJECT_BIN_JAM:
			error_str = multi_lang->err_reject_bin_jam;
			break;

		case MACHINE_ERR_FEEDER_JAM:
			error_str = multi_lang->err_feeder_jam;
			break;

		case MACHINE_ERR_TI:
			error_str = multi_lang->err_ti;
			break;

		case MACHINE_ERR_TA:
			error_str = multi_lang->err_ta;
			break;

		case MACHINE_ERR_COUNTERS_MISMATCH:
			error_str = multi_lang->err_counters_mismatch;
			break;

		case MACHINE_ERR_LOW_PRINT_QUALITY:
			error_str = multi_lang->err_low_print_quality;
			break;

		case MACHINE_ERR_GIS_DISCONNECTED:
			error_str = multi_lang->err_gis_disconnected;
			break;

		case MACHINE_ERR_CSV_SHEET_NUM:
			error_str = multi_lang->err_csv_sheet_num;
			break;

		case MACHINE_ERR_SHEET_FEEDER_REDIRECTION:
			error_str = multi_lang->err_sheet_feeder_redirection;
			break;

		case MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang->err_pci_computer_not_responding;
			break;

		case MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang->err_iij_computer_not_responding;
			break;

		case MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING:
			error_str = multi_lang->err_quadient_computer_not_responding;
			break;

		case MACHINE_ERR_PRINT_MAIN_FREEZE:
			error_str = multi_lang->err_print_main_freeze;
			break;

		case MACHINE_ERR_PRINT_COMPANION_FREEZE:
			error_str = multi_lang->err_print_companion_freeze;
			break;

		case MCAHINE_ERR_PRINT_INITIALIZATION_FREEZE:
			error_str = multi_lang->err_print_initialization_freeze;
			break;

		case MACHINE_ERR_PRINT_FINALIZING_FREEZE:
			error_str = multi_lang->err_print_finalizing_freeze;
			break;

		case MACHINE_ERR_STACKER_FULL:
   			error_str = multi_lang->err_stacker_full;
			break;

		case MACHINE_ERR_REJECT_BIN_FULL:
    			error_str = multi_lang->err_reject_bin_full;
			break;

		case MACHINE_ERR_PAPER_JAM_CONVAYOR:
			error_str = multi_lang->err_paper_jam_conveyor;
			break;
		case MACHINE_ERR_TAPE_MISSING:
			error_str = multi_lang->err_tape_missing;
			break;
		case MACHINE_ERR_MACHINE_MODE_ADAPTATION:
			error_str = multi_lang->err_machine_mode_adaptation_timeout;
			break;
		default:
			error_str = multi_lang->err_unknown_error;
	}

	return error_str;
}

void controler_set_machine_mode(int mode)
{
	if((mode >= 0) && (mode < GR_MODE_N))
	{
		machine_mode = mode;
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

		c_log_add_record_with_cmd(log_ref, "Nastaven režim Gremser stroje: %s", mode_str);
	}
}


void controler_print_break_req()
{
	if((machine_state != MACHINE_STATE_PRINT_MAIN) && (machine_state != MACHINE_STATE_PRINT_COMPANION) &&
		(machine_state != MACHINE_STATE_PRINT_FINISH) && (machine_state != MACHINE_STATE_JOB_FINISH) &&
		(machine_state != MACHINE_STATE_SAVE_Q_CSV) && (machine_state != MACHINE_STATE_WAIT) &&
		(machine_state != MACHINE_STATE_CLEAR_HOT_FOLDER) && (machine_state != MACHINE_STATE_CLEAR_TO_FINISH))
	{
		if((machine_cancel_req) == true)
		{
			controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
		}
	}
}

void controler_refresh_dir_list()
{
	pthread_mutex_lock(&(mutex));

	job_list_changed = 1;

	pthread_mutex_unlock(&(mutex));
}


uint8_t controler_job_list_changed()
{
	if(job_list_changed == 1)
	{
		pthread_mutex_lock(&(mutex));
		job_list_changed = 0;
		pthread_mutex_unlock(&(mutex));

		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t controler_split_csv(array_list * filtered_csv_list, char * address)
{
	uint8_t state = 0;

	if(filtered_csv_list != NULL)
	{
		c_string * split_csv = c_string_new();

		for(int i = 0; i < array_list_size(filtered_csv_list); i++)
		{
			c_string * report_item = array_list_get(filtered_csv_list, i);
			char * csv_content = util_load_csv(c_string_get_char_array(job_log_path), c_string_get_char_array(report_item), NULL);

			if(csv_content != NULL)
			{
				c_string_concat(split_csv, csv_content);

				if(i+1 != array_list_size(filtered_csv_list))
					c_string_concat(split_csv, "\n\n");

				free(csv_content);
			}
			else
			{
				state = 2;
			}
		}

		FILE * csv_out = fopen(address, "w");

		if(csv_out != NULL)
		{
			int32_t size_out = fwrite(c_string_get_char_array(split_csv), sizeof(char), c_string_len(split_csv), csv_out);

			if(size_out < 0)
				state = 3;

			fclose(csv_out);
		}

		c_string_finalize_v3(split_csv);
	}
	else
	{
		state = 1;
	}

	return state;
}

bool controler_date_lower_limit(int day_from, int month_from, int year_from, int day, int month, int year)
{
	bool A = (year_from < year);
	bool B = (year_from == year);
	bool C = (month_from < month);
	bool D = (month_from == month);
	bool E = (day_from < day);
	bool F = (day_from == day);

	return ((A || B) && (A || C || D) && (A || C || E || F) && ((!E) || (!F)) && ((!C) || (!D)) && ((!A) || (!B)));
}

bool controler_date_upper_limit(int day_to, int month_to, int year_to, int day, int month, int year)
{
	bool A = (year_to > year);
	bool B = (year_to == year);
	bool C = (month_to > month);
	bool D = (month_to == month);
	bool E = (day_to > day);
	bool F = (day_to == day);

 	return ((A || B) && (A || C || D) && (A || C || E || F) && ((!E) || (!F)) && ((!C) || (!D)) && ((!A) || (!B)));
}


array_list * controler_csv_manage_filter_report_csv(array_list * report_csv_list,
					int day_from, int month_from, int year_from,
					int day_to, int month_to, int year_to)
{
	array_list * filtered_job_list = array_list_new();

	if(report_csv_list != NULL)
	{
		for(int j = 0; j < array_list_size(report_csv_list); j++)
		{
			c_string * report_item = array_list_get(report_csv_list, j);

			if(report_item != NULL)
			{
				char * csv_content = util_load_csv(c_string_get_char_array(job_log_path), c_string_get_char_array(report_item), NULL);

				uint32_t year = 0;
				uint32_t month = 0;
				uint32_t day = 0;
				int i = 0;
				uint8_t row = 0;
				uint8_t item = 0;

				if(csv_content != NULL)
				{
					while((csv_content[i] != 0) && (row < 10) && ((row < 9) || (item < 2) || (isdigit(csv_content[i]) != 0)))
					{
						if(csv_content[i] == '\n')
							row ++;

						if((csv_content[i] == '/') && (row == 9))
							item ++;

						if((row == 9) && (isdigit(csv_content[i])))
						{
							switch(item)
							{
								case 0:
									day = day*10+(csv_content[i]-48);
								break;

								case 1:
									month = month*10+(csv_content[i]-48);
								break;

								case 2:
									year = year*10+(csv_content[i]-48);
								break;
							}
						}

						i++;
					}

					year += 2000;

					bool date_from = controler_date_lower_limit(day_from, month_from, year_from, day, month, year);
					bool date_to = controler_date_upper_limit(day_to,  month_to, year_to, day, month, year);

					if(date_from && date_to)
					{
						c_string * report_item_copy = NULL;

						report_item_copy = c_string_make_copy(report_item);

						if(report_item_copy != NULL)
							array_list_add(filtered_job_list, report_item_copy);

					//printf("%d | %d : csv %s -> %d/%d/%d x %d/%d/%d x %d/%d/%d\n", date_from, date_to, c_string_get_char_array(report_item),day_from, month_from, year_from, day,month,year, day_to, month_to, year_to);

					}

					free(csv_content);
				}
				else
				{
					printf("csv content not loaded %s/%s\n", c_string_get_char_array(job_log_path), c_string_get_char_array(report_item));
				}
			}
		}
	}

	return filtered_job_list;
}


uint8_t controler_job_successfully_loaded()
{
	for(int i = 0; i < array_list_size(job_list); i++)
	{
		q_job * job = NULL;
		job = array_list_get(job_list, i);

		if(job != NULL)
		{
			if(q_job_get_flag(job) == 'k')
			{
				if((q_job_get_sheet_number(job) == 0) || (q_job_get_stamp_number(job) == 0))
				{
					//printf("job info: %d, %d\n", q_job_get_sheet_number(job), q_job_get_stamp_number(job));
					return 1;
				}
			}
		}
		else
		{
			printf("job null\n");
			return 1;
		}
	}

	return 0;
}


void controler_initialize_companion_job_sequence()
{
	int job_index = -1;
	q_job *job = hot_find_job(job_list, c_string_get_char_array(printed_job_name), &job_index);

	if(job != NULL)
	{
		if(q_job_get_job_order(job) == 1)
		{
			job_info_set_order_name(info, "merge");
			job_info_set_job_name(info, "merge");
			job_info_generate_csv_name(info);

			job_info_add_job_record(info);
		}

		printed_job_index = job_index;

		if(q_job_get_flag(job) == 'e')
			controler_set_machine_state(MACHINE_STATE_SEND_RESPONSE_TO_IA);
		else if(q_job_get_flag(job) == 'p')
			controler_set_machine_state(MACHINE_STATE_COPY_COMPANION_PDF);
		else
			controler_set_machine_state(MACHINE_STATE_DELETE_STAMP_PDF);
	}
}

void controler_load_job_list()
{
	pthread_mutex_lock(&(mutex));

	if(job_list != NULL)
		array_list_destructor_with_release_v2(job_list, q_job_finalize);

	job_list = hot_read_dir_content(q_hotfolder_main_path);

	if((hot_compare_job_lists(job_list, job_list_pre) > 0) || (controler_job_successfully_loaded() > 0))
	{
		if(job_list_pre != NULL)
			array_list_destructor_with_release_v2(job_list_pre, q_job_finalize);

		job_list_pre = hot_copy_job_list(job_list);
		job_list_changed = 1;
	}

	/*
	if(clear_on_startup == 0)
	{
		controler_total_clear_hotfolder();
		clear_on_startup = 1;
	}
	*/

	pthread_mutex_unlock(&(mutex));
}

void controler_initialize_job_info_structure(char * order_name, char * job_name, int sheet_number, int stamp_number)
{
	job_info_set_order_name(info, order_name);
	job_info_set_job_name(info, job_name);
	job_info_generate_csv_name(info);

	job_info_set_total_sheet_number(info, sheet_number);
	job_info_set_total_stamp_number(info, stamp_number);
}

void controler_machine_state_read_hotfolder()
{
	if((c_timer_delay(hot_reading_interval, HOT_FOLDER_READING_INTERVAL) > 0) || (machine_state == MACHINE_STATE_NEXT) || machine_state == MACHINE_STATE_LOAD_NEXT_JOB_FILE)
	{
		if(machine_print_req == false)
			controler_load_job_list();

		if(c_string_len(printed_job_name) > 0)
		{
			if((((machine_print_req == true) && (machine_state == MACHINE_STATE_WAIT)) || (machine_state == MACHINE_STATE_NEXT &&
				(controler_job_successfully_loaded() == 0))))
			{
				int job_index = -1;

				q_job * job = hot_find_job(job_list, c_string_get_char_array(printed_job_name), &job_index);

				if((job != NULL) && (q_job_get_bkcore_csv_name(job) != NULL))
				{
					if(q_job_get_job_order(job) == (job_info_get_job_index(info)+2))
					{
						if(q_job_get_flag(job) == 'e')
						{
							controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
							printed_job_index = job_index;
						}
						else
						{
							if(((io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) == 0) || (q_job_get_pdf_name(job) != NULL)) &&
								((io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) == 0) || (q_job_get_camera_csv_name(job) != NULL)))
							{
								if(q_job_get_job_order(job) == 1)
								{
									controler_initialize_job_info_structure(q_job_get_order_name(job),
													q_job_get_job_name(job),
													q_job_get_sheet_number(job),
													q_job_get_stamp_number(job));
								}


								printed_job_index = job_index;
								job_info_add_job_record(info);

								controler_set_machine_state(MACHINE_STATE_PREPARE);
								file_delay = c_freq_millis();

								system("sh /home/stc/script.sh");
							}
						}
					}
					else
					{
						controler_set_machine_error(MACHINE_ERR_JOB_ORDER_MISMATCH);
					}
				}
			}
			else if((machine_state == MACHINE_STATE_LOAD_NEXT_JOB_FILE) && (controler_job_successfully_loaded() == 0))
			{
				controler_initialize_companion_job_sequence();
			}
		}
	}
}

void controler_machine_state_wait_for_ena_ready()
{
	if(feeder_status == MACHINE_FN_READY_TO_FEED)
	{
		if((ena_start_timer + 3000) <= c_freq_millis())
			controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
	}
	else
	{
		ena_start_timer = c_freq_millis();
	}
}

void controler_machine_state_clear_belt_after_jam()
{
	if(print_finishing_step  == 0)
	{
		if(feeder_status == MACHINE_FN_READY_TO_FEED)
		{
			if((ena_start_timer + 15000) <= c_freq_millis())
			{
				print_finishing_step = 1;

				feeded_sheet_counter_in_job = 0;
				stacked_sheet_counter_in_job = 0;
				rejected_sheet_counter_in_job = 0;

				c_string * q_feedback_csv_content = NULL;
				q_job * job = controler_get_printed_job();

				if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
				{
					q_feedback_csv_content = controler_compare_csv(job, NULL);

					util_copy_file(c_string_get_char_array(pci_hotfolder_out_path), "/home/stc/JOBCSV/CAM", q_job_get_camera_csv_name(job));
					util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), q_job_get_camera_csv_name(job));

					c_log_add_record_with_cmd(log_ref, "Mazání výstupního kamerového csv souboru po zácpě na dopravníku z hotfolderu: %s/%s",
								 c_string_get_char_array(pci_hotfolder_out_path),
								q_job_get_camera_csv_name(job));
				}
				else
				{
					char num_str[32];
					q_feedback_csv_content = c_string_new();

					for(int i = stacked_sheet_counter_in_job+rejected_sheet_counter_in_job+1; i < job_info_get_job_sheet_number(info, job_info_get_job_index(info))+1; i++)
					{
						if(i+1 < (job_info_get_job_sheet_number(info, job_info_get_job_index(info))+1))
							sprintf(num_str,"%d\n", i);
						else
							sprintf(num_str,"%d", i);

						c_string_concat(q_feedback_csv_content, num_str);
					}
				}

				if(q_feedback_csv_content != NULL)
				{
					util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path),
							q_job_get_bkcore_csv_name(job),
							c_string_get_char_array(q_feedback_csv_content));


					c_log_add_record_with_cmd(log_ref, "Uložení zpětnovazebního csv po zácpě na dopravníku na adresu: %s/%s",
									c_string_get_char_array(q_hotfolder_feedback_path),
									q_job_get_bkcore_csv_name(job));

				/*	util_save_csv("/home/stc",
							q_job_get_bkcore_csv_name(job),
							c_string_get_char_array(q_feedback_csv_content));
*/
					c_string_finalize_v3(q_feedback_csv_content);
				}
			}
		}
		else
		{
			ena_start_timer = c_freq_millis();
		}
	}
	else if(print_finishing_step == 1)
	{
		if(strcmp(c_string_get_char_array(print_controler_status), "Ready") != 0)
			print_finishing_step = 2;
		else
			print_finishing_step = 3;

		printf("Recognize state of job breaking\n");
	}
	else if(print_finishing_step == 2)
	{
		controler_printer_abbort_print(0);
		printf("Send P,A\n");
		timer = c_freq_millis();
		print_finishing_step = 3;
	}
	else
	{
		if((timer + 500) <= c_freq_millis())
		{
			printf("Send R,A\n");
			controler_printer_abbort_print(1);
			print_finishing_step = 0;
			controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);
			machine_jam_req = false;
		}
	}
}

void controler_machine_state_pause()
{
	/* from pause state can return to print state only if the conditions are met */
	if(machine_pause_req == false)
	{
		if((io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_1_RJ_full) == 0) &&
		(io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_2_SN_full) == 0))
		{
			feed_sheet = 0;
			error_code = MACHINE_ERR_NO_ERROR;

			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TA_cyc, 0);

			bkcore_csv_pos = feeded_sheet_counter_in_job;
			rejected_sheet_seq_counter = 0;

			if(stacked_sheet >= max_stacked_sheets)
			{
				controler_clear_stacking_counters();
			}

			if(machine_jam_req == true)
				controler_set_machine_state(MACHINE_STATE_CLEAR_BELT_AFTER_JAM);
			else
				controler_set_machine_state(MACHINE_STATE_WAIT_FOR_ENA_START);
		}
	}
	else if((fake_companion_req == true))
	{
		controler_set_machine_state(MACHINE_STATE_FAKE_COMPANION);
	}
}


void controler_clear_stacking_counters()
{
	stacked_sheet = 0;
	rejected_sheet = 0;
	feeded_sheet = 0;
}

int controler_set_stacket_sheets_offset()
{
	if(job_info_get_sheet_index_at_csv_position(info, 0, 0) > 1)
		return job_info_get_sheet_index_at_csv_position(info, 0, 0) - 1;
	else
		return 0;
}

void controler_machine_state_prepare()
{
	/* load bkcore csv content */
	if((file_delay+2000) < c_freq_millis())
	{
		if(controler_load_bkcore_csv() == STATUS_SUCCESS)
		{
			q_job * job = controler_get_printed_job();

			if(job != NULL)
			{
				uint8_t status = 0;

				/* copy pdf to gis hotfolder or based on MB0 input status */
				if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0 && ((companion_faked == false) || (q_job_get_flag(job) == 'p')))
				{
					status = util_dir_is_empty(c_string_get_char_array(gis_hotfolder_path));
					printf("gis dir empty: %d\n", status);

					if(status == 0)
					{
						status = util_copy_file(c_string_get_char_array(q_hotfolder_main_path),
												c_string_get_char_array(gis_hotfolder_path),
												q_job_get_pdf_name(job));


						c_log_add_record_with_cmd(log_ref, "Kopírování  pdf souboru do gisu při spuštění jobu: %s/%s -> %s/%s",
									 c_string_get_char_array(q_hotfolder_main_path),
									q_job_get_pdf_name(job), c_string_get_char_array(gis_hotfolder_path), q_job_get_pdf_name(job));
					}
				}

				if(status == 0)
				{
					if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0 && ((companion_faked == false) || (q_job_get_flag(job) == 'p')))
					{
						status = util_copy_file(c_string_get_char_array(q_hotfolder_main_path),
												c_string_get_char_array(pci_hotfolder_in_path),
												q_job_get_camera_csv_name(job));

						c_log_add_record_with_cmd(log_ref, "Kopírování kamerového csv souboru do kameray při spuštění jobu: %s/%s -> %s/%s",
									 c_string_get_char_array(q_hotfolder_main_path),
									q_job_get_camera_csv_name(job), c_string_get_char_array(pci_hotfolder_in_path), q_job_get_camera_csv_name(job));

					}

					if(status == 0)
					{
						controler_set_machine_state(MACHINE_STATE_READY_TO_START);

						timer = c_freq_millis();

						/* prepare job info structure for current job */
						int rows = 0, csv_line_index = 0, csv_pos = 0;
						char sheet_index[255];

						while(bkcore_csv_content[csv_pos] != 0)
						{
							if((bkcore_csv_content[csv_pos] == '\n') || (bkcore_csv_content[csv_pos+1] == 0))
							{
								if((rows > 4) || (q_job_get_flag(job) != 'k'))
								{
									sheet_index[csv_line_index] = 0;
									job_info_add_sheet_record(info, sheet_index);
									csv_line_index = 0;
								}

								rows ++;
							}
							else
							{
								if((rows > 4) || (q_job_get_flag(job) != 'k'))
								{
									if(isdigit(bkcore_csv_content[csv_pos]))
									{
										sheet_index[csv_line_index] = bkcore_csv_content[csv_pos];
										csv_line_index++;
									}
								}
							}

							csv_pos++;
						}

						c_log_add_record_with_cmd(log_ref,
						"Načten bkcore csv soubor s %d archy",
						job_info_get_job_sheet_number(info, job_info_get_job_index(info)));

						if(q_job_get_job_order(job) == 1)
							stacked_sheet_offset = controler_set_stacket_sheets_offset();
					}
					else
					{
						controler_set_machine_error(MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA);
					}
				}
				else
				{
					controler_set_machine_error(MACHINE_ERR_CANT_COPY_PDF_TO_GIS);
				}
			}
		}
		else
		{
			controler_set_machine_error(MACHINE_ERR_CANT_LOAD_BKCORE_CSV);
		}
	}
}



void controler_feed_control()
{
	if(machine_print_one_req == true)
	{
		if((bkcore_csv_pos) > feeded_sheet_counter_pre)
		{
			machine_pause_req = true;
			machine_print_one_req = false;
		}
	}

	feeded_sheet_counter_pre = bkcore_csv_pos;
}


/*
** send tab insert signal into control function and with reasing edge starts inserting the tab
** signal is send only if the stacked counter not equal with last send singal value
*/
void controler_tab_insert_runtime()
{
	if((stacked_sensor_for_ti_pre != stacker_status) && (stacker_status == MACHINE_SN_STACKING))
	{
		stacked_for_ti++;
	}

	stacked_sensor_for_ti_pre = stacker_status;


	if((tab_insert_automat > 0))
	{
		if((stacked_for_ti > 0) && (((stacked_for_ti) % tab_insert_sequence) == 0))
		{
			if((tab_insert_busy == false) && (last_ti_req != stacked_for_ti))
			{
				tab_insert_req = 1;
				last_ti_req = stacked_for_ti;
			}
		}
	}
}


/*
** todo výstup z TI který se po nastavení na true po aktivaci posuvu se musí být vždy 150 - 200 ms aktivovaný jinak je problém s posuvem
** a je nutné vyvolat chybovou hlášku "Problém s posuvem pásky"
*/
void controler_tab_insert_control()
{
	if(tab_insert_req > 0)
	{
		tab_insert_step = 1;
		tab_insert_req = 0;
		tab_insert_busy = true;
	}

#if 1
	if((tab_insert_step > 1) && (ti_fail_delay_check + 150) < c_freq_millis())
	{
		if(io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_7_SIGNAL_TAPE_OUT) > 0)
		{
			error_code = MACHINE_ERR_TAPE_MISSING;
		}
	}
#endif

	if(tab_insert_step == 1)
	{
		// cut for clear tab inserter
		io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_2_TI_tape_cut, 1);

		ti_fail_delay_check = c_freq_millis();
		tab_insert_step = 2;
	}
	else if(tab_insert_step == 2)
	{
		if((timer_tab_insert + 50) < c_freq_millis())
		{
			io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_2_TI_tape_cut, 0);
			tab_insert_step = 3;
		}
	}
	else if(tab_insert_step == 3)
	{
		//insert tab
		io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_3_TI_motor_drive, 1);
		timer_tab_insert = c_freq_millis();
		tab_insert_step = 4;
	}
	else if(tab_insert_step == 4)
	{
		if((timer_tab_insert + tab_insert_len) < c_freq_millis())
		{
			io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_3_TI_motor_drive, 0);
			timer_tab_insert = c_freq_millis();
			tab_insert_step = 5;
		}
	}
	else if(tab_insert_step == 5)
	{
		//cut tab
		if((timer_tab_insert + 100) < c_freq_millis() && (((stacked_for_ti) % tab_insert_sequence) >= ti_delay))
		{
			io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_2_TI_tape_cut, 1);
			timer_tab_insert = c_freq_millis();
			tab_insert_step = 6;
		}
	}
	else if(tab_insert_step == 6)
	{
		if((timer_tab_insert + 50) < c_freq_millis())
		{
			io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_2_TI_tape_cut, 0);
			tab_insert_step = 7;
		}
	}
	else if(tab_insert_step == 7)
	{
		tab_insert_busy = false;
	}
	else
	{
		tab_insert_step = 0;
	}

}

void controler_fun_control()
{
	if(machine_state == MACHINE_STATE_PRINT_COMPANION)
	{
		fan_intensity = fan_intensity_set;
	}
	else if((machine_state != MACHINE_STATE_PRINT_COMPANION) && (machine_state != MACHINE_STATE_WAIT))
	{
		fan_intensity = 0;
		fan_activity = 0;
	}
	else
	{
		fan_intensity = ((fan_activity > 0) ? fan_intensity_set : 0);
	}

	io_card_set_output(io_card_ref, IO_CARD_A2, A2_OUT_0_FUN_CONTROL, soft_pwm_run(fan_regulation, (uint8_t) fan_intensity));
}

void controler_set_fan_intensity(int intensity)
{
	if(intensity <= 100)
	{
		fan_intensity_set = intensity;

		c_log_add_record_with_cmd(log_ref, "Nastavena intenzita ventilátoru: %d%%", intensity);

		controler_update_config(CFG_GROUP_PRINT_PARAMS,
				CFG_PP_FAN_INTENSITY,
				CONFIG_TYPE_INT,
				&fan_intensity_set,
				"Nastavení intenzity ventilátoru bylo úspěšně aktualizováno.",
				"Nepodařilo se aktualizovat intenzitu ventilázoru!");

	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "Nelze nastavit intenzitu ventilázoru: %d", intensity);
	}
}


void controler_tab_insert_set_sequence(int32_t sequence)
{
	tab_insert_sequence = sequence;

	c_log_add_record_with_cmd(log_ref, "Nastaveno nastřelení oddělovacího proužku každých %d archů.", sequence);

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_TAB_INSERT_SEQUENCE,
			CONFIG_TYPE_INT,
			&tab_insert_sequence,
			"Nastavení sekvence pro separování archů oddělovacím proužkem aktualizováno.",
			"Nepodařilo se aktualizovat sekcenci pro separování oddělovacím proužkem!");
}

int32_t controler_tab_insert_get_sequence()
{
	return tab_insert_sequence;
}

void controler_tab_insert_set_automat(bool automat)
{
	tab_insert_automat = automat;
	stacked_for_ti = 0;

	c_log_add_record_with_cmd(log_ref, "Automatické nastřelovnání oddělovacího proužku nastaveno na: %s", ((tab_insert_automat == true) ? "TRUE" : "FALSE"));

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_TAB_INSERT_AUTOMAT,
			CONFIG_TYPE_BOOL,
			&tab_insert_automat,
			"Nastavení automatického nastřelování oddělovacího proužku aktualizováno.",
			"Nepodařilo se aktualizovat nastavení pro automatické nastřelování oddělovacího proužku!");
}

bool controler_tab_insert_get_automat()
{
	return tab_insert_automat;
}


void controler_tab_insert_set_cut_delay(int32_t sheets)
{
	ti_delay = sheets;

	c_log_add_record_with_cmd(log_ref, "Nastavena doba pro opoždění střihu nastřelovacího proužku, %d", sheets);

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_TAB_INSERT_CUT_DELAY,
			CONFIG_TYPE_INT,
			&ti_delay,
			"Nastavení opoždění střihu nastřelovacího proužku aktualizováno.",
			"Nepodařilo se aktualizovat opoždění střihu nastřelovacího proužku!");
}

int32_t controler_tab_insert_get_cut_delay()
{
	return ti_delay;
}

void controler_tab_inset_set_length(int32_t length)
{
	tab_insert_len = length;

	c_log_add_record_with_cmd(log_ref, "Nastavena doba pro nastřelování oddělovacího proužku na %d ms.", length);

	controler_update_config(CFG_GROUP_PRINT_PARAMS,
			CFG_PP_TAB_INSERT_LENGTH,
			CONFIG_TYPE_INT,
			&tab_insert_len,
			"Nastavení doby pro nastřelování oddělovacího proužku aktualizováno.",
			"Nepodařilo se aktualizovat dobu pro nastřelování oddělovacího proužku!");
}

int32_t controler_tab_insert_get_length()
{
	return tab_insert_len;
}

void controler_tab_insert_manual_insert()
{
	if(machine_state == MACHINE_STATE_WAIT)
		tab_insert_req = 1;
}

int controler_get_fan_intensity()
{
	return fan_intensity_set;
}

uint8_t controler_get_fan_activity()
{
	return fan_activity;
}

void controler_set_fan_activity(uint8_t activity)
{
	fan_activity = activity;
}

void controler_save_statistics(int8_t day)
{
	config_setting_t * root, *group, *sub_group, *settings;

	root = config_root_setting(&(cfg_ref));
	group = config_setting_get_member(root, CFG_GROUP_STATISTICS);

	if(config_setting_remove(group, CFG_STATISTICS_DAY_PRE) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, CFG_STATISTICS_DAY_PRE, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_day_pre(statistics_ref));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Uložen index dne pro statistiku: %d", machine_statistic_get_day_pre(statistics_ref));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit index dne pro statistiku: %s", config_error_text(&(cfg_ref)));
	}

	if(config_setting_remove(group, CFG_STATISTICS_TOTAL_FEEDED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_total_feeded_sheets(statistics_ref));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika všech naložených archů uložena: %d arch(ů)", machine_statistic_get_total_feeded_sheets(statistics_ref));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky všech naložených archů: %s", config_error_text(&(cfg_ref)));
	}

	if(config_setting_remove(group, CFG_STATISTICS_TOTAL_STACKED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_total_stacked_sheets(statistics_ref));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika všech naložených archů uložena: %d arch(ů)", machine_statistic_get_total_stacked_sheets(statistics_ref));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky všech vyložených archů: %s", config_error_text(&(cfg_ref)));
	}

	if(config_setting_remove(group, CFG_STATISTICS_TOTAL_REJECTED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_total_rejected_sheets(statistics_ref));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika všech vyhozených archů uložena: %d arch(ů)", machine_statistic_get_total_rejected_sheets(statistics_ref));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky všech vyhozených archů: %s", config_error_text(&(cfg_ref)));
	}


	if(config_setting_remove(group, CFG_STATISTICS_ERROR_RATE) == CONFIG_TRUE)
	{
		settings = config_setting_add(group, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
		config_setting_set_float(settings, machine_statistic_get_total_error_rate(statistics_ref));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika celkové chybovosti uložena: %f%", machine_statistic_get_total_error_rate(statistics_ref));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky celkové chybovosti: %s", config_error_text(&(cfg_ref)));
	}


	if(day == 0)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_SUN);
	else if(day == 1)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_MON);
	else if(day == 2)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_TUE);
	else if(day == 3)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_WED);
	else if(day == 4)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_THU);
	else if(day == 5)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_FRI);
	else if(day == 6)
		sub_group = config_setting_get_member(group, CFG_GROUP_STATISTICS_SAT);
	else
		return;

	if(config_setting_remove(sub_group, CFG_STATISTICS_TOTAL_FEEDED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(sub_group, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_feeded_sheets(statistics_ref, day));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika naložených archů uložena: %d arch(ů)", machine_statistic_get_feeded_sheets(statistics_ref, day));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky naložených archů: %s", config_error_text(&(cfg_ref)));
	}

	if(config_setting_remove(sub_group, CFG_STATISTICS_TOTAL_STACKED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(sub_group, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_stacked_sheets(statistics_ref, day));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika vyložených archů uložena: %d arch(ů)", machine_statistic_get_stacked_sheets(statistics_ref, day));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky vyložených archů: %s", config_error_text(&(cfg_ref)));
	}


	if(config_setting_remove(sub_group, CFG_STATISTICS_TOTAL_REJECTED_SHEETS) == CONFIG_TRUE)
	{
		settings = config_setting_add(sub_group, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
		config_setting_set_int(settings, machine_statistic_get_rejected_sheets(statistics_ref, day));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika vyhozených archů uložena: %d arch(ů)", machine_statistic_get_rejected_sheets(statistics_ref, day));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky vyhozených archů: %s", config_error_text(&(cfg_ref)));
	}


	if(config_setting_remove(sub_group, CFG_STATISTICS_ERROR_RATE) == CONFIG_TRUE)
	{
		settings = config_setting_add(sub_group, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
		config_setting_set_float(settings, machine_statistic_get_error_rate(statistics_ref, day));

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, "Statistika chybovosti uložena: %f%", machine_statistic_get_error_rate(statistics_ref, day));
	}
	else
	{
		c_log_add_record_with_cmd(log_ref,"Nelze uložit hodnoty statistiky chybovosti: %s", config_error_text(&(cfg_ref)));
	}
}



void controler_record_feeding_time()
{
	if(((stacked_sheet_counter_in_job > 0) && (sn_trig != stacker_status) && (stacker_status == MACHINE_SN_STACKING)))
	{
		time_for_nth_sheet[nth_sheet_time_index] = c_freq_millis() - last_feeded_sheet_millis;
		nth_sheet_time_index = (nth_sheet_time_index +1) % 4;

		last_feeded_sheet_millis = c_freq_millis();
	}
}


uint64_t controler_get_time_for_one_sheet()
{
	uint64_t dia_time = 0;

	if((stacked_sheet_counter > 0))
	{
		for(int i = 0; i < 4; i++)
		{
			dia_time += time_for_nth_sheet[i];
		}

		if(stacked_sheet_counter > 3)
			return (dia_time/4);
		else
			return dia_time/(stacked_sheet_counter);
	}
	else
	{
		return 0;
	}
}

int8_t controler_machine_slow_down()
{
	int x = 20;
	int8_t slmx = (stacked_sheet < (max_stacked_sheets - x));
	int8_t sgemx = (stacked_sheet >= (max_stacked_sheets - x));
	int8_t sgem = !(stacked_sheet >= max_stacked_sheets);
	int8_t fes = (feeded_sheet == stacked_sheet+rejected_sheet);

	return ((!slmx) && sgemx && sgem && fes) || (slmx && (!sgemx) && sgem);
}

void controler_machine_state_read_csv_line()
{
	/* load line from csv */
	q_job * job = controler_get_printed_job();

	/* if is set pause request, move to pause state after feed current sheet */
	if((machine_pause_req == true))
	{
		controler_set_machine_state(MACHINE_STATE_PAUSE);
	}
	else if(machine_cancel_req == true)
	{
		controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
	}
	else if((job != NULL) && ((fake_companion_req == true) || (companion_faked == true)) && (q_job_get_flag(job) != 'p'))
	{
		controler_set_machine_state(MACHINE_STATE_FAKE_COMPANION);
	}
	else
	{
		if(feeder_status == MACHINE_FN_READY_TO_FEED)
		{
			if(c_timer_delay(feeding_rate, feed_delay) > 0)
			{
				if(controler_machine_slow_down() > 0)
				{
					if(job != NULL)
					{
						/* process csv line */
						if(bkcore_csv_pos < job_info_get_job_sheet_number(info, job_info_get_job_index(info)))
						{
							c_timer_reset(print_timer);

							if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) == 0)
								job_info_set_sheet_record_result(info, "PASS", bkcore_csv_pos);

							if((q_job_get_flag(job) == 'k') || (((q_job_get_flag(job) == 'p') || (merged_companion_sheets > 0)) &&
								(sheet_source_confirmation == false) && (companion_sheet_source == SSOURCE_MAIN)))
							{
								controler_set_machine_state(MACHINE_STATE_PRINT_MAIN);
								printf("go to print main\n");
							}
							else if(((q_job_get_flag(job) == 'p') || (merged_companion_sheets > 0))  && (companion_sheet_source == SSOURCE_COMPANION))
							{
								printf("go to print compation\n");
								controler_set_machine_state(MACHINE_STATE_PRINT_COMPANION);
							}
							else if((q_job_get_flag(job) == 'p') && (companion_sheet_source == SSOURCE_MAIN) && (sheet_source_confirmation == true))
							{
								printf("go to wait for confirmation\n");
								controler_set_machine_state(MACHINE_STATE_WAIT_FOR_CONFIRMATION);
							}
							else
							{
								controler_set_machine_error(MACHINE_ERR_SHEET_FEEDER_REDIRECTION);
							}

							c_log_add_record_with_cmd(log_ref, "naložené archy: %d | pozice v csv souboru: %d | vyložené archy: %d | vyhozené archy: %d",
											feeded_sheet_counter, bkcore_csv_pos, stacked_sheet_counter, rejected_sheet_counter);

							bkcore_csv_pos++;
						}
						else
						{
							controler_set_machine_state(MACHINE_STATE_WAIT_FOR_PRINT_FINISH);
							delay_on_end = c_freq_millis();
						}
					}
					else
					{
						controler_set_machine_error(MACHINE_ERR_UNKNOWN_ERROR);
					}
				}
				else if(stacked_sheet >= max_stacked_sheets)
				{
					controler_set_machine_state(MACHINE_STATE_PAUSE);
					machine_pause_req = true;
				}
			}
		}
		else
		{
			timer = c_freq_millis();
		}
	}
}

void controler_machine_state_wait_for_confirmation()
{
	if((print_confirmation_req == true) && (feeder_status == MACHINE_FN_READY_TO_FEED) && (stacker_status == MACHINE_SN_STACKER_READY_TO_STACK))
	{
		controler_set_machine_state(MACHINE_STATE_PRINT_MAIN);
		print_confirmation_req = false;
	}
}


void controler_machine_state_stacker_error()
{
	if((feeder_status == MACHINE_FN_READY_TO_FEED) &&  (stacker_status == MACHINE_SN_STACKER_READY_TO_STACK))
	{
		if((timer + 5000) <= c_freq_millis())
		{
			controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);

			c_log_add_record_with_cmd(log_ref, "Změna pozice v bkcore csv souboru po chybě nakládání z %d na %d", bkcore_csv_pos, feeded_sheet_counter_in_job);

			printf("old: %d\n", bkcore_csv_pos);
			bkcore_csv_pos = feeded_sheet_counter_in_job;
			printf("new: %d\n", bkcore_csv_pos);

			if((error_code == MACHINE_ERR_STACKER_MULTIFUNCTION) || (error_code == MACHINE_ERR_STACKER_BOTTOM) ||
				(error_code == MACHINE_ERR_STACKER_JAM_CONVEYOR) || (error_code == MACHINE_ERR_STACKER_JAM_PILE))
			{
				error_code = MACHINE_ERR_NO_ERROR;
			}
		}
	}
	else
	{
		if(c_timer_delay(switch_off_ena_in_feeder_error, 120000) > 0)
		{
			machine_pause_req = true;
			controler_set_machine_state(MACHINE_STATE_PAUSE);
		}

		timer = c_freq_millis();
	}
}


void controler_machine_state_feeder_error()
{
	if(feeder_status == MACHINE_FN_READY_TO_FEED && (stacker_status == MACHINE_SN_STACKER_READY_TO_STACK))
	{
		if((timer + 5000) <= c_freq_millis())
		{
			c_log_add_record_with_cmd(log_ref, "Změna pozice v bkcore csv souboru po chybě nakládání z %d na %d", bkcore_csv_pos, stacked_sheet_counter_in_job+rejected_sheet_counter_in_job);

			printf("old: %d\n", bkcore_csv_pos);
			bkcore_csv_pos = stacked_sheet_counter_in_job+rejected_sheet_counter_in_job;
			feeded_sheet_counter_in_job = bkcore_csv_pos;
			printf("new: %d\n", bkcore_csv_pos);

			if((error_code == MACHINE_ERR_FEEDER_SHEET_MISSING) || (error_code == MACHINE_ERR_FEEDER_DOUBLE_SHEET) || (error_code == MACHINE_ERR_FEEDER_MULTIFUNCTION))
				error_code = MACHINE_ERR_NO_ERROR;

			controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);

			c_timer_reset(switch_off_ena_in_feeder_error);

			printf("%d: %d -> %d\n",feeder_status, bkcore_csv_pos, main_sheet_feed_request_counter);
		}
	}
	else
	{
		if(c_timer_delay(switch_off_ena_in_feeder_error, 120000) > 0)
		{
			machine_pause_req = true;
			controler_set_machine_state(MACHINE_STATE_PAUSE);
		}

		timer = c_freq_millis();
	}
}

void controler_machine_state_print_main()
{
	if(c_timer_delay(print_timer, 10000) > 0)
	{
		error_code = MACHINE_ERR_PRINT_MAIN_FREEZE;
		controler_set_machine_state(MACHINE_STATE_PAUSE);
		c_log_add_record_with_cmd(log_ref,
					"Došlo k překročení času na naložení kolkového archu -> krok: %d, status nakladače %d, index naloženého archu: %d",
					feed_sheet, feeder_status, feeded_sheet_counter_in_job);
		machine_pause_req = true;
	}

	/* wait for feeder ready */
	if((feed_sheet == 0) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		feed_sheet = 1;
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 1);
		timer = c_freq_millis();
	}
	else if(feed_sheet == 1)
	{
		/* feed one sheet */
		if(((timer+MACHINE_XBF_INTERVAL) <= c_freq_millis()))
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			feed_sheet = 2;
		}

		if((feeder_status == MACHINE_FN_FEEDING))
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			feed_sheet = 3;
			timer = c_freq_millis();
		}
	}
	else if(feed_sheet == 2)
	{
		if(feeder_status == MACHINE_FN_FEEDING)
		{
			feed_sheet = 3;
			timer = c_freq_millis();
		}

		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
	}
	else if((feed_sheet == 3) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		//if(timer + 600 < c_freq_millis())
		{
			feed_sheet = 0;
			main_sheet_feed_request_counter++;
			controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);

			c_timer_reset(print_timer);
		}

		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
	}

	/* feedeng error handling */
	controler_feeder_error_handler(feeder_status);
}

void controler_machine_state_print_companion()
{
	if(c_timer_delay(print_timer, 20000) > 0)
	{
		error_code = MACHINE_ERR_PRINT_COMPANION_FREEZE;

		controler_set_machine_state(MACHINE_STATE_PAUSE);

		c_log_add_record_with_cmd(log_ref, "Došlo k překročení času na naložení prokladového archu, krok: %d", feed_sheet);
		machine_pause_req = true;
	}

	/* check the state of companion feeder, TA_BF must be in false state */
	if((feed_sheet == 0) && (io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF) == 0))
	{
		/* set output TA_cyc to true state */
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TA_cyc, 1);
		timer = c_freq_millis();
		feed_sheet = 1;
	}
	else if(feed_sheet == 1)
	{
		if((timer + 3000) < c_freq_millis())
		{
			feed_sheet = 2;
			timer = c_freq_millis();
		}
	}
	else if(feed_sheet == 2)
	{
		/* wait for TA_BF true state or given time */
		if(((timer+MACHINE_XBF_INTERVAL) <= c_freq_millis()))
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TA_cyc, 0);
			feed_sheet = 3;
		}

		if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF) > 0)
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TA_cyc, 0);
			timer = c_freq_millis();
			feed_sheet = 4;
		}
	}
	else if(feed_sheet == 3)
	{
		/* wait for TA_BF true state */
		if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF) > 0)
		{
			feed_sheet = 4;
			timer = c_freq_millis();
		}
	}
	else if((feed_sheet == 4) && (io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF) == 0))
	{
		/* wait 10s for sure, that the companion sheet was successfuly feeded (response 2Hz flashing) */
		if(((timer+10000) <= c_freq_millis()) || ((io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_2_FEEDING_SENSOR) > 0) && ((timer+6000) <= c_freq_millis())))
		{
			/* wait for TA_BF true state, incremet the counter state and go to READ_CSV_LINE state */
			feed_sheet = 0;

			controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
			c_timer_reset(print_timer);
		}
	}
}



c_csv_struct * controler_analyze_csv(c_csv_struct * q_csv, c_csv_struct * c_csv)
{
        if((q_csv != NULL)  && (c_csv != NULL))
        {

                c_csv_struct * resp_csv = c_csv_new(',', '\n');

                if(resp_csv != NULL)
                {
                        c_csv_set_line_focuse(q_csv, 0);
                        c_csv_set_line_focuse(c_csv, 0);

                        while((c_csv_set_next_line(q_csv) != C_CSV_ERROR))
                        {
                                c_csv_set_next_line(c_csv);

                                c_string * q_ceel = NULL;
                                c_string * c_ceel = NULL;

                                while((q_ceel = c_csv_get_next_ceel_in_line(q_csv)) != NULL)
                                {
                                        if((c_ceel = c_csv_get_next_ceel_in_line(c_csv)) != NULL)
                                        {
                                                if(c_csv_get_focused_ceel_index_in_line(q_csv) > 1)
                                                {
                                                        if(strcmp(c_string_get_char_array(q_ceel), c_string_get_char_array(c_ceel)) != 0)
                                                        {
                                                                printf("%s <> %s\n", c_string_get_char_array(q_ceel), c_string_get_char_array(c_ceel));

                                                                c_csv_add_line(resp_csv);

								c_csv_add_ceel_to_line_with_init(resp_csv,
                                                                                  c_csv_get_line_number(resp_csv)-1,
                                                                                  c_string_get_char_array(c_csv_get_ceel_content_from_focused_line(q_csv, 0)));

								job_info_set_sheet_record_result(info, "FAIL", c_csv_get_focused_line_index(q_csv)-1);
                                                                break;
                                                        }
							else
							{
								job_info_set_sheet_record_result(info, "PASS", c_csv_get_focused_line_index(q_csv)-1);
							}
                                                }
                                        }
                                        else
                                        {
                                               char * str = c_string_get_char_array(q_ceel);
                                               printf("%s -> NULL\n", str);
                                               c_csv_add_line(resp_csv);

					       c_csv_add_ceel_to_line_with_init(resp_csv,
                                                                                c_csv_get_line_number(resp_csv)-1,
                                                                                c_string_get_char_array(c_csv_get_ceel_content_from_focused_line(q_csv, 0)));

						job_info_set_sheet_record_result(info, "FAIL", c_csv_get_focused_line_index(q_csv)-1);

                                               break;
                                        }
                                }
                        }

                        return resp_csv;
                }
        }

	return NULL;
}



c_string * controler_compare_csv(q_job * job, int * csv_lines)
{
	/* end job in Job */
	c_string * address = c_string_new_with_init(c_string_get_char_array(q_hotfolder_main_path));
	c_string_concat(address, "/");
	c_string_concat(address, q_job_get_camera_csv_name(job));

	c_csv_struct * q_csv = c_csv_parse_csv_from_file(c_string_get_char_array(address), ',', '\n');

	c_string_set_string(address, c_string_get_char_array(pci_hotfolder_out_path));
	c_string_concat(address, "/");
	c_string_concat(address, q_job_get_camera_csv_name(job));


	c_csv_struct * c_csv = c_csv_parse_csv_from_file(c_string_get_char_array(address), ',', '\n');

	c_string_finalize_v3(address);

	if((c_csv != NULL) && (q_csv != NULL))
	{
		c_csv_struct * q_feedback_csv = controler_analyze_csv(q_csv, c_csv);

		c_csv_finalize(c_csv);
		c_csv_finalize(q_csv);

		if(q_feedback_csv != NULL)
		{
			c_string * q_feedback_csv_content = c_csv_get_content(q_feedback_csv);

			c_log_add_record_with_cmd(log_ref, "Analýza csv souboru z Quadientu  a kamerového systému, počet vadných archů: %d", c_csv_get_line_number(q_feedback_csv));

			if(csv_lines != NULL)
				*csv_lines = c_csv_get_line_number(q_feedback_csv);

			free(q_feedback_csv);

			if(q_feedback_csv_content != NULL)
			{

				return q_feedback_csv_content;

			}
		}
	}

	return NULL;
}

void controler_machine_state_save_q_csv()
{
	if((timer+TIME_DELAY_ON_JOB_END) <= c_freq_millis())
	{
		q_job * job = controler_get_printed_job();

		if(job != NULL)
		{
			/* if the camera is disabled don't do the camera and quadient csv analyzing */
			if((io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0))
			{
				int line_number = 0;
				c_string * q_feedback_csv_content = controler_compare_csv(job, &line_number);

				if(q_feedback_csv_content != NULL)
				{
					if(line_number == rejected_sheet_counter_in_job)
					{
						util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path),
								q_job_get_bkcore_csv_name(job),
								c_string_get_char_array(q_feedback_csv_content));

						c_log_add_record_with_cmd(log_ref, "Uložení zpětnovazebního csv na adresu: %s/%s",
										c_string_get_char_array(q_hotfolder_feedback_path),
										q_job_get_bkcore_csv_name(job));
/*
						util_save_csv("/home/stc",
								q_job_get_bkcore_csv_name(job),
								c_string_get_char_array(q_feedback_csv_content));
*/

						c_string_finalize_v3(q_feedback_csv_content);

						controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);
					}
					else
					{
						c_log_add_record_with_cmd(log_ref,
							"Nesoulad vyhozených archů a náhrad z kamerového csv: %d x %d, obsah zpětnovazebního csv:\n%s",
							rejected_sheet_counter_in_job, line_number, c_string_get_char_array(q_feedback_csv_content));


						//controler_set_machine_error(MACHINE_ERR_COUNTERS_MISMATCH);



						/* code for mask the error of the counters mis match */
						if(line_number < rejected_sheet_counter_in_job)
						{
							int line_index = 0;
							int sheet_index = 0;
							int missing_sheets = rejected_sheet_counter_in_job - line_number;
							int sheet_order = 0;
							int last_position = 0;

							for(int i = 0; i < c_string_len(q_feedback_csv_content); i++)
							{
								if(c_string_get_char(q_feedback_csv_content, i) == '\n')
								{
									sheet_order ++;

									if(missing_sheets > 0)
									{
										if(line_index > 0)
										{
											if(sheet_order != sheet_index)
											{
												char str_index[8];
												sprintf(str_index, "%d\n", sheet_order);
												c_string_insert_substring(q_feedback_csv_content, str_index, last_position);
												missing_sheets --;
											}
										}
										else
										{
											sheet_order = sheet_index;
										}
									}

									if(missing_sheets == 0)
										break;

									last_position = i+1;
									line_index++;
									sheet_index = 0;
								}
								else if(isdigit(c_string_get_char(q_feedback_csv_content, i)))
								{
									sheet_index = (sheet_index *10) + (c_string_get_char(q_feedback_csv_content,i)-48);
								}
							}
						}

						util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path),
								q_job_get_bkcore_csv_name(job),
								c_string_get_char_array(q_feedback_csv_content));

						error_code = MACHINE_ERR_COUNTERS_MISMATCH;
						controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);


						/*
						machine_state = MACHINE_STATE_PAUSE;
						machine_pause_req = true;
						*/

				}	}
				else
				{
					controler_set_machine_error(MACHINE_ERR_ANALYZE_CAMERA_CSV);
				}
			}
			else
			{
				/* save empty response csv */
				util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");

				c_log_add_record_with_cmd(log_ref, "Uložení prázdného zpětnovazebního csv (bez kamerové kontroly) na adresu: %s/%s",
								c_string_get_char_array(q_hotfolder_feedback_path),
								q_job_get_bkcore_csv_name(job));

				controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);
			}
		}
		else
		{
			controler_set_machine_error(MACHINE_ERR_CANT_SAVE_F_CSV);
		}
	}
}

void controler_machine_state_ready_to_start()
{
	if(c_timer_delay(print_timer, 60000) > 0)
	{
		c_log_add_record_with_cmd(log_ref, "Nelze spustit tisk kvůli nesplněným podmínkám - feeder: %d, stacker: %d",
						feeder_status, stacker_status);
		machine_pause_req = true;
		controler_set_machine_state(MACHINE_STATE_PAUSE);
		error_code = MCAHINE_ERR_PRINT_INITIALIZATION_FREEZE;
	}

	if((feeder_status == MACHINE_FN_READY_TO_FEED) && stacker_status == MACHINE_SN_STACKER_READY_TO_STACK)
	{
		//q_job * job = array_list_get(job_list, printed_job_index);

		if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
		{
//|| (((timer + 10000) < c_freq_millis()) && (q_job_get_flag(job) == 'p'))
			if((((timer + 5000) < c_freq_millis())) &&
				(((((strcmp(c_string_get_char_array(print_controler_status), "Printing") == 0) || (companion_faked == true)) &&
				(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)) ||
				io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) == 0)))
			{
				c_timer_reset(print_timer);
				controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
			}
		}
		else if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
		{
			if(((strcmp(c_string_get_char_array(print_controler_status), "Printing") == 0) || companion_faked == true) && ((timer + 3000) < c_freq_millis()))
			{
				c_timer_reset(print_timer);
				controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
			}
		}
		else
		{
			if((timer + 5000) < c_freq_millis())
			{
				c_timer_reset(print_timer);
				controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
			}
		}
	}
	else
	{
		timer = c_freq_millis();
	}
}

void controler_machine_state_wait_for_print_finish()
{
	if(c_timer_delay(print_timer, 60000) > 0)
	{
		c_log_add_record_with_cmd(log_ref, "Došlo k překročení času na vyložení archů -> počítadlo naložených archů: %d,\
					počítadlo vyložených archů: %d, počítadlo vyhozených archů: %d, počet archů v jobu: %d",
					feeded_sheet_counter_in_job, stacked_sheet_counter_in_job, rejected_sheet_counter_in_job,
					job_info_get_job_sheet_number(info, job_info_get_job_index(info)));

		controler_set_machine_error(MACHINE_ERR_PRINT_FINALIZING_FREEZE);
	}

	if(((delay_on_end+25000) < c_freq_millis()) || (feeded_sheet_counter_in_job == (stacked_sheet_counter_in_job + rejected_sheet_counter_in_job) &&
			(delay_on_end+5000 < c_freq_millis())))
	{
		if((feeded_sheet_counter_in_job == job_info_get_job_sheet_number(info, job_info_get_job_index(info))) &&
			(feeded_sheet_counter_in_job == (stacked_sheet_counter_in_job + rejected_sheet_counter_in_job)))
		{
			timer = c_freq_millis();

			if(merged_companion_sheets == 0)
			{
				controler_set_machine_state(MACHINE_STATE_SAVE_Q_CSV);
				c_timer_reset(print_timer);
			}
			else
			{
				controler_set_machine_state(MACHINE_STATE_PRINT_FINISH);
			}
		}
		else
		{
			if(feeded_sheet_counter_in_job < job_info_get_job_sheet_number(info, job_info_get_job_index(info)))
			{
				controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
				printf("sheet differece in job: %d\n", job_info_get_job_sheet_number(info, job_info_get_job_index(info)) - feeded_sheet_counter_in_job);

				c_log_add_record_with_cmd(log_ref, "Nesoulad počítadel na konci jobu -> naložené arhy: %d, počet archů v jobu: %d, dopočet: %d", feeded_sheet_counter_in_job,
							job_info_get_job_sheet_number(info, job_info_get_job_index(info)),
							job_info_get_job_sheet_number(info, job_info_get_job_index(info)) - feeded_sheet_counter_in_job);
				bkcore_csv_pos = feeded_sheet_counter_in_job;
			}
			else if((feeded_sheet_counter_in_job > (stacked_sheet_counter_in_job + rejected_sheet_counter_in_job)))
			{
				int difference = (feeded_sheet_counter_in_job - (stacked_sheet_counter_in_job + rejected_sheet_counter_in_job));
				printf("sheet difference: %d\n", difference);
				bkcore_csv_pos = feeded_sheet_counter_in_job - difference;

				c_log_add_record_with_cmd(log_ref, "Nesoulad počítadel na konci jobu -> naložené arhy: %d, vyložené archy: %d, vyhozené archy: %d, přebývající: %d",
					feeded_sheet_counter_in_job, stacked_sheet_counter_in_job, rejected_sheet_counter_in_job, difference);

				controler_set_machine_state(MACHINE_STATE_READ_CSV_LINE);
			}
			else
			{
				c_log_add_record_with_cmd(log_ref, "Nesoulad počítadel na konci jobu -> naložené archy: %d, počet archů v jobu: %d, vyložené archy: %d, vyhozené archy: %d",
						 	feeded_sheet_counter_in_job, job_info_get_job_sheet_number(info, job_info_get_job_index(info)),
							stacked_sheet_counter_in_job, rejected_sheet_counter_in_job);
				controler_set_machine_error(MACHINE_ERR_COUNTERS_MISMATCH);
			}
		}
	}
}


void coontroler_machine_state_companion_generation_delay()
{
	if((companion_generation_delay + 1500) < c_freq_millis())
		controler_set_machine_state(MACHINE_STATE_LOAD_NEXT_JOB_FILE);
}

void controler_machine_state_copy_companion_pdf_file()
{
	q_job * job = controler_get_printed_job();

	if(job != NULL)
	{
		util_move_file(c_string_get_char_array(q_hotfolder_main_path), MERGE_DIRECTORY_ADDRESS, q_job_get_pdf_name(job));
		util_delete_file(c_string_get_char_array(q_hotfolder_main_path), q_job_get_bkcore_csv_name(job));
		util_move_file(c_string_get_char_array(q_hotfolder_main_path), MERGE_DIRECTORY_ADDRESS, q_job_get_camera_csv_name(job));

		util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");

		merged_companion_sheets++;

		char merged_companion_sheet_index_str[6];
		sprintf(merged_companion_sheet_index_str, "%d", merged_companion_sheets);
		job_info_add_sheet_record(info, merged_companion_sheet_index_str);
		printf("merged sheets: %d\n", merged_companion_sheets);

		array_list_clear(job_list);
		companion_generation_delay = c_freq_millis();
		controler_set_machine_state(MACHINE_STATE_COMPANION_GENERATION_DELAY);
	}
	else
	{
		controler_set_machine_error(MACHINE_ERR_UNKNOWN_ERROR);
	}
}

void controler_machine_state_delete_stamp_pdf_file()
{
	q_job * job = controler_get_printed_job();

	if(job != NULL)
	{
		util_delete_file(c_string_get_char_array(q_hotfolder_main_path), q_job_get_pdf_name(job));
		util_delete_file(c_string_get_char_array(q_hotfolder_main_path), q_job_get_bkcore_csv_name(job));
		util_delete_file(c_string_get_char_array(q_hotfolder_main_path), q_job_get_camera_csv_name(job));

		util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");

		array_list_clear(job_list);

		companion_generation_delay = c_freq_millis();
		controler_set_machine_state(MACHINE_STATE_COMPANION_GENERATION_DELAY);
	}
	else
	{
		controler_set_machine_error(MACHINE_ERR_UNKNOWN_ERROR);
	}
}

void controler_machine_state_merge_companion_pdf_files()
{
	char system_command_pdfunite[255];
	sprintf(system_command_pdfunite,"pdfunite %s/*pdf %s/%s", MERGE_DIRECTORY_ADDRESS, MERGE_DIRECTORY_ADDRESS, MERGED_PDF_FILE);
	system(system_command_pdfunite);

	char system_command_merge_csv[255];
	sprintf(system_command_merge_csv, "sed -i -e '$a\\' %s/*.csv && cat %s/*csv >> %s/%s", MERGE_DIRECTORY_ADDRESS, MERGE_DIRECTORY_ADDRESS, MERGE_DIRECTORY_ADDRESS, MERGED_CSV_FILE);
	system(system_command_merge_csv);

	c_timer_reset(print_timer);

	controler_set_machine_state(MACHINE_STATE_SEND_COMPANION_DATA);
}

void controler_machine_state_send_response_to_ia()
{
	q_job * job = controler_get_printed_job();

	if(job != NULL)
	{
		util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");
		util_delete_file(c_string_get_char_array(q_hotfolder_main_path), q_job_get_bkcore_csv_name(job));


		job_info_set_total_sheet_number(info, merged_companion_sheets);
		job_info_set_total_stamp_number(info, 0);

		controler_set_machine_state(MACHINE_STATE_MERGE_COMPANION_PDF_FILES);
	}
	else
	{
		controler_set_machine_error(MACHINE_ERR_UNKNOWN_ERROR);
	}
}


void controler_machine_state_send_companion_data()
{
	if(c_timer_delay(print_timer, 1000) > 0)
	{
		if (util_copy_file(MERGE_DIRECTORY_ADDRESS, c_string_get_char_array(gis_hotfolder_path), MERGED_PDF_FILE) == 0 &&
			util_copy_file(MERGE_DIRECTORY_ADDRESS, c_string_get_char_array(pci_hotfolder_in_path), MERGED_CSV_FILE) == 0)
		{
			c_timer_reset(print_timer);
			controler_set_machine_state(MACHINE_STATE_READY_TO_START);
		}
		else
		{
			controler_set_machine_error(MACHINE_ERR_CANT_COPY_PDF_TO_GIS);
		}
	}
}

void controler_clear_merge_directory()
{
	util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), MERGED_CSV_FILE);

	char system_command[255];
	sprintf(system_command, "rm %s/*", MERGE_DIRECTORY_ADDRESS);
	system(system_command);

	merged_companion_sheets = 0;
}

void controler_machine_state_finish_print_companion_pdf()
{
	util_move_file(MERGE_DIRECTORY_ADDRESS, c_string_get_char_array(q_hotfolder_backup_path), MERGED_PDF_FILE);
	util_move_file(MERGE_DIRECTORY_ADDRESS, c_string_get_char_array(q_hotfolder_backup_path), MERGED_CSV_FILE);

	controler_clear_merge_directory();
}

void controler_machine_state_fake_companion()
{
	if(fake_companion_req == true)
	{
		q_job * job = controler_get_printed_job();

		if(job != NULL)
		{
			if(q_job_get_flag(job) == 'k')
			{
				if(print_finishing_step  == 0)
				{
					if(strcmp(c_string_get_char_array(print_controler_status), "Ready") != 0)
						print_finishing_step = 1;
					else
						print_finishing_step = 3;
					printf("Recognize state of print breaking\n");
				}
				else if(print_finishing_step == 1)
				{
					controler_printer_abbort_print(0);
					printf("Send P,A\n");
					timer = c_freq_millis();
					print_finishing_step = 2;
				}
				else if(print_finishing_step == 2)
				{
					if((timer + 500) <= c_freq_millis())
					{
						printf("Send R,A\n");
						controler_printer_abbort_print(1);
						print_finishing_step = 3;
					}
				}
				else
				{
					if(strcmp(c_string_get_char_array(print_controler_status), "Ready") == 0)
					{
						fake_companion_req = false;
						companion_faked = true;

						if(q_job_get_flag(job) == 'k')
						{
							util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");

							c_log_add_record_with_cmd(log_ref, "Uložení prázdného zpětnovazebního csv (samostatné vytištění prokladu) na adresu: %s/%s",
											c_string_get_char_array(q_hotfolder_feedback_path),
											q_job_get_bkcore_csv_name(job));
						}

						print_finishing_step = 0;
						controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);
					}
				}
			}
			else if(q_job_get_flag(job) == 'p')
			{
				bkcore_csv_pos = 0;
				c_log_add_record_with_cmd(log_ref, "Opakování tisku prokladového archu.");
				controler_set_machine_state(MACHINE_STATE_PREPARE);
			}
			else
			{
				controler_set_machine_state(MACHINE_STATE_CLEAR_HOT_FOLDER);
			}
		}
	}
}


uint8_t controler_check_print_controler_status(char * print_controler_status)
{
	if(strcmp(print_controler_status, "Ready") != 0)
		return 1;
	else
		return 3;
}

uint8_t controler_print_abbort_sequence()
{
	controler_printer_abbort_print(0);
	return 2;
}

uint8_t controler_ripping_abbort_sequence()
{
	controler_printer_abbort_print(1);
	return 3;
}

q_job * controler_get_printed_job()
{
	q_job * job = NULL;

	if((printed_job_index >= 0) && (job_list != NULL))
		job = array_list_get(job_list, printed_job_index);

	return job;
}

char * controler_build_end_csv_file_name(char * job_name, int job_order)
{
	char * const_char = "_xxxx_e_bkcore.csv";
	char * csv_name = (char*) malloc(sizeof(char)*(strlen(job_name)+strlen(const_char)+1));

	sprintf(csv_name, "%s_%04d_e_bkcore.csv", job_name, job_order);

	return csv_name;
}

void controler_save_ending_response_csv_file(char * q_feedback_hotfolder_path, char * job_name, int job_order)
{
	char * csv_name = controler_build_end_csv_file_name(job_name, job_order);
	util_save_csv(q_feedback_hotfolder_path, csv_name, "");

	c_log_add_record_with_cmd(log_ref, "Uložení prázdného zpětnovazebního csv pro ukončení jobu, na adresu: %s/%s",
				q_feedback_hotfolder_path,
				csv_name);
	free(csv_name);
}

bool controler_is_conveyor_empty()
{
	return (feeded_sheet_counter_in_job == (stacked_sheet_counter + rejected_sheet_counter));
}

void controler_make_csv_analyze_after_print_break(q_job * job)
{
	c_string * q_feedback_csv_content = controler_compare_csv(job, NULL);

	job_info_generate_missing_sheet_records(info);

	util_copy_file(c_string_get_char_array(pci_hotfolder_out_path), "/home/stc/JOBCSV/CAM", q_job_get_camera_csv_name(job));
	util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), q_job_get_camera_csv_name(job));

	c_log_add_record_with_cmd(log_ref, "Mazání výstupního kamerového csv souboru z kamerového hotfolderu: %s/%s",
				 c_string_get_char_array(pci_hotfolder_out_path),
				q_job_get_camera_csv_name(job));

	c_string_finalize_v3(q_feedback_csv_content);
}

int8_t controler_end_job_printing(q_job * job)
{
	if(controler_is_conveyor_empty() == true || (c_timer_delay(print_timer, 10000) > 0) || (q_job_get_flag(job) == 'e'))
	{
		if(q_job_get_flag(job) != 'e')
		{
			job_info_set_end_status(info, false);

			if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
				controler_make_csv_analyze_after_print_break(job);
		}

		controler_save_ending_response_csv_file(c_string_get_char_array(q_hotfolder_feedback_path),
							q_job_get_job_name(job),
							q_job_get_job_order(job));

		c_timer_reset(print_timer);
		return 1;
	}

	return 0;
}

void controler_machine_state_print_break()
{
	if(print_finishing_step  == 0)
	{
		print_finishing_step = controler_check_print_controler_status(c_string_get_char_array(print_controler_status));
	}
	else if(print_finishing_step == 1)
	{
		print_finishing_step = controler_print_abbort_sequence();
		timer = c_freq_millis();
	}
	else if(print_finishing_step == 2)
	{
		if((timer + 500) <= c_freq_millis())
			print_finishing_step = controler_ripping_abbort_sequence();
	}
	else
	{
		q_job * job = controler_get_printed_job();

		if(job != NULL)
		{
			if(controler_end_job_printing(job) > 0)
				controler_set_machine_state(MACHINE_STATE_CLEAR_TO_FINISH);
		}
		else
		{
			controler_set_machine_state(MACHINE_STATE_CLEAR_TO_FINISH);
		}
	}
}

void controler_backup_file_from_quadient_hotfolder(char * hotfolder_path, char * backup_path, char * file_name, char * log_comment)
{
	if(file_name != NULL)
	{
		util_move_file(hotfolder_path, backup_path, file_name);

		c_log_add_record_with_cmd(log_ref, log_comment,
						 hotfolder_path, file_name,
						 backup_path,
						 file_name);
	}
}

void controler_clear_hotfolder_base(q_job * job)
{
	if(job != NULL)
	{
		controler_backup_file_from_quadient_hotfolder(c_string_get_char_array(q_hotfolder_main_path),
								c_string_get_char_array(q_hotfolder_backup_path),
								q_job_get_pdf_name(job),
								"Přesun pdf souboru při čištění hotfolderu: %s/%s -> %s/%s");

		controler_backup_file_from_quadient_hotfolder(c_string_get_char_array(q_hotfolder_main_path),
								c_string_get_char_array(q_hotfolder_backup_path),
								q_job_get_bkcore_csv_name(job),
								"Přesun bkcore csv souboru při čištění hotfolderu: %s/%s -> %s/%s");

		controler_backup_file_from_quadient_hotfolder(c_string_get_char_array(q_hotfolder_main_path),
								c_string_get_char_array(q_hotfolder_backup_path),
								q_job_get_camera_csv_name(job),
								"Přesun kamerového csv souboru při čištění hotfolderu: %s/%s -> %s/%s");
	}
}

void controler_clear_hotfolder()
{
	if(printed_job_index >= 0)
	{
		q_job* job = (q_job*) array_list_get(job_list, printed_job_index);
		controler_clear_hotfolder_base(job);
	}
}

void controler_total_clear_hotfolder()
{
	if((job_list != NULL) && (q_hotfolder_feedback_path != NULL))
	{
		for(int i = 0; i < array_list_size(job_list); i++)
		{
			q_job * job = array_list_get(job_list, i);

			if(job != NULL)
			{
				char * file_name = controler_build_end_csv_file_name(q_job_get_job_name(job), q_job_get_job_order(job));
				util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), file_name,  "");
				free(file_name);

				c_log_add_record_with_cmd(log_ref, "Uložení prázdného zpětnovazebního csv na vyčištění hotfolderu na adresu: %s/%s",
								c_string_get_char_array(q_hotfolder_feedback_path),
								file_name);

				controler_clear_hotfolder_base(job);
			}
		}
	}
}



void controler_machine_state_clear_hotfolder()
{
	/* delete csv from camera hotfolder */
	if((printed_job_index >= 0) && (io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0))
	{
		q_job * job = controler_get_printed_job();

		if(job != NULL)
		{
			util_copy_file(c_string_get_char_array(pci_hotfolder_out_path), "/home/stc/JOBCSV/CAM", q_job_get_camera_csv_name(job));
			util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), q_job_get_camera_csv_name(job));


			c_log_add_record_with_cmd(log_ref, "Mazání výstupního kamerového csv souboru z kamerového hotfolderu: %s/%s",
							 c_string_get_char_array(pci_hotfolder_out_path),
							q_job_get_camera_csv_name(job));
		}
	}

	controler_clear_merge_directory();

	if((c_string_len(printed_job_name) > 0))
		controler_clear_hotfolder();

	if(machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER)
		controler_set_machine_state(MACHINE_STATE_JOB_FINISH);
	else
		controler_set_machine_state(MACHINE_STATE_PRINT_FINISH);
}

void controler_machine_finish_job()
{
	controler_save_statistics(machine_statistic_get_day());

	printed_job_index = -1;
	bkcore_csv_pos = 0;

	feed_sheet = 0;

	print_finishing_step = 0;

	print_finishing_step = 0;
	timer = 0;

	sn_trig = 0;
	camera_trig = 0;
	rj_trig = 0;
	ti_trig = 0;

	if(print_timer != NULL)
		c_timer_reset(print_timer);

	machine_cancel_req = false;
	machine_print_req = false;

	machine_print_one_req = false;
	print_confirmation_req = false;
	machine_jam_req = false;

	feeded_sheet_counter_in_job = 0;
	stacked_sheet_counter_in_job = 0;
	rejected_sheet_counter_in_job = 0;
}

void controler_machine_finish_print()
{
	if((info != NULL) && (c_string_len(printed_job_name) > 0))
	{
		util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), "companion_merge.csv");

		char * time_date = util_get_time_string();

		controler_machine_state_finish_print_companion_pdf();

		job_info_set_feeded_sheet_number(info, feeded_sheet_counter-companion_sheet_feed_request_counter);
		job_info_set_stacked_sheet_number(info, stacked_sheet_counter-companion_sheet_feed_request_counter);
		job_info_set_rejected_sheet_number(info, rejected_sheet_counter);

		if((stacked_sheet_counter -  companion_sheet_feed_request_counter) == job_info_get_total_sheet_number(info))
			job_info_set_wrong_sheet_number(info, 0);

		c_string * report_csv_content = job_info_generate_csv(info, time_date);

		stacked_sheet_offset = 0;

		if(report_csv_content != NULL)
		{
			util_save_csv(c_string_get_char_array(job_log_path),
				c_string_get_char_array(job_info_get_csv_name(info)),
				c_string_get_char_array(report_csv_content));

				c_log_add_record_with_cmd(log_ref, "Uložení reportového csv na adresu: %s/%s",
								c_string_get_char_array(job_log_path),
								c_string_get_char_array(job_info_get_csv_name(info)));
		}
		else
		{

			c_log_add_record_with_cmd(log_ref, "Nelze vygenerovat reportovací csv!" );
			printf("report csv content is missing\n");
		}

		free(time_date);
		job_info_clear(info);
	}

	for(int i = 0; i < 4; i++)
	{
		time_for_nth_sheet[i] = 0;
	}

	last_feeded_sheet_millis = 0;
	nth_sheet_time_index = 0;

	if(printed_job_name != NULL)
		c_string_clear(printed_job_name);

	if(bkcore_csv_content != NULL)
	{
		free(bkcore_csv_content);
		bkcore_csv_content = NULL;
	}

	main_sheet_feed_request_counter = 0;
	companion_sheet_feed_request_counter = 0;
	stacked_sheet_counter = 0;
	rejected_sheet_counter = 0;
	rejected_sheet_seq_counter = 0;
	feeded_sheet_counter = 0;
	tab_insert_counter = 0;
	camera_sensor_trigger_counter = 0;
	error_code = MACHINE_ERR_NO_ERROR;
	machine_pause_req = false;

	stacked_sheet = 0;
	rejected_sheet = 0;
	feeded_sheet = 0;

	fake_companion_req = false;
	companion_faked = false;
}


void controler_machine_state_finish()
{
	controler_machine_finish_job();

	if(machine_state == MACHINE_STATE_JOB_FINISH)
	{
		controler_set_machine_state(MACHINE_STATE_NEXT);
	}
	else
	{
		controler_machine_finish_print();
		controler_set_machine_state(MACHINE_STATE_WAIT);
	}
}


uint8_t controler_get_feeder_status()
{
	return feeder_status;
}

uint8_t controler_get_stacker_status()
{
	return stacker_status;
}


int32_t controler_get_stacked_sheets()
{
	return stacked_sheet_counter+stacked_sheet_offset;
}


const char * controler_get_feeder_status_string(uint8_t status)
{
	const char * status_string = NULL;
	lang * multi_lang = multi_lang_get(lang_index);

	switch(status)
	{
		case MACHINE_FN_OFF:
			status_string = multi_lang->err_feeder_off;
		break;

		case MACHINE_FN_E_STOP:
			status_string = multi_lang->err_feeder_e_stop;
		break;

		case MACHINE_FN_FEEDER_MULTIFUNCTION:
			status_string = multi_lang->err_feeder_multifunction;
		break;

		case MACHINE_FN_FEEDER_READY:
			status_string = multi_lang->feeder_status_ready;
		break;

		case MACHINE_FN_READY_TO_FEED:
			status_string = multi_lang->feeder_status_ready_to_feed;
		break;

		case MACHINE_FN_DOUBLE_SHEET:
			status_string = multi_lang->err_feeder_double_sheet;
		break;

		case MACHINE_FN_MISSING_SHEET:
			status_string = multi_lang->err_feeder_sheet_missing;
		break;

		case MACHINE_FN_FEEDING:
			status_string = multi_lang->feeder_status_feeding;
		break;

		default:
			status_string = "Unknown state!";
		break;
	}

	return status_string;
}

const char * controler_get_stacker_status_string(uint8_t status)
{
	const char * status_string = NULL;
	lang * multi_lang = multi_lang_get(lang_index);

	switch(status)
	{
		case  MACHINE_SN_OFF:
			status_string = multi_lang->err_stacker_off;
		break;

		case MACHINE_SN_STACKER_MULTIFUNCTION:
			status_string = multi_lang->err_stacker_multifunction;
		break;

		case MACHINE_SN_BOTTOM:
			status_string = multi_lang->err_stacker_bottom;
		break;

		case MACHINE_SN_STACKER_READY:
			status_string = multi_lang->stacker_status_ready;
		break;

		case MACHINE_SN_STACKER_READY_TO_STACK:
			status_string = multi_lang->stacker_status_ready_to_stack;
		break;

		case MACHINE_SN_JAM_CONVEYOR:
			status_string = multi_lang->err_stacker_jam_conveyor;
		break;

		case MACHINE_SN_JAM_PILE:
			status_string = multi_lang->err_stacker_jam_pile;
		break;

		case MACHINE_SN_STACKING:
			status_string = multi_lang->stacker_status_stacking;
		break;

		default:
			status_string = "Unknown state!";
		break;
	}

	return status_string;
}


int32_t controler_get_total_sheet_number()
{
	return job_info_get_total_sheet_number(info);
}

int32_t controler_get_feeded_sheets()
{
	return feeded_sheet_counter;
}


int32_t controler_get_feeded_companion_sheets()
{
	return companion_sheet_feed_request_counter;
}


int32_t controler_get_rejected_sheets()
{
	return rejected_sheet_counter;
}

int32_t controler_get_tab_inserts()
{
	return tab_insert_counter;
}


int controler_get_sheet_number_in_job()
{
	if(info != NULL)
		return job_info_get_job_sheet_number(info, job_info_get_job_index(info));
	else
		return 0;
}

int controler_get_rejected_sheet_in_job()
{
	return rejected_sheet_counter_in_job;
}

void controler_set_machine_error(uint8_t code)
{
	controler_set_machine_state(MACHINE_STATE_ERROR);
	error_code = code;
}


void controler_set_machine_state(int new_machine_state)
{
	machine_state_pre = machine_state;
	machine_state = new_machine_state;
}

void controler_feeder_error_handler(uint8_t feeder_status)
{
	if((feeder_status == MACHINE_FN_DOUBLE_SHEET) || (feeder_status == MACHINE_FN_MISSING_SHEET) || (feeder_status == MACHINE_FN_FEEDER_MULTIFUNCTION))
	{
		feed_sheet = 0;
		/* set only error code for error identifying but without error state for print recovery */
		if(feeder_status == MACHINE_FN_DOUBLE_SHEET)
			error_code = MACHINE_ERR_FEEDER_DOUBLE_SHEET;
		else if(feeder_status == MACHINE_FN_MISSING_SHEET)
			error_code = MACHINE_ERR_FEEDER_SHEET_MISSING;
		else if((feeder_status == MACHINE_FN_FEEDER_MULTIFUNCTION))
			error_code = MACHINE_ERR_FEEDER_MULTIFUNCTION;

		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
		controler_set_machine_state(MACHINE_STATE_FEEDER_ERROR);
	}

	if(feeder_status == MACHINE_FN_OFF)
		controler_set_machine_error(MACHINE_ERR_FEEDER_OFF);

	if(feeder_status == MACHINE_FN_E_STOP)
		controler_set_machine_error(MACHINE_ERR_FEEDER_E_STOP);
}

void controler_safety_system_in()
{
	/* ena state control */
	if((machine_state != MACHINE_STATE_WAIT) && (machine_state != MACHINE_STATE_ERROR) && (machine_state != MACHINE_STATE_PRINT_BREAK) &&
		(machine_state != MACHINE_STATE_PRINT_FINISH) && machine_state < MACHINE_STATE_LOAD_NEXT_JOB_FILE &&
		(machine_state != MACHINE_STATE_CLEAR_TO_FINISH) && machine_state != MACHINE_STATE_PAUSE)
	{
		if(((feeder_status == MACHINE_FN_FEEDER_READY) || (feeder_status == MACHINE_FN_FEEDER_MULTIFUNCTION) || (feeder_status == MACHINE_FN_FEEDING) || (feeder_status == MACHINE_FN_READY_TO_FEED)) &&
			 ((stacker_status == MACHINE_SN_STACKER_READY) || (stacker_status == MACHINE_SN_STACKER_READY_TO_STACK) || stacker_status == MACHINE_SN_STACKING))
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 1);
			ena_timer = c_freq_millis();
		}
	}
	else
	{	if(ena_force == false)
		{
			if(io_card_get_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA) > 0)
			{
				if(((ena_timer+10000) <= c_freq_millis()) || (machine_state == MACHINE_STATE_WAIT))
					io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA, 0);
			}
			else
			{
				ena_timer = c_freq_millis();
			}
		}
	}


	controler_fun_control();

	controler_tab_insert_runtime();
	controler_tab_insert_control();


	/* check day for statistics counting, if the day is changed, current day is cleared */
	machine_statistic_check_day(statistics_ref);

	/* safety protection of input states */
	if((io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_6_LNA_E_stop) == 0) && (machine_state != MACHINE_STATE_PAUSE))
	{
		controler_set_machine_error(MACHINE_ERR_E_STOP);
	}
	else
	{
		if((error_code == MACHINE_ERR_E_STOP) && (machine_state == MACHINE_STATE_ERROR))
		{
			error_code = MACHINE_ERR_NO_ERROR;
			machine_cancel_req = false;
			machine_print_req = false;
			machine_pause_req = false;
			machine_error_reset_req = false;
			controler_set_machine_state(MACHINE_STATE_PRINT_BREAK);
		}
	}

	if(machine_state == MACHINE_STATE_READ_CSV_LINE)
	{
		if((io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_1_RJ_full) > 0))
		{
			error_code = MACHINE_ERR_REJECT_BIN_FULL;
			machine_pause_req = true;
		}
		else if(io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_2_SN_full))
		{
			error_code = MACHINE_ERR_STACKER_FULL;
			machine_pause_req = true;
		}
	}

	if((rejected_sheet_seq_counter >= rejected_sheet_for_stop) && (rejected_sheet_for_stop != 0))
	{
		error_code = MACHINE_ERR_LOW_PRINT_QUALITY;
		machine_pause_req = true;
	}

#if 1
	/* if some got error then the counters on the end are ignored */
	if(io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_5_camera_trigger) > 0)
	{
		error_code = MACHINE_ERR_PAPER_JAM_CONVAYOR;

		if(machine_state != MACHINE_STATE_WAIT)
		{
			machine_pause_req = true;
			machine_jam_req = true;
		}
	}
	else
	{
		if((machine_state == MACHINE_STATE_WAIT) && (error_code == MACHINE_ERR_PAPER_JAM_CONVAYOR))
			error_code = MACHINE_ERR_NO_ERROR;
	}


	if(io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_4_RJ_jam) > 0)
	{
		error_code = MACHINE_ERR_REJECT_BIN_JAM;

		if(machine_state != MACHINE_STATE_WAIT)
		{
			machine_pause_req = true;
			machine_jam_req = true;
		}
	}
	else
	{
		if((machine_state == MACHINE_STATE_WAIT) && (error_code == MACHINE_ERR_REJECT_BIN_JAM))
			error_code = MACHINE_ERR_NO_ERROR;
	}


	if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_9_FN_jam) > 0)
	{
		error_code = MACHINE_ERR_FEEDER_JAM;

		if(machine_state != MACHINE_STATE_WAIT)
		{
			machine_pause_req = true;
			machine_jam_req = true;
		}
	}
	else
	{
		if((machine_state == MACHINE_STATE_WAIT) && (error_code == MACHINE_ERR_FEEDER_JAM))
			error_code = MACHINE_ERR_NO_ERROR;
	}
#endif
	/*
	if((roundf(c_freq_measure_current(ti_freq, io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc)))) >= 2.0)
	{
		error_code = MACHINE_ERR_TI;
		machine_pause_req = true;
	}
	*/


	if((roundf(c_freq_measure_current(ti_freq, io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_10_TA_BF)))) >= 2.0)
	{
		error_code = MACHINE_ERR_TA;

		if(machine_state != MACHINE_STATE_WAIT)
			machine_pause_req = true;
	}
	else
	{
		if((machine_state == MACHINE_STATE_WAIT) && (error_code == MACHINE_ERR_TA))
			error_code = MACHINE_ERR_NO_ERROR;
	}


#if 1
	/* network connection checking */
	if(c_timer_delay(network_connection_timer, 500) > 0)
	{
		if(controler_quadient_network_connected() == true)
		{
			if(error_code == MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING)

				machine_error_reset_req = true;
			else
				machine_error_reset_req = false;
		}
		else
		{
			if(error_code == MACHINE_ERR_NO_ERROR)
				controler_set_machine_error(MACHINE_ERR_QUADIENT_COMPUTER_NOT_RESPONDING);
		}

		if(controler_pci_network_connected() == true)
		{
			if(error_code == MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING)
				machine_error_reset_req = true;
			else
				machine_error_reset_req = false;
		}
		else
		{
			if((machine_mode == GR_PRINT_INSPECTION) || (machine_mode == GR_INSPECTION))
			{
				if(error_code == MACHINE_ERR_NO_ERROR)
					controler_set_machine_error(MACHINE_ERR_PCI_COMPUTER_NOT_RESPONDING);
			}
		}

		if(controler_iij_network_connected() == true)
		{
			if(error_code == MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING)
				machine_error_reset_req = true;
			else
				machine_error_reset_req = false;
		}
		else
		{
			if(machine_mode == GR_PRINT_INSPECTION || machine_mode == GR_PRINT)
			{
				if(error_code == MACHINE_ERR_NO_ERROR)
					controler_set_machine_error(MACHINE_ERR_IIJ_COMPUTER_NOT_RESPONDING);
			}
		}

		if(controler_iij_is_connected() == STATUS_CLIENT_CONNECTED)
		{
			if(error_code == MACHINE_ERR_GIS_DISCONNECTED)
				machine_error_reset_req = true;
			else
				machine_error_reset_req = false;
		}
		else
		{
			if((machine_mode == GR_PRINT_INSPECTION) || (machine_mode == GR_PRINT))
			{
				if((iij_tcp_connection_req == true) && (c_timer_delay(iij_connection_try_timer, 1000) > 0))
					controler_iij_connect();

				if(error_code == MACHINE_ERR_NO_ERROR)
					controler_set_machine_error(MACHINE_ERR_GIS_DISCONNECTED);
			}
		}
	}
#endif

	if((machine_state != MACHINE_STATE_WAIT) && (machine_state != MACHINE_STATE_READY_TO_START))
	{
		controler_record_feeding_time();
		controler_machine_stacker_counter(stacker_status);
		controler_machine_tab_insert_counter();
		controler_machine_reject_counter();
		//controler_machine_camera_counter();

		if((io_card_get_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA) > 0) && (error_code != MACHINE_ERR_FEEDER_JAM) && (error_code != MACHINE_ERR_FEEDER_DOUBLE_SHEET))
			contorler_sheet_counter();
	}
	

	controler_feed_control();
}

void controler_safety_system_out()
{
#if 1
	controler_machine_mode_control();

	/* print active when printer printing */
	io_card_set_output(io_card_ref,
			IO_CARD_A1,
			A1_OUT_5_PRN_active,
			io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_2_FEEDING_SENSOR) &&
			(io_card_get_output(io_card_ref, IO_CARD_A1, A1_OUT_10_ENA) > 0));

#if 0
	/* activation of gremser machine */
	if((machine_state != MACHINE_STATE_WAIT) && (machine_state != MACHINE_STATE_NEXT))
	{
		if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_3_PRN_rdy, 1);
		}
	}
	else
	{
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_3_PRN_rdy, 0);
	}
#else
	io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_3_PRN_rdy, 1);
#endif

	/* switch off outputs if the error ocures */
	if(machine_state == MACHINE_STATE_ERROR)
	{
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_12_TI_ins, 0);
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_14_INHIBIT_NEXT_SHEET, 0);
	}
	else
	{
		if((machine_state != MACHINE_STATE_WAIT))
		{
			/* counters */
			if((machine_state != MACHINE_STATE_PRINT_BREAK) && (machine_state != MACHINE_STATE_CLEAR_HOT_FOLDER) &&
				(machine_state != MACHINE_STATE_JOB_FINISH) && (machine_state != MACHINE_STATE_CLEAR_TO_FINISH) &&
				(machine_state != MACHINE_STATE_PRINT_FINISH))
			{
				//controler_counter_check_sum(this);
				//controler_stacker_error_handler(stacker_status);
			}
		}
	}

#endif
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
void controler_machine_mode_control()
{
	int cur_machine_mode = io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) + 2*io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1);

	controler_machine_mode_adaptation_protection_time_out(cur_machine_mode, machine_mode);
	machine_mode_runtime_switching(cur_machine_mode, machine_mode);
}

void machine_mode_runtime_switching(int cur_machine_mode, int machine_mode)
{
	if(machine_state == MACHINE_STATE_WAIT)
	{
		if(cur_machine_mode != machine_mode)
		{
			if(cur_machine_mode == machine_mode_pre)
			{
				/* machine mode changed from gui interface, need change output values */
				io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, machine_mode%2);
				io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_1_BM1, (machine_mode/2)%2);
			}
			else
			{
				/* machine mode changed from gremser machine, need adapt internal state */
				machine_mode = cur_machine_mode;
			}
		}
		else
		{
			/* reflection input state to output */
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0));
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_1_BM1, io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1));
		}

		machine_mode_pre = cur_machine_mode;
	}
	else
	{
		/* machine mode changed from gui interface, need change output values */
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_0_BM0, machine_mode%2);
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_1_BM1, (machine_mode/2)%2);
	}
}

void controler_machine_mode_adaptation_protection_time_out(int gremser_machine_mode, int local_machine_mode)
{
	if(machine_state != MACHINE_STATE_ERROR)
	{
		if(gremser_machine_mode != local_machine_mode)
		{
			if(((machine_mode_timer_for_adaptation+10000) < c_freq_millis()) && (error_code == MACHINE_ERR_NO_ERROR))
				error_code = MACHINE_ERR_MACHINE_MODE_ADAPTATION;
		}
		else
		{
			machine_mode_timer_for_adaptation = c_freq_millis();

			if(error_code == MACHINE_ERR_MACHINE_MODE_ADAPTATION)
				error_code = MACHINE_ERR_NO_ERROR;
		}
	}
}

void controler_machine_stacker_counter(uint8_t stacker_status)
{
	/* sheets in stacker counter */
	if((sn_trig != stacker_status) && (stacker_status == MACHINE_SN_STACKING))
	{
		stacked_sheet_counter ++;
		stacked_sheet ++;
		stacked_sheet_counter_in_job ++;

		machine_statistic_increment_stacked_sheets(statistics_ref);

		rejected_sheet_seq_counter = 0;
	}

	sn_trig = stacker_status;
}


void controler_machine_tab_insert_counter()
{
	uint8_t ti_trig_val = io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc);

	if((ti_trig != ti_trig_val) && (ti_trig_val > 0))
	{
		tab_insert_counter ++;
	}

	ti_trig = ti_trig_val;
}


double controler_get_statistics_error_rate()
{
	return machine_statistic_get_total_error_rate(statistics_ref);
}

int64_t controler_get_statistics_total_feeded_sheets()
{
	return machine_statistic_get_total_feeded_sheets(statistics_ref);
}

int64_t controler_get_statistics_total_stacked_sheets()
{
	return machine_statistic_get_total_stacked_sheets(statistics_ref);
}

int64_t controler_get_statistics_total_rejected_sheets()
{
	return machine_statistic_get_total_rejected_sheets(statistics_ref);
}

double controler_get_statistics_on_day_error_rate(int8_t day)
{
	if((day >= 0) && (day < 7))
		return machine_statistic_get_error_rate(statistics_ref, day);
	else
		return machine_statistic_get_error_rate(statistics_ref,  machine_statistic_get_day());
}

int64_t controler_get_statistics_on_day_total_feeded_sheets(int8_t day)
{
	if((day >= 0) && (day < 7))
		return machine_statistic_get_feeded_sheets(statistics_ref, day);
	else
		return machine_statistic_get_feeded_sheets(statistics_ref, machine_statistic_get_day());
}

int64_t controler_get_statistics_on_day_total_stacked_sheets(int8_t day)
{
	if((day >= 0) && (day < 7))
		return machine_statistic_get_stacked_sheets(statistics_ref, day);
	else
		return machine_statistic_get_stacked_sheets(statistics_ref, machine_statistic_get_day());
}

int64_t controler_get_statistics_on_day_total_rejected_sheets(int8_t day)
{
	if((day >= 0) && (day < 7))
		return machine_statistic_get_rejected_sheets(statistics_ref, day);
	else
		return machine_statistic_get_rejected_sheets(statistics_ref, machine_statistic_get_day());
}

void controler_machine_reject_counter()
{
	uint8_t rj_trig_val = io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_3_RJ_cnt);

	if((rj_trig != rj_trig_val) && (rj_trig_val > 0))
	{
		rejected_sheet_counter ++;
		rejected_sheet_seq_counter++;
		complet_rejected++;
		rejected_sheet_counter_in_job++;

		machine_statistic_increment_rejected_sheets(statistics_ref);

		rejected_sheet++;
	}

	rj_trig = rj_trig_val;
}

void controler_machine_camera_counter()
{
	uint8_t camera_trig_val = io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_5_camera_trigger);

	if((camera_trig != camera_trig_val) && (camera_trig_val > 0))
	{
		camera_sensor_trigger_counter++;
	}

	/* trigger previous state record */
	camera_trig = camera_trig_val;
}

int8_t controler_printer_abbort_print(uint8_t step)
{
	int8_t ret_val = -1;
	char * res = NULL;

	if(step == 0)
	{
		res = com_tcp_transaction(iij_tcp_ref, "P,A\n", COMM_TCP_IO_BUFFER_SIZE);
		if(res != NULL)
			ret_val = 1;
	}
	else
	{
		res = com_tcp_transaction(iij_tcp_ref, "R,A\n", COMM_TCP_IO_BUFFER_SIZE);
		if(res != NULL)
			ret_val = 2;
	}

	return ret_val;
}

void controler_free_report_information_struct(rep_struct * this)
{
	if(this->order_name != NULL)
	{
		free(this->order_name);
		this->order_name = NULL;
	}

	if(this->job_name != NULL)
	{
		free(this->job_name);
		this->job_name = NULL;
	}

	if(this->finish_state != NULL)
	{
		free(this->finish_state);
		this->finish_state = NULL;
	}

	if(this->date_time != NULL)
	{
		free(this->date_time);
		this->date_time = NULL;
	}

	free(this);
}

rep_struct * controler_load_report_information(char * report_csv_name)
{
	rep_struct * job_report = NULL;
	char * report_csv_content = util_load_csv(c_string_get_char_array(job_log_path), report_csv_name, NULL);

	if(report_csv_content != NULL)
	{
		job_report = (rep_struct *) malloc(sizeof(rep_struct));
		int pos = 0, par = 0;
		bool read_enable = false;
		int temp_int_val = 0;
		c_string * buffer = c_string_new();

		while((report_csv_content[pos] != 0) && (par < 10))
		{
			switch(par)
			{
				case 0:
					if(report_csv_content[pos] == '\n')
					{
						job_report->order_name = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->order_name, c_string_get_char_array(buffer));
					}
					else if(report_csv_content[pos] == ':')
					{
						read_enable = true;
						pos++;
					}
					else if(read_enable == true)
					{
						c_string_add_char(buffer, report_csv_content[pos]);
					}
				break;

				case 1:
					if(report_csv_content[pos] == '\n')
					{
						job_report->job_name = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->job_name, c_string_get_char_array(buffer));
					}
					else if(report_csv_content[pos] == ':')
					{
						read_enable = true;
						pos++;
					}
					else if(read_enable == true)
					{
						c_string_add_char(buffer, report_csv_content[pos]);
					}
				break;

				case 2:
					if(report_csv_content[pos] == '\n')
					{
						job_report->finish_state = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->finish_state, c_string_get_char_array(buffer));
					}
					else
					{
						c_string_add_char(buffer, report_csv_content[pos]);
					}

				break;


				case 3:
					if(report_csv_content[pos] == '\n')
					{
						job_report->feeded_sheet_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;

				case 4:
					if(report_csv_content[pos] == '\n')
					{
						job_report->stacked_sheet_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;

				case 5:
					if(report_csv_content[pos] == '\n')
					{
						job_report->rejected_sheet_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;


				case 6:
					if(report_csv_content[pos] == '\n')
					{
						job_report->wrong_sheet_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;

				case 7:
					if(report_csv_content[pos] == '\n')
					{
						job_report->sheet_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;


				case 8:
					if(report_csv_content[pos] == '\n')
					{
						job_report->stamp_number = temp_int_val;
					}
					else
					{
						if(isdigit(report_csv_content[pos]))
							temp_int_val = (temp_int_val*10) + (report_csv_content[pos]-48);
					}
				break;

				case 9:
					if(report_csv_content[pos] == '\n')
					{
						job_report->date_time = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->date_time, c_string_get_char_array(buffer));
					}
					else if((report_csv_content[pos] == ':') && (read_enable == false))
					{
						read_enable = true;
						pos++;
					}
					else if(read_enable == true)
					{
						c_string_add_char(buffer, report_csv_content[pos]);
					}
				break;
			}

			if(report_csv_content[pos] == '\n')
			{
				par ++;
				read_enable = false;
				c_string_clear(buffer);
				temp_int_val = 0;
			}

			pos ++;
		}

		free(report_csv_content);
	}

	return job_report;
}

char * controler_machine_get_state_str()
{
	char * status_str = NULL;

	if(io_manual == false)
	{
		switch(machine_state)
		{
			case MACHINE_STATE_WAIT:
				status_str = "Čekání na spuštění jobu";
			break;

			case MACHINE_STATE_PREPARE:
				status_str = "Příprava dat pro tisk";
			break;

			case MACHINE_STATE_PRINT_MAIN:
				status_str = "Tisk";
			break;

			case MACHINE_STATE_SAVE_Q_CSV:
				status_str = "Generování csv pro Quadient.";
			break;

			case MACHINE_STATE_CLEAR_HOT_FOLDER:
				status_str = "Čištění hotfolderů.";
			break;

			case MACHINE_STATE_NEXT:
				status_str = "Čekání na další soubory jobu.";
			break;

			case MACHINE_STATE_PRINT_FINISH:
				status_str = "Dokončování procesu tisku.";
			break;

			case MACHINE_STATE_CLEAR_TO_FINISH:
				status_str = "Čištění hotfolderu na konci tisku.";
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
				status_str = "Čtění řádku datového csv souboru.";
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

			case MACHINE_STATE_FEEDER_ERROR:
				status_str = "Chyba nakladače, čekání na nápravu.";
			break;

			case MACHINE_STATE_WAIT_FOR_CONFIRMATION:
				status_str = "Očekávání potvrzení naložení prokladu.";
			break;

			case MACHINE_STATE_WAIT_FOR_PRINT_FINISH:
				status_str = "Čekání na vyložení archů";
			break;

			case MACHINE_STATE_WAIT_FOR_ENA_START:
				status_str = "Čekání na spuštění stroje";
			break;

			case MACHINE_STATE_STACKER_ERROR:
				status_str = "Chyba vykladače, čekání na nápravu.";
			break;

			case MACHINE_STATE_JAM_EVENT:
				status_str = "Zácpa na dopravníku";
			break;

			case MACHINE_STATE_CLEAR_BELT_AFTER_JAM:
				status_str = "Vyprázdnění dopravníkového pásu.";
			break;

			case MACHINE_STATE_FAKE_COMPANION:
				status_str = "Maskování tisku prokladu.";
			break;

			case MACHINE_STATE_LOAD_NEXT_JOB_FILE:
				status_str = "Načítám proklady...";
			break;

			case MACHINE_STATE_SEND_RESPONSE_TO_IA:
				status_str = "Načítám proklady...";
			break;

			case MACHINE_STATE_COPY_COMPANION_PDF:
				status_str = "Načítám proklady...";
			break;

			case MACHINE_STATE_DELETE_STAMP_PDF:
				status_str = "Načítám proklady...";
			break;

			case MACHINE_STATE_MERGE_COMPANION_PDF_FILES:
				status_str = "Příprava dat pro tisk.";
			break;

			case MACHINE_STATE_PREPARE_FOR_PRINT_COMPANION_PDF:
				status_str = "Čekám na spuštění stroje.";
			break;

			case MACHINE_STATE_PRINT_COMPANION_PDF:
				status_str = "Tisk prokladových archů.";
			break;

			case MACHINE_STATE_FINISH_PRINT_COMPANION_PDF:
				status_str = "Ukončování tisku.";
			break;

			default:
				status_str = "Stroj je v neznámém stavu!!!";
		}
	}
	else
	{
		status_str = "Stroj v manuálním režimu ovládání IO";
	}

	return status_str;
}

void controler_machine_log_monitor()
{
	if(machine_state != machine_state_trig)
	{
		if(machine_state != MACHINE_STATE_ERROR)
			c_log_add_record_with_cmd(log_ref, "%d - %s", machine_state, controler_machine_get_state_str());
		else
			c_log_add_record_with_cmd(log_ref, "%d - %s: %s", machine_state, controler_machine_get_state_str(), controler_get_error_str());
	}

	machine_state_trig = machine_state;
}


void controler_counter_check_sum()
{
	int32_t non_stacked = camera_sensor_trigger_counter - stacked_sheet_counter - stacked_sheet_counter;
	int32_t rest = (feeded_sheet_counter_in_job) - (stacked_sheet_counter + rejected_sheet_counter);

	/* if the non stacked sheets are int the specific interval check next else set error */
	if((non_stacked >= 0) && (non_stacked <= non_stacked_upper_limit))
	{
		int32_t non_revided = (rest - non_stacked);
		if((non_revided < 0) || (non_revided > non_revided_upper_limit))
		{
			controler_set_machine_error(MACHINE_ERR_COUNTERS_MISMATCH);
		}
	}
	else
	{
		controler_set_machine_error(MACHINE_ERR_COUNTERS_MISMATCH);
	}
}


void controler_stacker_error_handler(uint8_t stacker_status)
{
	if((machine_state == MACHINE_STATE_PRINT_MAIN) || (machine_state == MACHINE_STATE_PRINT_COMPANION))
	{
		if(stacker_status == MACHINE_SN_OFF)
			controler_set_machine_error(MACHINE_ERR_STACKER_OFF);
	}

	if(stacker_status == MACHINE_SN_STACKER_MULTIFUNCTION)
	{
		error_code = MACHINE_ERR_STACKER_MULTIFUNCTION;
		controler_set_machine_state(MACHINE_STATE_STACKER_ERROR);
	}

	if(stacker_status == MACHINE_SN_BOTTOM)
	{
		error_code = MACHINE_ERR_STACKER_BOTTOM;
		controler_set_machine_state(MACHINE_STATE_STACKER_ERROR);
	}

	if(stacker_status == MACHINE_SN_JAM_CONVEYOR)
	{
		error_code = MACHINE_ERR_STACKER_JAM_CONVEYOR;
		controler_set_machine_state(MACHINE_STATE_STACKER_ERROR);
	}

	if(stacker_status == MACHINE_SN_JAM_PILE)
	{
		error_code = MACHINE_ERR_STACKER_JAM_PILE;
		controler_set_machine_state(MACHINE_STATE_STACKER_ERROR);
	}
}


uint8_t controler_load_bkcore_csv()
{
	q_job * job = controler_get_printed_job();

	if(job != NULL)
	{
		if(q_hotfolder_main_path != NULL)
		{
			bkcore_csv_content = util_load_csv(c_string_get_char_array(q_hotfolder_main_path),
										q_job_get_bkcore_csv_name(job),
										 &(bkcore_csv_size));

			if(bkcore_csv_content != NULL)
			{
				printf("%d - obsah bkcore souboru:\n%s\n", bkcore_csv_size, bkcore_csv_content);
				c_log_add_record_with_cmd(log_ref, "Načten obsah bkcore csv souboru.");
				return STATUS_SUCCESS;
			}
		}
	}

	c_log_add_record_with_cmd(log_ref, "obsah souboru bkcore csv nelze načíst!");

	return STATUS_GENERAL_ERROR;
}

void * controler_gis_runtime_state_reading(void * param)
{
	com_tcp_send(iij_tcp_ref, "P,L\n");
	int id = -1;

	while(controler_iij_is_connected() == STATUS_CLIENT_CONNECTED)
	{
		char * state_msg = com_tcp_recv(iij_tcp_ref);

		if(state_msg != NULL)
		{
			while(*state_msg != 0)
			{
				if(*state_msg == 'I')
				{
					int msg_id = controler_gis_load_status_id(&state_msg);
					state_msg++;

					if(*state_msg == 'S')
					{
						if(id == -1)
							id = msg_id;

						if(msg_id == id)
						{
							controler_parse_gis_status(state_msg);
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

		usleep(5000);
	}

	if(print_controler_status != NULL)
		c_string_set_string(print_controler_status, "Unknown");

	com_tcp_disconnect(iij_tcp_ref);
	c_log_add_record_with_cmd(log_ref, "Spojení s GISem ukončeno.");

	return NULL;
}

int controler_gis_load_status_id(char ** msg)
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

void controler_parse_gis_status(char * status_str)
{
	status_str = status_str+2;
	c_string_clear(print_controler_status);

	while((*status_str != ',') && (*status_str != 0) && (*status_str != '\n'))
	{
		c_string_add_char(print_controler_status, *status_str);
		status_str++;
	}
}

uint8_t controler_iij_try_connect()
{
	uint8_t res = com_tcp_connect(iij_tcp_ref);

	if(res == STATUS_CLIENT_CONNECTED)
	{
		if(strcmp(com_tcp_recv(iij_tcp_ref), "N,C\n") == 0)
			pthread_create(&(iij_gis_state_reading_thread), 0, &controler_gis_runtime_state_reading, NULL);
		else
			res = STATUS_CONNECTION_ERROR;
	}

	return res;
}

uint8_t controler_pci_try_connect()
{
	return com_tcp_connect(pci_tcp_ref);
}

void controler_update_config(char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail)
{
	config_setting_t * root, *group, *settings;

	root = config_root_setting(&(cfg_ref));
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

		config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
		c_log_add_record_with_cmd(log_ref, log_msg_ok);
	}
	else
	{
		c_log_add_record_with_cmd(log_ref, "%s: %s",log_msg_fail, config_error_text(&(cfg_ref)));
	}
}


uint8_t controler_load_config()
{
	config_setting_t * settings;
	const char * setting_val_str;
	int setting_val_int;
	double setting_val_float;

	/* load hotfolder settings */
	settings = config_lookup(&(cfg_ref), CFG_GROUP_HOTFOLDER);
	if(settings == NULL)
		return 1;

	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_MAIN, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(q_hotfolder_main_path, (char*) setting_val_str);
	else
		return 2;

	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_BACKUP, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(q_hotfolder_backup_path, (char*) setting_val_str);
	else
		return 3;

	if(config_setting_lookup_string(settings, CFG_HOT_QUADIENT_FEEDBACK, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(q_hotfolder_feedback_path, (char*) setting_val_str);
	else
		return 4;

	if(config_setting_lookup_string(settings, CFG_HOT_PCI_IN, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(pci_hotfolder_in_path, (char*) setting_val_str);
	else
		return 5;

	if(config_setting_lookup_string(settings, CFG_HOT_PCI_OUT, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(pci_hotfolder_out_path, (char*) setting_val_str);
	else
		return 6;

	if(config_setting_lookup_string(settings, CFG_HOT_GIS, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(gis_hotfolder_path, (char*) setting_val_str);
	else
		return 7;

	if(config_setting_lookup_string(settings, CFG_HOT_REP_CSV, &setting_val_str) == CONFIG_TRUE)
		c_string_set_string(job_log_path, (char*) setting_val_str);
	else
		return 8;


	/* load network settings */
	settings = config_lookup(&(cfg_ref), CFG_GROUP_NETWORK);

	if(config_setting_lookup_string(settings, CFG_NETWORK_GIS_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		com_tcp_set_ip_addr(iij_tcp_ref, (char*) setting_val_str);
	else
		return 9;

	if(config_setting_lookup_int(settings, CFG_NETWORK_GIS_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		com_tcp_set_tcp_port(iij_tcp_ref, setting_val_int);
	else
		return 10;


	if(config_setting_lookup_string(settings, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_monitor_set_ip_address(quadient_comm_monitor, (char*) setting_val_str);
	else
		return 9;

	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_monitor_set_tcp_port(quadient_comm_monitor, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_PCI_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_monitor_set_ip_address(pci_comm_monitor, (char*) setting_val_str);
	else
		return 9;

	if(config_setting_lookup_int(settings, CFG_NETWORK_PCI_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_monitor_set_tcp_port(pci_comm_monitor, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_IIJ_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_monitor_set_ip_address(iij_comm_monitor, (char*) setting_val_str);
	else
		return 9;

	if(config_setting_lookup_int(settings, CFG_NETWORK_IIJ_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_monitor_set_tcp_port(iij_comm_monitor, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		comm_monitor_set_ip_address(quadient_comm_monitor, (char*) setting_val_str);
	else
		return 9;

	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		comm_monitor_set_tcp_port(quadient_comm_monitor, setting_val_int);
	else
		return 10;



	/* load print params settings */
	settings = config_lookup(&(cfg_ref), CFG_GROUP_PRINT_PARAMS);

	if(config_setting_lookup_int(settings, CFG_PP_SHEET_FOR_STOP, &setting_val_int) == CONFIG_TRUE)
		rejected_sheet_for_stop = setting_val_int;
	else
		return 11;

	if(config_setting_lookup_int(settings, CFG_PP_MAX_SHEET_IN_STACKER, &setting_val_int) == CONFIG_TRUE)
		max_stacked_sheets = setting_val_int;
	else
		return 12;



	if(config_setting_lookup_int(settings, CFG_PP_COMPAION_SHEET_SOURCE, &setting_val_int) == CONFIG_TRUE)
	{
		if((setting_val_int) >= 0 && (setting_val_int < SSOURCE_N))
			companion_sheet_source = setting_val_int;
		else
			return 13;
	}
	else
	{
		return 14;
	}


	if(config_setting_lookup_bool(settings, CFG_PP_SHEET_SOURCE_CONFIRMATION, &setting_val_int) == CONFIG_TRUE)
		sheet_source_confirmation = setting_val_int;
	else
		return 15;


	if(config_setting_lookup_int(settings, CFG_PP_NON_STACKED_SHEET_LIMIT, &setting_val_int) == CONFIG_TRUE)
		non_stacked_upper_limit = setting_val_int;
	else
		return 16;


	if(config_setting_lookup_int(settings, CFG_PP_NON_REVIDED_SHEET_LIMIT, &setting_val_int) == CONFIG_TRUE)
		non_revided_upper_limit = setting_val_int;
	else
		return 17;

	/*
	if(config_setting_lookup_int(settings, CFG_PP_GR_MACHINE_MODE, &setting_val_int) == CONFIG_TRUE)
		machine_mode = setting_val_int;
	else
		return 18;
*/

	if(config_setting_lookup_int(settings, CFG_PP_FEED_DELAY, &setting_val_int) == CONFIG_TRUE)
		feed_delay = setting_val_int;
	else
		return 19;


	if(config_setting_lookup_int(settings, CFG_PP_FAN_INTENSITY, &setting_val_int) == CONFIG_TRUE)
		fan_intensity_set = setting_val_int;
	else
		return 20;

	if(config_setting_lookup_int(settings, CFG_PP_TAB_INSERT_LENGTH, &setting_val_int) == CONFIG_TRUE)
		tab_insert_len = setting_val_int;
	else
		return 21;

	if(config_setting_lookup_int(settings, CFG_PP_TAB_INSERT_CUT_DELAY, &setting_val_int) == CONFIG_TRUE)
		ti_delay = setting_val_int;
	else
		return 22;

	if(config_setting_lookup_int(settings, CFG_PP_TAB_INSERT_SEQUENCE, &setting_val_int) == CONFIG_TRUE)
		tab_insert_sequence = setting_val_int;
	else
		return 23;

	if(config_setting_lookup_bool(settings, CFG_PP_TAB_INSERT_AUTOMAT, &setting_val_int) == CONFIG_TRUE)
		tab_insert_automat = setting_val_int;
	else
		return 24;


	/* load language settings */
	settings = config_lookup(&(cfg_ref), CFG_GROUP_LANGUAGE);

	if(config_setting_lookup_int(settings, CFG_LANG_INDEX, &setting_val_int) == CONFIG_TRUE)
		lang_index = setting_val_int;
	else
		return 25;


	settings = config_lookup(&(cfg_ref), CFG_GROUP_STATISTICS);

	if(config_setting_lookup_int(settings, CFG_STATISTICS_DAY_PRE, &setting_val_int) == CONFIG_TRUE)
		machine_statistic_set_day_pre(statistics_ref, setting_val_int);
	else
		return 25;

	if(config_setting_lookup_int(settings, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, &setting_val_int) == CONFIG_TRUE)
		machine_statistic_set_total_feeded_sheets(statistics_ref, setting_val_int);
	else
		return 25;

	if(config_setting_lookup_int(settings, CFG_STATISTICS_TOTAL_STACKED_SHEETS, &setting_val_int) == CONFIG_TRUE)
		machine_statistic_set_total_stacked_sheets(statistics_ref, setting_val_int);
	else
		return 25;

	if(config_setting_lookup_int(settings, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, &setting_val_int) == CONFIG_TRUE)
		machine_statistic_set_total_rejected_sheets(statistics_ref, setting_val_int);
	else
		return 25;

	if(config_setting_lookup_float(settings, CFG_STATISTICS_ERROR_RATE, &setting_val_float) == CONFIG_TRUE)
		machine_statistic_set_total_error_rate(statistics_ref, setting_val_float);
	else
		return 25;


	config_setting_t * sub_settings = NULL;

	for(int day_index = 0; day_index < 7; day_index++)
	{
		if(day_index == 0)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_SUN);
		else if(day_index == 1)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_MON);
		else if(day_index == 2)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_TUE);
		else if(day_index == 3)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_WED);
		else if(day_index == 4)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_THU);
		else if(day_index == 5)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_FRI);
		else if(day_index == 6)
			sub_settings = config_setting_get_member(settings, CFG_GROUP_STATISTICS_SAT);
		else
			return 59;

		if((sub_settings != NULL) && (config_setting_lookup_int(sub_settings, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, &setting_val_int) == CONFIG_TRUE))
			machine_statistic_restore_feeded_sheets(statistics_ref, setting_val_int, day_index);
		else
			return 26+(day_index*4);

		if((sub_settings != NULL) && (config_setting_lookup_int(sub_settings, CFG_STATISTICS_TOTAL_STACKED_SHEETS, &setting_val_int) == CONFIG_TRUE))
			machine_statistic_restore_stacked_sheets(statistics_ref, setting_val_int, day_index);
		else
			return 26+(day_index*4);

		if((sub_settings != NULL) && (config_setting_lookup_int(sub_settings, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, &setting_val_int) == CONFIG_TRUE))
			machine_statistic_restore_rejected_sheets(statistics_ref, setting_val_int, day_index);
		else
			return 27+(day_index*4);

		if((sub_settings != NULL) && (config_setting_lookup_float(sub_settings, CFG_STATISTICS_ERROR_RATE, &setting_val_float) == CONFIG_TRUE))
			machine_statistic_restore_error_rate(statistics_ref, setting_val_float, day_index);

		else
			return 28+(day_index*4);

	}

	return 0;
}

void controler_initialize_variables()
{
	/* machine handler status and control variables */
	machine_state = MACHINE_STATE_PRINT_FINISH;
	machine_state_trig = MACHINE_STATE_WAIT;
	machine_state_pre = MACHINE_STATE_WAIT;
	printed_job_name = c_string_new();
	machine_pause_req = false;
	machine_print_req = false;
	machine_error_reset_req = false;
	machine_cancel_req = false;
	machine_print_one_req = false;
	machine_jam_req = false;

	stacked_sheet_offset = 0;

	companion_generation_delay = 0;

	complet_feeded = 0;
	complet_rejected = 0;

	stacked_for_ti = 0;
	stacked_sensor_for_ti_pre = 0;

	fake_companion_req = false;
	companion_faked = false;

 	merged_companion_sheets = 0;


	tab_insert_step = 0;
	tab_insert_busy = false;
	last_ti_req = 0;
	timer_tab_insert = 0;
	tab_insert_req = 0;

	fan_regulation = soft_pwm_new(1000, 100);
	fan_intensity = 0;
	fan_activity = 0;

	machine_mode = GR_PRINT_INSPECTION;

	machine_mode_timer_for_adaptation = 0;

	iij_tcp_connection_req = true;

	iij_connection_try_timer = c_timer_new();

	wait_for_files_ready = 0;

	feed_sensor_timer = c_timer_new();
	ena_timer = 0;
	ena_start_timer = 0;
	delay_on_end = 0;

	file_delay = 0;

	manual_feed = false;
	ena_force = false;

	io_manual = false;

	network_connection_timer = c_timer_new();
	feeding_rate = c_timer_new();

	feeder_status = 0;
	stacker_status = 0;

	clear_on_startup = 0;

	error_code = 0;
	//machine_mode = io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) + 2*io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1);

	job_list = NULL;
	job_list_pre = NULL;
	job_list_changed = 0;

	manual_feed_xbf = 0;

	printed_job_index = -1;
	bkcore_csv_pos = 0;

	q_hotfolder_main_path = c_string_new();
	q_hotfolder_feedback_path = c_string_new();
	q_hotfolder_backup_path = c_string_new();
	pci_hotfolder_in_path = c_string_new();
	pci_hotfolder_out_path = c_string_new();
	gis_hotfolder_path = c_string_new();
	job_log_path = c_string_new();

	timer = c_freq_millis();

	main_sheet_feed_request_counter = 0;
	companion_sheet_feed_request_counter = 0;
	stacked_sheet_counter = 0;
	rejected_sheet_counter = 0;
	rejected_sheet_seq_counter = 0;
	tab_insert_counter = 0;
	camera_sensor_trigger_counter = 0;
	feeded_sheet_counter = 0;
	feeded_sheet_counter_in_job = 0;
	feeded_sheet_counter_pre = 0;

	stacked_sheet = 0;
	rejected_sheet = 0;
	feeded_sheet = 0;

	sheet_sensor_pre = 0;

	machine_mode_pre = 0;
	print_confirmation_req = false;

	/* GIS printer status */
	print_controler_status = c_string_new_with_init("Unknown");
}


char * controler_get_gis_status()
{
	return c_string_get_char_array(print_controler_status);
}


void controler_default_config()
{
	config_setting_t *root, *hotfolder, *network, *print_params, *language, *settings, *statistics;
	config_setting_t *statistics_mon, *statistics_tue, *statistics_wed, *statistics_thu, *statistics_fri, *statistics_sat, *statistics_sun;

	/* create root settings */
	root = config_root_setting(&(cfg_ref));

	/* create groups */
	hotfolder = config_setting_add(root, CFG_GROUP_HOTFOLDER, CONFIG_TYPE_GROUP);
	network = config_setting_add(root, CFG_GROUP_NETWORK, CONFIG_TYPE_GROUP);
	print_params = config_setting_add(root, CFG_GROUP_PRINT_PARAMS, CONFIG_TYPE_GROUP);
	language = config_setting_add(root, CFG_GROUP_LANGUAGE, CONFIG_TYPE_GROUP);
	statistics = config_setting_add(root, CFG_GROUP_STATISTICS, CONFIG_TYPE_GROUP);

	statistics_sun  = config_setting_add(statistics, CFG_GROUP_STATISTICS_SUN, CONFIG_TYPE_GROUP);
	statistics_mon  = config_setting_add(statistics, CFG_GROUP_STATISTICS_MON, CONFIG_TYPE_GROUP);
	statistics_tue  = config_setting_add(statistics, CFG_GROUP_STATISTICS_TUE, CONFIG_TYPE_GROUP);
	statistics_wed  = config_setting_add(statistics, CFG_GROUP_STATISTICS_WED, CONFIG_TYPE_GROUP);
	statistics_thu  = config_setting_add(statistics, CFG_GROUP_STATISTICS_THU, CONFIG_TYPE_GROUP);
	statistics_fri  = config_setting_add(statistics, CFG_GROUP_STATISTICS_FRI, CONFIG_TYPE_GROUP);
	statistics_sat  = config_setting_add(statistics, CFG_GROUP_STATISTICS_SAT, CONFIG_TYPE_GROUP);


	/* save hotfolder settings */
	c_string_set_string(q_hotfolder_main_path, Q_HOT_FOLDER_ADDR);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_MAIN, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_HOT_FOLDER_ADDR);

	c_string_set_string(q_hotfolder_feedback_path, Q_FEEDBACK_ADDR);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_FEEDBACK, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_FEEDBACK_ADDR);

	c_string_set_string(q_hotfolder_backup_path, Q_JOB_BACKUP);
	settings = config_setting_add(hotfolder, CFG_HOT_QUADIENT_BACKUP, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, Q_JOB_BACKUP);

	c_string_set_string(pci_hotfolder_in_path, PCI_HOT_FOLDER_ADDR_IN);
	settings = config_setting_add(hotfolder, CFG_HOT_PCI_IN, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, PCI_HOT_FOLDER_ADDR_IN);

	c_string_set_string(pci_hotfolder_out_path, PCI_HOT_FOLDER_ADDR_OUT);
	settings = config_setting_add(hotfolder, CFG_HOT_PCI_OUT, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, PCI_HOT_FOLDER_ADDR_OUT);

	c_string_set_string(gis_hotfolder_path, GIS_HOT_FOLDER);
	settings = config_setting_add(hotfolder, CFG_HOT_GIS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, GIS_HOT_FOLDER);

	c_string_set_string(job_log_path, JOB_INFO_CSV_PATH);
	settings = config_setting_add(hotfolder, CFG_HOT_REP_CSV, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, JOB_INFO_CSV_PATH);


	/* save print parameters settings */
	rejected_sheet_for_stop = 10;
	settings = config_setting_add(print_params, CFG_PP_SHEET_FOR_STOP, CONFIG_TYPE_INT);
	config_setting_set_int(settings, rejected_sheet_for_stop);

	max_stacked_sheets = 2500;
	settings = config_setting_add(print_params, CFG_PP_MAX_SHEET_IN_STACKER, CONFIG_TYPE_INT);
	config_setting_set_int(settings, max_stacked_sheets);

	controler_set_companion_sheet_source(SSOURCE_COMPANION);
	settings = config_setting_add(print_params, CFG_PP_COMPAION_SHEET_SOURCE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, SSOURCE_COMPANION);

	controler_set_sheet_source_confirmation(false);
	settings = config_setting_add(print_params, CFG_PP_SHEET_SOURCE_CONFIRMATION, CONFIG_TYPE_BOOL);
	config_setting_set_bool(settings, false);

	non_stacked_upper_limit = 2;
	settings = config_setting_add(print_params, CFG_PP_NON_STACKED_SHEET_LIMIT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 2);

	non_revided_upper_limit = 3;
	settings = config_setting_add(print_params, CFG_PP_NON_REVIDED_SHEET_LIMIT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 3);


	feed_delay = 100;
	settings = config_setting_add(print_params, CFG_PP_FEED_DELAY, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 500);

	fan_intensity_set = 50;
	settings = config_setting_add(print_params, CFG_PP_FAN_INTENSITY, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 50);


	tab_insert_len = 100;
	settings = config_setting_add(print_params, CFG_PP_TAB_INSERT_LENGTH, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 500);

	ti_delay = 3;
	settings = config_setting_add(print_params, CFG_PP_TAB_INSERT_CUT_DELAY, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 3);

	tab_insert_sequence = 500;
	settings = config_setting_add(print_params, CFG_PP_TAB_INSERT_SEQUENCE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 500);

	tab_insert_automat = false;
	settings = config_setting_add(print_params, CFG_PP_TAB_INSERT_AUTOMAT, CONFIG_TYPE_BOOL);
	config_setting_set_bool(settings, false);

/*
	machine_mode = GR_PRINT_INSPECTION;
	settings = config_setting_add(print_params, CFG_PP_GR_MACHINE_MODE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, GR_PRINT_INSPECTION);
*/
	/* save network settings */
	com_tcp_set_ip_addr(iij_tcp_ref,  DEFAULT_IP_ADDRESS_GIS);
	settings = config_setting_add(network, CFG_NETWORK_GIS_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_GIS);

	com_tcp_set_tcp_port(iij_tcp_ref, DEFAULT_TCP_PORT_GIS);
	settings = config_setting_add(network, CFG_NETWORK_GIS_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_TCP_PORT_GIS);

	comm_monitor_set_tcp_port(quadient_comm_monitor, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_monitor_set_ip_address(quadient_comm_monitor, DEFAULT_IP_ADDRESS_QUADIENT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_QUADIENT);

	comm_monitor_set_tcp_port(pci_comm_monitor, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_monitor_set_ip_address(pci_comm_monitor, DEFAULT_IP_ADDRESS_PCI);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_PCI);

	comm_monitor_set_tcp_port(iij_comm_monitor, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	comm_monitor_set_ip_address(iij_comm_monitor, DEFAULT_IP_ADDRESS_GIS);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_GIS);


	/* save default settings for total statistics */
	settings = config_setting_add(statistics, CFG_STATISTICS_DAY_PRE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);


	/* save default settings for statistics on sunday */
	settings = config_setting_add(statistics_sun, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sun, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sun, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sun, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);



	/* save default settings for statistics on monday */
	settings = config_setting_add(statistics_mon, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_mon, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_mon, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_mon, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);


	/* save default settings for statistics on tuesday */
	settings = config_setting_add(statistics_tue, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_tue, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_tue, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_tue, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);


	/* save default settings for statistics on wednsday */
	settings = config_setting_add(statistics_wed, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_wed, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_wed, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_wed, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);

	/* save default settings for statistics on thuesday */
	settings = config_setting_add(statistics_thu, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_thu, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_thu, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_thu, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);


	/* save default settings for statistics on friday */
	settings = config_setting_add(statistics_fri, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_fri, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_fri, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_fri, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);


	/* save default settings for statistics on saturday */
	settings = config_setting_add(statistics_sat, CFG_STATISTICS_TOTAL_FEEDED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sat, CFG_STATISTICS_TOTAL_STACKED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sat, CFG_STATISTICS_TOTAL_REJECTED_SHEETS, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	settings = config_setting_add(statistics_sat, CFG_STATISTICS_ERROR_RATE, CONFIG_TYPE_FLOAT);
	config_setting_set_int(settings, 0);



	/* save language settings */
	lang_index = 0;
	settings = config_setting_add(language, CFG_LANG_INDEX, CONFIG_TYPE_INT);
	config_setting_set_int(settings, 0);

	config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
}

bool controler_job_index_in_limit(int job_index)
{
	return ((job_index >=0) && (job_index < array_list_size(job_list)));
}

char * controler_return_job_string_info(char * (*q_job_fce)(q_job* job), int job_index)
{
	if(job_list != NULL)
	{
		if(controler_job_index_in_limit(job_index))
		{
			q_job * job = NULL;
			job = array_list_get(job_list, job_index);

			if(job != NULL)
				return q_job_fce(job);
		}
	}

	return NULL;
}

int controler_return_job_integer_info(int (*q_job_fce)(q_job* job), int job_index)
{
	if(job_list != NULL)
	{
		if(controler_job_index_in_limit(job_index))
		{
			q_job * job = NULL;
			job = array_list_get(job_list, job_index);

			if(job != NULL)
				return q_job_fce(job);
		}
	}

	return -1;
}

char * controler_get_job_order_name(int job_index)
{
	return controler_return_job_string_info(q_job_get_order_name, job_index);
}

void controler_lock_thread()
{
	pthread_mutex_lock(&(mutex));
}

void controler_unlock_thread()
{
	pthread_mutex_unlock(&(mutex));
}

char * controler_get_job_name(int job_index)
{
	return controler_return_job_string_info(q_job_get_job_name, job_index);
}

char * controler_get_job_pdf_name(int job_index)
{
	return controler_return_job_string_info(q_job_get_pdf_name, job_index);
}

char * controler_get_job_bkcore_csv_name(int job_index)
{
	return controler_return_job_string_info(q_job_get_bkcore_csv_name, job_index);
}

char * controler_get_job_camera_csv_name(int job_index)
{
	return controler_return_job_string_info(q_job_get_camera_csv_name, job_index);
}


char * controler_get_job_nominal_value(int job_index)
{
	return controler_return_job_string_info(q_job_get_nominal, job_index);
}

char * controler_get_job_date_time(int job_index)
{
	return controler_return_job_string_info(q_job_get_date_time, job_index);
}

char * controler_get_job_stamp_type(int job_index)
{
	return controler_return_job_string_info(q_job_get_stamp_type, job_index);
}


int controler_get_job_stamp_number(int job_index)
{
	return controler_return_job_integer_info(q_job_get_stamp_number, job_index);
}

int controler_get_job_sheet_number(int job_index)
{
	return controler_return_job_integer_info(q_job_get_sheet_number, job_index);
}

int controler_get_job_order(int job_index)
{
	return controler_return_job_integer_info(q_job_get_job_order, job_index);
}

int controler_get_job_queue_size()
{
	if(job_list != NULL)
		return array_list_size(job_list);
	else
		return 0;
}

char controler_get_job_flag(int job_index)
{
	if(controler_job_index_in_limit(job_index))
		return q_job_get_flag(array_list_get(job_list, job_index));
	else
		return 0;
}

const char * controler_get_printed_job_name()
{
	return (const char*) c_string_get_char_array(printed_job_name);
}


comm_monitor * comm_monitor_new(bool run_monitor)
{
	comm_monitor * this = malloc(sizeof(comm_monitor));
	
	this->run_monitor = run_monitor;
	this->comm_status = false;
	
	this->comm = com_tcp_new();

	
	return this;
}

bool comm_monitor_status(comm_monitor * this)
{
	return this->comm_status || this->run_monitor == false;
}

uint8_t comm_monitor_set_ip_address(comm_monitor * this, char * ip_address)
{
	return com_tcp_set_ip_addr(this->comm, ip_address);
}

uint8_t comm_monitor_set_tcp_port(comm_monitor * this, int tcp_port)
{
	return com_tcp_set_tcp_port(this->comm, tcp_port);
}

int comm_monitor_get_tcp_port(comm_monitor * this)
{
	return com_tcp_get_tcp_port(this->comm);
}

char * comm_monitor_get_ip_address(comm_monitor * this)
{
	return com_tcp_get_ip_addr(this->comm);
}

void comm_monitor_finalize(comm_monitor * this)
{
	free(this);
}


void * comm_monitor_testing_thread(void * param)
{
	comm_monitor * this = (comm_monitor*) param;

	while(this->run_monitor)
	{
		if(com_tcp_is_connected(this->comm) == STATUS_CLIENT_CONNECTED)
		{
			char * recv = com_tcp_transaction(this->comm, "ping", 255);
			
			/*
			if(strcmp(recv, "ok") != 0)
				this->comm_status = false;
			*/
	
			com_tcp_disconnect(this->comm);

			sleep(1);
		}
		else
		{
			if(com_tcp_connect(this->comm) == STATUS_CLIENT_CONNECTED)
				this->comm_status = true;
			else
				this->comm_status = false;
				
			sleep(5);
		}
	}

	return NULL;
}
