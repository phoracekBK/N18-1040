#ifndef _INTERFACE_H_
#define _INTERFACE_H_




#include <features.h>
#include <libconfig.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <comedilib.h>
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
#include <unistd.h>
#include <time.h>

#include <c_freq.h>
#include <platform.h>
#include <aclib.h>
#include <c_regulation.h>

/* system configuration */
#include "config.h"

/* controler modules */
#include "com_tcp/com_tcp.h"

#ifdef PLATFORM_STC_PC
#include "io_card/io_card.h"
#else
#include "io_card_sim/io_card.h"
#endif

#include "hotfolder/hotfolder.h"
#include "info_struct/info_struct.h"
#include "machine_statistic/machine_statistic.h"


#endif
