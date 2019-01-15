#include <LWiFi.h>
#define enA 15
#define enB 10
#define RwheelPin1 14
#define RwheelPin2 13
#define LwheelPin1 12
#define LwheelPin2 11
#define RSonarT 6
#define RSonarE 7
#define FSonarT 2
#define FSonarE 3
#define LSonarT 4
#define LSonarE 5
#define SSID "CSIE-WLAN-Sparq"
#define PASSWD "wificsie"
#define RedPin 0
#define GreenPin 16
#define BluePin 17
#define TCP_IP "192.168.209.33"
#define TCP_PORT 5000

//.........................................................................GLOBAL VARIABLE................................................
time_t Current_Wifi_Time = 0, Final_Wifi_Time = 0;
float FResult = 0, LResult, RResult, PFResult, PRResult;
int  dir , iRTurn, iLTurn;
int StartX, StartY, CurX, CurY, Cur_Moving, Cur_Moving_Count, Adjust_Right_Count, Adjust_Left_Count; //Wifi usage getting location
//1 front wal1
//2 front left wall
//3 front right wall
//4 front left right wall
//5 left right wall
//6 left wall
//7 right wall
//8 no wall
//9 back wall
//10 back left wall
//11 back right wall
//12 back front wall
//13 back left front wall
//14 back right front wall
//15 back left right wall
int maze[8][8] = {{10, 5, 5, 5, 2, 10, 5, 4}, {11, 2, 10, 5, 3, 9, 5, 2}, {15, 3, 9, 5, 5, 1, 13, 12},
  {10, 5, 1, 10, 6, 8, 3, 14}, {12, 15, 1, 9, 3, 9, 2, 13}, {14, 5, 8, 7, 5, 1, 12, 12},
  {15, 12, 12, 15, 5, 1, 9, 3}, {15, 7, 7, 5, 4, 14, 11, 4}
};
static char buf[32];//Wifi
static int messageLen;//Wifi
WiFiClient wificlient;//Wifi
//..................................................................END GLOBAL VARIABLE..................................................................

//..................................................................FUNCTION REQUIRED....................................................
float get_distance(int , int) ;//Get distance from sonar
void MStraight();
void MStraight_Both_Wall();
void MStraight_Right_Wall();
void MStraight_Left_Wall();
void MStraight_Without_Wall();
void Stop();
void TLeft();
void TRight();
void Pos00();
void Pos70();
void Pos07() ;
void Pos77() ;
void TBack();
float get_Distance_With_Side(int i);//Get the distance with only one side
void Calibrate();//Make car perpendicular to a wall
void getLocation();//get location from server
void convertPosition();//Convert the location from char to int
void  checkDirectiorn(int);//To check whether now the car is facing the same direction as i
bool checkIniPosition();
int Final_Side(int, int);
void Turn_To_Specificed_Side(int, int);
void Adjust_Left();
void Adjust_Right();
void Adjust_Strongly_Right();
void Adjust_Strongly_Left();
void TRight_Precisly();
void TLeft_Precisly();
void TBack_Precisly();
void Tilt_To_Left();
void Tilt_To_Right();
void Send_Message();
bool Receive_Message();
//..........................................................END FUNCTION .....................................................


