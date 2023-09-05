#include "DeepSleep.h"
#include "ServerInterface.h"
extern int posted;
void DeepSleepHandler(){
     int time = millis();
     while(millis()-time<= 60000) postImage();
     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
     Serial.println("Going to sleep now");
     Serial.flush(); 
     esp_deep_sleep_start();
}
