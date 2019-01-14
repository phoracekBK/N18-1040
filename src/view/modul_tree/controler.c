
#include "interface.h"
#include "controler_interface.h"


#define IO_SIMULATION true

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


#define PCI_COMM FALSE

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

/**************************************************** global variables **********************************************************/

config_t cfg_ref;

c_log * log_ref;

pthread_t machine_handler_thread;
pthread_t iij_gis_state_reading_thread;

com_tcp * iij_tcp_ref;
com_tcp * pci_tcp_ref;
	
/* connection testing, communication with network_responder utility */
com_tcp * pci_connection;
com_tcp * iij_connection;
com_tcp * quadient_connection;
	
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

c_pulse * ena_pulse;

uint8_t manual_freed;


/**************************************************** functions declarations *****************************************************/

uint8_t controler_iij_try_connect();
uint8_t controler_pci_try_connect();


void * controler_connection_testing_thread(void * param);

void controler_default_config();
uint8_t controler_load_config();
void controler_update_config(char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail);
void controler_initialize_variables();

void controler_safety_system_in();
void controler_safety_system_out();
int8_t controler_printer_abbort_print(uint8_t step);
void controler_set_machine_error(uint8_t error_code);

/* todo: make unit tests */
int8_t controler_csv_analyze();
void controler_csv_compare(char * q_csv, char * c_csv);
int32_t controler_save_response_csv(char * name);


void  controler_clear_hotfolder();
void controler_total_clear_hotfolder();
uint8_t controler_check_job_consistency(char * name,  uint8_t mod);
uint8_t controler_load_bkcore_csv();

void * controler_machine_handler(void * param);
/* todo: make unit tests */
void controler_machine_state_read_hotfolder();
void controler_machine_state_print_main();
void controler_machine_state_error();
void controler_machine_state_print_companion();
void controler_machine_state_wait_for_confirmation();
void controler_machine_state_read_csv_line();
void controler_machine_state_pause();
void controler_machine_state_clear_hotfolder();
void controler_machine_state_prepare();
void controler_machine_state_ready_to_start();
void controler_machine_state_save_q_csv();
void controler_machine_state_wait_for_print_finish();
void controler_machine_state_finish();
void controler_machine_state_print_break();

void controler_stacker_error_handler();
void controler_feeder_error_handler(uint8_t feeder_status);
void controler_machine_mode_control();
void controler_machine_stacker_counter();
void controler_machine_tab_insert_counter();
void controler_counter_check_sum();
void controler_machine_reject_counter();
void controler_machine_camera_counter();
void controler_machine_log_monitor();
int8_t controler_machine_slow_down();

void * controler_gis_runtime_state_reading(void * param);
int controler_gis_load_status_id(char ** msg);
void controler_parse_gis_status(char * status_str);




/******************************************** interface functions definitions *******************************************************/