void setup() {
  Cur_Moving_Count = 0;
  Adjust_Right_Count = 0;
  Adjust_Left_Count = 0;
  Current_Wifi_Time = millis();
  Serial.begin(9600);
  while (!Serial);
  for (int i = 10; i < 18; i++) {
    pinMode(i, OUTPUT);
  }
  digitalWrite(enA, HIGH);
  //pinMode(b_Pin, OUTPUT);
  digitalWrite(enB, HIGH);
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 255);
  //.....................................................................................WIFI.............................................
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(SSID, PASSWD);
    Serial.print("status");
    Serial.println(status);
  }
  Serial.print("OK");
  const int ip = WiFi.localIP();
  // Conenct to AP successfully
  wificlient.connect(TCP_IP, TCP_PORT);
  //等待開啟 Serial。
  delay(1000);
  wificlient.write("join Toshi B");
  //...............................................................END WiFi.............................................................

}
void loop() {
  /*
    do {
    FResult = get_Distance_With_Side(1);
    MStraight();
    } while (FResult > 7);
    Stop();
    delay(100);
    TBack();
    TBack_Precisly();
    Stop();
    delay(100);
    do {
    delay(1000);
    } while (true);
  */
  /*
    FResult = get_distance(FSonarT, FSonarE);
    while (FResult > 15) {
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 0);
    MStraight_Left_Wall();
    FResult = get_distance(FSonarT, FSonarE);
    }
    Stop();
    delay(2000);
  */
  /*
    while(!Receive_Message());
    Serial.println("kkkkk");
  */
  /*
    FResult = get_Distance_With_Side(1);
    do {
      MStraight();
      FResult = get_Distance_With_Side(1);
    } while (FResult > 8);
    TBack();
    Stop();
    delay(2000);
  */

  do {
    Receive_Message();
  } while (!Receive_Message());
  Pos07();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(25000);

  Pos00();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(25000);

  Pos70();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(30000);
  Pos77();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  Pos07();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(25000);

  Pos00();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(25000);
  Pos70();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  delay(30000);
  Pos77();
  Stop();
  Current_Wifi_Time = millis();
  for (int i = 0; i < 5; i++) {
    Send_Message();
  }
  /*
    for (int i = 0; i < 5; i++) {
    Send_Message();
    }
    Serial.println("Sent");
    do {
    Receive_Message();
    } while (!Receive_Message());
    Serial.println("RECEIVE");
    for (int i = 0; i < 10; i++) {
    Send_Message();
    }
    delay(10000);
  */
  /*
     LResult = get_Distance_With_Side(3);
     Serial.print("LResult");
     Serial.println(LResult);
     FResult = get_Distance_With_Side(1);
     do {
     LResult = get_Distance_With_Side(3);
     RResult = get_Distance_With_Side(4);
     Serial.print("LResult");
     Serial.println(LResult);
     Serial.print("RResult");
     Serial.println(RResult);
     FResult = get_Distance_With_Side(1);
     MStraight();
     } while (FResult > 3);
  */
  /*
    for (int i = 0; i < 140; i++) {
    Adjust_Right();
    }
    Stop();
    delay(500);
    for (int i = 0; i < 60; i++) {
    Adjust_Strongly_Left();
    }
    Stop();
    delay(1000);
  */
  /*
    int Result = get_distance(LSonarT, LSonarE);
    Serial.print("Left:");
    Serial.println(Result);
    Result = get_distance(RSonarT, RSonarE);
    Serial.print("Right:");
    Serial.println(Result);
  */
}
void Send_Message() {
  wificlient.write("send-to monkey OK");
  delay(1000);
}

bool Receive_Message() {
  int i = 0;
  while (wificlient.available()) {
    buf[i++] = wificlient.read();
    delayMicroseconds(10);
  }
  if (i != 0) {
    buf[i] = '\0';
  }
  Serial.print(buf);
  Serial.println("");
  if (buf[17] == 'O' && buf[18] == 'K') {
    return true;
  }
  else {
    return false;
  }

  delay(1000);
}
void TBack_Precisly(int side) {
  RResult = get_Distance_With_Side(4);
  LResult = get_Distance_With_Side(3);
  Serial.println(RResult);
  switch (side) {
    case 1: {
        for (int i = 0; i < 60; i++) {
          if (RResult < 13 || RResult > 20) {
            Tilt_To_Left();
            RResult = get_distance(RSonarT, RSonarE);
          }
          if (RResult > 15) {
            Tilt_To_Right();
            RResult = get_distance(RSonarT, RSonarE);
          }
          if ((RResult < 15) && (RResult > 13)) {
            break;
          }
        }
        break;
      }
    case 2: {
        for (int i = 0; i < 60; i++) {
          if (LResult < 13 || LResult > 20) {
            Tilt_To_Right();
            LResult = get_distance(LSonarT, LSonarE);
          }
          if (LResult > 15) {
            Tilt_To_Left();
            LResult = get_distance(LSonarT, LSonarE);
          }
          if ((LResult < 15) && (LResult > 13)) {
            break;
          }
        }
        break;
      }
  }
}
void TRight_Precisly() {
  LResult = get_distance(LSonarT, LSonarE);
  for (int i = 0; i < 30; i++) {
    if (0.95 * LResult < 9) {
      Tilt_To_Right();
      LResult = get_distance(LSonarT, LSonarE);
    }
    if (1.05 * LResult > 11) {
      Tilt_To_Left();
      LResult = get_distance(LSonarT, LSonarE);
    }
    if ( (LResult < 11) && (LResult > 9)) {
      break;
    }
  }
}

