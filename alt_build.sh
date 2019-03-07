#!/bin/sh

rm -v ./*.o 

M_VIEW="./src/view/view.c"
M_MAIN="./src/main.c"
M_CONTROLER="./src/view/modul_tree/controler.c"
#M_IO_CARD="./src/view/modul_tree/io_card/io_card.c"
M_IO_CARD="./src/view/modul_tree/io_card_sim/io_card.c"
M_HOTFOLDER="./src/view/modul_tree/hotfolder/hotfolder.c"
M_Q_JOB="./src/view/modul_tree/hotfolder/q_job/q_job.c"
M_UTIL="./src/view/modul_tree/hotfolder/util/util.c"
M_INFO_STRUCT="./src/view/modul_tree/info_struct/info_struct.c"
M_LANG="./src/view/modul_tree/multi_lang/lang.c"
M_COM_TCP="./src/view/modul_tree/com_tcp/com_tcp.c"
M_STATISTIC="./src/view/modul_tree/machine_statistic/machine_statistic.c"

M_LIB="-I$HOME/lib/include -L$HOME/lib/ -lc_freq -laclib -lsettings_button  $(pkg-config --libs --cflags gtk+-3.0) $(libgcrypt-config --libs --cflags) -pthread -lconfig -lcomedi -lm"

gcc  $M_MAIN $M_VIEW $M_CONTROLER $M_IO_CARD $M_HOTFOLDER $M_Q_JOB $M_UTIL $M_INFO_STRUCT $M_LANG $M_COM_TCP $M_STATISTIC -o ./bin/host_bk_linux64 -g -std=gnu11 -Wall $M_LIB 

