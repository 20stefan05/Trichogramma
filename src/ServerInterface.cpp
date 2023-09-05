#include "ServerInterface.h"

#define TIME_TO_SLEEP 5
#define uS_TO_S_FACTOR 1000000ULL

#define SSID        "Orange-Tekwill"
#define PASSWORD    ""

const int SIOD = 21; //SDA
const int SIOC = 22; //SCL

const int VSYNC = 34;
const int HREF = 35;

const int XCLK = 32;
const int PCLK = 33;

const int D0 = 27;
const int D1 = 17;
const int D2 = 16;
const int D3 = 15;
const int D4 = 14;
const int D5 = 13;
const int D6 = 12;
const int D7 = 4;

const int TFT_DC = 2;
const int TFT_CS = 5;


const uint16_t port = 80;
const char * host = "ec2-3-74-43-73.eu-central-1.compute.amazonaws.com"; // ip or dns

OV7670 *camera;

WiFiMulti wifiMulti;
unsigned char bmpHeader[BMP::headerSize];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void cameraInit(){
  camera = new OV7670(OV7670::Mode::QQVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
  BMP::construct16BitHeader(bmpHeader, camera->xres, camera->yres);
}
int getFrame(){
    return camera->oneFrame();
}
void WIFIInit(){
  wifiMulti.addAP(SSID, PASSWORD);
  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  }
  timeClient.begin();
  timeClient.setTimeOffset(3600*3);
}
int postImage()
{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      Serial.print("Connecting to ");
      Serial.println(host);
      if (!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return 0;
      }
      
      while(getFrame()==0);
      while(!timeClient.update()) {
      timeClient.forceUpdate();
      }
      String head = "--Stefan\r\nContent-Disposition: form-data; name=\"file\"; filename=\"";
      String formattedDate = timeClient.getFormattedTime();
      String head2 = ".bmp\"\r\nContent-Type: image/bmp\r\n\r\n";
      head.concat(formattedDate);
      head.concat(head2);
      String tail = "\r\n--Stefan--\r\n";
      uint32_t imageLen = camera->xres * camera->yres * 2 + BMP::headerSize;
      uint32_t extraLen = head.length() + tail.length();
      uint32_t totalLen = imageLen + extraLen;
      client.println("POST /uploads HTTP/1.1");
      client.println("Host: ec2-3-74-43-73.eu-central-1.compute.amazonaws.com:80");
      client.println("Content-Length: " + String(totalLen));
      client.println("Content-Type: multipart/form-data; boundary=Stefan");
      client.println();
      client.print(head);
      client.write(bmpHeader, BMP::headerSize);
      client.write(camera->frame, camera->xres * camera->yres * 2); 
      client.print(tail);
      // Free resources
      client.stop();
      return 1;
    }
    else {
      Serial.println("WiFi Disconnected");
      return 0;
    }
  
}
