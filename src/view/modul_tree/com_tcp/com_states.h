#ifndef _COM_STATES_H_
#define _COM_STATES_H_

/**
** @ingroup InternalStates
** Macro defines success status of connection try.
*/
#define STATUS_SUCCESS 0

/**
** @ingroup InternalStates
** Macro defines socket creation error status of connection try.
*/
#define STATUS_SOCKET_ERROR 1

/**
** @ingroup InternalStates
** Macro defines ip address conversion error status of connection try
*/
#define STATUS_IP_ADDRESS_ERROR 2

/**
** @ingroup InternalStates
** Macro defines socket connection error status of connection try
*/
#define STATUS_CONNECTION_ERROR 3

/**
** @ingroup InternalStates
** Macro defines error, try set network parametr when client is stil connected.
*/
#define STATUS_CLIENT_CONNECTED 4

/**
** @ingroup InternalStates
** Macro defines error tcp port value out of range: <0:65535>
*/
#define STATUS_PORT_OUT_OF_RANGE 5

#define STATUS_CLIENT_DISCONNECTED 6

#define STATUS_COMMUNICATION_ERROR 7


/**
** @ingroup InternalStates
** Macro defines state of the called operation from core layer interface, access denied to the interface function
*/
#define STATUS_GENERAL_ERROR 255

#endif
