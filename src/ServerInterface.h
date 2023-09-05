#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H
#include "OV7670.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "BMP.h"
#include "Arduino.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

void cameraInit();
int getFrame();
void WIFIInit();
int postImage();

#endif
