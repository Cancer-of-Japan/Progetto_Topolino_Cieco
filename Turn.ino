#include <LBLE.h>
#include <LBLEPeriphral.h>
#include <NewPing.h>
#include <LWiFi.h>

const int r_Pin = 0;//Red led
const int  g_Pin = 8;//Green led
const int b_Pin = 9;//Blue ledled

const int f_check_Pin = 1;//Red led
const int  r_check_Pin = 16;//Green led
const int l_check_Pin = 17;//Blue ledled

# define MaxDistance 100

# define engine1 10
# define rightpin1 13
# define rightpin2 14
# define leftpin1 11
# define leftpin2 12
# define engine2 15

// 在全域空間定義一個只有一個資料特性的服務
// LBLEService AService("19B150 15-E8F2-537E-4F6C-D154768A1214");
// LBLECharacteristicInt ARead("19B 15011-E8F2-537E-4F6C-D 154768A1214", LBLE_READ | LBLE_WRITE);
//front sensor
const int Trig_Pins_0 = 2;
const int Echo_Pins_0 = 3;
//right sensor
const int Trig_Pins_1 = 6;
const int Echo_Pins_1 = 7;
//left sensor
const int Trig_Pins_2 = 4;
const int Echo_Pins_2 = 5;

int old_f_result, old_r_result, old_l_result, f_result, r_result, l_result;
int result_front = 0;
int result_right = 0;
int result_left = 0;

boolean frontwall ;
boolean leftwall ;
boolean rightwall ;
boolean first_turn;

int RMS ;
int LMS ;
int wallDistance = 19;
int turnWallDistance = 19;

void setup()
{
  Serial.begin(9600);

  ////************************* LED Part *************************////
  pinMode(r_Pin, OUTPUT);
  pinMode(g_Pin, OUTPUT);
  pinMode(b_Pin, OUTPUT);
  pinMode(f_check_Pin, OUTPUT);
  pinMode(r_check_Pin, OUTPUT);
  pinMode(l_check_Pin, OUTPUT);

  ////************************* Wi-Fi Part *************************////

  ////************************* Machine Part *************************////


  pinMode(engine1, OUTPUT);
  pinMode(engine2, OUTPUT);
  pinMode(rightpin1, OUTPUT);
  pinMode(rightpin2, OUTPUT);
  pinMode(leftpin1, OUTPUT);
  pinMode(leftpin2, OUTPUT);
  analogWrite(engine1, 255);
  analogWrite(engine2, 255);

  // LBLE.begin(); // 開啟BLE功能

  // while (!LBLE.ready()) {
  //   delay(10);
  // }
  // Serial.println("BLE ready");

  // Serial.print("Device Address = ["); // 印出裝置位址
  // Serial.print(LBLE.getDeviceAddress());
  // Serial.println("]");

  // AService.addAttribute(ARead); //將 ARead 這個資料特性添加到 A 這個服務當中

  // //// 將 A 這個服務添加到 GATT 伺服器之中
  // LBLEPeripheral.addService(AService);
  // LBLEPeripheral.begin();
  // LBLEAdvertisementData advertisement;
  // advertisement.configAsConnectableDevice("Slow AF & Smart ass Car");
  // LBLEPeripheral.advertise(advertisement);
}

