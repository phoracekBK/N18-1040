#!/bin/sh

rm -v ./*.o 

#gcc -c src/*c  -I$HOME/lib/include -g -L$HOME/lib/ -lc_freq -lc_unit_lib -larray_list_lib -lsettings_button -lc_log -lm $(pkg-config --libs --cflags gtk+-3.0) -Wall -std=c11

gcc $(find ./src -name "*.c") -o ./bin/host_bk_linux64 -g -I$HOME/lib/include -L$HOME/lib/ -lc_freq -laclib -lsettings_button  $(pkg-config --libs --cflags gtk+-3.0) -std=gnu11 -Wall $(libgcrypt-config --libs --cflags) -pthread -lconfig -lcomedi -lm 

