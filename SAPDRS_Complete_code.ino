#include <WiFi.h>  // Include the WiFi library
#include <ArduinoOTA.h>  // Include the OTA (Over-the-Air) updates library
#include <U8g2lib.h>  // Include the U8g2 library for OLED display
#include <SPI.h>  // Include the SPI library for communication
#include <Adafruit_MPU6050.h>  // Include the Adafruit MPU6050 library for the accelerometer and gyroscope
#include <Adafruit_Sensor.h>  // Include the Adafruit Sensor library for sensor abstractions
#include <HardwareSerial.h>  // Include the HardwareSerial library for serial communication
#include <TinyGPS++.h>  // Include the TinyGPS++ library for GPS data
#include <Wire.h>  // Include the Wire library for I2C communication
#include <freertos/FreeRTOS.h>  // Include the FreeRTOS library for multitasking
#include <freertos/task.h>  // Include the FreeRTOS task library

const char* ssid = "INFINITY";  // Define the SSID of the WiFi network
const char* password = "Kd8637333961!!";  // Define the password of the WiFi network

#define EN1 35  // Define the EN1 pin
#define IN1 32  // Define the IN1 pin
#define IN2 33  // Define the IN2 pin
#define EN2 25  // Define the EN2 pin
#define IN3 26  // Define the IN3 pin
#define IN4 27  // Define the IN4 pin
#define al  34  // Define the al pin

U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);  // Initialize the OLED display

Adafruit_MPU6050 mpu;  // Create an MPU6050 object
HardwareSerial SerialGSM(1);  // Create a hardware serial object for GSM communication
TinyGPSPlus gps;  // Create a TinyGPS++ object
HardwareSerial SerialGPS(2);  // Create a hardware serial object for GPS communication

#define GPSBaud 9600  // Define the baud rate for GPS communication

