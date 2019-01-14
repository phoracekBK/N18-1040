#include "com_tcp.h"

struct _com_tcp_
{
	int sockfd;
	struct sockaddr_in serv_addr;
	pthread_mutex_t mutex;

	char* io_buffer;
};

/**
** @ingroup Communication
** Constructor for comm_tcp_client class, initialization of socket structure.
** @return Pointer to comm_tcp_client class object.
*/
com_tcp * com_tcp_new()
{
	com_tcp * this = (com_tcp*) malloc(sizeof(com_tcp));

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
uint8_t com_tcp_connect(com_tcp * client)
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


uint8_t com_tcp_is_connected(com_tcp * client)
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

void com_tcp_disconnect(com_tcp * client)
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
uint8_t com_tcp_set_ip_addr(com_tcp * client, char * ip_addr)
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


char * com_tcp_get_ip_addr(com_tcp * client)
{
	return inet_ntoa(client->serv_addr.sin_addr);
}

int com_tcp_get_tcp_port(com_tcp * client)
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
uint8_t com_tcp_set_tcp_port(com_tcp * client, int tcp_port)
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
char * com_tcp_transaction(com_tcp * client, char* msg, int max_rcv)
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


int com_tcp_send(com_tcp * client, char* str)
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
char * com_tcp_recv(com_tcp * client)
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
void com_tcp_finalize(com_tcp * client)
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

