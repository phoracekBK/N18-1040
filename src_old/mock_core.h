/************************************************ structures declarations ********************************/
struct _comm_tcp_;
typedef struct _comm_tcp_ comm_tcp;

struct _job_info_;
typedef struct _job_info_ job_info;

struct _info_struct_;
typedef struct _info_struct_ info_struct;

struct _io_card_;
typedef struct _io_card_ io_card;

struct _mock_core_;
typedef struct _mock_core_ mock_core;


/******************************************** structures definitions ********************************************************/
struct _mock_core_
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