/**
** @ingroup Core
** Constructor for core layer, define main functionality of the program.
** @return Pointer to new core class object.
*/
int8_t controler_init()
{
	uint64_t s_time = c_freq_millis();

	log_ref = c_log_new(LOG_PATH, LOG_FILE_NAME, "Inicializace jádra...");
	c_log_set_log_direction(log_ref, 0);
	
	if(log_ref != NULL)
	{
		pthread_mutex_init(&(mutex), NULL);

		io_card_ref = io_card_new();

		iij_tcp_ref = com_tcp_new();
		//pci_tcp_ref = com_tcp_new();
		iij_connection = com_tcp_new();
		pci_connection = com_tcp_new();
		quadient_connection = com_tcp_new();

	

		ti_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);
		ta_freq = c_freq_new(MACHINE_CYCLE_TIMING, unit_ms);

		info = job_info_new(JOB_INFO_CSV_PATH);


		/* initialize all core variables */
		controler_initialize_variables();

		/* load configuration from configuration file */
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


		if(pthread_create(&(pci_conn_therad), 0, controler_connection_testing_thread, pci_connection) == 0)
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači PCI vytvořeno.");
		else
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači PCI se nepodařilo vytvořit!");

		if(pthread_create(&(iij_conn_thread), 0, controler_connection_testing_thread, iij_connection) == 0)
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači IIJ vytvořeno.");
		else
			c_log_add_record_with_cmd(log_ref, "Vlákno pro sledování připojení k počítači IIJ se nepodařilo vytvořit!");

		if(pthread_create(&(quadient_conn_thread), 0, controler_connection_testing_thread, quadient_connection) == 0)
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
	com_tcp_finalize(iij_tcp_ref);
	//com_tcp_finalize(pci_tcp_ref);
	
	config_write_file(&(cfg_ref), CONFIGURATION_FILE_PATH);
	config_destroy(&(cfg_ref));


	array_list_destructor(&(job_list));
	array_list_destructor(&(job_list_pre));

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



uint8_t controler_iij_network_connected()
{
	return com_tcp_is_connected(iij_connection);
}

uint8_t controler_pci_network_connected()
{
	return com_tcp_is_connected(pci_connection);
}

uint8_t controler_quadient_network_connected()
{
	return com_tcp_is_connected(quadient_connection);
}

void controler_sheet_source_confirmation()
{
	print_confirmation_req = true;
}

uint8_t controler_machine_status_val()
{
	return machine_state;
}

uint8_t controler_print_start(const char * job_name)
{
	uint8_t status = STATUS_SUCCESS;
	int job_name_len = strlen(job_name);

	if(c_string_len(printed_job_name) == 0)
	{
		if(job_name_len > 0)
		{
			c_string_set_string(printed_job_name, (char*) job_name);
			
			int ttl = 0;
			while(((machine_state == MACHINE_STATE_WAIT)) && ttl < 5)
			{
				machine_print_req = true;
				usleep(HOT_FOLDER_READING_INTERVAL);
				ttl++;
			}

			machine_print_req = false;
	
			if(ttl >= 5)
			{
				status = STATUS_GENERAL_ERROR;
				c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěný!");
			}
			else
			{
				if(machine_state == MACHINE_STATE_ERROR)
				{
					status = STATUS_GENERAL_ERROR;
					c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku zkončil chybou: %d - %s", error_code, (char*) controler_get_error_str());
				}
				else
				{
					c_log_add_record_with_cmd(log_ref, "Spuštěn tisk jobu %s.", c_string_get_char_array(printed_job_name));
				}
			}	
		}
		else
		{
			status = STATUS_GENERAL_ERROR;
			c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěšný! Nebylo specifikováno jméno jobu.");
		}
	}
	else
	{
		status = STATUS_GENERAL_ERROR;
		c_log_add_record_with_cmd(log_ref, "Pokus o spuštění tisku neúspěšný! Tisk jiného jobu již běží (%s).", c_string_get_char_array(printed_job_name));
	}

	return status;
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
	
		while((ttl < 3) || (machine_state != MACHINE_STATE_PAUSE))
		{
			usleep(HOT_FOLDER_READING_INTERVAL/1000);
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
			c_log_add_record_with_cmd(log_ref, "Obnovení jobu neúspěšné. Obslužné vlákno se nachízí v chybovém stavu: %d - %s", machine_state, controler_get_error_str());
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

	while((machine_state == MACHINE_STATE_ERROR) && (ttl < 2))
	{
		machine_error_reset_req = true;
		usleep(HOT_FOLDER_READING_INTERVAL);
		ttl++;
	}
						
	machine_error_reset_req = false;

	if(machine_state == MACHINE_STATE_ERROR)
	{
		c_log_add_record_with_cmd(log_ref, "Pokus o reset chybového stavu neúspěšný. Obslužné vlákno se nachází v chybě: %d - %s", error_code, controler_get_error_str());
		res = STATUS_GENERAL_ERROR;
	}
	else
	{	
		c_log_add_record_with_cmd(log_ref, "Reset chybového stavu.");
	}
	
	return res;
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




char ** controler_get_a1_input_labels()
{
	return (char**) io_card_get_a1_in_labels();
}

char ** controler_get_a2_input_labels()
{
	return (char **) io_card_get_a1_in_labels();
}

char ** controler_get_a1_output_labels()
{
	return (char **) io_card_get_a1_out_labels();
}

char ** controler_get_a2_output_labels()
{
	return (char**) io_card_get_a1_out_labels();
}


uint8_t controler_get_card_input(int card, int addr)
{
	return io_card_get_input(io_card_ref, card, addr);
}


uint8_t controler_get_card_output(int card, int addr)
{
	return io_card_get_output(io_card_ref, card, addr);
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
			CFG_HOT_QUADIENT_MAIN, 
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
			CFG_HOT_QUADIENT_MAIN, 
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
	return com_tcp_get_ip_addr(quadient_connection);
}

char * controler_iij_network_get_ip_address()
{
	return com_tcp_get_ip_addr(iij_connection);
}

char * controler_pci_network_get_ip_address()
{
	return com_tcp_get_ip_addr(pci_connection);
}


uint8_t controler_quadient_network_set_ip_address(char * ip_address)
{
	if(controler_quadient_network_connected() != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = com_tcp_set_ip_addr(quadient_connection, ip_address);
		
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
	if(controler_iij_network_connected() != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = com_tcp_set_ip_addr(iij_connection, ip_address);
		
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
	if(controler_pci_network_connected() != STATUS_CLIENT_CONNECTED)
	{
		uint8_t res = com_tcp_set_ip_addr(pci_connection, ip_address);
		
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
		c_log_add_record_with_cmd(log_ref, "Spojení s GISem navázáno.");
	else
		c_log_add_record_with_cmd(log_ref, "Spojení s GISem nebylo navázáno!");

	return res;
}

uint8_t controler_iij_is_connected()
{
	return com_tcp_is_connected(iij_tcp_ref);
}

uint8_t controler_iij_disconnect()
{
	com_tcp_disconnect(iij_tcp_ref);
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
		/* read all inputs */
#if IO_SIMULATION == FALSE
		io_card_sync_inputs(io_card_ref);
		controler_safety_system_in();
#endif

		controler_machine_log_monitor();

		if((machine_state == MACHINE_STATE_WAIT) || (machine_state == MACHINE_STATE_NEXT))
			controler_machine_state_read_hotfolder();

		if(machine_state == MACHINE_STATE_PREPARE)
			controler_machine_state_prepare();
		
		if(machine_state == MACHINE_STATE_READY_TO_START)
			controler_machine_state_ready_to_start();

		if(machine_state == MACHINE_STATE_READ_CSV_LINE)
			controler_machine_state_read_csv_line();

		if(machine_state == MACHINE_STATE_WAIT_FOR_CONFIRMATION)
			controler_machine_state_wait_for_confirmation();

		if(machine_state == MACHINE_STATE_PRINT_COMPANION)
			controler_machine_state_print_companion();

		if(machine_state == MACHINE_STATE_PRINT_MAIN)
			controler_machine_state_print_main();
		
		if(machine_state == MACHINE_STATE_PAUSE)
			controler_machine_state_pause();

		if(machine_state == MACHINE_STATE_WAIT_FOR_PRINT_FINISH)
			controler_machine_state_wait_for_print_finish();

		if(machine_state == MACHINE_STATE_SAVE_Q_CSV)
			controler_machine_state_save_q_csv();

		if(machine_state == MACHINE_STATE_PRINT_BREAK)
			controler_machine_state_print_break();

		if((machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER) || (machine_state == MACHINE_STATE_CLEAR_TO_FINISH))
		{
			controler_machine_state_clear_hotfolder();
		}

		if((machine_state == MACHINE_STATE_PRINT_FINISH) || (machine_state == MACHINE_STATE_JOB_FINISH))
			controler_machine_state_finish();

		/* machine error state handling */
		if(machine_state == MACHINE_STATE_ERROR)
			controler_machine_state_error();

#if IO_SIMULATION == FALSE
		/* write all outputs */
		controler_safety_system_out();
		io_card_sync_outputs(io_card_ref);
#endif
		if((machine_state == MACHINE_STATE_WAIT) || (machine_state == MACHINE_STATE_ERROR) || 
			(machine_state == MACHINE_STATE_NEXT) || (machine_state == MACHINE_STATE_PAUSE))
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

void controler_machine_state_error()
{
	if(machine_error_reset_req == true)
	{
		error_code = MACHINE_ERR_NO_ERROR;
		machine_state = MACHINE_STATE_PRINT_BREAK;
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

		controler_update_config(CFG_GROUP_PRINT_PARAMS, 
				CFG_PP_GR_MACHINE_MODE, 
				CONFIG_TYPE_INT, 
				&mode, 
				"Nastavení režimu Gremser dopravníku aktualizováno.", 
				"Nepodařilo se aktualizovat režim Gremser dopravníku!");
	}
}


void controler_refresh_dir_list()
{
	pthread_mutex_lock(&(mutex));

	job_list_changed = 1;
	
	pthread_mutex_unlock(&(mutex));
}

void controler_machine_state_read_hotfolder()
{
	if(machine_print_req == false)
	{
		if(job_list != NULL)
			array_list_destructor_with_release_v2(job_list, q_job_finalize);
			
		job_list = hot_read_dir_content(q_hotfolder_main_path);	

		
		if(hot_compare_job_lists(job_list, job_list_pre) > 0)
		{
			if(job_list_pre != NULL)
				array_list_destructor_with_release_v2(job_list_pre, q_job_finalize);	

			job_list_pre = hot_copy_job_list(job_list);
			job_list_changed = 1;
		}
	}
	else if((machine_cancel_req) == true && (machine_state == MACHINE_STATE_NEXT))
	{
		machine_state = MACHINE_STATE_PRINT_BREAK;
	}

	if(c_string_len(printed_job_name) > 0)
	{
		if(((machine_print_req == true) && (machine_state == MACHINE_STATE_WAIT)) || (machine_state == MACHINE_STATE_NEXT))
		{
			int job_index = -1;
			q_job * job = hot_find_job(job_list, c_string_get_char_array(printed_job_name), &job_index);

			if((job != NULL) && (q_job_get_bkcore_csv_name(job) != NULL))
			{	
				if(q_job_get_job_order(job) == (job_info_get_job_index(info)+2))
				{
					if(q_job_get_flag(job) == 'e')
					{
						machine_state = MACHINE_STATE_PRINT_BREAK;
						printed_job_index = job_index;
					}
					else
					{
						if((q_job_get_pdf_name(job) != NULL) && (q_job_get_camera_csv_name(job) != NULL))
						{	
							if(machine_state == MACHINE_STATE_WAIT)
							{
								job_info_set_order_name(info, q_job_get_job_name(job));
								job_info_generate_csv_name(info);
							}
	
							printed_job_index = job_index;
							job_info_add_job_record(info);
							machine_state = MACHINE_STATE_PREPARE;
						}
					}
				}
				else
				{
					controler_set_machine_error(MACHINE_ERR_JOB_ORDER_MISMATCH);
				}
			}
		}
	}
}
void controler_machine_state_pause()
{
	/* from pause state can return to print state only if the conditions are met */
	if(machine_pause_req == false)
	{
		if((stacked_sheet_counter < 2500) && 
		(io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_1_RJ_full) == 0) && 
		(io_card_get_input(io_card_ref,IO_CARD_A2, A2_IN_2_SN_full) == 0))
		{
			machine_state = MACHINE_STATE_READ_CSV_LINE;
		}
	}
}

void controler_machine_state_prepare()
{
	/* load bkcore csv content */
	if(controler_load_bkcore_csv() == STATUS_SUCCESS)
	{	
		q_job * job = array_list_get(job_list, printed_job_index);
		
		uint8_t status = 0;

		/* copy pdf to gis hotfolder or based on MB0 input status */
		if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
			status = util_copy_file(c_string_get_char_array(q_hotfolder_main_path), 
									c_string_get_char_array(gis_hotfolder_path), 
									q_job_get_pdf_name(job));
		
		if(status == 0)
		{
			if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
				status = util_copy_file(c_string_get_char_array(q_hotfolder_main_path), 
										c_string_get_char_array(pci_hotfolder_in_path), 
										q_job_get_camera_csv_name(job));

			if(status == 0)
			{
				machine_state = MACHINE_STATE_READY_TO_START;
				timer = c_freq_millis();

				/* prepare job info structure for current job */
				int rows = 0, csv_line_index = 0, csv_pos = 0;
				char sheet_index[255];
				int total_sheet_number = 0;
				int total_stamp_number = 0;

				while(bkcore_csv_content[csv_pos] != 0)
				{
					if(isdigit(bkcore_csv_content[csv_pos]))	
					{
						if((rows == 0) && (q_job_get_flag(job) == 'k'))
						{
							total_stamp_number = ((total_stamp_number*10) + (bkcore_csv_content[csv_pos]-'0'));
						}	
						else if((rows == 1) && (q_job_get_flag(job) == 'k'))
						{
							if(q_job_get_job_order(job) == 1)
								total_sheet_number = ((total_sheet_number*10) + 
												(bkcore_csv_content[csv_pos]-'0'));					
						}
						else
						{
							printf("%c", bkcore_csv_content[csv_pos]);
							sheet_index[csv_line_index] = bkcore_csv_content[csv_pos];
							csv_line_index++;
						}
					}

					if((bkcore_csv_content[csv_pos]) == '\n')
					{
						if((rows > 1) || (q_job_get_flag(job) != 'k'))
						{	
							sheet_index[csv_line_index] = 0;
							job_info_add_sheet_record(info, sheet_index);
							csv_line_index = 0;
						}
						
						rows ++;
					}

					csv_pos++;
				}

				job_info_set_total_sheet_number(info, total_sheet_number);
				job_info_set_total_stamp_number(info, total_stamp_number);

				if(csv_line_index > 0)
				{
					printf("add sheet record\n");
					sheet_index[csv_line_index] = 0;
					job_info_add_sheet_record(info, sheet_index);
				}
			}
			else
				controler_set_machine_error(MACHINE_ERR_CANT_COPY_CSV_TO_CAMERA);
		}			
		else
			controler_set_machine_error(MACHINE_ERR_CANT_COPY_PDF_TO_GIS);
	}
	else
	{
		controler_set_machine_error(MACHINE_ERR_CANT_LOAD_BKCORE_CSV);
	}
}
int8_t controler_machine_slow_down()
{
	int x = 10;
	int8_t slmx = (stacked_sheet_counter < (max_stacked_sheets - x));
	int8_t sgemx = (stacked_sheet_counter >= (max_stacked_sheets - x));
	int8_t sgem = !(stacked_sheet_counter >= max_stacked_sheets);
	int8_t fes = ((feeded_main_sheet_counter+feeded_companion_sheet_counter) == stacked_sheet_counter+rejected_sheet_counter);

	return ((!slmx) && sgemx && sgem && fes) || (slmx && (!sgemx) && sgem);
}

void controler_machine_state_read_csv_line()
{
	/* if is set pause request, move to pause state after feed current sheet */
	if((machine_pause_req == true))
	{
		machine_state = MACHINE_STATE_PAUSE;
	}
	else if(machine_cancel_req == true)
	{
		machine_state = MACHINE_STATE_PRINT_BREAK;
	}
	else
	{
		if(controler_machine_slow_down() > 0)
		{
			/* load line from csv */
			q_job * job = array_list_get(job_list, printed_job_index);
		
			/* process csv line */
			if(bkcore_csv_pos < job_info_get_job_sheet_number(info, job_info_get_job_index(info)))
			{
				if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) == 0)
					job_info_set_sheet_record_result(info, "PASS", bkcore_csv_pos);


				if((q_job_get_flag(job) == 'k') || ((q_job_get_flag(job) == 'p') && 
					(sheet_source_confirmation == false) && (companion_sheet_source == SSOURCE_MAIN))) 
				{
					machine_state = MACHINE_STATE_PRINT_MAIN;
					printf("go to print main\n");
				}
				else if((q_job_get_flag(job) == 'p') && (companion_sheet_source == SSOURCE_COMPANION))
				{
					printf("go to print compation\n");
					machine_state = MACHINE_STATE_PRINT_COMPANION;
				}
				else if((q_job_get_flag(job) == 'p') && (companion_sheet_source == SSOURCE_MAIN) && (sheet_source_confirmation == true))
				{
					printf("go to wait for confirmation\n");
					machine_state = MACHINE_STATE_WAIT_FOR_CONFIRMATION;
				}
				else
				{
					controler_set_machine_error(MACHINE_ERR_SHEET_FEEDER_REDIRECTION);
				}

				bkcore_csv_pos++;
			}
			else
			{
				machine_state = MACHINE_STATE_WAIT_FOR_PRINT_FINISH;
			}
		}
		else if(stacked_sheet_counter >= max_stacked_sheets)
		{
			machine_state = MACHINE_STATE_PAUSE;
		}
	}
}



void controler_machine_state_wait_for_confirmation()
{
	if(print_confirmation_req == true)
	{
		machine_state = MACHINE_STATE_PRINT_MAIN;
		print_confirmation_req = false;
	}
}


void controler_machine_state_print_main()
{
	uint8_t feeder_status = 0; 

	/* gremser machine control */
	feeder_status = io_card_get_bit_value(io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2); 
	
	/* wait for feeder ready */	
	if((feed_sheet == 0))
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
			//feed_sheet = 3;	//only for testing skip the point 2
			timer = c_freq_millis();
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
		}

		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
	}
	else if((feed_sheet == 3) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		if(timer + 700 < c_freq_millis())
		{
			feed_sheet = 0;
			feeded_main_sheet_counter++;
			machine_state = MACHINE_STATE_READ_CSV_LINE;
		}

		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
	}

	/* feedeng error handling */
	controler_feeder_error_handler(feeder_status);
}

void controler_machine_state_print_companion()
{
	/* check the state of companion feeder, TI_incyc must be in false state */
	if((feed_sheet == 0) && (io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) == 0))
	{
		/* set output TI_ins to true state */
		io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 1);
		timer = c_freq_millis();
		feed_sheet = 1;
	}
	else if(feed_sheet == 1)
	{	
		/* wait for TI_incyc true state or given time */
		if(((timer+MACHINE_XBF_INTERVAL) <= c_freq_millis()))
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 0);
			feed_sheet = 2;
			//feed_sheet = 3; // only for testing
		}

		if(io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) > 0)
		{
			io_card_set_output(io_card_ref, IO_CARD_A1, A1_OUT_9_TI_ins, 0);
			feed_sheet = 3;
		}
	}
	else if(feed_sheet == 2)
	{	
		/* wait for TI_incyc true state */
		if(io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) > 0)
		{
			feed_sheet = 3;
		}
	}
	else if((feed_sheet == 3) && (io_card_get_input(io_card_ref, IO_CARD_A2, A2_IN_0_TI_incyc) == 0))
	{
		/* wait for TI_incyc true state, incremet the counter state and go to READ_CSV_LINE state */	
		feed_sheet = 0;
		feeded_companion_sheet_counter++;
		machine_state = MACHINE_STATE_READ_CSV_LINE;
	}
}

