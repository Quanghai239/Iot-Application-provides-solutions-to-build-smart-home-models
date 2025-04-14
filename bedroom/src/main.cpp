#include <Arduino.h>
#include <FirebaseESP32.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <cstring>
#include <time.h>
#include "DecisionTree.h"
Eloquent::ML::Port::DecisionTree weatherClassifier;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <DHT.h>
///////////////DINH NGHIA
#define output1 0
#define output5 18
// #define output6 25
// #define output7 26
// #define output9 22
#define NUMPIXELS 16
#define PIN     4
#define button1 23
#define button2 14
#define fireSensor 34 
#define WATERSENSOR 35
#define output2 12
///////////////LEDRGB

float t;
float h;
float hic;
int prediction;
float temp ;
float humi;
#define DHTPIN 32    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

////
#ifdef __AVR__
#include <avr/power.h>
#endif
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int red;
int green;
int blue;
int ledRgb = 0;
bool ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500; // Đặt thời gian nhấp nháy (500ms = 0.5s)
/////WATERSENSOR
int valuews = 0;
float waterLevelCm;
#define MAX_SENSOR_VALUE 3600
#define MAX_WATER_LEVEL_CM 5.0 
bool pumpRunning = false;
///////////////FIRESENSOR
bool fireValue = false;
bool ledButton3 = false;
bool ledButton4 = false;
int gas;
///////////////BUTTON
volatile bool button1State = false;
volatile bool button2State = false;
const int buttonDebounce = 500;
unsigned long currentMillis ;
unsigned long pretimePress = 0;
unsigned long pretime2Press = 0;
unsigned long receivePress = 0;
const int receiveDebounce = 9000;
bool diNgu = false;
bool raNgoai = false;
////////timer
// hw_timer_t   *timer       = NULL;
// volatile bool timer_flag  = false;
// hw_timer_t   *timer2      = NULL;
// volatile bool timer2_flag = false;
// hw_timer_t   *timer3      = NULL;
// volatile bool timer3_flag = false;
// hw_timer_t   *timer4      = NULL;
// volatile bool timer4_flag = false;
hw_timer_t *timers[4];  // Mảng chứa 4 bộ đếm thời gian
volatile bool timer_flags[4] = {false, false, false, false};  // Cờ trạng thái
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
///////////////
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData firebaseData;
FirebaseData fbdo;
FirebaseData fbdo4;
FirebaseData fbdo5;
FirebaseData fbdo7;
FirebaseData fbdo8;
FirebaseData fbdo9;
FirebaseData fbdo11;
FirebaseData fbdo12;
FirebaseData fbdo13;
FirebaseData fbdo14;
FirebaseData fbdo16;
FirebaseData fbdo17;
//////////DENPHONGNGU
bool led1State = false;
static int brightness1 = 0;
int pwmValue1;
/////////
String paths   = "/BT3";
String paths4  = "/EMERGYMODE_FLOOR1";
String paths5  = "/LB3";
String paths7  = "/LEDRGB";
String paths8  = "/DI_NGU";
String paths9  = "/TIME";
String paths11 = "/WS";
String paths12  = "/HUMIDITY";
String paths13 = "/DHT11";
String paths14 = "/PREDICTION";
String paths16 = "/RA_NGOAI";
String paths17 = "/GAS";
///////////////WIFI
// const char ssid[] = "GIANG L2";
// const char pass[] = "0393286953";
const char ssid[] = "wifi";
const char pass[] = "12345678";
// const char ssid[] = "Hoai Phuong";
// const char pass[] = "LTHP190498@";
// const char ssid[] = "Cà Phê Võng";
// const char pass[] = "";
// const char ssid[] = "GIANG L2";
// const char pass[] = "0393286953";
// const char ssid[] = "Nhà Thông Minh";
// const char pass[] = "0981805945";
// const char ssid[] = "Iphone 6s";
// const char pass[] = "12345678";
// const char ssid[] = "NHOM12";
// const char pass[] = "01234567";
///////SENDFIREBASE
void update1Firebase(){
  if (Firebase.setFloat(firebaseData, paths11, waterLevelCm)) {
    Serial.println("Đã gửi giá trị mực nước lên Firebase: " + String(waterLevelCm));
  } else {
    Serial.println("Firebase failed: " + firebaseData.errorReason());
  }
}
void update2Firebase(){
  if (Firebase.setFloat(firebaseData, paths12, h)) {
    Serial.println("Đã gửi giá trị độ ẩm lên Firebase: " + String(h));
  } else {
    Serial.println("Firebase failed: " + firebaseData.errorReason());
  }
}
void update3Firebase(){
  if (Firebase.setFloat(firebaseData, paths13, t)) {
    Serial.println("Đã gửi giá trị nhiệt độ lên Firebase: " + String(t));
  } else {
    Serial.println("Firebase failed: " + firebaseData.errorReason());
  }  
}
void update6Firebase(){
if (Firebase.setBool(firebaseData, paths, led1State)) {
    Serial.println("Đã gửi led1 lên Firebase");
  } else {
    Serial.println("Lỗi khi gửi led1 lên Firebase: " + firebaseData.errorReason());
  }
}
void update9Firebase() {
  if (Firebase.setBool(firebaseData, paths4, ledButton4)) {  //EMERGYMODE_FLOOR1
    Serial.println("Firebase updated EMERGYMODE_FLOOR1 successfully.");
  } else {
    Serial.println("Firebase failed to update EMERGYMODE_FLOOR1: " + firebaseData.errorReason());
  }
}
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
if (note == "e" || note == "f") { // Chỉ lưu khi note là "a" hoặc "b"
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

    if (data.streamPath() == paths) { 
    if (!raNgoai) { 
      if(!diNgu){
                led1State = boolValue; 
                if (led1State) {
                    Serial.println("LED1: Bật");
                    digitalWrite(output1, HIGH);
                    if (brightness1 > 0) { 
                        pwmValue1 = map(brightness1, 0, 100, 0, 255);
                        analogWrite(output1, pwmValue1);
                    }
                } else {
                    Serial.println("LED1: Tắt");
                    digitalWrite(output1, LOW);
                    analogWrite(output1, 0);
                }
              }else{
                Serial.println("Không thể bật LED1 khi đang ngủ.");
              }
            } else {
                Serial.println("Không thể bật LED1 khi đang ra ngoài.");
            }
    } 
    else if (data.streamPath() == paths8) {
          diNgu = boolValue;
          if (diNgu) {
            Serial.println("Ra ngoài: Tắt cả LED1 và LED2.");
            led1State = false;
            digitalWrite(output1, LOW);
            update6Firebase();
            analogWrite(output1, 0);

          } else {
              Serial.println("Ở trong nhà: Không thay đổi trạng thái LED.");
        }
        }else if (data.streamPath() == paths7) {
        if(!diNgu){
        ledRgb = value; // 
        Serial.println("RGB Data: " + String(ledRgb)); 
        red = (ledRgb >> 16) & 0xFF; 
        green = (ledRgb>> 8) & 0xFF;
        blue =ledRgb & 0xFF;
        Serial.println("R: " + String(red) + ", G: " + String(green) + ", B: " + String(blue));
        // analogWrite(redrgb, red); 
        // analogWrite(greenrgb, green); 
        // analogWrite(bluergb, blue); 
        for(int i=0; i<NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(red, green, blue)); //R,G,B
          pixels.show();
        } 
        } else{
          Serial.println("Đang ngủ không được xập xình.");
        }

      }else if (data.streamPath() == paths16) {
          raNgoai = boolValue;
          if (raNgoai) {
            Serial.println("Ra ngoài: Tắt cả LED1 và LED2.");
            led1State = false;
            digitalWrite(output1, LOW);
            update6Firebase();
            analogWrite(output1, 0);
          } else {
              Serial.println("Ở trong nhà: Không thay đổi trạng thái LED.");
            }
            } else if (data.streamPath() == paths5 ) { // Kiểm tra MUC SANG DEN 1 và 2
            
            if (value >= 0 && value <= 100) { // Chỉ nhận giá trị từ 0 đến 100
                if (data.streamPath() == paths5 && brightness1 != value) { // Chỉ gửi khi có sự thay đổi cho paths5
                   if (led1State) {
                   brightness1 = value;
                    pwmValue1 = map(brightness1, 0, 100, 0, 255);
                    analogWrite(output1, pwmValue1);
                    Serial.println("Brightness1 (MUC SANG DEN 1): " + String(brightness1));
                   }else {
                Serial.println("Không thể điều chỉnh độ sáng LED 1 khi đèn đang tắt.");
            }
          } 
        
  // } else if(data.streamPath() == paths17) {  //WARNINGMODE_FLOOR1
  //   ledButton3 = boolValue;
  //   if(ledButton3){
  //     Serial.println("WARNINGMODE_FLOOR1: ON");
  //   }else{
  //     Serial.println("WARNINGMODE_FLOOR1: OFF");
  //   }
  }
  
}else if(data.streamPath() == paths4){    //EMERGYMODE_FLOOR1
    ledButton4 = boolValue;
    if(ledButton4){
      Serial.println("EMERGYMODE_FLOOR1: ON");
    }else{
      Serial.println("EMERGYMODE_FLOOR1: OFF");
    }
  }
  else if (data.streamPath() == paths17) {
          gas = value;
          
        }
}
//////////////////////////////
void streamCallback(StreamData data) {
  Serial.println("Stream 1 received data!");
  printStreamData(data);
}
void streamCallback4(StreamData data) {
  Serial.println("Stream 4 received data!");
  printStreamData(data);
}
void streamCallback5(StreamData data) {
  Serial.println("Stream 5 received data!");
  printStreamData(data);
}
void streamCallback7(StreamData data) {
  Serial.println("Stream 7 received data!");
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
void streamCallback16(StreamData data) {
  Serial.println("Stream 16 received data!");
  printStreamData(data);
}
void streamCallback17(StreamData data) {
  Serial.println("Stream 17 received data!");
  printStreamData(data);
}
void streamTimeoutCallback(bool timeout) {
    if (timeout) {
        Serial.println("Stream timeout, reconnecting...");
        Firebase.beginStream(fbdo, paths);
        Firebase.beginStream(fbdo4, paths4);
        Firebase.beginStream(fbdo5, paths5);
        Firebase.beginStream(fbdo7, paths7);
        Firebase.beginStream(fbdo8, paths8);
        Firebase.beginStream(fbdo9, paths9);
        Firebase.beginStream(fbdo16, paths16);
        Firebase.beginStream(fbdo17, paths17);
    }
}

////TIMER
////TIMER
// void IRAM_ATTR onTimer() {
//   timer_flag = true;  
// }
// void IRAM_ATTR onTimer2() {
//   timer2_flag = true;
// }
// void IRAM_ATTR onTimer3() {
//   timer3_flag = true;
// }
// void IRAM_ATTR onTimer4() {
//   timer4_flag = true;
// }
// Hàm ngắt chung
void IRAM_ATTR onTimer1() { timer_flags[0] = true; }
void IRAM_ATTR onTimer2() { timer_flags[1] = true; }
void IRAM_ATTR onTimer3() { timer_flags[2] = true; }
void IRAM_ATTR onTimer4() { timer_flags[3] = true; }
void setupTimers() {
    const int prescaler = 80;  // 80MHz / 80 = 1MHz (1 tick = 1µs)
    const int intervals[4] = {4000000, 6000000, 10000000, 1000000};  // µs
    void (*callbacks[4])() = {onTimer1, onTimer2, onTimer3, onTimer4};

    for (int i = 0; i < 4; i++) {
        timers[i] = timerBegin(i, prescaler, true);  // Khởi tạo timer
        timerAttachInterrupt(timers[i], callbacks[i], true);  // Gắn hàm ngắt
        timerAlarmWrite(timers[i], intervals[i], true);  // Đặt khoảng thời gian
        timerAlarmEnable(timers[i]);  // Bật timer
    }
}


////////BUTTON
void IRAM_ATTR button1Push(){
  button1State = true;
  // button1Pressed = true;
}
void IRAM_ATTR button2Push(){
  button2State = true;
  // button2Pressed = true;
}
void blinkLED() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState; // Đảo trạng thái LED
        digitalWrite(output2, ledState);
    }
}


