#!/bin/sh

gcc -c src/*c  -I$HOME/lib/include -g -L$HOME/lib/ -lc_freq -lc_unit_lib -larray_list_lib -lsettings_button -lc_log -lm $(pkg-config --libs --cflags gtk+-3.0) -Wall -std=c11

gcc *.o -o ./bin/host_bk_linux64 -g -I$HOME/lib/include -L$HOME/lib/ -lc_freq -lc_unit_lib -larray_list_lib -lsettings_button -lc_log $(pkg-config --libs --cflags gtk+-3.0) -std=c11 -Wall $(libgcrypt-config --libs --cflags) -lc_string -pthread -lconfig -lcomedi -lm