const unsigned char logo[] PROGMEM = {  // Define the logo in program memory
  // Add the logo bitmap data here
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x1f, 0xff, 0xff, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x1f, 0xff, 0xff, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x1f, 0xff, 0xfe, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1f, 0xff, 0xfc, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1f, 0xff, 0xf8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1f, 0x83, 0xf0, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1f, 0x87, 0xf1, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1f, 0x8f, 0xe3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x9f, 0xc7, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x87, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbe, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbc, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xb8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xb0, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xa1, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x83, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x87, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x87, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x8f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x9f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

float latitude;  // Define a variable to store the latitude
float longitude;  // Define a variable to store the longitude

const int led = 23;  // Define the LED pin (white)
const int p = 2;  // Define the PIR sensor pin (green)
const int button = 13;  // Define the button pin
const int t1 = 17;  // Define the ultrasonic sensor 1 trigger pin
const int e1 = 5;  // Define the ultrasonic sensor 1 echo pin
const int t2 = 16;  // Define the ultrasonic sensor 2 trigger pin
const int e2 = 4;  // Define the ultrasonic sensor 2 echo pin

long duration;  // Define a variable to store the duration of ultrasonic pulse
float distance;  // Define a variable to store the calculated distance

TaskHandle_t Task1;  // Define a task handle for Task1
TaskHandle_t Task2;  // Define a task handle for Task2

void Task1Code(void* parameter);  // Declare Task1Code function
void Task2Code(void* parameter);  // Declare Task2Code function
void sendSMS(const char* phoneNumber);  // Declare sendSMS function
void alsms(const char* phoneNumber);  // Declare alsms function
int accidentCount = 0;  // Define a variable to count the number of accidents

void setup() {

  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  pinMode(button, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor
  pinMode(p, INPUT);  // Set PIR sensor pin as input
  pinMode(al, INPUT);  // Set alcohol sensor pin as input
  pinMode(e1, INPUT);  // Set ultrasonic sensor 1 echo pin as input
  pinMode(t1, OUTPUT);  // Set ultrasonic sensor 1 trigger pin as output
  pinMode(e2, INPUT);  // Set ultrasonic sensor 2 echo pin as input
  pinMode(t2, OUTPUT);  // Set ultrasonic sensor 2 trigger pin as output
  pinMode(led, OUTPUT);  // Set LED pin as output
  pinMode(IN1, OUTPUT);  // Set motor driver input pin 1 as output
  pinMode(IN2, OUTPUT);  // Set motor driver input pin 2 as output
  pinMode(IN3, OUTPUT);  // Set motor driver input pin 3 as output
  pinMode(IN4, OUTPUT);  // Set motor driver input pin 4 as output
  pinMode(EN1, OUTPUT);  // Set motor driver enable pin 1 as output
  pinMode(EN2, OUTPUT);  // Set motor driver enable pin 2 as output

  oled.begin();  // Initialize the OLED display
  oled.clear();  // Clear the OLED display
  oled.setFont(u8g2_font_ncenB08_tr);  // Set the font for the OLED display
  oled.firstPage();  // Prepare the first page for drawing
  do {
    oled.drawBitmap(0, 0, 128 / 8, 64, logo);  // Draw the logo on the OLED display
  } while (oled.nextPage());  // Continue to the next page if there is one

  delay(10000);  // Wait for 10 seconds

  oled.clearBuffer();  // Clear the display buffer
  oled.setFont(u8g2_font_ncenB18_tf);  // Set the font for the OLED display
  oled.setCursor(5, 20);  // Set cursor position
  oled.println("Welcome");  // Print "Welcome" on the OLED display
  oled.setCursor(42, 60);  // Set cursor position
  oled.println("Sir");  // Print "Sir" on the OLED display
  oled.sendBuffer();  // Send the buffer to the OLED display
  delay(2000);  // Wait for 2 seconds

  SerialGSM.begin(9600, SERIAL_8N1, 19, 18);  // Initialize serial communication for GSM at 9600 baud rate
  SerialGPS.begin(9600, SERIAL_8N1, 14, 12);  // Initialize serial communication for GPS at 9600 baud rate
  delay(100);  // Wait for 100 milliseconds

  if (!mpu.begin()) {  // Check if the MPU6050 sensor initialization failed
    Serial.println("Failed to find MPU6050 chip");  // Print error message to serial monitor
    while (1) delay(10);  // Wait indefinitely
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);  // Set the accelerometer range
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);  // Set the gyroscope range
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);  // Set the filter bandwidth
  Serial.println("Sensor initialized.");  // Print initialization message to serial monitor
  oled.clearBuffer();  // Clear the display buffer
  oled.setFont(u8g2_font_ncenB14_tf);  // Set the font for the OLED display
  oled.setCursor(20, 15);  // Set cursor position
  oled.println("Sensor");  // Print "Sensor" on the OLED display
  oled.setCursor(5, 50);  // Set cursor position
  oled.println("Initialized");  // Print "Initialized" on the OLED display
  oled.sendBuffer();  // Send the buffer to the OLED display
  delay(3000);  // Wait for 3 seconds

  Serial.println("Booting");  // Print booting message to serial monitor
  oled.clearBuffer();  // Clear the display buffer
  oled.setFont(u8g2_font_ncenB18_tf);  // Set the font for the OLED display
  oled.setCursor(5, 40);  // Set cursor position
  oled.println("Booting...");  // Print "Booting..." on the OLED display
  oled.sendBuffer();  // Send the buffer to the OLED display
  delay(2000);  // Wait for 2 seconds

  WiFi.begin(ssid, password);  // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    delay(1000);  // Wait for 1 second
    Serial.println("Connecting to WiFi...");  // Print connecting message to serial monitor
  }
  Serial.println("Connected to WiFi");  // Print connected message to serial monitor
  oled.clearBuffer();  // Clear the display buffer
  oled.setFont(u8g2_font_ncenB12_tf);  // Set the font for the OLED display
  oled.setCursor(0, 20);  // Set cursor position
  oled.println("Connected");  // Print "Connected" on the OLED display
  oled.setCursor(40, 40);  // Set cursor position
  oled.println("To");  // Print "To" on the OLED display
  oled.setCursor(40, 60);  // Set cursor position
  oled.println("WIFI..");  // Print "WIFI.." on the OLED display
  oled.sendBuffer();  // Send the buffer to the OLED display
  delay(1000);  // Wait for 1 second

  ArduinoOTA.onStart([]() {  // Define the OTA onStart callback
    String type;  // Create a string to hold the update type
    if (ArduinoOTA.getCommand() == U_FLASH) {  // Check if the command is to update the sketch
      type = "sketch";  // Set type to "sketch"
    } 
    else {  // Otherwise
      type = "filesystem";  // Set type to "filesystem"
    }
    Serial.println("Start updating " + type);  // Print start updating message to serial monitor
  });

  ArduinoOTA.onEnd([]() {  // Define the OTA onEnd callback
    Serial.println("\nEnd");  // Print end message to serial monitor
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {  // Define the OTA onProgress callback
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));  // Print progress message to serial monitor
  });

  ArduinoOTA.onError([](ota_error_t error) {  // Define the OTA onError callback
    Serial.printf("Error[%u]: ", error);  // Print error message to serial monitor
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");  // Print auth failed message if authentication error
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");  // Print begin failed message if beginning error
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");  // Print connect failed message if connection error
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");  // Print receive failed message if receiving error
    else if (error == OTA_END_ERROR) Serial.println("End Failed");  // Print end failed message if ending error
  });

  ArduinoOTA.begin();  // Begin OTA updates

  oled.clearBuffer();  // Clear the display buffer
  oled.setFont(u8g2_font_ncenB18_tf);  // Set the font for the OLED display
  oled.setCursor(2, 20);  // Set cursor position
  oled.println("OTA");  // Print "OTA" on the OLED display
  oled.setCursor(2, 60);  // Set cursor position
  oled.println("Ready...");  // Print "Ready..." on the OLED display
  oled.sendBuffer();  // Send the buffer to the OLED display
  delay(2000);  // Wait for 2 seconds

  Serial.println("OTA ready");  // Print OTA ready message to serial monitor
  Serial.print("IP address: ");  // Print IP address label to serial monitor
  Serial.println(WiFi.localIP());  // Print local IP address to serial monitor

  xTaskCreatePinnedToCore(Task1Code, "Task1", 10000, NULL, 1, &Task1, 0);  // Create Task1 on core 0
  xTaskCreatePinnedToCore(Task2Code, "Task2", 10000, NULL, 1, &Task2, 1);  // Create Task2 on core 1
}

