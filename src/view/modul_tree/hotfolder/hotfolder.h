#ifndef _HOTFOLDER_H_
#define _HOTFOLDER_H_


#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

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

#include <array_list.h>
#include <c_string.h>

#include "q_job/q_job.h"
#include "util/util.h"









array_list * hot_read_dir_content(c_string * q_hotfolder_main_path);
q_job * hot_find_job(array_list * job_list, char * name, int32_t * index);
array_list * hot_copy_job_list(array_list * job_list);
uint8_t hot_compare_job_lists(array_list * job_list, array_list * job_list_pre);

#endif
