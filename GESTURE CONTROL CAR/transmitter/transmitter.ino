#include <WiFi.h>
#include <Wire.h>
#include <esp_now.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu; 
int x ,y;

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0xA6, 0xE9, 0x14};
typedef struct message { 
  char command[10] ;
};
message myData;
esp_now_peer_info_t peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  mpu.begin();
  delay(1000);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
delay(1000);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
delay(1000);
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) == ESP_OK){
    Serial.println("Syccesfully added peer");
    return;
  }
}
 
void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  x =map(a.acceleration.x,-9,9,-10,10);//myData.x_val
  y =map(a.acceleration.y,-9,9,-10,10) ;//myData.y_val
     
  if(x < -4){
    strcpy(myData.command, "FORWARD");
    Serial.println("FORWARD");
  }
  else if(x > 4){
    strcpy(myData.command, "BACKWARD");
    Serial.println("BACKWARD");
  }
   else if(y < -4){
    strcpy(myData.command, "LEFT");
    Serial.println("LEFT");
  }
   else if(y > 4){
    strcpy(myData.command, "RIGHT");
    Serial.println("RIGHT");
  }
  else{
    strcpy(myData.command, "STOP");
    Serial.println("STOP");
  }

  

  esp_err_t res=esp_now_send(broadcastAddress, (uint8_t*) &myData, sizeof(myData));
  if(res==ESP_OK){
    Serial.println("sucess");
  }
  else{
    Serial.println("failed");
  }
  delay(100);
  
}