void controler_machine_state_save_q_csv()
{
	if((timer+TIME_DELAY_ON_JOB_END) < c_freq_millis())
	{
		q_job * job = array_list_get(job_list, printed_job_index);
		
		if(job != NULL)
		{
			/* if the camera is disabled don't do the camera and quadient csv analyzing */
			if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
			{
				/* end job in Job */
				if(controler_csv_analyze() == STATUS_SUCCESS)
				{	 
					if(controler_save_response_csv(q_job_get_bkcore_csv_name(job)) >= 0)
						machine_state = MACHINE_STATE_CLEAR_HOT_FOLDER;
					else
						controler_set_machine_error(MACHINE_ERR_CANT_SAVE_F_CSV);
				}
				else
				{
					controler_set_machine_error(MACHINE_ERR_ANALYZE_CAMERA_CSV);
				}
			}
			else
			{
				/* save empty response csv */
				util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");
				machine_state = MACHINE_STATE_CLEAR_HOT_FOLDER;
			}
		}
		else
		{
			controler_set_machine_error(MACHINE_ERR_CANT_SAVE_F_CSV);
		}
	}
}


int32_t controler_save_response_csv(char * name)
{
	if(feedback_csv != NULL)
	{	
		int32_t res = util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), name, c_string_get_char_array(feedback_csv));

		util_save_csv("/home/stc", name, c_string_get_char_array(feedback_csv));
		c_string_finalize(&(feedback_csv));

		return res;
	}

	return -1;
}



