#ifndef _COM_TCP_H_
#define _COM_TCP_H_


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
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
#include <fcntl.h> 
#include <unistd.h> 

#include "com_states.h"


/** @ingroup configuration Time interva for correct receave the respond from TCP socket in microseconds. */
#define TCP_NETWORK_TTL_U 10000 

/** @ingroup configuration Time interva for correct receave the respond from TCP socket in seconds. */
#define TCP_NETWORK_TTL_S 0


#define COMM_TCP_IO_BUFFER_SIZE 1024



struct _com_tcp_;
typedef struct _com_tcp_ com_tcp;



com_tcp * com_tcp_new();
uint8_t com_tcp_set_ip_addr(com_tcp * client, char * ip_addr);
uint8_t com_tcp_set_tcp_port(com_tcp * client, int port);
char * com_tcp_get_ip_addr(com_tcp * client);
int com_tcp_get_tcp_port(com_tcp * client);
uint8_t com_tcp_connect(com_tcp * client);
char * com_tcp_transaction(com_tcp * client, char* msg, int max_rcv);
uint8_t com_tcp_is_connected(com_tcp * client);
void com_tcp_disconnect(com_tcp * client);
void com_tcp_finalize(com_tcp * client);
int com_tcp_send(com_tcp * client, char* str);
char * com_tcp_recv(com_tcp * client);


#endif
