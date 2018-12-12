#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <gio/gio.h>
#include <gio/gnetworking.h>

#define MAX_CONN_NUM 5
#define TTL 5
#define IO_BUFFER_SIZE 128

struct _comm_interface_;
typedef struct _comm_interface_ comm_interface;


struct _comm_interface_
{
        uint16_t tcp_port;
        GSocketService * net_service;
};

char * vs_gui_state_line_set_time_label()
{
        //time formating
        time_t my_time;
        struct tm* time_info;
        time(&my_time);
        time_info = localtime(&my_time);
	char * time_str = malloc(sizeof(char)*23);
        strftime(time_str, 22, "%d/%m/%y - %H:%M:%S", time_info);

	return time_str;
}

void response_server_thread(GSocket * socket)
{
	char * time_str_con = vs_gui_state_line_set_time_label();

	GSocketAddress * sockaddr = g_socket_get_remote_address(socket, NULL);
        GInetAddress * addr = g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(sockaddr));
	char * client_ip = g_inet_address_to_string(addr);
        g_object_unref(sockaddr);

        g_socket_set_timeout (socket, TTL);
        g_socket_set_blocking(socket, TRUE);
	
	printf("%s - client connected - %s\n", time_str_con, client_ip);

	char* buffer = (char*) malloc(sizeof(char)*IO_BUFFER_SIZE);
        GError * soc_error = NULL;
        int size_in = 0;

	while((size_in = g_socket_receive(socket, buffer, IO_BUFFER_SIZE, NULL, &soc_error)) > 0)
	{
		if(strcmp(buffer, "ping") == 0)
		{
			sprintf(buffer, "ok");
		}
		else
		{
			sprintf(buffer, "error");
		}

		g_socket_send(socket, buffer, strlen(buffer), NULL, &soc_error);	
	}


	char * time_str_discon = vs_gui_state_line_set_time_label();

	printf("%s - client disconnected - %s\n", time_str_discon, client_ip);	

	free(buffer);
	g_free(client_ip);
	free(time_str_con);
	free(time_str_discon);
}

static comm_interface * comm_interface_new(int tcp_port)
{
        comm_interface * this = (comm_interface*) malloc(sizeof(comm_interface));

        this->tcp_port = tcp_port;

        g_networking_init ();
        this->net_service = g_threaded_socket_service_new(MAX_CONN_NUM);
        GError * error = NULL;
        g_socket_listener_add_inet_port ((GSocketListener*) this->net_service, tcp_port, NULL, &error);
        return this;
}

static void comm_insterface_start(comm_interface * this)
{
        g_socket_service_start(this->net_service);
}

static gboolean incoming_callback (GSocketService *service, GSocketConnection *connection, GObject *source_object, gpointer param)
{
        g_print("Received Connection from client!\n");

        GSocket * socket =  g_socket_connection_get_socket(connection);
        response_server_thread(socket);

        return FALSE;
}


int main(int argv, char ** argc)
{

#if defined(_WIN32) || defined(_WIN64)
	HWND var = GetConsoleWindow();
	ShowWindow(var, SW_HIDE);
#endif

	int tcp_port = 1500;

	if(argv > 1)
	{
		tcp_port = 0;
		int exp = 0;
		while(isdigit(argc[1][exp]))
		{
			tcp_port = (tcp_port * 10) + (argc[1][exp]-48);
			exp++;
		}

		if((tcp_port < 0) && (tcp_port > 65564))
		{
			printf("Wrong tcp port value: %d\n", tcp_port);
			return 1;
		}	
	}

	printf("Responder listening on port: %d\n", tcp_port);
		
	gtk_init(&argv, &argc);

	comm_interface * comm_interface_ref = comm_interface_new(tcp_port);
	g_signal_connect(G_OBJECT(comm_interface_ref->net_service), "run",  G_CALLBACK (incoming_callback), NULL);

	gtk_main();


#if defined(_WIN32) || defined(_WIN64)
	ShowWindow(var, SW_SHOW);
#endif

	return 0;
}

