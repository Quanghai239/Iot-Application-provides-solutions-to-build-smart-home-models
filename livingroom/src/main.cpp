#include <Arduino.h>
#include <FirebaseESP32.h>
#include <WiFi.h>
#include <time.h>
#include <cstring>
///////////////////DEFINE
#define PIRSR501 16
#define output1 2
#define output4 12//LED1
#define button1 22
#define CBAS 35
#define output3 17//CBAS
///////////////////KHAI BAO FIREBASE 
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData firebaseData;
FirebaseData fbdo;
FirebaseData fbdo5;
FirebaseData fbdo8;
FirebaseData fbdo9;
FirebaseData fbdo13;
FirebaseData fbdo15;
FirebaseData fbdo16;
String paths   = "/BT1";
String paths5  = "/LB1";
String paths8  = "/TIME";
String paths13 = "/TIET_KIEM_NANG_LUONG";
String paths15 = "/THUC_DAY";
String paths16 = "/RA_NGOAI";
String paths9  = "/DI_NGU";
///////////////
// const char ssid[] = "Cà Phê Võng";
// const char pass[] = "";
const char ssid[] = "wifi";
const char pass[] = "12345678";
// const char ssid[] = "GIANG L2";
// const char pass[] = "0393286953";
// const char ssid[] = "Nhà Thông Minh";
// const char pass[] = "0981805945";
// const char ssid[] = "Iphone 6s";
// const char pass[] = "12345678";
// const char ssid[] = "NHOM12";
// const char pass[] = "01234567";
// const char ssid[] = "PTIT.HCM_SV";
// const char pass[] = "";
// const char ssid[] = "tiendatne";
// const char pass[] = "tiendatnghitet";
// const char ssid[] = "realme_C21Y";
// const char pass[] = "134679tuong";
// const char ssid[] = "Nguyen";
// const char pass[] = "nguyen1082003";
//////////////////////////////
///PIRSR501
int valuepir;
////CAMBIENANHSANG
int valueas = 0;
bool savenl = true;
///////DIEUKKHIENANHSANG
bool led1State = false; 
volatile bool button1State = false;
const int buttonDebounce = 500;
unsigned long currentMillis ;
unsigned long pretimePress = 0;\
static int brightness1 = 0;
int pwmValue1;
bool thucDay = false;
bool raNgoai = false;
bool diNgu = false;
////////timer
hw_timer_t   *timer       = NULL;
volatile bool timer_flag  = false;
hw_timer_t   *timer2      = NULL;
volatile bool timer2_flag = false;
hw_timer_t   *timer3      = NULL;
volatile bool timer3_flag = false;
hw_timer_t   *timer4      = NULL;
volatile bool timer4_flag = false;
////////////////HEN GIO
char currentTimeStr[9]; // Để lưu thời gian dạng HH:MM:SS
char currentDateStr[11]; //
struct Alarm {
    String time;  // Lưu thời gian báo thức dưới dạng chuỗi "HH:MM:SS"
    String note;  // Lưu chuỗi note tương ứng
};
// Tạo mảng để lưu nhiều báo thức và note
Alarm alarms[20]; // Lưu tối đa 20 báo thức với các note khác nhau
int alarmCount = 0;

