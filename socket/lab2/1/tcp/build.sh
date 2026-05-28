#!/bin/bash

gcc -Wall -Werror multiprocess_tcp_server.c -o proc_server
gcc -Wall -Werror tcp_client.c -o client
gcc -Wall -Werror multithread_tcp_server.c -o thread_server