void TLeft_Precisly() {
  RResult = get_distance(RSonarT, RSonarE);
  for (int i = 0; i < 30; i++) {
    if (0.95 * RResult < 10) {
      Tilt_To_Left();
      RResult = get_distance(RSonarT, RSonarE);
    }
    if (1.05 * RResult > 10) {
      Tilt_To_Right();
      RResult = get_distance(RSonarT, RSonarE);
    }
    if ((RResult > 9) && (RResult < 11)) {
      break;
    }
  }
}
void Tilt_To_Left() {
  analogWrite(RwheelPin1, 80);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 80);
  delay(10);
}
void Tilt_To_Right() {
  analogWrite(RwheelPin1, 0);
  analogWrite(RwheelPin2, 80);
  analogWrite(LwheelPin1, 80);
  analogWrite(LwheelPin2, 0);
  delay(10);
}
void Adjust_Right() {
  analogWrite(RwheelPin1, 90);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 120);
  analogWrite(LwheelPin2, 0);
  delay(10);
}
void Adjust_Left() {
  analogWrite(RwheelPin1, 120);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 90);
  analogWrite(LwheelPin2, 0);
  delay(10);
}
int Final_Side(int Pattern, int Turn) {
  int Final_Side;
  switch (Pattern) {
    case 1:
      switch (Turn) {
        case 1:
          Final_Side = 7;
          break;
        case 2:
          Final_Side = 6;
          break;
        case 3:
          Final_Side = 8;
          break;
        default:
          Final_Side = Pattern;
          break;
      }
      break;
    case 2:
      switch (Turn) {
        case 1:
          Final_Side = 3;
          break;
        case 2:
          Final_Side = 6;
          break;
        case 3:
          Final_Side = 7;
        default:
          Final_Side = Pattern;
          break;
      }
      break;
    case 3:
      switch (Turn) {
        case 1:
          Final_Side = 7;
          break;
        case 2:
          Final_Side = 2;
          break;
        case 3:
          Final_Side = 6;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 4:
      switch (Turn) {
        case 1:
          Final_Side = 3;
          break;
        case 2:
          Final_Side = 2;
          break;
        case 3:
          Final_Side = 8;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 5:
      switch (Turn) {
        case 1:
          Final_Side = 1;
          break;
        case 2:
          Final_Side = 1;
          break;
        case 3:
          Final_Side = 8;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 6:
      switch (Turn) {
        case 1:
          Final_Side = 1;
          break;
        case 2:
          Final_Side = 8;
          break;
        case 3:
          Final_Side = 7;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 7:
      switch (Turn) {
        case 1:
          Final_Side = 8;
          break;
        case 2:
          Final_Side = 1;
          break;
        case 3:
          Final_Side = 6;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 9:
      switch (Turn) {
        case 1:
          Final_Side = 6;
          break;
        case 2:
          Final_Side = 7;
          break;
        case 3:
          Final_Side = 1;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 10:
      switch (Turn) {
        case 1:
          Final_Side = 2;
          break;
        case 2:
          Final_Side = 7;
          break;
        case 3:
          Final_Side = 3;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 11:
      switch (Turn) {
        case 1:
          Final_Side = 6;
          break;
        case 2:
          Final_Side = 3;
          break;
        case 3:
          Final_Side = 2;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 12:
      switch (Turn) {
        case 1:
          Final_Side = 5;
          break;
        case 2:
          Final_Side = 5;
          break;
        case 3:
          Final_Side = 12;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 13:
      switch (Turn) {
        case 1:
          Final_Side = 4;
          break;
        case 2:
          Final_Side = 5;
          break;
        case 3:
          Final_Side = 14;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 14:
      switch (Turn) {
        case 1:
          Final_Side = 5;
          break;
        case 2:
          Final_Side = 4;
          break;
        case 3:
          Final_Side = 13;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    case 15:
      switch (Turn) {
        case 1:
          Final_Side = 2;
          break;
        case 2:
          Final_Side = 3;
          break;
        case 3:
          Final_Side = 4;
          break;
        default :
          Final_Side = Pattern;
          break;
      }
      break;
    default:
      Final_Side = Pattern;
      break;
  }
  return Final_Side;
}
bool checkIniPosition() {
  if (buf[9] != '-' && buf[12] != '-') {
    return false;
  }
  else {
    return true;
  }
}

void checkDirection(int i) {//To check whether now the car is facing the same direction as i
  int result;
  do {
    FResult = get_distance(FSonarT, FSonarE);
    LResult = get_distance(LSonarT, LSonarE);
    RResult = get_distance(RSonarT, RSonarE);
  } while (FResult == 0);
  if (FResult < 30) {
    if (LResult < 30) {
      if (RResult < 30) {
        result = 4;
      }
      else {//FResult & LResult <30 but RResult >30
        result = 2;
      }
    }
    else if (RResult < 30) {//FResult & RResult <30 while LResult>30
      result = 3;
    }
    else {//FResult<30 while LResult & RResult>30
      result = 1;
    }
  }
  else if (LResult < 30) {
    if (RResult < 30) {//LResult & RResult<30 while FResult<30
      result = 5;
    }
    else {//LResult<30 while FResult RResult>30
      result = 6;
    }
  }
  else if (RResult < 30) {//RResult<30 while FResult & LResult>30
    result = 7;
  }
  else {//All >30
    result = 8;
  }
  if (result != i) {
    TLeft();
    analogWrite(RwheelPin1, 0);
    analogWrite(RwheelPin2, 150);
    analogWrite(LwheelPin1, 0);
    analogWrite(LwheelPin2, 150);
    delay(150);
    Calibrate();
    checkDirection(i);
  }
}

void getLocation() {
  wificlient.write("position");
  int i = 0;
  while (wificlient.available()) {
    buf[i++] = wificlient.read();
    delayMicroseconds(10);
  }
  if (i != 0) {
    buf[i] = '\0';
  }
  i = 0;
  wificlient.write("position");
  Current_Wifi_Time = millis();
  i++;
  convertPosition();
}

void convertPosition() {
  CurX = buf[9] - '0';
  CurY = buf[11] - '0';
  /*
    Serial.print("Current X :");
    Serial.println(CurX);
    Serial.print("Current Y :");
    Serial.println(CurY);
  */
}

float get_Distance_With_Side(int i) {//Get the distance with only one side
  float x;
  if (i == 1) {
    do {
      x = get_distance(FSonarT, FSonarE);
    } while (x == 0);
  }
  else if (i == 3) {
    do {
      x = get_distance(LSonarT, LSonarE);
    } while (x == 0);
  }
  else if (i == 4) {
    do {
      x = get_distance(RSonarT, RSonarE);
    } while (x == 0);
  }
  return x;
}
void Calibrate() {
  int i;
  do {
    FResult = get_distance(FSonarT, FSonarE);
    LResult = get_distance(LSonarT, LSonarE);
    RResult = get_distance(RSonarT, RSonarE);
  } while (FResult == 0);
  if (FResult > 30) {
    if (LResult > 13) {
      if (RResult < 13) {
        i = 4;
      }
      else {
        i = 1;
      }
    }
    else {
      i = 3;
    }
  }
  else {
    i = 1;
  }
  PFResult = get_Distance_With_Side(i);
  if (PFResult < 8) {
    analogWrite(RwheelPin1, 0);
    analogWrite(RwheelPin2, 120);
    analogWrite(LwheelPin1, 0);
    analogWrite(LwheelPin2, 120);
    delay(100);
  }
  else if (PFResult > 13) {
    analogWrite(RwheelPin1, 120);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 120);
    analogWrite(LwheelPin2, 0);
    delay(100);
  }
  bool ccc;
  do {
    ccc = false;
    FResult = PFResult;
    Serial.print("FResult:");
    Serial.println(FResult);
    if (i == 1 || i == 4) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 120);
      delay(100);
    }
    else if (i == 3) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 120);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 0);
      delay(80);
    }
    PFResult = get_distance(FSonarT, FSonarE);
    if (PFResult < FResult) {
      if (FResult - PFResult > 0.5) {
        ccc = true;
      }
    }
    else {
      if (PFResult - FResult > 0.8) {
        ccc = true;
      }
    }
  } while (ccc);

  //delay(2290);
  Stop();
  delay(500);
}
//.........................................................................START OF 00............................................................
void Pos00() {
  FResult = get_Distance_With_Side(1);
  if (FResult < 30) {
    TBack();
  }
  for (int i = 0; i < 2; i++) {
    do {
      FResult = get_Distance_With_Side(1);
      MStraight();
    } while (FResult > 6);
    Stop();
    delay(100);
    TRight();
    TRight_Precisly();
    Stop();
    delay(100);
  }
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 6);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TRight();
  TRight_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(1000);
}
//...........................................................................END OF 00...............................................................

//.............................................................................START OF 07.....................................................
void Pos07() {
  FResult = get_Distance_With_Side(1);
  if (FResult < 30) {
    TBack();
    for (int i = 0; i < 60; i++) {
      Adjust_Right();
    }
    for (int i = 0; i < 30; i++) {
      Adjust_Left();
    }
  }
  do {
    FResult = get_Distance_With_Side(1);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(200);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(200);
  do {
    RResult = get_distance(RSonarT, RSonarE);
    MStraight();
  } while (RResult < 30);
  for (int i = 0; i < 35; i++) {
    MStraight();
  }
  Stop();
  delay(200);
  TRight();
  Stop();
  delay(200);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TRight();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TRight();
  TRight_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(1000);
}
//....................................................................................END OF 07......................................................


//.............................................................................START OF 77.....................................................
void Pos77() {
  FResult = get_Distance_With_Side(1);
  if (FResult < 30) {
    TBack();

  }
  do {
    FResult = get_Distance_With_Side(1);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TRight();
  TRight_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 10);
  Stop();
  delay(100);
  TRight();
  Stop();
  delay(100);
  do {
    LResult = get_distance(LSonarT, LSonarE);
    MStraight();
  } while (LResult < 25);
  for (int i = 0; i < 20; i++) {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
  }
  for (int i = 0; i < 30; i++) {
    Adjust_Right();
  }
  for (int i = 0; i < 15; i++) {
    Adjust_Left();
  }
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TRight();
  TRight_Precisly();
  Stop();
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  delay(100);
  Stop();
  delay(1000);
}
//....................................................................................END OF 77......................................................

//....................................................................................START OF 70......................................................
void Pos70() {
  FResult = get_Distance_With_Side(1);
  if (FResult < 30) {
    TBack();
    analogWrite(RwheelPin1, 0);
    analogWrite(RwheelPin2, 80);
    analogWrite(LwheelPin1, 0);
    analogWrite(LwheelPin2, 80);
    delay(300);
    for (int i = 0; i < 70; i++) {
      MStraight();
    }
    Stop();
    delay(1000);
  }
  else {
    for (int i = 0; i < 60; i++) {
      MStraight();
    }
  }
  Stop();
  delay(100);
  TLeft();
  Stop();
  delay(100);
  for (int i = 0; i < 40; i++) {
    MStraight();
  }
  do {
    FResult = get_distance(FSonarT, FSonarE);
    RResult = get_distance(RSonarT, RSonarE);
    RResult = get_distance(LSonarT, LSonarE);
    MStraight();
  } while (FResult < 30 || RResult < 30 || LResult < 30);
  for (int i = 0; i < 30; i++) {
    MStraight();
  }
  Stop();
  delay(100);
  TRight();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    RResult = get_distance(RSonarT, RSonarE);
    MStraight();
  } while (RResult < 30);
  for (int i = 0; i < 30; i++) {
    MStraight();
  }
  Stop();
  delay(300);
  TRight();
  Stop();
  delay(500);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  TRight();
  TRight_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(100);
  TLeft();
  TLeft_Precisly();
  Stop();
  delay(100);
  do {
    FResult = get_distance(FSonarT, FSonarE);
    MStraight();
  } while (FResult > 8);
  Stop();
  delay(1000);
}
float get_distance(int trig, int echo) { //計算距離
  float duration;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);//給予trig 10us TTL pulse,讓模組發射聲波
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 4000000);//紀錄echo電位從high到low的時間，就是超音波來回的時間，若4秒內沒收到超音波則回傳0
  return duration / 29 / 2;// 聲速340m/s ，換算後約每29微秒走一公分，超音波來回所以再除2
}
//........................................................................MOVING...................................................................
void MStraight() {
  LResult = get_distance(LSonarT, LSonarE);
  if (LResult < 25 && RResult < 25 ) {
    MStraight_Both_Wall();
    Serial.println("MS_BW");

  }
  else if ((LResult >= 25 && RResult < 25) ) {
    MStraight_Right_Wall();
    Serial.println("MS_RW");
  }
  else if ((RResult >= 25 && LResult < 25)) {
    MStraight_Left_Wall();
    Serial.println("MS_LW");
  }
  else {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
    Serial.println("MS_WW");
    delay(10);
  }
}

void MStraight_Without_Wall() {
  if ( Adjust_Right_Count > 5 ) {
    Serial.print("Cur_Moving:4");
    Cur_Moving = 4;
  }
  else if (Adjust_Left_Count > 5) {
    Serial.print("Cur_Moving:3");
    Cur_Moving = 3;
  }
  else {
    Cur_Moving = 0;
  }
  if (Cur_Moving == 3) {
    Adjust_Strongly_Right();
    Cur_Moving_Count++;
    if (Cur_Moving_Count > 10) {
      Cur_Moving = 4;
      Adjust_Left_Count = 0;
      Adjust_Right_Count = 0;
      Cur_Moving_Count = 0;
    }
  }
  else if (Cur_Moving == 4) {
    Adjust_Strongly_Left();
    Cur_Moving_Count++;
    if (Cur_Moving_Count > 10) {
      Cur_Moving = 3;
      Adjust_Left_Count = 0;
      Adjust_Right_Count = 0;
      Cur_Moving_Count = 0;
    }
  }
  else {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
    delay(10);
    Cur_Moving = 0;
  }
}

void MStraight_Both_Wall() {
  LResult = get_distance(LSonarT, LSonarE);
  RResult = get_distance(RSonarT, RSonarE);
  Serial.println(RResult);
  if (get_distance(LSonarT, LSonarE) > 10 && get_distance(RSonarT, RSonarE) < 11 ) {
    if (get_distance(LSonarT, LSonarE) > 19 && get_distance(RSonarT, RSonarE) < 6 ) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 220);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 250);
      delay(250);
      Stop();
      delay(500);
      analogWrite(RwheelPin1, 250);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 80);
      analogWrite(LwheelPin2, 0);
      Serial.println("BWF1");
      delay(200);
      Stop();
      delay(500);
      analogWrite(RwheelPin1, 80);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 250);
      analogWrite(LwheelPin2, 0);
      delay(90);
      Stop();
      delay(500);
    }
    else {
      Adjust_Left();
      Adjust_Left_Count++;
      Adjust_Right_Count = Adjust_Right_Count - 1 ;
      if (Adjust_Right_Count < 0) {
        Adjust_Right_Count = 0;
      }
    }
  }
  else if (get_distance(LSonarT, LSonarE) < 11 && get_distance(RSonarT, RSonarE) > 10) {
    if  ((get_distance(LSonarT, LSonarE)) < 6 && get_distance(RSonarT, RSonarE) > 19) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 250);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 220);
      delay(250);
      Stop();
      delay(500);
      analogWrite(RwheelPin1, 80);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 250);
      analogWrite(LwheelPin2, 0);
      Serial.println("BWF2");
      delay(200);
      Stop();
      delay(500);
      analogWrite(RwheelPin1, 250);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 80);
      analogWrite(LwheelPin2, 0);
      delay(110);
      Stop();
      delay(500);
    }
    else {
      Adjust_Right();
      Serial.println("BWF2");
      Adjust_Right_Count++;
      Adjust_Left_Count = Adjust_Left_Count - 1;
      if (Adjust_Left_Count < 0) {
        Adjust_Left_Count = 0;
      }
    }
  }
  else {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
    Serial.println("NW");
    delay(10);
  }
}
void MStraight_Left_Wall() {
  LResult = get_Distance_With_Side(3);
  Serial.println(LResult);
  if (LResult < 10 || LResult > 150) {
    if (LResult < 5 || LResult > 150) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 250);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 220);
      delay(250);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 80);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 200);
      analogWrite(LwheelPin2, 0);
      delay(200);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 150);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 150);
      analogWrite(LwheelPin2, 0);
      delay(150);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 250);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 120);
      analogWrite(LwheelPin2, 0);
      delay(150);
      Stop();
      delay(500);
      Serial.println("MS_LW_Back");
    }
    else {
      Adjust_Right();
      Cur_Moving_Count = 0;
      Adjust_Right_Count++;
      Adjust_Left_Count = Adjust_Left_Count - 2;
      if (Adjust_Left_Count < 0) {
        Adjust_Left_Count = 0;
      }
    }
  }
  else if (LResult > 11) {
    Adjust_Left();
    Cur_Moving_Count = 0;
    Adjust_Left_Count++;
    Adjust_Right_Count = Adjust_Right_Count - 2;
    if (Adjust_Right_Count < 0) {
      Adjust_Right_Count = 0;
    }
  }
  else {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
    Serial.println("NW");
    delay(10);
  }
}

