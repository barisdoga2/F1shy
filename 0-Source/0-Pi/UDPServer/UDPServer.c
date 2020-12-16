#include "UDPServer.h"

int sockfd; 
struct sockaddr_in servaddr, cliaddr; 
int len;
char ioBuffer[MAX_LEN]; 
void (*onRecvCbk)(char*);

void InitUDPServer(void (*ptr)(char*)){
    onRecvCbk = ptr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    //struct timeval tv;
    //tv.tv_sec = 0;
    //tv.tv_usec = 1;
    //if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    //    perror("Error");

    int flags = fcntl(sockfd,F_GETFL,0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        printf("ERROR: Couldn't bind the socket while init of UDPServer!");
}

void UpdateUDPServer(){
    len = sizeof(cliaddr);
    int ret = recvfrom(sockfd, ioBuffer, MAX_LEN, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len); 
    
    if(ret != -1){
        ioBuffer[ret] = '\0'; 
        onRecvCbk(ioBuffer);
    }
}

void SendTo(const char* msg){
    sendto(sockfd, msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
}