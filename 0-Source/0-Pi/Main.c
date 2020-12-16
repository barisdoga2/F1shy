#include <ctype.h>
#include <wiringPi.h>

#include "Utils/Utils.h"
#include "UDPServer/UDPServer.h"

#define TARGET_UPDATES_PER_SEC   20
#define TARGET_UPDATES_MS        1000 / TARGET_UPDATES_PER_SEC

long long totalUpdates = 0;
long long startTime = 0;


void OnPacketReceived(char* msg){
    printf("UDPPacket Received: %s\n", msg);
    for(char* c=msg; *c=toupper(*c); ++c);
    SendTo(msg);
    printf("UDPPacket Sent: %s\n", msg);
}

void Init(){

    wiringPiSetup();
    InitLogger();
    InitUDPServer(&OnPacketReceived);

}

void Update(){

    UpdateUDPServer();
    UpdateLogger();

}

int main(){

    // Init
    Init();

    // Main Loop
    long long timer = 0, delta = 0, current = 0, endTime = getCurrentMillis();
    startTime = getCurrentMillis();
    while(1){
        current = getCurrentMillis();
        delta = current - endTime;
        endTime = getCurrentMillis();
        timer += delta;

        if(timer >= TARGET_UPDATES_MS){
            totalUpdates++;
            Update();
            printf("UpdateTook: %llu \t UpdatesPerSecond: %lf\n", getCurrentMillis() - endTime, 1000 / ((float)(getCurrentMillis() - startTime) / totalUpdates));
            timer -= TARGET_UPDATES_MS;
            mssleep(1);
        }
        mssleep(1);
    }
    return 0;
}