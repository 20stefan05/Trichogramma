#include "DeepSleep.h"
#include "ServerInterface.h"
void setup() 
{
    Serial.begin(115200);
    WIFIInit();
    cameraInit();
    DeepSleepHandler();
}

void loop()
{
}