void loop()
{
  ////************************* LED Part *************************////
  // analogWrite(r_Pin, 0);
  // analogWrite(g_Pin, 0);
  // analogWrite(b_Pin, 255);
  //delay(1000);


  ////************************* Server Part *************************////

  ////************************* Map Part *************************////


  ////************************* Machine Control Part *************************////

  result_front = get_distance_front( Trig_Pins_0, Echo_Pins_0);
  Serial.println("front");
  Serial.print(result_front);
  //ARead.setValue(result_front);
  Serial.println("cm");
  result_right = get_distance_right( Trig_Pins_1, Echo_Pins_1);
  Serial.println("right");
  Serial.print(result_right);
  //ARead.setValue(result_right);
  Serial.println("cm");
  result_left = get_distance_left( Trig_Pins_2, Echo_Pins_2);
  Serial.println("left");
  Serial.print(result_left);
  //ARead.setValue(result_left);
  Serial.println("cm");
  //delay(600);
  walls(result_front,result_right,result_left);
  //Blocked on right and Left
  if (frontwall == false && rightwall == true && leftwall == true) {
    Serial.println("Avanti  !");
    analogWrite(f_check_Pin, 255);
    analogWrite(r_check_Pin, 0);
    analogWrite(l_check_Pin, 0);
    GoForward_Adujust();
    
    //delay(100);
  }
  // Bloecked on Right
  if (frontwall == false && rightwall == true && leftwall ==  false) {
    Serial.println("Avanti  !");
    analogWrite(f_check_Pin, 255);
    analogWrite(r_check_Pin, 0);
    analogWrite(l_check_Pin, 0);
    GoForward_Adujust();
  }
  //nothing to block
  if (frontwall == false && rightwall == false && leftwall ==  false) {
    Serial.println("Right A!");
    analogWrite(f_check_Pin, 0);
    analogWrite(r_check_Pin, 255);
    analogWrite(l_check_Pin, 0);
    TurnRightFunction_N();
  }
  //blocked on left
  if (frontwall == false && rightwall == false && leftwall ==  true) {
    Serial.println("Right B !");
    analogWrite(f_check_Pin, 0);
    analogWrite(r_check_Pin, 255);
    analogWrite(l_check_Pin, 0);
    TurnRightFunction_N();
  }
  //blocked on front, Precise
  if (frontwall == true && rightwall == false && leftwall ==  false ){
    Serial.println("Right C!");
    analogWrite(f_check_Pin, 0);
    analogWrite(r_check_Pin, 255);
    analogWrite(l_check_Pin, 0);
    TurnRightFunction();

  }

  //blocked on front and left, Precise
  if (frontwall == true && rightwall == false && leftwall ==  true) {
    Serial.println("Right D!");
    analogWrite(f_check_Pin, 0);
    analogWrite(r_check_Pin, 255);
    analogWrite(l_check_Pin, 0);
    TurnRightFunction();
  }
  //bloecked on fornt and right,turn left
  if (frontwall == true && rightwall == true && leftwall ==  false) {
    analogWrite(f_check_Pin, 0);
    analogWrite(r_check_Pin, 0);
    analogWrite(l_check_Pin, 255);
    TurnLeftFunction();

  }
  //Blocked in all direction
  if (frontwall == true && rightwall == true && leftwall ==  true) {
    Serial.println("Shit! We are Surrounded Man!");
    analogWrite(f_check_Pin, 255);
    analogWrite(r_check_Pin, 255);
    analogWrite(l_check_Pin, 255);
    // analogWrite(1, 0);
    // analogWrite(16, 0);
    // analogWrite(17, 0);
    Rotate();
    delay(1000);
    AdjustRL();

  }
}

void AdjustRL() {
  TurnLeftPrecisly();
}

void Rotate() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 160);
  analogWrite(leftpin1, 160);
  analogWrite(leftpin2, 0);
}


void Stop() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 0);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 0);
}

void AdjustToLeft() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 120);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 80);
}

void AdjustToRight() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 80);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 120);
}

void TiltToLeft() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 150);
  analogWrite(leftpin1, 150);
  analogWrite(leftpin2, 0);
}

void TiltToRight() {
  analogWrite(rightpin1, 150);
  analogWrite(rightpin2, 0);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 150);
}

void GoForward_Adujust() {
  for (int i = 0; i < 3; i++) {
    float result_right_func = get_distance_right(Trig_Pins_1, Echo_Pins_1);
    float result_left_func = get_distance_left(Trig_Pins_2, Echo_Pins_2);

    if (0.9 * result_left_func < 11 || 1.1 * result_right_func > 11) {
      AdjustToRight();
      delay(10);
      //Stop();
    }
    if (1.1 * result_left_func > 11|| 0.9 * result_right_func < 11 ) {
      AdjustToLeft();
      delay(10);
      //Stop();
    }
  }
}

void GoForward() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 160);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 160);
}

void GoForwardSlowly() {
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 100);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 100);
}

void TurnRightFunction() {

  Stop();
  delay(500);
  cheatfront();
  Stop();
  delay(500);
  TurnRight(result_front, result_left);
  delay(600);
  Stop();
  delay(500);
  TurnRightPrecisly();
  Stop();
  delay(500);
  CheatFrontPrecisly();
  Stop();
  delay(100);

}

void TurnRightFunction_N() {

  Stop();
  delay(500);
  GoForwardSlowly();
  delay(200);
  Stop();
  delay(10);
  TurnRight(result_front, result_right);
  delay(600);
  Stop();
  delay(500);
  CheatFrontPrecisly();
  Stop();
  delay(100);
}