///////SENDFIREBASE
void update5Firebase(){
if (Firebase.setInt(firebaseData, paths5, brightness1)) {
    Serial.println("Đã gửi giá trị ánh sáng lên Firebase: " + String(brightness1) + "%");
  } else {
    Serial.println("Lỗi khi gửi giá trị ánh sáng lên Firebase: " + firebaseData.errorReason());
  }
}
void update6Firebase(){
if (Firebase.setBool(firebaseData, paths, led1State)) {
    Serial.println("Đã gửi led1 lên Firebase");
  } else {
    Serial.println("Lỗi khi gửi led1 lên Firebase: " + firebaseData.errorReason());
  }
}
////////////STREAM FIREBASE
void printStreamData(StreamData data) {
  int value;
  // String stringValue;
  boolean boolValue;
  if (data.dataType() == "int") {
    value  = data.intData();
  // } else if (data.dataType() == "string") {
  //   stringValue  = data.stringData();
  } else if (data.dataType() == "boolean") {
    boolValue = data.boolData();
  } else if (data.dataType() == "json") {
        // Lấy dữ liệu JSON từ Firebase
        FirebaseJson jsonData = data.jsonObject();
        // Khai báo biến để lưu giá trị hour, minute và note
        FirebaseJsonData jsonHourData, jsonMinuteData, jsonNoteData;
        int hour = 0;
        int minute = 0;
        String note = "";
        // Kiểm tra và lấy giá trị từ Firebase cho giờ, phút và note
        if (jsonData.get(jsonHourData, "/hour")) {
            hour = jsonHourData.intValue;
            Serial.println("Giờ: " + String(hour));
        }
        if (jsonData.get(jsonMinuteData, "/minute")) {
            minute = jsonMinuteData.intValue;
            Serial.println("Phút: " + String(minute));
        }
        if (jsonData.get(jsonNoteData, "/note")) {
            note = jsonNoteData.stringValue;
            Serial.println("Note: " + note);
        }
        // Định dạng thời gian thành "HH:MM:SS"
        char buffer[9];
        sprintf(buffer, "%02d:%02d:00", hour, minute);
        String alarmTime = String(buffer);
        // Lưu thời gian báo thức và note vào mảng
if (note == "a" || note == "b") { // Chỉ lưu khi note là "a" hoặc "b"
    if (alarmCount < 20) { // Kiểm tra nếu mảng chưa đầy
        alarms[alarmCount].time = alarmTime;
        alarms[alarmCount].note = note;
        alarmCount++;
        Serial.println("Đã lưu báo thức: " + alarmTime + " với note: " + note);

        // Chuyển đổi thời gian thành định dạng "HH,MM,SS"
        String hour = alarmTime.substring(0, 2);
        String minute = alarmTime.substring(3, 5);
        String second = alarmTime.substring(6, 8);
    }
} else {
    Serial.println("Note không hợp lệ, không lưu báo thức!");
}
  }else {
    Serial.print("Other data type");
  }
  if (data.streamPath() == paths) { // Kiểm tra LED1
    if (!raNgoai) { // Chỉ cho phép bật/tắt LED1 khi không ra ngoài
      led1State = boolValue; // Lưu trạng thái bật/tắt LED1
        if (led1State) {
          Serial.println("LED1: Bật");
          digitalWrite(output4, HIGH);
          if (brightness1 > 0) { // Kiểm tra và áp dụng độ sáng khi bật
          pwmValue1 = map(brightness1, 0, 100, 0, 255);
          analogWrite(output4, pwmValue1);
          }
        } else {
          Serial.println("LED1: Tắt");
          digitalWrite(output4, LOW);
          analogWrite(output4, 0);
          }
      } else {
          Serial.println("Không thể bật LED1 khi đang ra ngoài.");
        }
  } 
     else if (data.streamPath() == paths13) { // Kiểm tra paths13
      savenl = boolValue; // Lưu giá trị boolean vào biến savenl
        if (savenl) {
            Serial.println("paths13: Bật đọc cảm biến.");
        } else {
            Serial.println("paths13: Tắt đọc cảm biến.");
        }
    } else if (data.streamPath() == paths15) { 
        thucDay = boolValue;
        if(thucDay) {
          Serial.println("Điều khiển bằng cảm biến");
        } else {}         
      } else if (data.streamPath() == paths16) {
          raNgoai = boolValue;
          if (raNgoai) {
            Serial.println("Ra ngoài: Tắt cả LED1 và LED2.");
            led1State = false;
            digitalWrite(output4, LOW);
            update6Firebase();
            analogWrite(output4, 0);
        
          } else {
              Serial.println("Ở trong nhà: Không thay đổi trạng thái LED.");
            }
        } else if (data.streamPath() == paths5 ) { 
            if (value >= 0 && value <= 100) { // Chỉ nhận giá trị từ 0 đến 100
              if (data.streamPath() == paths5 && brightness1 != value) { // Chỉ gửi khi có sự thay đổi cho paths5
                if (led1State) {
                  brightness1 = value;
                  pwmValue1 = map(brightness1, 0, 100, 0, 255);
                  analogWrite(output4, pwmValue1);
                  Serial.println("Brightness1 (MUC SANG DEN 1): " + String(brightness1));
                }else {
                Serial.println("Không thể điều chỉnh độ sáng LED 1 khi đèn đang tắt.");
                }
              } 
             } 
        }  
else if (data.streamPath() == paths9) {
          diNgu = boolValue;
      
        }
}