void loop() {}

void Task1Code(void* parameter) { 
  while (1) {  // Infinite loop
    ArduinoOTA.handle();  // Handle OTA updates
    while (SerialGPS.available() > 0) {  // Check if data is available from the GPS
      gps.encode(SerialGPS.read());  // Encode the GPS data
      if (gps.location.isUpdated()) {  // Check if the GPS location is updated
        latitude = gps.location.lat();  // Update the latitude
        longitude = gps.location.lng();  // Update the longitude
      }
    }
    sensors_event_t a, g, temp;  // Create sensor event variables
    mpu.getEvent(&a, &g, &temp);  // Get sensor events for accelerometer, gyroscope, and temperature
    if (abs(g.gyro.x) > 2 || abs(g.gyro.y) > 2 || abs(g.gyro.z) > 2) {  // Check if the gyroscope readings exceed threshold
      accidentCount++;  // Increment accident count if threshold is exceeded
    } 
    else {
      accidentCount = 0;  // Reset accident count if threshold is not exceeded
    }
    if (accidentCount >= 2) {  // Check if accident count is greater than or equal to 2
      // Confirm after 3 consecutive detections
      oled.clearBuffer();  // Clear OLED display buffer
      oled.setCursor(2, 10);  // Set cursor position on OLED display
      oled.setFont(u8g2_font_ncenB10_tf);  // Set font for OLED display
      oled.println("Accident Alert!!!");  // Print accident alert message
      oled.setCursor(30, 30);  // Set cursor position on OLED display
      oled.println("Send SMS");  // Print send SMS message
      oled.setCursor(22, 50);  // Set cursor position on OLED display
      oled.println("TO Hospital");  // Print message to hospital
      oled.sendBuffer();  // Send buffer to OLED display
      sendSMS("+919064564189");  // Send SMS to hospital
      delay(6000);  // Delay for 6 seconds

      oled.clearBuffer();  // Clear OLED display buffer
      oled.setCursor(2, 10);  // Set cursor position on OLED display
      oled.println("Accident Alert!!!");  // Print accident alert message
      oled.setCursor(23, 30);  // Set cursor position on OLED display
      oled.println("Send SMS");  // Print send SMS message
      oled.setCursor(20, 50);  // Set cursor position on OLED display
      oled.println("To Police");  // Print message to police
      oled.sendBuffer();  // Send buffer to OLED display
      sendSMS("+918918673369");  // Send SMS to police
      delay(6000);  // Delay for 6 seconds

      oled.clearBuffer();  // Clear OLED display buffer
      oled.setCursor(2, 10);  // Set cursor position on OLED display
      oled.println("Accident Alert!!!");  // Print accident alert message
      oled.setCursor(20, 30);  // Set cursor position on OLED display
      oled.println("Send SMS To");  // Print send SMS message
      oled.setCursor(2, 50);  // Set cursor position on OLED display
      oled.println("Family Member");  // Print message to family member
      oled.sendBuffer();  // Send buffer to OLED display
      sendSMS("+919382395550");  // Send SMS to family member
      delay(6000);  // Delay for 6 seconds
    } 
    else {
      oled.clearBuffer();  // Clear OLED display buffer
      oled.setFont(u8g2_font_ncenB12_tf);  // Set font for OLED display
      oled.setCursor(5, 20);  // Set cursor position on OLED display
      oled.println("Safe Drive");  // Print safe drive message
      oled.setCursor(40, 60);  // Set cursor position on OLED display
      oled.println("Save Life");  // Print save life message
      oled.sendBuffer();  // Send buffer to OLED display
      delay(10);  // Delay for 10 milliseconds
    }
    delay(100);  // Delay for 100 milliseconds
  }
}

