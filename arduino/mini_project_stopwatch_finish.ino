#if 1
#define SERVER_NAME "10.10.141.85"
#define LOGID "ARDUINO"
char getSensorId[10] = "CSB_LIN";
#endif

#if 0
#define SERVER_NAME "10.10.141.86"
#define LOGID "JAB_ARD"
char getSensorId[10] = "JAB_SQL";
#endif

#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include "SoftwareSerial.h"
#include <TimerOne.h>
#include "WiFiEsp.h"

#define DEBUG
#define AP_SSID "embA"
#define AP_PASS "embA1234"
//#define SERVER_NAME "10.10.141.86"
#define SERVER_PORT 5000
//#define LOGID "JAB_ARD"
#define PASSWD "PASSWD"
#define SEAT_NO 2


const int startStopButtonPin = 2; // 시작/정지 버튼
const int resetButtonPin = 3;     // 리셋 버튼
#define TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 8 //ECHO 핀 설정 (초음파 받는 핀)
#define CDS_PIN A0
#define WIFIRX 6   //6:RX-->ESP8266 TX
#define WIFITX 7   //7:TX -->ESP8266 RX
#define LED_BUILTIN_PIN 13
#define CMD_SIZE 50
#define ARR_CNT 5

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long startTime = 0;
unsigned long pausedTime = 0;
unsigned long currentTime = 0;
unsigned long usingTime = 0;
unsigned long btnTime = 0;

bool isRunning = false;
char StartTime[16];
char PausedTime[16];
char RunningTime[16];
char UsingTime[16];
char BtnTime[16];
char message[16];
int secCount = 0;
int timer1Flag = 0;
int btn1State;
int btn2State;
int btn1Flag = 0;
int btn2Flag = 1;
int sensorTime;
long duration, distance;
int cds;
char sendBuf[CMD_SIZE];
//char getSensorId[10] = "JAB_SQL";
SoftwareSerial wifiSerial(WIFIRX, WIFITX);
WiFiEspClient client;