void streamCallback(StreamData data) {
  Serial.println("Stream 1 received data!");
  printStreamData(data);
}
void streamCallback5(StreamData data) {
  Serial.println("Stream 5 received data!");
  printStreamData(data);
}
void streamCallback8(StreamData data) {
  Serial.println("Stream 8 received data!");
  printStreamData(data);
}
void streamCallback9(StreamData data) {
  Serial.println("Stream 9 received data!");
  printStreamData(data);
}
void streamCallback13(StreamData data) {
  Serial.println("Stream 13 received data!");
  printStreamData(data);
}
void streamCallback15(StreamData data) {
  Serial.println("Stream 15 received data!");
  printStreamData(data);
}
void streamCallback16(StreamData data) {
  Serial.println("Stream 16 received data!");
  printStreamData(data);
}
void streamTimeoutCallback(bool timeout) {
    if (timeout) {
        Serial.println("Stream timeout, reconnecting...");
        Firebase.beginStream(fbdo, paths);
        // Firebase.beginStream(fbdo17, paths17);
        Firebase.beginStream(fbdo5, paths5);
        Firebase.beginStream(fbdo9, paths9);
        // Firebase.beginStream(fbdo7, paths7);
        Firebase.beginStream(fbdo8, paths8);
        Firebase.beginStream(fbdo13, paths13);
        Firebase.beginStream(fbdo15, paths15);
        Firebase.beginStream(fbdo16, paths16);
        // Firebase.beginStream(fbdo9, paths9);
        // Firebase.beginStream(fbdo10, paths10);
    }
}
////TIMER
void IRAM_ATTR onTimer() {
  timer_flag = true;  
}
void IRAM_ATTR onTimer2() {
  timer2_flag = true;
}
void IRAM_ATTR onTimer3() {
  timer3_flag = true;
}
void IRAM_ATTR onTimer4() {
  timer4_flag = true;
}
////////BUTTON
void IRAM_ATTR button1Push(){
  button1State = true;
  // button1Pressed = true;
}
void setup() {
    Serial.begin(9600); 
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    { 
        Serial.print(".");

    } 
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  ///////////////////////
    config.host = "iot-arduino-40f94-default-rtdb.asia-southeast1.firebasedatabase.app";
    config.api_key = "AIzaSyDglobI51-CImZeGcvW3uca3j399N1ArRE";
    auth.user.email = "nhom12@gmail.com";
    auth.user.password = "1234567";
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);
    Firebase.beginStream(fbdo, paths);
    Firebase.beginStream(fbdo5, paths5);
    Firebase.beginStream(fbdo8, paths8);
    Firebase.beginStream(fbdo9, paths9);
    Firebase.beginStream(fbdo13, paths13);
    Firebase.beginStream(fbdo15, paths15);
    Firebase.beginStream(fbdo16, paths16);
    if(Firebase.ready()){
    Serial.println("Connected with FIREBASE!");
    }else{
    Serial.println("Failed to connect with FIREBASE!");
    }
    Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo5, streamCallback5, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo8, streamCallback8, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo9, streamCallback9, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo13, streamCallback13, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo15, streamCallback15, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo16, streamCallback16, streamTimeoutCallback);
    //////////INPUTOUTPUT
    pinMode(PIRSR501, INPUT);
    pinMode(output1, OUTPUT);
    digitalWrite(output1, LOW);
    pinMode(output3, OUTPUT);
    pinMode(output4, OUTPUT);
    ////THOI GIAN THUC
   configTime(25200, 0, "pool.ntp.org", "time.nist.gov");
     ///TIMER1
  timer = timerBegin(0, 80, true); 
  timerAttachInterrupt(timer, &onTimer, true); 
  timerAlarmWrite(timer, 2000000, true); 
  timerAlarmEnable(timer);
