#include "core.h"
#include "io_mapping.h"

/************************************************ constants ************************************/
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


/************************************************ structures declarations ********************************/
struct _comm_tcp_;
typedef struct _comm_tcp_ comm_tcp;

struct _job_info_;
typedef struct _job_info_ job_info;

struct _info_struct_;
typedef struct _info_struct_ info_struct;

struct _io_card_;
typedef struct _io_card_ io_card;


/******************************************** structures definitions ********************************************************/
struct _core_
{
	model * model_ref;

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

	uint8_t manual_freed;
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


/**************************************************** functions declarations *****************************************************/

uint8_t core_iij_try_connect(core * this);
uint8_t core_pci_try_connect(core * this);


void * core_connection_testing_thread(void * param);

void core_default_config(core * this);
uint8_t core_load_config(core* this);
void core_update_config(core * this, char * group_name, char * var_name, int type, void * data, char * log_msg_ok, char* log_msg_fail);
void core_initialize_variables(core * this);

void core_safety_system_in(core * this);
void core_safety_system_out(core * this);
int8_t core_printer_abbort_print(core * this, uint8_t step);
void core_set_machine_error(core * this, uint8_t error_code);

/* todo: make unit tests */
int8_t core_csv_analyze(core * this);
void core_csv_compare(core * this, char * q_csv, char * c_csv);
int32_t core_save_response_csv(core * this, char * name);


void  core_clear_hotfolder(core * this);
void core_total_clear_hotfolder(core * this);
uint8_t core_check_job_consistency(core * this, char * name,  uint8_t mod);
uint8_t core_load_bkcore_csv(core * this);

void * core_machine_handler(void * param);
/* todo: make unit tests */
void core_machine_state_read_hotfolder(core * this);
void core_machine_state_print_main(core * this);
void core_machine_state_error(core * this);
void core_machine_state_print_companion(core * this);
void core_machine_state_wait_for_confirmation(core * this);
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



job_info * job_info_new(char * csv_address);
int job_info_get_job_index(job_info * this);
int job_info_get_printed_sheet_number(job_info * this);
void job_info_set_order_name(job_info * this, char* order_name);
void job_info_clear(job_info * this);
void info_struct_finalize(void * this);
void job_info_add_job_record(job_info * this);
void job_info_generate_missing_sheet_records(job_info * this);
int8_t job_info_add_sheet_record(job_info * this, char * sheet_order);
void job_info_set_sheet_record_result(job_info * this, char * result, int index);
int32_t job_info_generate_csv(job_info * this);
int8_t job_info_generate_csv_name(job_info * this);
void job_info_finalize();




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









/******************************************** interface functions definitions *******************************************************/

/**
** @ingroup Core
** Constructor for core layer, define main functionality of the program.
** @return Pointer to new core class object.
*/
core * core_new(model * model_ref)
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


const char* core_get_error_str(core * this)
{
	const char * error_str = NULL;
	const lang * multi_lang = model_get_lang_structure(this->model_ref, this->lang_index);
	
	switch(this->error_code)
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


void core_sheet_source_confirmation(core * this)
{
	this->print_confirmation_req = true;
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

/************************************************************ functions definitions ********************************************/


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

void core_machine_state_error(core * this)
{
	if(this->machine_error_reset_req == true)
	{
		this->error_code = MACHINE_ERR_NO_ERROR;
		this->machine_state = MACHINE_STATE_PRINT_BREAK;
	}	
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

void core_machine_state_read_hotfolder(core * this)
{
	if(this->machine_print_req == false)
	{
		if(this->job_list != NULL)
			array_list_destructor_with_release_v2(this->job_list, q_job_finalize);
			
		this->job_list = model_read_dir_content(this->q_hotfolder_main_path);	

		
		if(model_compare_job_lists(this->job_list, this->job_list_pre) > 0)
		{
			
			if(this->job_list_pre != NULL)
				array_list_destructor_with_release_v2(this->job_list_pre, q_job_finalize);	

			this->job_list_pre = model_copy_job_list(this->job_list);
			this->job_list_changed = 1;
		}
	}
	else if((this->machine_cancel_req) == true && (this->machine_state == MACHINE_STATE_NEXT))
	{
		this->machine_state = MACHINE_STATE_PRINT_BREAK;
	}

	if(c_string_len(this->printed_job_name) > 0)
	{
		if(((this->machine_print_req == true) && (this->machine_state == MACHINE_STATE_WAIT)) || (this->machine_state == MACHINE_STATE_NEXT))
		{
			int job_index = -1;
			q_job * job = model_find_job(this->job_list, c_string_get_char_array(this->printed_job_name), &job_index);

			if((job != NULL) && (q_job_get_bkcore_csv_name(job) != NULL))
			{	
				if(q_job_get_job_order(job) == (job_info_get_job_index(this->info)+2))
				{
					if(q_job_get_flag(job) == 'e')
					{
						this->machine_state = MACHINE_STATE_PRINT_BREAK;
						this->printed_job_index = job_index;
					}
					else
					{
						if((q_job_get_pdf_name(job) != NULL) && (q_job_get_camera_csv_name(job) != NULL))
						{	
							if(this->machine_state == MACHINE_STATE_WAIT)
							{
								job_info_set_order_name(this->info, q_job_get_job_name(job));
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

void core_machine_state_prepare(core * this)
{
	/* load bkcore csv content */
	if(core_load_bkcore_csv(this) == STATUS_SUCCESS)
	{	
		q_job * job = array_list_get(this->job_list, this->printed_job_index);
		
		uint8_t status = 0;

		/* copy pdf to gis hotfolder or based on MB0 input status */
		if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) > 0)
			status = util_copy_file(c_string_get_char_array(this->q_hotfolder_main_path), 
									c_string_get_char_array(this->gis_hotfolder_path), 
									q_job_get_pdf_name(job));
		
		if(status == 0)
		{
			if(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0)
				status = util_copy_file(c_string_get_char_array(this->q_hotfolder_main_path), 
										c_string_get_char_array(this->pci_hotfolder_in_path), 
										q_job_get_camera_csv_name(job));

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
						if((rows == 0) && (q_job_get_flag(job) == 'k'))
						{
							this->info->total_stemps_number = ((this->info->total_stemps_number*10) + (this->bkcore_csv_content[csv_pos]-'0'));
						}	
						else if((rows == 1) && (q_job_get_flag(job) == 'k'))
						{
							if(q_job_get_job_order(job) == 1)
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
						if((rows > 1) || (q_job_get_flag(job) != 'k'))
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


				if((q_job_get_flag(job) == 'k') || ((q_job_get_flag(job) == 'p') && (this->sheet_source_confirmation == false) && (this->companion_sheet_source == SSOURCE_MAIN))) 
				{
					this->machine_state = MACHINE_STATE_PRINT_MAIN;
					printf("go to print main\n");
				}
				else if((q_job_get_flag(job) == 'p') && (this->companion_sheet_source == SSOURCE_COMPANION))
				{
					printf("go to print compation\n");
					this->machine_state = MACHINE_STATE_PRINT_COMPANION;
				}
				else if((q_job_get_flag(job) == 'p') && (this->companion_sheet_source == SSOURCE_MAIN) && (this->sheet_source_confirmation == true))
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
	if((this->feed_sheet == 0))
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
			this->feed_sheet = 2;
			//this->feed_sheet = 3;	//only for testing skip the point 2
			this->timer = c_freq_millis();
		}

		if((feeder_status == MACHINE_FN_FEEDING))
		{
			io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
			this->feed_sheet = 3;
			this->timer = c_freq_millis();
		}
	}
	else if(this->feed_sheet == 2)
	{
		if(feeder_status == MACHINE_FN_FEEDING)
		{
			this->feed_sheet = 3;
		}

		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
	}
	else if((this->feed_sheet == 3) && (feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		if(this->timer + 700 < c_freq_millis())
		{
			this->feed_sheet = 0;
			this->feeded_main_sheet_counter++;
			this->machine_state = MACHINE_STATE_READ_CSV_LINE;
		}

		io_card_set_output(this->io_card_ref, IO_CARD_A1, A1_OUT_7_XBF, 0);
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
			this->feed_sheet = 2;
			//this->feed_sheet = 3; // only for testing
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
					if(core_save_response_csv(this, q_job_get_bkcore_csv_name(job)) >= 0)
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
				util_save_csv(c_string_get_char_array(this->q_hotfolder_feedback_path), q_job_get_bkcore_csv_name(job), "");
				this->machine_state = MACHINE_STATE_CLEAR_HOT_FOLDER;
			}
		}
		else
		{
			core_set_machine_error(this, MACHINE_ERR_CANT_SAVE_F_CSV);
		}
	}
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



void core_machine_state_ready_to_start(core * this)
{
	uint8_t feeder_status = io_card_get_bit_value(this->io_card_ref, IO_CARD_A1, A1_IN_11_FN0, A1_IN_12_FN1, A1_IN_5_FN2); 

	if(((strcmp(c_string_get_char_array(this->print_controller_status), "Printing") == 0) || 
		(io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) == 0)) && 
		(feeder_status == MACHINE_FN_READY_TO_FEED))
	{
		if((this->timer + 1000) < c_freq_millis())
		{
			c_pulse_reset(this->ena_pulse);
			this->machine_state = MACHINE_STATE_READ_CSV_LINE;
		}
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
				if(q_job_get_flag(job) != 'e')
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
				char * csv_name = (char*) malloc(sizeof(char)*(strlen(q_job_get_job_name(job))+strlen(const_char)+1));

				sprintf(csv_name, "%s_%d_e_bkcore_.csv", q_job_get_job_name(job), q_job_get_job_order(job));
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


void core_clear_hotfolder_base(core * this, q_job * job)
{
	if(job != NULL)
	{
		if(q_job_get_pdf_name(job) != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), q_job_get_pdf_name(job));

		if(q_job_get_bkcore_csv_name(job) != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), q_job_get_bkcore_csv_name(job));

		if(q_job_get_camera_csv_name(job) != NULL)
			util_move_file(c_string_get_char_array(this->q_hotfolder_main_path), c_string_get_char_array(this->q_hotfolder_backup_path), q_job_get_camera_csv_name(job));
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
	for(int i = 0; i < array_list_size(this->job_list); i++)
	{
		q_job * job = (q_job*) array_list_get(this->job_list, i);
		core_clear_hotfolder_base(this, job);
	}
}



void core_machine_state_clear_hotfolder(core * this)
{
	/* delete csv from camera hotfolder */
	if((this->printed_job_index >= 0) && (io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1) > 0))
	{
		q_job * job = array_list_get(this->job_list, this->printed_job_index);
		if(job != NULL)
			util_delete_file(c_string_get_char_array(this->pci_hotfolder_out_path), q_job_get_camera_csv_name(job));
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


void core_set_machine_error(core * this, uint8_t error_code)
{
	this->machine_state = MACHINE_STATE_ERROR;
	this->error_code = error_code;
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


int8_t core_printer_abbort_print(core * this, uint8_t step)
{
	int8_t ret_val = -1;
	char * res = NULL;

	if(step == 0)
	{
		res = comm_tcp_transaction(this->iij_tcp_ref, "P,A\n", COMM_TCP_IO_BUFFER_SIZE);
		if(res != NULL)
			ret_val = 1;
	}
	else
	{
		res = comm_tcp_transaction(this->iij_tcp_ref, "R,A\n", COMM_TCP_IO_BUFFER_SIZE);
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
int8_t core_csv_analyze(core* this)
{
	q_job * job = (q_job*) array_list_get(this->job_list, this->printed_job_index);
	
	if(job != NULL)
	{
		char * q_csv = util_load_csv(c_string_get_char_array(this->q_hotfolder_main_path), q_job_get_camera_csv_name(job), NULL);
		char * c_csv = util_load_csv(c_string_get_char_array(this->pci_hotfolder_out_path), q_job_get_camera_csv_name(job), NULL);
	
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


void core_machine_log_monitor(core * this)
{
	if(this->machine_state != this->machine_state_pre)
		c_log_add_record_with_cmd(this->log, "%d - %s", this->machine_state, core_machine_get_state_str(this));

	this->machine_state_pre = this->machine_state;
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


uint8_t core_load_bkcore_csv(core * this)
{
	q_job * job = (q_job*) array_list_get(this->job_list, this->printed_job_index);

	if(job != NULL)
	{
		this->bkcore_csv_content = util_load_csv(c_string_get_char_array(this->q_hotfolder_main_path), 
										q_job_get_bkcore_csv_name(job),
										 &(this->bkcore_csv_size));

		if(this->bkcore_csv_content != NULL)
		{
			c_log_add_record_with_cmd(this->log, "Načten obsah bkcore csv souboru.");
			return STATUS_SUCCESS;
		}
	}

	c_log_add_record_with_cmd(this->log, "obsah souboru bkcore csv nelze načíst!");

	return STATUS_GENERAL_ERROR;
}

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

	/*
	if(config_setting_lookup_int(settings, CFG_PP_GR_MACHINE_MODE, &setting_val_int) == CONFIG_TRUE)
		this->machine_mode = setting_val_int;
	else
		return 18;
*/
	

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
		this->machine_mode = io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_0_MBR0) + 2*io_card_get_input(this->io_card_ref, IO_CARD_A1, A1_IN_1_MBR1);

		this->job_list = array_list_new();
		this->job_list_pre = array_list_new();
		this->job_list_changed = 0;

		this->ena_pulse = c_pulse_new();
		this->manual_freed = 0;

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

/*
	this->machine_mode = GR_PRINT_INSPECTION;
	settings = config_setting_add(print_params, CFG_PP_GR_MACHINE_MODE, CONFIG_TYPE_INT);
	config_setting_set_int(settings, GR_PRINT_INSPECTION);	
*/
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

void * core_connection_testing_thread(void * param)
{
	comm_tcp * this = (comm_tcp*) param;

	while(TRUE)
	{
		if(comm_tcp_is_connected(this) == STATUS_CLIENT_CONNECTED)
		{
			char * recv = comm_tcp_transaction(this, "ping", 255);

			if(recv == NULL)
			{
				comm_tcp_disconnect(this);	

			}
			else
			{
		//		printf("%s - %s\n", comm_tcp_get_ip_addr(this), recv);
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

/************************************************** job info class *******************************************/

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

int8_t job_info_add_sheet_record(job_info * this, char * sheet_order)
{
	if(array_list_size(this->job_list) > 0)
	{
		info_struct * sheet_info = (info_struct*) malloc(sizeof(info_struct));
		sheet_info->sheet_order = (char*) malloc(sizeof(char)*(strlen(sheet_order)+1));
		strcpy(sheet_info->sheet_order, sheet_order);

		sheet_info->result = NULL;

		array_list_add(array_list_get(this->job_list, job_info_get_job_index(this)), sheet_info);

		return 1;
	}
	else
	{
		return -1;
	}
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

int32_t job_info_generate_csv(job_info * this)
{
	if(array_list_size(this->job_list) > 0)
	{
		c_string_add(this->csv_content, "Job: ");
		c_string_add(this->csv_content, c_string_get_char_array(this->order_name));

		if(this->end_status == true)
			c_string_add(this->csv_content, "\nCorectly finished\n");
		else
			c_string_add(this->csv_content, "\nPrematurely finished\n");

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

int8_t job_info_generate_csv_name(job_info * this)
{
	if(c_string_len(this->order_name) > 0)
	{
		c_string_set_string(this->csv_name, c_string_get_char_array(this->order_name));
		c_string_concat(this->csv_name, ".csv");
		return 1;
	}
	else
	{
		return -1;
	}
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


/************************************************** io card **************************************************/

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
	this->A1_out = 0;
	this->A2_out = 0;

	comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A1_out, 0);
	comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A2_out, 0);

	comedi_close(this->ref_a1);
	comedi_close(this->ref_a2);

	free(this);
}



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





/****************************************************************** unit tests **************************************************
*********************************************************************************************************************************/

#if CORE_TEST_MODE

core * core_object_prepare();
void core_object_finalize(core * this);

uint8_t core_constructor_suite_case();
uint8_t job_info_constructor_suite_case();
uint8_t core_machine_function_suite_case();

void core_unit_test()
{
	if(cu_initialize("./unit_test_log", "test_log") == CU_SUCCESS)
	{
		printf("C unit library successfully initialized\n");

		cu_add_suite_case("Core constructor test", core_constructor_suite_case);
		cu_add_suite_case("Core machine function test", core_machine_function_suite_case);
	
		cu_add_suite_case("Job info constructor test", job_info_constructor_suite_case);

		cu_run_test();
		cu_finalize();
	}
	else
	{
		fprintf(stderr, "Error C unit library initializing.\n");
	}
}

/**************************** unit test functions *************************************/

uint8_t core_constructor_suite_case()
{
	uint8_t res = 0;
	//model * model_ref = model_new();
	//core * this = core_new(model_ref);

	


	return res;
}

uint8_t core_machine_function_suite_case()
{
	uint8_t res = 0;
	core * this = core_object_prepare();
	
	

	core_object_finalize(this);

	return res;
}


uint8_t job_info_constructor_suite_case()
{
	uint8_t res = 0;

	job_info * this = job_info_new(".");

	res += cu_assert_ptr_not_equal("Memory alocation", this, NULL);
	
	job_info_clear(this);
	res += cu_assert_pass("Try clear empty job info structure, no memory coruption");
	
	int size_out = job_info_generate_csv(this);
	res += cu_assert_bool_equal("Try to generate empty sheet list with no jobs and without generated order name and csv file name", size_out < 0, TRUE);

	int8_t csv_generation_res = job_info_generate_csv_name(this);
	res += cu_assert_int_equal("Try generate csv file name without set order name", csv_generation_res, -1);

	job_info_set_order_name(this, "test_file");
	csv_generation_res = job_info_generate_csv_name(this);
	res += cu_assert_int_equal("Try generate csv file name after set order name", csv_generation_res, 1);
	res += cu_assert_string_equal("Check generated csv file name", c_string_get_char_array(this->csv_name),"test_file.csv");

	size_out = job_info_generate_csv(this);
	res += cu_assert_bool_equal("Try to generate empty sheet list with no jobs", size_out < 0, TRUE);

	int8_t add_sheet_res = job_info_add_sheet_record(this, "0001");
	res += cu_assert_int_not_equal("Try to add sheet recort to empty job list", add_sheet_res, 1);

	job_info_add_job_record(this);
	add_sheet_res = job_info_add_sheet_record(this, "0001");
	res += cu_assert_int_equal("Try to add sheet recort after adding new job record", add_sheet_res, 1);

	
	job_info_generate_missing_sheet_records(this);
	res += cu_assert_pass("Try to generate missing sheet records without setting number of sheets in job, no memory coruption");

	this->total_sheet_number = 25;
	job_info_generate_missing_sheet_records(this);
	array_list * last_job_record = array_list_get(this->job_list, array_list_size(this->job_list)-1);
	res += cu_assert_pass("Try to generate missing sheet records with setting number of sheets in job, no memory coruption");
	res += cu_assert_int_equal("Check complet sheet records in last job record", array_list_size(last_job_record), this->total_sheet_number);
	
	int missing_sheet_order_str_num = 0;
	for(int i=0; i < array_list_size(array_list_get(this->job_list, array_list_size(this->job_list)-1));i++)
	{
		char temp_str[12];
		sprintf(temp_str,"%d",i+1);
		info_struct * sheet_info = array_list_get(last_job_record,i);
		if((strcmp(temp_str, sheet_info->sheet_order) == 0) && (strcmp(sheet_info->result, "FAIL") == 0))
			missing_sheet_order_str_num++;
	}

	res += cu_assert_int_equal("Check generated missing sheet order strings", array_list_size(last_job_record), missing_sheet_order_str_num);

	size_out = job_info_generate_csv(this);
	res += cu_assert_bool_equal("Try to generate report csv file with jobs and sheets", size_out > 0, TRUE);

	job_info_clear(this);
	res += cu_assert_int_equal("Try to clear job info structure, job list cleared", array_list_size(this->job_list), 0);
	res += cu_assert_int_equal("Try to clear job info structure, csv content cleared", c_string_len(this->csv_content) , 0);
	res += cu_assert_int_equal("Try to clear job info structure, sheet number variable cleared", this->total_sheet_number, 0);
	res += cu_assert_int_equal("Try to clear job info structure, job list cleared", array_list_size(this->job_list), 0);
	res += cu_assert_int_equal("Try to clear job info structure, job order name cleared", c_string_len(this->order_name), 0);

	job_info_finalize(this);
	res += cu_assert_pass("Memory release, no memory coruption");

	return res;
}

/**************************** utility functions ***************************************/

core * core_object_prepare()
{
	core * this = malloc(sizeof(core));

	return this;
}

void core_object_finalize(core * this)
{
	if(this != NULL)
		free(this);
}

#endif