void Task2Code(void* parameter) {
  while (1) {  // Infinite loop
    ArduinoOTA.handle();  // Handle OTA updates
    int pir = digitalRead(p);  // Read the PIR sensor value
    int alc = analogRead(al);  // Read the alcohol sensor value
    int b = digitalRead(button);  // Read the button value
    int v = alc / 10;  // Convert alcohol sensor value to a smaller range
    Serial.println(pir);  // Print the PIR sensor value to serial monitor
    Serial.println(v);  // Print the converted alcohol sensor value to serial monitor
    // oled.clearBuffer();  // Clear the OLED display buffer (commented out)
    // oled.setFont(u8g2_font_ncenB12_tf);  // Set the font for the OLED display (commented out)
    // oled.setCursor(40, 40);  // Set cursor position on OLED display (commented out)
    // oled.println(pir);  // Print PIR sensor value on the OLED display (commented out)
    // oled.sendBuffer();  // Send buffer to OLED display (commented out)
    // delay(100);  // Wait for 100 milliseconds (commented out)
    if (pir == 1) {  // Check if PIR sensor is triggered
      if (v <= 340) {  // Check if alcohol sensor value is within safe range
        if (b == 1) {  // Check if button is pressed
          analogWrite(EN1, 100);  // Set motor speed
          digitalWrite(IN1, HIGH);  // Set motor direction
          digitalWrite(IN2, LOW);  // Set motor direction
          delay(100);  // Wait for 100 milliseconds
          digitalWrite(t1, LOW);  // Set ultrasonic sensor trigger low
          delayMicroseconds(5);  // Wait for 5 microseconds
          digitalWrite(t1, HIGH);  // Set ultrasonic sensor trigger high
          delayMicroseconds(20);  // Wait for 20 microseconds
          digitalWrite(t1, LOW);  // Set ultrasonic sensor trigger low

          duration = pulseIn(e1, HIGH);  // Read pulse duration from ultrasonic sensor
          distance = (duration / 2) * 0.0343;  // Calculate distance from pulse duration
          delay(100);  // Wait for 100 milliseconds

          // Serial.println(distance);  // Print distance to serial monitor (commented out)
          delay(100);  // Wait for 100 milliseconds

          if (distance <= 5) {  // Check if distance is less than or equal to 5 cm
            digitalWrite(led, HIGH);  // Turn on LED
            delay(100);  // Wait for 100 milliseconds
            digitalWrite(IN3, LOW);  // Stop motor
            digitalWrite(IN4, LOW);  // Stop motor
            delay(100);  // Wait for 100 milliseconds
          } 
          else {
            digitalWrite(led, LOW);  // Turn off LED
            delay(100);  // Wait for 100 milliseconds
            analogWrite(EN2, 75);  // Set motor speed
            digitalWrite(IN3, LOW);  // Set motor direction
            digitalWrite(IN4, HIGH);  // Set motor direction
            delay(100);  // Wait for 100 milliseconds
          }

        } 
        else {  // If button is not pressed
          analogWrite(EN1, 100);  // Set motor speed
          digitalWrite(IN1, HIGH);  // Set motor direction
          digitalWrite(IN2, LOW);  // Set motor direction
          delay(100);  // Wait for 100 milliseconds
          digitalWrite(t2, LOW);  // Set ultrasonic sensor trigger low
          delayMicroseconds(5);  // Wait for 5 microseconds
          digitalWrite(t2, HIGH);  // Set ultrasonic sensor trigger high
          delayMicroseconds(20);  // Wait for 20 microseconds
          digitalWrite(t2, LOW);  // Set ultrasonic sensor trigger low

          duration = pulseIn(e2, HIGH);  // Read pulse duration from ultrasonic sensor
          distance = (duration / 2) * 0.0343;  // Calculate distance from pulse duration
          delay(100);  // Wait for 100 milliseconds

          // Serial.println(distance);  // Print distance to serial monitor (commented out)
          delay(100);  // Wait for 100 milliseconds

          if (distance <= 5) {  // Check if distance is less than or equal to 5 cm
            digitalWrite(led, HIGH);  // Turn on LED
            delay(100);  // Wait for 100 milliseconds
            digitalWrite(IN3, LOW);  // Stop motor
            digitalWrite(IN4, LOW);  // Stop motor
            delay(100);  // Wait for 100 milliseconds
          } 
          else {
            digitalWrite(led, LOW);  // Turn off LED
            analogWrite(EN2, 75);  // Set motor speed
            digitalWrite(IN4, LOW);  // Set motor direction
            digitalWrite(IN3, HIGH);  // Set motor direction
            delay(100);  // Wait for 100 milliseconds
          }
        }
      } 
      else {  // If alcohol sensor value is not within safe range
        // sendSMS1("+918637333961","The Driver is drunk.");  // Send SMS indicating driver is drunk (commented out)
        alsms("+918637333961");  // Send alcohol sensor SMS
        delay(6000);  // Wait for 6 seconds
        // analogWrite(EN1, 150);  // Set motor speed (commented out)
        // digitalWrite(IN1, LOW);  // Stop motor (commented out)
        // digitalWrite(IN2, LOW);  // Stop motor (commented out)
        // delay(100);  // Wait for 100 milliseconds (commented out)

        digitalWrite(IN3, LOW);  // Stop motor
        digitalWrite(IN4, LOW);  // Stop motor
        analogWrite(EN1, 100);  // Set motor speed
        digitalWrite(IN1, LOW);  // Stop motor
        digitalWrite(IN2, LOW);  // Stop motor
        delay(100);  // Wait for 100 milliseconds
      }
    } 
    else {  // If PIR sensor is not triggered
      // sendSMS1("+918637333961","The Driver is drunk.");  // Send SMS indicating driver is drunk (commented out)

      digitalWrite(IN3, LOW);  // Stop motor
      digitalWrite(IN4, LOW);  // Stop motor
      delay(100);  // Wait for 100 milliseconds
      analogWrite(EN1, 100);  // Set motor speed
      digitalWrite(IN1, LOW);  // Stop motor
      digitalWrite(IN2, LOW);  // Stop motor
      delay(100);  // Wait for 100 milliseconds
    }

    delay(100);  // Wait for 100 milliseconds
  }
}