///TIMER2
  timer2 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer2, &onTimer2, true);
  timerAlarmWrite(timer2, 3000000, true);
  timerAlarmEnable(timer2);
  ////TIMER3
  timer3 = timerBegin(3, 80, true);
  timerAttachInterrupt(timer3, &onTimer3, true);
  timerAlarmWrite(timer3, 10000000, true);
  timerAlarmEnable(timer3);
  ////TIMER4
  timer4 = timerBegin(2, 80, true);
  timerAttachInterrupt(timer4, &onTimer4, true);
  timerAlarmWrite(timer4, 1000000, true);
  timerAlarmEnable(timer4);
    ////BUTTON
  //BUTTON1
  pinMode(button1 , INPUT_PULLDOWN); //14pin
  attachInterrupt(digitalPinToInterrupt(button1), button1Push, FALLING);
}


void loop() {
if(timer_flag){
  timer_flag = false;
  if(diNgu){
   valuepir = digitalRead(PIRSR501);
 
    if (valuepir == 1) {
      Serial.println("Có người!");
      digitalWrite(output1, HIGH);
    } else {
      Serial.println("Không có người.");
      digitalWrite(output1, LOW);
    }
} else{
   Serial.println("KHONG DOC CAM BIEN.");
   digitalWrite(output1, LOW);
}
}
if(timer2_flag){
  timer2_flag = false;
   if(savenl){
  if(thucDay){
  update5Firebase();
  }else{}
  }
}
if(timer3_flag){
  timer3_flag = false;
  if (led1State) {
    if(savenl){
      valueas = analogRead(CBAS);
      Serial.println("Chỉ số ánh sáng: ");
      Serial.println(valueas);
      if(thucDay){ 
        brightness1 = map(valueas, 0, 4095, 0, 100);
        Serial.print("Ánh sáng: ");
        Serial.print(brightness1);
        Serial.println("%");
        pwmValue1 = map(valueas, 0, 4095, 0, 255);
        analogWrite(output4, pwmValue1);
      }
    }
  } else{
      Serial.println("LED1 tắt. Đặt giá trị brightness1 = 0.");
  
    }
}
if(timer4_flag){
  timer4_flag = false;
  struct tm timeinfo;
        // Lấy thời gian
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Không thể lấy thời gian");
            return;
        }
        // Định dạng thời gian thành "HH:MM:SS"
        snprintf(currentTimeStr, sizeof(currentTimeStr), "%02d:%02d:%02d", 
                 timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        // Định dạng ngày thành "DD/MM/YYYY"
        snprintf(currentDateStr, sizeof(currentDateStr), "%02d/%02d/%04d",
                 timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900); // Cộng thêm 1900 cho năm
        // In ra thời gian và ngày tháng
        Serial.print("Thời gian hiện tại: ");
        Serial.println(currentTimeStr);
        Serial.print("Ngày hiện tại: ");
        Serial.println(currentDateStr);
        // Kiểm tra và kích hoạt báo thức
         for (int i = 0; i < alarmCount; i++) {
            if (alarms[i].time == String(currentTimeStr)) {
                // Báo thức kích hoạt
                Serial.println("Báo thức!!! Thời gian khớp với: " + alarms[i].time);
                if (alarms[i].note == "a") {
                    digitalWrite(output4, HIGH);
                    Firebase.setBool(firebaseData, "/BT1", true);
                    Serial.println("Gửi /BT1 = true lên Firebase");

                } else if (alarms[i].note == "b") {
                    digitalWrite(output4, LOW);
                    Firebase.setBool(firebaseData, "/BT1", false);
                    Serial.println("Gửi /BT1 = false lên Firebase");
                }
                for (int j = i; j < alarmCount - 1; j++) {
                    alarms[j] = alarms[j + 1];  // Dịch chuyển phần tử lên
                }
                alarmCount--; // Giảm số lượng báo thức
                i--; // Giảm chỉ số để không bỏ qua phần tử tiếp theo
            
            }                       
}
}
if (!raNgoai) {
if (button1State){
    if((millis() - pretimePress) > buttonDebounce){
      pretimePress = millis();
      led1State = !led1State;
      update6Firebase();  
      if(led1State){
        digitalWrite(output4, HIGH);
        Serial.println("DEN1: ON");
        if (brightness1 > 0) { // Kiểm tra và áp dụng độ sáng khi bật
            pwmValue1 = map(brightness1, 0, 100, 0, 255);
            analogWrite(output4, pwmValue1);
        }
      }else{
        Serial.println("DEN1: OFF");
        digitalWrite(output4, LOW);
        analogWrite(output4, 0);
      }
    }
    button1State = false;
  }
}
}



