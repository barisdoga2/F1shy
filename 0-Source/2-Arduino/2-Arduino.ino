
#define DEBUG
//#define DEBUG_IMU
//#define DEBUG_SENSOR
#define DEBUG_MOTOR

#include "IMU.h"
#include "SensorDriver.h"
#include "MotorDriver.h"

#define TARGET_UPDATES_PER_SEC 20
#define TARGET_UPDATES_MS 1000 / TARGET_UPDATES_PER_SEC

#ifdef DEBUG
  #define DPRINT(x) Serial.print(x)
  #define DPRINTLN(x) Serial.println(x)
#else
  #define DPRINT(x)
  #define DPRINTLN(x)
#endif

char serial_in_buffer[100];
int serial_last_byte = 0;

void onSerialCommandReceived(){
    char* parts[18];
    int partcount = 0;
    parts[partcount++] = serial_in_buffer;
    char* ptr = serial_in_buffer;
    while(*ptr) {
        if(*ptr == ' ') {
            *ptr = 0;
            parts[partcount++] = ptr + 1;
        }
        ptr++;
    }

    if(parts[0][0] == '0'){
      int motor = atoi(parts[1]);
      int speed = atoi(parts[2]);
      SetSpeed(motor, speed);
    }
}

void setup2(){    
    int isInitOK = 0;
    isInitOK |= InitIMU();
    isInitOK |= InitSensorDriver() << 1;
    isInitOK |= InitMotorDriver() << 2;
    
    if(isInitOK != 0){
        DPRINT(F("Error in init phase: "));
        DPRINTLN(isInitOK);
    }
}

void loop2(){
    if (Serial.available() > 0){
      char c = Serial.read();
      if(c == 10){
        serial_in_buffer[serial_last_byte++] = '\0'; 
        onSerialCommandReceived();
        serial_last_byte = 0;
      }else{
        serial_in_buffer[serial_last_byte++] = c;
      }
    }
  
    int isUpdateOK = 0;
    isUpdateOK |= UpdateIMU();
    isUpdateOK |= UpdateSensorDriver() << 1;
    isUpdateOK |= UpdateMotorDriver() << 2;

    if(isUpdateOK != 0){
        DPRINT(F("Error in update phase: "));
        DPRINTLN(isUpdateOK);
    }
}

void setup(){
    Serial.begin(115200);
    unsigned long start = millis();
    DPRINTLN(F("Initializing."));
    setup2();
    DPRINT(F("Init phaze took: "));
    DPRINTLN(millis() - start);
}

unsigned long startTime;
unsigned long totalUpdates;
unsigned long ll = 0;
void loop(){

    unsigned long timer = 0, endTime = millis();
    startTime = millis();
    while (1){
        ll++;
        unsigned long current = millis();
        unsigned long delta = current - endTime;
        endTime = current;
        timer += delta;

        if (timer >= TARGET_UPDATES_MS){
          totalUpdates++;
          DPRINT(F("UpdateTook: "));
          DPRINT(millis() - endTime);
          DPRINT(F(" \t UpdatesPerSecond: "));
          DPRINT(1000 / ((float)(millis() - startTime) / totalUpdates));
          DPRINT(F(" \t Current: "));
          DPRINTLN(current);
          loop2();
          timer -= TARGET_UPDATES_MS;
        }
    }
}