void TurnLeftFunction() {

  Stop();
  delay(500);
  cheatfront();
  Stop();
  delay(500);
  TurnLeft(result_front, result_left);
  delay(600);
  Stop();
  delay(500);
  TurnLeftPrecisly();
  Stop();
  delay(500);
  CheatFrontPrecisly();
  Stop();
  delay(100);
}

void cheatfront() {
  int result_front_func = get_distance_front( Trig_Pins_0, Echo_Pins_0);
  while (result_front_func > 7) {
    GoForwardSlowly();
    delay(10);
    Stop();
    result_front_func = get_distance_front( Trig_Pins_0, Echo_Pins_0);
  }
}

void CheatFrontPrecisly() {
  int result_front_func = get_distance_front(Trig_Pins_0, Echo_Pins_0);
  int result_right_func = get_distance_right(Trig_Pins_1, Echo_Pins_1);
  int result_left_func = get_distance_left(Trig_Pins_2, Echo_Pins_2);
  walls(result_front_func,result_right_func,result_left_func);
  boolean temp_f, temp_r, temp_l;
  temp_f = frontwall;
  temp_r = rightwall;
  temp_l = leftwall;
  while (frontwall == temp_f && rightwall == temp_r && leftwall ==  temp_l) {
    Serial.println("Not yet");
    GoForwardSlowly();
    delay(10);
    result_front_func = get_distance_front( Trig_Pins_0, Echo_Pins_0);
    result_right_func = get_distance_right( Trig_Pins_1, Echo_Pins_1);
    result_left_func = get_distance_left( Trig_Pins_2, Echo_Pins_2);
    walls(result_front_func,result_right_func,result_left_func);

  }

}

void TurnRightPrecisly() {
  int result_right_func = get_distance_right( Trig_Pins_1, Echo_Pins_1);
  int result_left_func = get_distance_left( Trig_Pins_2, Echo_Pins_2);
  boolean check = false;
  for (int i = 0; i < 10; i++) {
    if (result_left_func < 0.9 * 11) {
      TiltToLeft();
      delay(10);
      result_left_func =  get_distance_left(Trig_Pins_2, Echo_Pins_2);
    }
    if (result_left_func > 1.1 * 11) {
      TiltToRight();
      delay(10);
      result_left_func =  get_distance_left(Trig_Pins_2, Echo_Pins_2);
    }
  }
}

void TurnLeftPrecisly() {
  int result_right_func = get_distance_right( Trig_Pins_1, Echo_Pins_1);
  int result_left_func = get_distance_left( Trig_Pins_2, Echo_Pins_2);
  boolean check = false;
  for (int i = 0; i < 10; i++) {
    if (result_right_func < 0.9 * 11) {
      TiltToLeft();
      delay(10);
      result_right_func =  get_distance_right(Trig_Pins_1, Echo_Pins_1);
    }
    if (result_right_func > 1.1 * 11) {
      TiltToRight();
      delay(10);
      result_right_func =  get_distance_right(Trig_Pins_1, Echo_Pins_1);
    }
  }
}

void TurnRight(int result_front, int result_right){
  Serial.println("TURN RIGHT");
  analogWrite(rightpin1, 120);
  analogWrite(rightpin2, 0);
  analogWrite(leftpin1, 0);
  analogWrite(leftpin2, 120);
}

void TurnLeft(int result_front, int result_left){
  Serial.println("TURN LEFT");
  analogWrite(rightpin1, 0);
  analogWrite(rightpin2, 120);
  analogWrite(leftpin1, 120);
  digitalWrite(leftpin2, 0);
}

// get distance on front side
float get_distance_front(int trig, int echo){
  float duration;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, HIGH);
  delayMicroseconds( 10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 5000000);
  return duration / 29 / 2;
}

//get distance on right side
float get_distance_right(int trig, int echo){
  float duration;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, HIGH);
  delayMicroseconds( 10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 5000000);
  return duration / 29 / 2;
}

//get distance on left side
float get_distance_left(int trig, int echo){
  float duration;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, HIGH);
  delayMicroseconds( 10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 5000000);
  return duration / 29 / 2;
}

void walls(int f_result, int r_result, int l_result){
  if ( f_result <= wallDistance ) {
    frontwall = true ;
  }
  else {
    frontwall = false ;
  }
  if ( r_result <= wallDistance ) {
    rightwall = true ;
  }
  else {
    rightwall = false ;
  }
  if ( l_result <= wallDistance ) {
    leftwall = true ;
  }
  else {
    leftwall = false ;
  }
}