#include <LBLE.h>
#include <LBLEPeriphral.h>
#include <NewPing.h>
#include <LWiFi.h>

#define SSID "CSIE-WLAN-Sparq"
#define PASSWD "wificsie"
#define TCP_IP "192.168.209.31"
#define TCP_PORT 5000

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
LBLEService AService("19B150 15-E8F2-537E-4F6C-D154768A1214");
LBLECharacteristicInt ARead("19B 15011-E8F2-537E-4F6C-D 154768A1214", LBLE_READ | LBLE_WRITE);

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
int wallDistance = 15;
int turnWallDistance = 19;

WiFiClient wificlient;
static char buf[32];
static int messageLen;




void setup(){

  Serial.begin(9600);
  ////************************* Wi-Fi Part *************************////
  while (!Serial);
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(SSID, PASSWD);
  }
  Serial.println("wifi link");

  // Conenct to AP successfully
  while (!wificlient.connect(TCP_IP, TCP_PORT));
  Serial.println("register ready");
  delay(1000);
  wificlient.write("join Toshi B");

  ////************************* Machine Part *************************////

  LBLE.begin(); // 開啟BLE功能

  while (!LBLE.ready()) {
    delay(10);
  }
  Serial.println("BLE ready");

  Serial.print("Device Address = ["); // 印出裝置位址
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]");

  AService.addAttribute(ARead); //將 ARead 這個資料特性添加到 A 這個服務當中

  //// 將 A 這個服務添加到 GATT 伺服器之中
  LBLEPeripheral.addService(AService);
  LBLEPeripheral.begin();
  LBLEAdvertisementData advertisement;
  advertisement.configAsConnectableDevice("Slow AF & Smart ass Car");
  LBLEPeripheral.advertise(advertisement);

}

void loop()
{
  ////************************* LED Part *************************////
  
  int i = 0;
    while (wificlient.available()) {
        buf[i++] = wificlient.read();
        delayMicroseconds(10);
    }
    if (i != 0) {
      buf[i] = '\0';
      Serial.println(buf);
    }
    i = 0;
    wificlient.write("position");
    delay(5000);     
  Send_Message();
  Receive_Message();
  //delay(1000);


}


void Send_Message() {
  wificlient.write("send-to monkey O");
  delay(2000);
}

void Receive_Message() {
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
  delay(100);
}
