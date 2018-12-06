
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif


#include <c_string.h>
#include <array_list.h>
#include <c_freq.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <features.h>
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


#define COMM_TCP_IO_BUFFER_SIZE 1024


#define STATUS_CONNECTION_ERROR 1
#define STATUS_CLIENT_CONNECTED 0
#define STATUS_CLIENT_DISCONNECTED 2
#define  STATUS_IP_ADDRESS_ERROR 128
#define STATUS_SUCCESS 3
#define STATUS_PORT_OUT_OF_RANGE 4
#define STATUS_SOCKET_ERROR 255


#define TCP_IP_ADDRESS "192.168.50.16"
#define TCP_PORT_VALUE 2000

#define TCP_NETWORK_TTL_S 1
#define TCP_NETWORK_TTL_U 0

#define GIS_HOT_PATH "/media/stc/gis_hot"
#define PDF_INPUT_ADDR "./pdf"

#define GIS_CMD_NETWORK_CONNECTED "N,C\n"
#define GIS_CMD_PRINT_LISTENER "P,L\n"
#define GIS_CMD_PRINT_ABORT "P,A\nP,A\n"


#define GIS_LOAD_STATUS_SUCCESS 0
#define GIS_LOAD_STATUS_ERROR 255

#define GIS_LOAD_TEST_DELAY 20


struct _comm_tcp_;
typedef struct _comm_tcp_ comm_tcp;


struct _gis_load_;
typedef struct _gis_load_ gis_load;

struct _comm_tcp_
{
	int sockfd;
	struct sockaddr_in serv_addr;
	pthread_mutex_t mutex;

	char* io_buffer;
};

struct _gis_load_
{
	comm_tcp * gis_tcp_ref;
	c_string * gis_status_string;
};


/*********************************************** functions declarations ************************************/

comm_tcp * comm_tcp_new();
uint8_t comm_tcp_set_ip_addr(comm_tcp * client, char * ip_addr);
uint8_t comm_tcp_set_tcp_port(comm_tcp * client, int port);
uint8_t comm_tcp_connect(comm_tcp * client);
char * comm_tcp_transaction(comm_tcp * client, char* msg, int max_rcv);
uint8_t comm_tcp_is_connected(comm_tcp * client);
void comm_tcp_disconnect(comm_tcp * client);
void comm_tcp_finalize(comm_tcp * client);
int comm_tcp_send(comm_tcp * client, char* str);
char * comm_tcp_recv(comm_tcp * client);
char * comm_tcp_transaction(comm_tcp * client, char* msg, int max_rcv);




array_list * fs_read_dir_content(char * address);
uint8_t fs_parse_file_name(char * file_name);
int32_t fs_str_ends_with(char* str, const char * suffix, int offset);
void fs_delete_file(char* path, char * name);
uint8_t fs_copy_file(char * src, char * dest, char * name);

uint8_t gis_load_runtime_state_reading(gis_load * this);
int gis_load_read_status_id(char ** msg);
void gis_load_abbort_print(gis_load * this);	
void gis_load_parse_status(gis_load * this, char * status_str);
void gis_load_delay(int delay);



/************************************************* functions definitions ***********************************/


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





array_list * fs_read_dir_content(char * address)
{
	DIR * dir_ref = opendir(address);
	
	array_list * file_list = array_list_new();

	/* clear job list form previous reading iteration */
	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;
		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				if (fs_str_ends_with(dir_cont->d_name, ".pdf", 0) == 0)
				{
					c_string * file_name = c_string_new_with_init(dir_cont->d_name);
					array_list_add(file_list, file_name);
				}
				else
				{
					fs_delete_file(address, dir_cont->d_name);					
				}
			}
		}

		closedir(dir_ref);
	}

	return file_list;
}


int32_t fs_str_ends_with(char* str, const char * suffix, int offset)
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



