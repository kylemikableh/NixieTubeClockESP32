#include <WiFi.h>
#include "time.h"
#include <pthread.h>

const char* ssid       = "Mikolajczyk";
const char* password   = "mickeymouse";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

pthread_t thread;
int returnValue;
int id = 1;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S:");
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  Serial.printf("%ld \n", current_time.tv_usec / 10000);
}

void getNTPUpdate() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n\n\n\nCONNECTED\n\n\n\n");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void *updateThread(void *threadid) {
  while(true)
  {
    delay(10000);
    getNTPUpdate();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello World");
  getNTPUpdate();
  returnValue = pthread_create(&thread, NULL, updateThread, (void *)id);
  printLocalTime();
}

void loop()
{
  delay(10);
  printLocalTime();
}