void sendSMS(const char* phoneNumber) {
  String locationLink = "Accident detected!!! -> https://maps.google.com/?q=";  // Create the message with accident alert and link to location
  locationLink += String(latitude, 6) + "," + String(longitude, 6);  // Append the latitude and longitude to the location link

  SerialGSM.println("AT+CMGF=1");  // Set GSM module to text mode
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.print("AT+CMGS=\"");  // Start the AT command to send SMS
  SerialGSM.print(phoneNumber);  // Insert the phone number to send SMS
  SerialGSM.println("\"");  // End the AT command to send SMS
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.print(locationLink);  // Send the location link as the SMS message
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.write(26);  // Send the ASCII code for CTRL+Z to indicate the end of the message
  delay(500);  // Wait for 500 milliseconds
  Serial.print("Sent SMS to: ");  // Print message to serial monitor
  Serial.println(phoneNumber);  // Print phone number to serial monitor
  Serial.print("Message: ");  // Print message to serial monitor
  Serial.println(locationLink);  // Print location link to serial monitor
}

void alsms(const char* phoneNumber) {
  String locationLink = "The Driver is drunk.";  // Create the message indicating the driver is drunk
  locationLink += "Location -> https://maps.google.com/?q=";  // Add the link to location
  locationLink += String(latitude, 6) + "," + String(longitude, 6);  // Append the latitude and longitude to the location link

  SerialGSM.println("AT+CMGF=1");  // Set GSM module to text mode
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.print("AT+CMGS=\"");  // Start the AT command to send SMS
  SerialGSM.print(phoneNumber);  // Insert the phone number to send SMS
  SerialGSM.println("\"");  // End the AT command to send SMS
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.print(locationLink);  // Send the location link as the SMS message
  delay(500);  // Wait for 500 milliseconds
  SerialGSM.write(26);  // Send the ASCII code for CTRL+Z to indicate the end of the message
  delay(500);  // Wait for 500 milliseconds
  Serial.print("Sent SMS to: ");  // Print message to serial monitor
  Serial.println(phoneNumber);  // Print phone number to serial monitor
  Serial.print("Message: ");  // Print message to serial monitor
  Serial.println(locationLink);  // Print location link to serial monitor
}