void fs_delete_file(char* path, char * name)
{
	char file_addr[255];
	sprintf(file_addr, "%s/%s", path, name);				
	remove(file_addr);
}
uint8_t fs_copy_file(char * src, char * dest, char * name)
{
	FILE  *ptr_src, *ptr_dest;
	int  in;
	char src_addr[255];
	char dest_addr[255];

	sprintf(src_addr, "%s/%s", src, name);
	sprintf(dest_addr, "%s/%s", dest, name);

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


gis_load * gis_load_new()
{
	gis_load * this = (gis_load*) malloc(sizeof(gis_load));

	this->gis_tcp_ref = comm_tcp_new();
	comm_tcp_set_ip_addr(this->gis_tcp_ref, TCP_IP_ADDRESS);
	comm_tcp_set_tcp_port(this->gis_tcp_ref, TCP_PORT_VALUE);

	uint8_t res = comm_tcp_connect(this->gis_tcp_ref);

	if(res == STATUS_CLIENT_CONNECTED)
	{
		if(strcmp(comm_tcp_recv(this->gis_tcp_ref), GIS_CMD_NETWORK_CONNECTED) == 0)
		{
			this->gis_status_string = c_string_new();
			return this;
		}
	}

	comm_tcp_finalize(this->gis_tcp_ref);
	free(this);
	return NULL;
}

void gis_load_delay(int delay)
{
	
	for(int i = delay; i > 0; i--)
	{
		if(i < 9)
			printf("\b\b%d ", i);
		else
			printf("\b\b\b%d ",i);

		fflush(stdout);
		sleep(1);
	}

	printf("\b\bDelay %ds\n", delay);
}

void gis_load_abbort_print(gis_load * this)
{
	comm_tcp_transaction(this->gis_tcp_ref, GIS_CMD_PRINT_ABORT, COMM_TCP_IO_BUFFER_SIZE);
}

uint8_t gis_load_runtime_state_reading(gis_load* this)
{
	comm_tcp_send(this->gis_tcp_ref, GIS_CMD_PRINT_LISTENER);
	int id = -1;
	uint64_t timer = c_freq_millis();
	int status = 0;

	while(comm_tcp_is_connected(this->gis_tcp_ref) == STATUS_CLIENT_CONNECTED)
	{
		char * state_msg = comm_tcp_recv(this->gis_tcp_ref);
		
		if(state_msg != NULL)
		{
			while(*state_msg != 0)
			{
				timer = c_freq_millis();
				if(*state_msg == 'I')
				{
					int msg_id = gis_load_read_status_id(&state_msg);
					state_msg++;
		
					if(*state_msg == 'S')
					{
						if(id == -1)
							id = msg_id;

						if(msg_id == id)
						{
							gis_load_parse_status(this, state_msg);

							printf("%s\n", c_string_get_char_array(this->gis_status_string));
							
							if((strcmp(c_string_get_char_array(this->gis_status_string), "Ready") == 0) && (status == 1))
							{
								status = 2;		
							}
							else if((strcmp(c_string_get_char_array(this->gis_status_string), "Printing") == 0) && (status == 0))
							{
								gis_load_delay(10);
								gis_load_abbort_print(this);
								status = 1;
							} 

							break;
						}
					}			
				}
				else
				{	
					while(*state_msg != '\n'){state_msg++;}
					state_msg++;
				}
			}
		}

		if(status == 2)
			break;

		if(timer+60000 < c_freq_millis())
			return GIS_LOAD_STATUS_ERROR;
	}
	
	return GIS_LOAD_STATUS_SUCCESS;

}

int gis_load_read_status_id(char ** msg)
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

void gis_load_parse_status(gis_load * this, char * status_str)
{
	status_str = status_str+2;
	c_string_clear(this->gis_status_string);

	while((*status_str != ',') && (*status_str != 0) && (*status_str != '\n'))
	{
		c_string_add_char(this->gis_status_string, *status_str);
		status_str++;
	}
}



int main(int argv, char ** argc)
{
	gis_load * gis_load_ref = gis_load_new();

	if(gis_load_ref == NULL)
	{
		printf("GIS not connected... Programm exit with error!\n");
		return 2;
	}

	printf("Start GIS load testing? (y/n) ");

	if(getchar() != 'y')
		return 1;

	while(true)
	{
		array_list * file_list = fs_read_dir_content(PDF_INPUT_ADDR);
			
		if(array_list_size(file_list) > 0)
		{	
			printf("Loading pdf files:\n");

			for(int i = 0; i < array_list_size(file_list); i++)
				printf("%d - %s\n", i+1, c_string_get_char_array((c_string*) array_list_get(file_list, i)));

			printf("\nStarting GIS load testing...\n");
		
			int status = 0;
			for(int i = 0; i <array_list_size(file_list); i++)
			{
				printf("Waiting for hotfolder clearing:   ");
				int j = 10;
				array_list * file_list_gis_hot = fs_read_dir_content(GIS_HOT_PATH);

				while((array_list_size(file_list_gis_hot)) > 0 && (j > 0))
				{
					if(j < 9)
						printf("\b\b%i ", j);
					else
						printf("\b\b\b%i ", j);

					fflush(stdout);
					sleep(1);
					j--;

					if(file_list_gis_hot != NULL)
						array_list_destructor_with_release(&file_list_gis_hot, c_string_finalize_v2);

					file_list_gis_hot = fs_read_dir_content(GIS_HOT_PATH);
				}

				
				if(array_list_size(file_list_gis_hot) > 0)
				{
					for(int k = 0; k > array_list_size(file_list_gis_hot); k++)
					{
						fs_delete_file(GIS_HOT_PATH, c_string_get_char_array((c_string*) array_list_get(file_list_gis_hot, k)));
						printf("\nHotfolder clear\n");
					}
				}
				else
				{
					printf("Clear\n");
				}

				array_list_destructor_with_release(&file_list_gis_hot, c_string_finalize_v2);

				gis_load_delay(GIS_LOAD_TEST_DELAY-5);

				/* copy n-th pdf file to gis hot folder */
				if(fs_copy_file(PDF_INPUT_ADDR, GIS_HOT_PATH, c_string_get_char_array((c_string *) array_list_get(file_list, i))) == 0)
					printf("copy pdf %s to path %s\n", c_string_get_char_array(array_list_get(file_list, i)), GIS_HOT_PATH);
	
				/* wait for the gis response */
				status += gis_load_runtime_state_reading(gis_load_ref);
				gis_load_delay(5);
			}
		
			if(status > 0)
			{
				printf("GIS load test ends with proccessing error!\n");
				return 4;
			}
			else
				printf("GIS load test ends successfully.\n");
		}
		else
		{
			printf("No pdf files available!\nProgram exit with error.\n");
			return 3;
		}

		if(file_list != NULL)
			array_list_destructor_with_release(&file_list, c_string_finalize_v2);
	}

	printf("Program exit successfully\n");

	return 0;
}
