#!/bin/bash

#sudo killall tunserver
#killall python3

#source env.sh
gcc -o proxyserver smb2Name.c smb2tools.c short_name_list.c proxyserver1.c smb_session_list.c unicode2utf8.c fname_decode.c -lpthread
cd dump_file 
sudo rm *.txt
cp ../proxyserver ./proxyserver
sudo ./proxyserver 192.168.50.2 445 192.168.51.100