void setup()
{
  lcd.init();                      // LCD 초기화
  lcd.backlight();                 // LCD 배경화면 켜기

  pinMode(startStopButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(CDS_PIN, INPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);  //D13

  lcd.setCursor(0,0);
  lcd.print("StopWatch : ");
  lcd.setCursor(0,1);
  lcd.print("Connecting....");
  
  Timer1.initialize(1000000);         //1000000uS ==> 1Sec
  Timer1.attachInterrupt(timerIsr);  // timerIsr to run every 1 seconds

  Serial.begin(9600);
//  Serial.begin(115200);              //DEBUG

  wifi_Setup();
  Serial.println("Start setup()");
  lcd.setCursor(0,1);
  lcd.print("Press to Start");
}

void loop()
{
  if (client.available()) {
    socketEvent();
  }  

//----------------------------------timer -------------
 if(timer1Flag)
  {
    timer1Flag = 0;
  /// SENSOR
    //distance
    digitalWrite(TRIG, HIGH);  // trigPin에서 초음파 발생(echoPin도 HIGH)        
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH);    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
    distance = ((float)(340 * duration) / 1000) / 2; 
    //cds
    cds = analogRead(CDS_PIN);  

    //serial print
    Serial.print("distance : ");
    Serial.print(distance);
    Serial.print("mm, ");
    Serial.print("cds : ");
    Serial.println(cds);

  //테스트출력
    Serial.print("isRunning : ");
    Serial.println(isRunning);
    Serial.print("btn1State : ");
    Serial.println(btn1State);   
    Serial.print("btn1Flag : ");
    Serial.println(btn1Flag);   

    Serial.print("btn2State : ");//테스트 출력
    Serial.println(btn2State);//테스트 출력    
    Serial.print("btn2Flag : ");
    Serial.println(btn2Flag);   

    //Serial.print("StartTime : ");
    //Serial.println(StartTime);
    Serial.print("currentTime : ");
    Serial.println(currentTime);
    Serial.print("pausedTime : ");
    Serial.println(pausedTime);

    Serial.print("UsingTime : ");
    if(btn1Flag == 1 && btn2Flag == 1)
    {
      usingTime = millis()-btnTime;
    }
    
    if(btn1Flag == 0 && btn2Flag == 0)
    {
      usingTime = usingTime - pausedTime;
    }
    
    int seconds = (usingTime / 1000) % 60;
    int minutes = (usingTime / 60000) % 60;
    int hours = (usingTime / 3600000) % 60;  
    sprintf(UsingTime, "%02d:%02d:%02d", hours, minutes, seconds);
    Serial.println(UsingTime);


    Serial.print("StartTime : ");
    Serial.println(StartTime);
    
    Serial.print("RunningTime : ");
    Serial.println(RunningTime);
    Serial.println("");
  }   
//----------------------------------button -------------
  // 1번 버튼 눌렀을 때
  btn1State = digitalRead(startStopButtonPin);
  if(digitalRead(startStopButtonPin)==HIGH)   
  {
    btn1Flag = 1;
    btn2Flag = 1;
    Serial.print("btn1State : ");
    Serial.println(btn1State);    
    btnTime = millis();
    delay(500);
    if(!isRunning)  //1. 정지 상태 일 때
    {
      if(pausedTime == 0)    // 처음 시작하는 경우
      {
        startTime = millis();
        int seconds = (startTime / 1000) % 60;
        int minutes = (startTime / 60000) % 60;
        int hours = (startTime / 3600000) % 60;
        sprintf(StartTime, "%02d:%02d:%02d", hours, minutes, seconds);

        Serial.print("millis: ");//테스트 출력
        Serial.println(startTime);//테스트 출력
      
      //SQL전송
        sprintf(sendBuf, "[%s]START@%d\n", getSensorId, SEAT_NO);
           Serial.println(sendBuf);//test
        client.write(sendBuf, strlen(sendBuf));
        client.flush();

      }
      else        // 이어서 시작하는 경우
      {
        startTime = millis() - pausedTime;
        pausedTime = 0;      // pausedTime 초기화
      }
      isRunning = true;
      lcd.setCursor(0,1);
      lcd.print("Running...    ");
    }
    /*
    else    // 2. 동작 중 일 때
    {
      pausedTime = millis() - startTime;
      isRunning = false;
      lcd.setCursor(0, 1); 
      lcd.print("Paused....    ");
    }
    */
  }
  // 2번 버튼 눌렀을 때
  btn2State = digitalRead(resetButtonPin);
  if(digitalRead(resetButtonPin)==HIGH)   
  {
    btn1Flag = 0;
    btn2Flag = 0;

    Serial.print("btn2State : ");//테스트 출력
    Serial.println(btn2State);//테스트 출력
    delay(500);
    isRunning = false;
    pausedTime = 0;     // pausedTime 초기화
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stopwatch: ");
    lcd.setCursor(0,1);
    lcd.print("Press to Start");

   //SQL전송(end+time)
     //문자열 치환 
      char send_UsingTime[16];
      strcpy(send_UsingTime, UsingTime);   //send_UsingTime <- UsingTime 복사
      char *Using = strstr(send_UsingTime, ":");
      while (Using != NULL)
      {
        Using = strstr(send_UsingTime+1, ":");
        strncpy(Using, "\-", 1);
      }   
        Serial.print("send_UsingTime: ");//test
        Serial.println(send_UsingTime);//test

      char send_RunningTime[16];
      strcpy(send_RunningTime, RunningTime);   //send_RunningTime <- RunningTime 복사
      char *Study = strstr(send_RunningTime, ":");
      while (Study != NULL)
      {
        Study = strstr(send_RunningTime+1, ":");
        strncpy(Study, "\-", 1);
      }   
        Serial.print("send_RunningTime: ");//test
        Serial.println(send_RunningTime);//test

      sprintf(sendBuf, "[%s]END@%d@%s@%s\n", getSensorId, SEAT_NO,send_UsingTime, send_RunningTime);
       Serial.println(sendBuf);//test
      client.write(sendBuf, strlen(sendBuf));
      client.flush();    
  }
//----------------------------------state--------------------------
  if(!isRunning)    // 정지 상태
  {
    //int milliseconds = pausedTime % 1000;
    int seconds = (pausedTime / 1000) % 60;
    int minutes = (pausedTime / 60000) % 60;
    int hours = (pausedTime / 3600000) % 60;
    sprintf(PausedTime, "%02d:%02d:%02d", hours, minutes, seconds);

    lcd.setCursor(0, 0);
    if (pausedTime != 0)
    {
      //sprintf(PausedTime, "Paused  %02d:%02d:%02d",hours,minutes,seconds);
      lcd.print("Paused  ");
      lcd.print(PausedTime);
    }

    if (( (distance <= 100) && (cds <= 400) ) && (pausedTime != 0) )     // ON 조건 : AND
    {
      startTime = millis() - pausedTime;
      pausedTime = 0;      // pausedTime 초기화
      lcd.setCursor(0, 1);
      lcd.print("Running....  ");
      isRunning = 1;
    }
  }
  else if(isRunning)   // 실행 중
  {
    currentTime = millis() - startTime;

    //int milliseconds = currentTime % 1000;
    int seconds = (currentTime / 1000) % 60;
    int minutes = (currentTime / 60000) % 60;   
    int hours = (currentTime / 3600000) % 60;
    sprintf(RunningTime, "%02d:%02d:%02d", hours, minutes, seconds);

    lcd.setCursor(0, 0);
    //sprintf(RunningTime, "RunTime %02d:%02d:%02d", hours, minutes, seconds);
    lcd.print("RunTime ");
    lcd.print(RunningTime);

    if ( (distance > 100) || (cds > 400) && (pausedTime == 0) )    // OFF 조건 : OR 
    {
      pausedTime = millis() - startTime;
      lcd.setCursor(0, 1);

      if(distance > 100) 
        sprintf(message, "Distance....  ");
      if(cds > 400)    
        sprintf(message, "No Phone....  ");
      
      lcd.print(message);
      isRunning = 0; 
    }  
  }
}