void MStraight_Right_Wall() {
  RResult = get_distance(RSonarT, RSonarE);
  Serial.println(RResult);
  if (RResult < 10 || RResult > 1000) {
    if (RResult < 5 || RResult > 1000) {
      analogWrite(RwheelPin1, 0);
      analogWrite(RwheelPin2, 220);
      analogWrite(LwheelPin1, 0);
      analogWrite(LwheelPin2, 250);
      delay(350);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 200);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 80);
      analogWrite(LwheelPin2, 0);
      delay(200);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 150);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 150);
      analogWrite(LwheelPin2, 0);
      delay(200);
      Stop();
      delay(100);
      analogWrite(RwheelPin1, 80);
      analogWrite(RwheelPin2, 0);
      analogWrite(LwheelPin1, 250);
      analogWrite(LwheelPin2, 0);
      delay(150);
      Stop();
      delay(100);
      Serial.println("MS_RW_Back");
    }
    else {
      Adjust_Left();
      Adjust_Left_Count++;
      Adjust_Right_Count = Adjust_Right_Count - 2;
      if (Adjust_Right_Count < 0) {
        Adjust_Right_Count = 0;
      }
    }
  }
  else if (RResult > 11) {
    Adjust_Right();
    delay(10);
    Adjust_Right_Count++;
    Adjust_Left_Count = Adjust_Left_Count - 2;
    if (Adjust_Left_Count < 0) {
      Adjust_Left_Count = 0;
    }
  }
  else {
    analogWrite(RwheelPin1, 100);
    analogWrite(RwheelPin2, 0);
    analogWrite(LwheelPin1, 100);
    analogWrite(LwheelPin2, 0);
    Serial.println("NW");
    delay(10);
  }
}
void Stop() {
  analogWrite(RwheelPin1, 0);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 0);
}

void TLeft() {
  analogWrite(RwheelPin1, 160);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 140);
  Cur_Moving = 0;
  delay(450);
}

void TRight() {
  analogWrite(RwheelPin1, 0);
  analogWrite(RwheelPin2, 140);
  analogWrite(LwheelPin1, 160);
  analogWrite(LwheelPin2, 0);
  Cur_Moving = 0;
  delay(500);
}

void TBack() {
  analogWrite(RwheelPin1, 155);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 150);
  delay(450);
  analogWrite(RwheelPin1, 0);
  analogWrite(RwheelPin2, 120);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 120);
  delay(150);
  analogWrite(RwheelPin1, 155);
  analogWrite(RwheelPin2, 0);
  analogWrite(LwheelPin1, 0);
  analogWrite(LwheelPin2, 150);
  delay(400);
  Cur_Moving = 0;

}