double calculate_heat_index(double temperature, double humidity) {
  // Coefficients for the heat index formula
  double c1 = -42.379;
  double c2 = 2.04901523;
  double c3 = 10.14333127;
  double c4 = -0.22475541;
  double c5 = -6.83783e-3;
  double c6 = -5.481717e-2;
  double c7 = 1.22874e-3;
  double c8 = 8.5282e-4;
  double c9 = -1.99e-6;
  //temperature = (t * 9.0 / 5.0) + 32.0;
  // Calculate the heat index
  double heat_index = (c1 + (c2 * temperature) + (c3 * humidity) + (c4 * temperature * humidity) +
                       (c5 * pow(temperature, 2)) + (c6 * pow(humidity, 2)) +
                       (c7 * pow(temperature, 2) * humidity) + (c8 * temperature * pow(humidity, 2)) +
                       (c9 * pow(temperature, 2) * pow(humidity, 2)));

  // Adjustments for specific conditions
  if (humidity < 13 && (80 <= temperature && temperature <= 112)) {
      double adjustment = ((13 - humidity) / 4.0) * sqrt((17 - fabs(temperature - 95.0)) / 17);
      heat_index -= adjustment;
  } else if (humidity > 85 && (80 <= temperature && temperature <= 87)) {
      double adjustment = ((humidity - 85) / 10.0) * ((87 - temperature) / 5.0);
      heat_index += adjustment;
  }
  heat_index = (heat_index - 32) * 5.0 / 9.0;
  return heat_index;
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
    Firebase.beginStream(fbdo4, paths4);
    Firebase.beginStream(fbdo5, paths5);
    Firebase.beginStream(fbdo7, paths7);
    Firebase.beginStream(fbdo8, paths8);
    Firebase.beginStream(fbdo9, paths9);
    Firebase.beginStream(fbdo16, paths16);
    Firebase.beginStream(fbdo17, paths17);
    if(Firebase.ready()){
    Serial.println("Connected with FIREBASE!");
    }else{
    Serial.println("Failed to connect with FIREBASE!");
    }
    Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo4, streamCallback4, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo5, streamCallback5, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo7, streamCallback7, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo8, streamCallback8, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo9, streamCallback9, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo16, streamCallback16, streamTimeoutCallback);
    Firebase.setStreamCallback(fbdo17, streamCallback17, streamTimeoutCallback);
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();
    analogSetAttenuation(ADC_11db);
    pinMode(fireSensor, INPUT);
    pinMode(output1, OUTPUT);
    pinMode(output2, OUTPUT);
    pinMode(output5, OUTPUT);
    // pinMode(output6, OUTPUT);
    // pinMode(output7, OUTPUT);
    // pinMode(output9, OUTPUT);
  ////THOI GIAN THUC
  configTime(25200, 0, "pool.ntp.org", "time.nist.gov");
  setupTimers();
  //BUTTON1
  pinMode(button1 , INPUT_PULLDOWN); //14pin
  attachInterrupt(digitalPinToInterrupt(button1), button1Push, FALLING);