//-------------------function----------------------------
void socketEvent() {
  int i = 0;
  char* pToken;
  char* pArray[ARR_CNT] = { 0 };
  char recvBuf[CMD_SIZE] = { 0 };
  int len;

  len = client.readBytesUntil('\n', recvBuf, CMD_SIZE);
  //  recvBuf[len] = '\0';
  client.flush();
#ifdef DEBUG
  Serial.print("recv : ");
  Serial.println(recvBuf);
#endif
  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL) {
    pArray[i] = pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }
  if (!strncmp(pArray[1], " New connected", 4))  // New Connected
  {
    Serial.write('\n');
    return;
  } else if (!strncmp(pArray[1], " Alr", 4))  //Already logged
  {
    Serial.write('\n');
    client.stop();
    server_Connect();
    return;
  } else if (!strcmp(pArray[1], "LED")) {
    if (!strcmp(pArray[2], "ON")) {
      digitalWrite(LED_BUILTIN_PIN, HIGH);
    } else if (!strcmp(pArray[2], "OFF")) {
      digitalWrite(LED_BUILTIN_PIN, LOW);
    }
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);
  } else if (!strcmp(pArray[1], "GETSENSOR")) {
    sensorTime = atoi(pArray[2]);
    strcpy(getSensorId, pArray[0]);
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);
  } else if (!strcmp(pArray[1], "GETTIME")) {
  //문자열 치환 
    char send_UsingTime[16];
    strcpy(send_UsingTime, UsingTime);   //send_UsingTime <- UsingTime 복사
    char *Using = strstr(send_UsingTime, ":");
    while (Using != NULL)
    {
      Using = strstr(send_UsingTime+1, ":");
      strncpy(Using, "\-", 1);
    }   
      Serial.print("send_UsingTime: ");//test
      Serial.println(send_UsingTime);//test

    char send_RunningTime[16];
    strcpy(send_RunningTime, RunningTime);   //send_RunningTime <- RunningTime 복사
    char *Study = strstr(send_RunningTime, ":");
    while (Study != NULL)
    {
      Study = strstr(send_RunningTime+1, ":");
      strncpy(Study, "\-", 1);
    }   
      Serial.print("send_RunningTime: ");//test
      Serial.println(send_RunningTime);//test

    sprintf(sendBuf, "[%s]TIME@%d@%s@%s\n", getSensorId,SEAT_NO,send_UsingTime, send_RunningTime);
      Serial.println(sendBuf);//test
  } 
  client.write(sendBuf, strlen(sendBuf));
  client.flush();

#ifdef DEBUG
  Serial.print(", send : ");
  Serial.print(sendBuf);
#endif
}
void timerIsr() {
  secCount++;
  if(secCount >= 1)
  {
  timer1Flag = 1;
  secCount = 0;
  }
}
void wifi_Setup() {
  wifiSerial.begin(38400);
  wifi_Init();
  server_Connect();
}
void wifi_Init() {
  do {
    WiFi.init(&wifiSerial);
    if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI
      Serial.println("WiFi shield not present");
#endif
    } else
      break;
  } while (1);

#ifdef DEBUG_WIFI
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(AP_SSID);
#endif
  while (WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {
#ifdef DEBUG_WIFI
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
#endif
  }
#ifdef DEBUG_WIFI
  Serial.println("You're connected to the network");
  printWifiStatus();
#endif
}
int server_Connect() {
#ifdef DEBUG_WIFI
  Serial.println("Starting connection to server...");
#endif

  if (client.connect(SERVER_NAME, SERVER_PORT)) {
#ifdef DEBUG_WIFI
    Serial.println("Connected to server");
#endif
    client.print("[" LOGID ":" PASSWD "]");
  } else {
#ifdef DEBUG_WIFI
    Serial.println("server connection failure");
#endif
  }
}
void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

