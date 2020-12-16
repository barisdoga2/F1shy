#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <fcntl.h>

#define PORT    4445
#define MAX_LEN 1000

void InitUDPServer(void (*ptr)(char*));
void UpdateUDPServer();
void SendTo(const char* ms);

 #endif