//BUTTON2
  pinMode(button2 , INPUT_PULLDOWN); //12pin
  attachInterrupt(digitalPinToInterrupt(button2), button2Push , FALLING);   
  Wire.begin();
  lcd.begin(16,2);          // Khởi tạo LCD
  lcd.backlight();     // Bật đèn nền LCD
  lcd.setCursor(0, 0); // Đặt con trỏ tại hàng 0, cột 0
  lcd.print("Xin chao ESP32!"); // In ra LCD
  dht.begin();
}

void loop() {
  //////////////////////////////////////
    // put your main code here, to run repeatedly:
  if((millis() - receivePress) > receiveDebounce ){
    receivePress = millis();
       // Read humidity
    h = dht.readHumidity();
    // Read temperature as Celsius
     t = dht.readTemperature();
    temp = (t * 9.0 / 5.0) + 32.0;
    // Compute heat index in Celsius (the default)
     hic = calculate_heat_index(temp,h);
    float input[3] = {temp,h,hic};
    // Giving input values to predict Function
    int prediction = weatherClassifier.predict(input);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loi doc DHT11");

    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("T:");
      lcd.print(t);
      lcd.print(" C");

      lcd.setCursor(8, 0);
      lcd.print("H:");
      lcd.print(h);
      lcd.print(" %");
      if(prediction == 0){
        Serial.println("Foggy \U0001F32B");
        lcd.setCursor(0, 1);
        lcd.print("Foggy!");
        Firebase.setString(firebaseData, paths14, "FOGGY");
      }
      else if(prediction == 1){
        Serial.println("Rainy \U0001F327");
        lcd.setCursor(0, 1);
        lcd.print("Rainy!");
        Firebase.setString(firebaseData, paths14, "RAINY");
      }
      else if(prediction == 2){
        Serial.println("Cloudy \u2601️");
        lcd.setCursor(0, 1);
        lcd.print("Cloudy!");
        Firebase.setString(firebaseData, paths14, "CLOUDY");
      }
      else if(prediction == 3){
        Serial.println("Sunny \u2600");
        lcd.setCursor(0, 1);
        lcd.print("Sunny!");
        Firebase.setString(firebaseData, paths14, "SUNNY");
    }

    }
    
  
  
    // Print values of temperature, humidity and heat index to serial monitor
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C  Heat index: "));
    Serial.print(hic);
    Serial.println(F("°C"));

    // Checking Condition of weather
    Serial.print("Prediction: ");
    // Serial.print(prediction);  
    Serial.println(); 
  }

  //////////////////////////////////////
   for (int i = 0; i < 4; i++) {
        if (timer_flags[i]) {
            timer_flags[i] = false;
            
            switch (i) {
                case 0:  // Timer 1: Đọc cảm biến lửa
                    fireValue = digitalRead(fireSensor);
                    Serial.print("Giá trị fireValue thứ nhất: ");
                    Serial.println(fireValue);
                    break;

                case 1:  // Timer 2: Cập nhật Firebase
                     update1Firebase();
                     update2Firebase();
                     update3Firebase();
                    break;

                case 2:  // Timer 3: Đọc cảm biến nước
                   valuews = analogRead(WATERSENSOR);
                  waterLevelCm = (float)valuews / MAX_SENSOR_VALUE * MAX_WATER_LEVEL_CM;

// Làm tròn giá trị đến 1 chữ số thập phân
                  waterLevelCm = round(waterLevelCm * 10) / 10.0;

                 
                Serial.print("Mực nước: ");
                Serial.print(waterLevelCm, 1);  // Giới hạn 1 chữ số thập phân
                Serial.println(" cm");
                    break;

                case 3:  // Timer 4: Lấy thời gian và kiểm tra báo thức
                    struct tm timeinfo;
                    if (!getLocalTime(&timeinfo)) {
                        Serial.println("Không thể lấy thời gian");
                        return;
                    }
                    
                    snprintf(currentTimeStr, sizeof(currentTimeStr), "%02d:%02d:%02d", 
                             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                    snprintf(currentDateStr, sizeof(currentDateStr), "%02d/%02d/%04d",
                             timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

                    Serial.print("Thời gian hiện tại: ");
                    Serial.println(currentTimeStr);
                    Serial.print("Ngày hiện tại: ");
                    Serial.println(currentDateStr);

                    // Kiểm tra báo thức
                    for (int j = 0; j < alarmCount; j++) {
                        if (alarms[j].time == String(currentTimeStr)) {
                            Serial.println("Báo thức!!! Thời gian khớp với: " + alarms[j].time);
                            
                            if (alarms[j].note == "e") {
                                digitalWrite(output1, HIGH);
                                Firebase.setBool(firebaseData, "/BT3", true);
                                Serial.println("Gửi /BT3 = true lên Firebase");
                            } else if (alarms[j].note == "f") {
                                digitalWrite(output1, LOW);
                                Firebase.setBool(firebaseData, "/BT3", false);
                                Serial.println("Gửi /BT3 = false lên Firebase");
                            }

                            // Xóa báo thức đã kích hoạt
                            for (int k = j; k < alarmCount - 1; k++) {
                                alarms[k] = alarms[k + 1];
                            }
                            alarmCount--;
                            j--;  // Kiểm tra lại phần tử tại vị trí j
                        }
                    }
                    break;
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
         digitalWrite(output1, HIGH);
        Serial.println("DEN1: ON");
        if (brightness1 > 0) { // Kiểm tra và áp dụng độ sáng khi bật
            pwmValue1 = map(brightness1, 0, 100, 0, 255);
            analogWrite(output1, pwmValue1);
        }
      }else{
        Serial.println("DEN1: OFF");
        digitalWrite(output1, LOW);
        analogWrite(output1, 0);
       
      }
    }
    button1State = false;
  }
}
// if(ledButton4){
//   digitalWrite(output9, HIGH); 
// } else {
//   digitalWrite(output9, LOW); 
// }

if(ledButton4 || !fireValue || gas > 1500 ){
      
       blinkLED(); // Gọi hàm nhấp nháy
      // digitalWrite(output6, HIGH);
      // digitalWrite(output7, HIGH);
    }else{
      digitalWrite(output2, LOW);
      
      // digitalWrite(output6, LOW);
      // digitalWrite(output7, LOW);
} 

if(button2State){
    if((millis() - pretime2Press) > buttonDebounce){
      pretime2Press = millis();
       ledButton4 = !ledButton4;
       update9Firebase();
      if(ledButton4){
        Serial.println("EMERMODE :ON ");
      }
      else{
        Serial.println("EMERMODE : OFF");
      }
    }
    button2State = false;
  }
   if (waterLevelCm < 1 && !pumpRunning) {
        digitalWrite(output5, HIGH); 
        pumpRunning = true;          
    }
    
    if (waterLevelCm >= 4 && pumpRunning) {
        digitalWrite(output5, LOW);   
        pumpRunning = false;         
    }
}