void controler_machine_state_ready_to_start()
{
	uint8_t feeder_status = io_card_get_bit_value(io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2); 

	if(((strcmp(c_string_get_char_array(print_controler_status), "Printing") == 0) || 
		(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) == 0)) && 
		(feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		if((timer + 1000) < c_freq_millis())
		{
			c_pulse_reset(ena_pulse);
			machine_state = MACHINE_STATE_READ_CSV_LINE;
		}
	}
}

void controler_machine_state_wait_for_print_finish()
{	
	//if((feeded_main_sheet_counter+feeded_companion_sheet_counter)  == (stacked_sheet_counter + rejected_sheet_counter))
	{
		timer = c_freq_millis();
		machine_state = MACHINE_STATE_SAVE_Q_CSV;
	}
}

void controler_machine_state_print_break()
{
	if(printed_job_index >= 0)
	{
		q_job * job = array_list_get(job_list, printed_job_index);

		if(job != NULL)
		{
			/* wait for machine done the printing */
			//if((feeded_main_sheet_counter+feeded_companion_sheet_counter)  == (stacked_sheet_counter + rejected_sheet_counter))
			{
				/* analyze the output csv */
				if(q_job_get_flag(job) != 'e')
				{
					job_info_set_end_status(info, false);

					if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
					{
						controler_csv_analyze();
						job_info_generate_missing_sheet_records(info);
					}
				}

				/* save feedback csv file with 'e' flag */
				char * const_char = "_xxxx_e_bkcore.csv";
				char * csv_name = (char*) malloc(sizeof(char)*(strlen(q_job_get_job_name(job))+strlen(const_char)+1));

				sprintf(csv_name, "%s_%d_e_bkcore_.csv", q_job_get_job_name(job), q_job_get_job_order(job));
				util_save_csv(c_string_get_char_array(q_hotfolder_feedback_path), csv_name, "");

				free(csv_name);
		
				if(io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
					controler_printer_abbort_print(0);

				/* end Job */
				machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
			}
		}
		else
		{
			machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
		}
	}
	else
	{
		/* end Job */
		machine_state = MACHINE_STATE_CLEAR_TO_FINISH;
	}
}


void controler_clear_hotfolder_base(q_job * job)
{
	if(job != NULL)
	{
		if(q_job_get_pdf_name(job) != NULL)
			util_move_file(c_string_get_char_array(q_hotfolder_main_path), c_string_get_char_array(q_hotfolder_backup_path), q_job_get_pdf_name(job));

		if(q_job_get_bkcore_csv_name(job) != NULL)
			util_move_file(c_string_get_char_array(q_hotfolder_main_path), c_string_get_char_array(q_hotfolder_backup_path), q_job_get_bkcore_csv_name(job));

		if(q_job_get_camera_csv_name(job) != NULL)
			util_move_file(c_string_get_char_array(q_hotfolder_main_path), c_string_get_char_array(q_hotfolder_backup_path), q_job_get_camera_csv_name(job));
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
	for(int i = 0; i < array_list_size(job_list); i++)
	{
		q_job * job = (q_job*) array_list_get(job_list, i);
		controler_clear_hotfolder_base(job);
	}
}



void controler_machine_state_clear_hotfolder()
{
	/* delete csv from camera hotfolder */
	if((printed_job_index >= 0) && (io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0))
	{
		q_job * job = array_list_get(job_list, printed_job_index);
		if(job != NULL)
			util_delete_file(c_string_get_char_array(pci_hotfolder_out_path), q_job_get_camera_csv_name(job));
	}

	if((c_string_get_char_array(printed_job_name) != NULL))
		controler_clear_hotfolder();

	if(machine_state == MACHINE_STATE_CLEAR_HOT_FOLDER)
	{
		machine_state = MACHINE_STATE_JOB_FINISH;
	}
	else
	{
		machine_state = MACHINE_STATE_PRINT_FINISH;
	}
}


void controler_machine_state_finish()
{	
	printed_job_index = -1;
	bkcore_csv_pos = 0;

	feed_sheet = 0;

	sn_trig = 0;					
	camera_trig = 0;					
	rj_trig = 0;					
	ti_trig = 0;
	
	c_pulse_reset(ena_pulse);

	machine_cancel_req = false;
	machine_print_req = false;
	machine_pause_req = false;
	print_confirmation_req = false;

	if(machine_state == MACHINE_STATE_JOB_FINISH)
	{
		machine_state = MACHINE_STATE_NEXT;
	}
	else
	{
		char * time_date = util_get_time_string();
		job_info_generate_csv(info, time_date);
		free(time_date);
		job_info_clear(info);
		
		c_string_clear(printed_job_name);

		feeded_main_sheet_counter = 0;
		feeded_companion_sheet_counter = 0;
		stacked_sheet_counter = 0;
		rejected_sheet_counter = 0;
		rejected_sheet_seq_counter = 0;
		tab_insert_counter = 0;
		inspected_sheet_counter = 0;

		machine_state = MACHINE_STATE_WAIT;		
	}
}


int32_t controler_get_stacked_sheets()
{
	return stacked_sheet_counter;
}


int32_t controler_get_total_sheet_number()
{
	return job_info_get_total_sheet_number(info);
}

int32_t controler_get_feeded_sheets()
{
	return feeded_main_sheet_counter;
}


int32_t controler_get_feeded_companion_sheets()
{
	return feeded_companion_sheet_counter;
}


int32_t controler_get_rejected_sheets()
{
	return rejected_sheet_counter;
}

int32_t controler_get_tab_inserts()
{
	return tab_insert_counter;
}

void controler_set_machine_error(uint8_t error_code)
{
	machine_state = MACHINE_STATE_ERROR;
	error_code = error_code;
}


void controler_feeder_error_handler(uint8_t feeder_status)
{
	if(feeder_status == MACHINE_FN_DOUBLE_SHEET)
	{
		controler_set_machine_error(MACHINE_ERR_FEEDER_DOUBLE_SHEET);
	}

	if(feeder_status == MACHINE_FN_MISSING_SHEET)
	{
		controler_set_machine_error(MACHINE_ERR_FEEDER_SHEET_MISSING);
	}

	if(feeder_status == MACHINE_FN_FEEDER_MULTIFUNCTION)
	{
		controler_set_machine_error(MACHINE_ERR_FEEDER_MULTIFUNCTION);
	}
	
	if(feeder_status == MACHINE_FN_OFF)
	{
		controler_set_machine_error(MACHINE_ERR_FEEDER_OFF);
	}
	
	if(feeder_status == MACHINE_FN_E_STOP)
	{
		controler_set_machine_error(MACHINE_ERR_FEEDER_E_STOP);
	}
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
int8_t controler_csv_analyze()
{
	q_job * job = (q_job*) array_list_get(job_list, printed_job_index);
	
	if(job != NULL)
	{
		char * q_csv = util_load_csv(c_string_get_char_array(q_hotfolder_main_path), q_job_get_camera_csv_name(job), NULL);
		char * c_csv = util_load_csv(c_string_get_char_array(pci_hotfolder_out_path), q_job_get_camera_csv_name(job), NULL);
	
		feedback_csv = c_string_new();

		if((q_csv != NULL) && (c_csv != NULL))
		{
			controler_csv_compare(q_csv, c_csv);
	
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


/*
** utility function for avoid duplicity code
** writes the index of wrong recognized sheet to output csv file for quadient
*/
void controler_write_to_csv(c_string * feedback_csv, job_info * info, uint32_t index)
{
	info_struct* sheet_info = job_info_get_sheet_info(info, job_info_get_job_index(info), index);
	c_string_add(feedback_csv, (char*) sheet_info->sheet_order);
			
	if(index+1 < job_info_get_job_sheet_number(info, job_info_get_job_index(info)))
		c_string_concat(feedback_csv, "\n");
}

/*
** utility function for avoid duplicity code
** if some part of the csv file from camera missing then fill the output csv for quadient remaining 
** sheet indexes.
*/
void controler_csv_fill_missing(c_string * feedback_csv, job_info * info, uint32_t * index)
{
	while(*index < job_info_get_job_sheet_number(info, job_info_get_job_index(info)))
	{
		controler_write_to_csv(feedback_csv, info, *index);
		job_info_set_sheet_record_result(info, "FAIL", *index);
		(*index)++;
	}
}

void controler_csv_compare(char * q_csv, char * c_csv)
{
	uint32_t q_pos = 0, c_pos = 0, index = 0;

	while((q_csv[q_pos] != 0))
	{		
		/* new line finding */
		if((q_csv[q_pos] == '\n'))
		{
			job_info_set_sheet_record_result(info, "PASS", index);

			while(c_csv[c_pos] != q_csv[q_pos])
			{
				if(c_csv[c_pos] != 0)
				{
					c_pos++;
				}
				else
				{
					index++;
					controler_csv_fill_missing(feedback_csv, info, &index);
					return;
				}
			}
			index++;
		}
		
		/* characters testing */
		if(c_csv[c_pos] != q_csv[q_pos])
		{
			controler_write_to_csv(feedback_csv, info, index);
			job_info_set_sheet_record_result(info, "FAIL", index);

			/* jump to end of line */
			while((q_csv[q_pos] != '\n') || (c_csv[c_pos] != '\n'))
			{
				/* if the part on the end of the camera csv file missing */
				if(c_csv[c_pos] == 0)
				{
					index++;
					controler_csv_fill_missing(feedback_csv, info, &index);
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



rep_struct * hot_load_report_information(char * report_csv_name)
{	
	rep_struct * job_report = NULL;
	char * report_csv_content = util_load_csv(controler_get_job_report_hotfolder_path(), report_csv_name, NULL);

	if(report_csv_content != NULL)
	{
		job_report = (rep_struct *) malloc(sizeof(rep_struct));
		int pos = 0, par = 0;
		bool read_enable = false;
		int temp_int_val = 0;
		c_string * buffer = c_string_new();

		while((report_csv_content[pos] != 0) && (par < 6))
		{
			switch(par)
			{
				case 0:
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
						c_string_add_char(buffer, report_csv_content[pos]);	
				break;

				case 1:
					if(report_csv_content[pos] == '\n')
					{
						job_report->job_name = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->finish_state, c_string_get_char_array(buffer));
					}
					else
					{
						c_string_add_char(buffer, report_csv_content[pos]);
					}
					break;

				case 2:
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
		
				case 3:
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
		
				case 4:
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
	
				case 5:
					if(report_csv_content[pos] == '\n')
					{
						job_report->job_name = malloc(sizeof(char) * (c_string_len(buffer)+1));
						strcpy(job_report->date_time, c_string_get_char_array(buffer));
					}
					else if((report_csv_content[pos] == ':') && (read_enable == false))
					{
						read_enable = true;
						pos++;
					}
					else if(read_enable == true)
						c_string_add_char(buffer, report_csv_content[pos]);	
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




char* controler_machine_get_state_str()
{
	char * status_str = NULL;

	switch(machine_state)
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



void controler_machine_log_monitor()
{
	if(machine_state != machine_state_pre)
		c_log_add_record_with_cmd(log_ref, "%d - %s", machine_state, controler_machine_get_state_str());

	machine_state_pre = machine_state;
}


void controler_counter_check_sum()
{
	int32_t non_stacked = inspected_sheet_counter - stacked_sheet_counter - stacked_sheet_counter;
	int32_t rest = (feeded_main_sheet_counter + feeded_companion_sheet_counter) - (stacked_sheet_counter + rejected_sheet_counter);

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


uint8_t controler_load_bkcore_csv()
{
	q_job * job = (q_job*) array_list_get(job_list, printed_job_index);

	if(job != NULL)
	{
		bkcore_csv_content = util_load_csv(c_string_get_char_array(q_hotfolder_main_path), 
										q_job_get_bkcore_csv_name(job),
										 &(bkcore_csv_size));

		if(bkcore_csv_content != NULL)
		{
			c_log_add_record_with_cmd(log_ref, "Načten obsah bkcore csv souboru.");
			return STATUS_SUCCESS;
		}
	}

	c_log_add_record_with_cmd(log_ref, "obsah souboru bkcore csv nelze načíst!");

	return STATUS_GENERAL_ERROR;
}

void * controler_gis_runtime_state_reading(void * param)
{
	com_tcp_send(iij_tcp_ref, "P,L\n");
	int id = -1;
	uint64_t timer = 0;

	while(controler_iij_is_connected() == STATUS_CLIENT_CONNECTED)
	{
		char * state_msg = com_tcp_recv(iij_tcp_ref);
		
		if(state_msg != NULL)
		{
			timer = c_freq_millis();

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
#if 0
		if((timer+60000) <= c_freq_millis())
			controler_iij_disconnect();
#endif
	}
	
	c_string_set_string(print_controler_status, "Unknown");
	controler_iij_disconnect();
	
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
		{
			pthread_create(&(iij_gis_state_reading_thread), 0, &controler_gis_runtime_state_reading, NULL);
		}
		else
		{
			res = STATUS_CONNECTION_ERROR;
		}
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
		com_tcp_set_ip_addr(quadient_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		com_tcp_set_tcp_port(quadient_connection, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_PCI_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		com_tcp_set_ip_addr(pci_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_PCI_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		com_tcp_set_tcp_port(pci_connection, setting_val_int);
	else
		return 10;
	
	if(config_setting_lookup_string(settings, CFG_NETWORK_IIJ_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		com_tcp_set_ip_addr(iij_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_IIJ_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		com_tcp_set_tcp_port(iij_connection, setting_val_int);
	else
		return 10;

	if(config_setting_lookup_string(settings, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, &setting_val_str) == CONFIG_TRUE)
		com_tcp_set_ip_addr(quadient_connection, (char*) setting_val_str);
	else
		return 9;
	
	if(config_setting_lookup_int(settings, CFG_NETWORK_QUADIENT_PC_TCP_PORT, &setting_val_int) == CONFIG_TRUE)
		com_tcp_set_tcp_port(quadient_connection, setting_val_int);
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
		return 14;


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
	

	/* load language settings */
	settings = config_lookup(&(cfg_ref), CFG_GROUP_LANGUAGE);

	if(config_setting_lookup_int(settings, CFG_LANG_INDEX, &setting_val_int) == CONFIG_TRUE)
		lang_index = setting_val_int;
	else
		return 19;


	return 0;
}

void controler_initialize_variables()
{

		/* machine handler status and control variables */
		machine_state = MACHINE_STATE_PRINT_FINISH;
		machine_state_pre = MACHINE_STATE_WAIT;
		printed_job_name = c_string_new();
		machine_pause_req = false;
		machine_print_req = false;
		machine_error_reset_req = false;
		machine_cancel_req = false;

		error_code = 0;
		//machine_mode = io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) + 2*io_card_get_input(io_card_ref, IO_CARD_A1, A1_IN_1_MBR1);

		job_list = array_list_new();
		job_list_pre = array_list_new();
		job_list_changed = 0;

		ena_pulse = c_pulse_new();
		manual_freed = 0;

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

		feeded_main_sheet_counter = 0;
		feeded_companion_sheet_counter = 0;
		stacked_sheet_counter = 0;
		rejected_sheet_counter = 0;
		rejected_sheet_seq_counter = 0;
		tab_insert_counter = 0;
		inspected_sheet_counter = 0;

	
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
	config_setting_t *root, *hotfolder, *network, *print_params, *language, *settings;

	/* create root settings */
	root = config_root_setting(&(cfg_ref));

	/* create groups */
	hotfolder = config_setting_add(root, CFG_GROUP_HOTFOLDER, CONFIG_TYPE_GROUP);
	network = config_setting_add(root, CFG_GROUP_NETWORK, CONFIG_TYPE_GROUP);
	print_params = config_setting_add(root, CFG_GROUP_PRINT_PARAMS, CONFIG_TYPE_GROUP);
	language = config_setting_add(root, CFG_GROUP_LANGUAGE, CONFIG_TYPE_GROUP);

	
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

	com_tcp_set_tcp_port(quadient_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	com_tcp_set_ip_addr(quadient_connection, DEFAULT_IP_ADDRESS_QUADIENT);
	settings = config_setting_add(network, CFG_NETWORK_QUADIENT_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_QUADIENT);

	com_tcp_set_tcp_port(pci_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	com_tcp_set_ip_addr(pci_connection, DEFAULT_IP_ADDRESS_PCI);
	settings = config_setting_add(network, CFG_NETWORK_PCI_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_PCI);

	com_tcp_set_tcp_port(iij_connection, DEFAULT_NETWORK_RESPONDER_TCP_PORT);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_TCP_PORT, CONFIG_TYPE_INT);
	config_setting_set_int(settings, DEFAULT_NETWORK_RESPONDER_TCP_PORT);

	com_tcp_set_ip_addr(iij_connection, DEFAULT_IP_ADDRESS_GIS);
	settings = config_setting_add(network, CFG_NETWORK_IIJ_PC_IP_ADDRESS, CONFIG_TYPE_STRING);
	config_setting_set_string(settings, DEFAULT_IP_ADDRESS_GIS);


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
	if(controler_job_index_in_limit(job_index))
	{
		return q_job_fce(array_list_get(job_list, job_index));
	}

	return NULL;
}

int controler_return_job_integer_info(int (*q_job_fce)(q_job* job), int job_index)
{
	if(controler_job_index_in_limit(job_index))
	{
		return q_job_fce(array_list_get(job_list, job_index));
	}

	return -1;
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

char * controler_get_job_date_time(int job_index)
{
	return controler_return_job_string_info(q_job_get_date_time, job_index);
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
	return array_list_size(job_list);
}

char controler_get_job_flag(int job_index)
{
	if(controler_job_index_in_limit(job_index))
	{
		return q_job_get_flag(array_list_get(job_list, job_index));
	}
	else
	{
		return 0;
	}
}

const char * controler_get_printed_job_name()
{
	return (const char*) c_string_get_char_array(printed_job_name);
}

void * controler_connection_testing_thread(void * param)
{
	com_tcp * this = (com_tcp*) param;

	while(true)
	{
		if(com_tcp_is_connected(this) == STATUS_CLIENT_CONNECTED)
		{
			char * recv = com_tcp_transaction(this, "ping", 255);

			if(recv == NULL)			
				com_tcp_disconnect(this);	
			else
		//		printf("%s - %s\n", com_tcp_get_ip_addr(this), recv);

			sleep(1);
		}
		else
		{
			com_tcp_connect(this);
			sleep(5);
		}
	}

	return NULL;
}



/************************************************** unit testing ***********************************************/


uint8_t machine_functions_suite_case()
{
	uint8_t res = 0;



	return res;	
}


void controler_unit_test()
{
	if(cu_initialize("./unit_test_log", "test_log") == CU_SUCCESS)
	{
		printf("C unit library successfully initialized\n");
		cu_add_suite_case("Machine functions test ", machine_functions_suite_case);

		cu_add_suite_case("Job info constructor test", job_info_constructor_suite_case);

		cu_run_test();
		cu_finalize();
	}
	else
	{
		fprintf(stderr, "Error C unit library initializing.\n");
	}
}
