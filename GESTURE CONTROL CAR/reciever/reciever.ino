#include <esp_now.h>
#include <WiFi.h>

void forward();
void backward();
void right();
void left();
void stop();

int F_R_FW = 12;
int F_R_BW = 13;
int F_L_FW = 14;
int F_L_BW = 15;
int B_R_FW = 16;
int B_R_BW = 17;
int B_L_FW = 18;
int B_L_BW = 19;

typedef struct message {
  char command[10];
} message;
message DATA;

void REC_DATA(const esp_now_recv_info *mac, const unsigned char *incomingData, int len) {
  memcpy(&DATA, incomingData, sizeof(DATA));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(DATA.command);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  pinMode(F_R_FW, OUTPUT);
  pinMode(F_R_BW, OUTPUT);
  pinMode(F_L_FW, OUTPUT);
  pinMode(F_L_BW, OUTPUT);
  pinMode(B_R_FW, OUTPUT);
  pinMode(B_R_BW, OUTPUT);
  pinMode(B_L_FW, OUTPUT);
  pinMode(B_L_BW, OUTPUT);

  if (esp_now_init() == ESP_OK) {
    Serial.println("ESP-NOW initialized");
  }

  esp_now_register_recv_cb(REC_DATA);
}

void loop() {
  if (strcmp(DATA.command, "FORWARD") == 0) {
    forward();
  }   
  else if (strcmp(DATA.command, "BACKWARD") == 0) {
    backward();
  }   
  else if (strcmp(DATA.command, "RIGHT") == 0) {
    right();
  }
  else if (strcmp(DATA.command, "LEFT") == 0) {
    left();
  }  
  else {
    stop();
  }

  delay(100);  // Adjust delay as needed
}

void forward() {
  digitalWrite(F_R_FW, HIGH);
  digitalWrite(F_L_FW, HIGH);
  digitalWrite(B_R_FW, HIGH);
  digitalWrite(B_L_FW, HIGH);

  digitalWrite(F_R_BW, LOW);
  digitalWrite(F_L_BW, LOW);
  digitalWrite(B_R_BW, LOW);
  digitalWrite(B_L_BW, LOW);
}

void backward() {
  digitalWrite(F_R_BW, HIGH);
  digitalWrite(F_L_BW, HIGH);
  digitalWrite(B_R_BW, HIGH);
  digitalWrite(B_L_BW, HIGH);

  digitalWrite(F_R_FW, LOW);
  digitalWrite(F_L_FW, LOW);
  digitalWrite(B_R_FW, LOW);
  digitalWrite(B_L_FW, LOW);
}

void right() {
  digitalWrite(F_L_FW, HIGH);
  digitalWrite(B_L_FW, HIGH);

  digitalWrite(F_R_FW, LOW);
  digitalWrite(B_R_FW, LOW);
  digitalWrite(F_R_BW, LOW);
  digitalWrite(F_L_BW, LOW);
  digitalWrite(B_R_BW, LOW);
  digitalWrite(B_L_BW, LOW);
}

void left() {
  digitalWrite(F_R_FW, HIGH);
  digitalWrite(B_R_FW, HIGH);

  digitalWrite(F_L_FW, LOW);
  digitalWrite(B_L_FW, LOW);
  digitalWrite(F_R_BW, LOW);
  digitalWrite(F_L_BW, LOW);
  digitalWrite(B_R_BW, LOW);
  digitalWrite(B_L_BW, LOW);
}

void stop() {
  digitalWrite(F_R_FW, LOW);
  digitalWrite(F_L_FW, LOW);
  digitalWrite(B_R_FW, LOW);
  digitalWrite(B_L_FW, LOW);
  digitalWrite(F_R_BW, LOW);
  digitalWrite(F_L_BW, LOW);
  digitalWrite(B_R_BW, LOW);
  digitalWrite(B_L_BW, LOW);
}
