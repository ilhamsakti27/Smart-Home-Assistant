//INI FINALLLLLLLLLLLLLLLLLLLLLLL

#include <WiFi.h>
#include <WebServer.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "SSD1306Wire.h"
#include <ESP32Servo.h> //Library untuk Servo

#define LED2            32                                // for 1st lamp
#define LED3            33                                // for 2nd lamp
#define LED4            25                                // for 3rd lamp
#define BUZZER          26                                // for Buzzer
//OLED related variables
#define OLED_ADDR 0x3c
#define OLED_SDA 21       //4 //TTGO board without SD Card has OLED SDA connected to pin 4 of ESP32
#define OLED_SCL 22       //15 //TTGO board without SD Card has OLED SCL connected to pin 15 of ESP32
#define OLED_RST 16       //Optional, TTGO board contains OLED_RST connected to pin 16 of ESP32
#define OLED_VCC 15

const int trigPin = 12;
const int echoPin = 13;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;
Servo myservo; //Buat object 1 buah motor servo

#define WLAN_SSID       "OPPO A53"                              // Your SSID
#define WLAN_PASS       "zxvdjd733"           // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                                // use 8883 for SSL
#define AIO_USERNAME    "ilhamssakti27"                     // Replace it with your username
#define AIO_KEY         "aio_Dbtp17tcTEkX0Y2cvG9Vp7UnsKmC"  // Replace with your Project Auth Key

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe lampu2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu2"); // FeedName --- FOR LED2
Adafruit_MQTT_Subscribe lampu3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu3"); // FeedName --- FOR LED3
Adafruit_MQTT_Subscribe lampu4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu4"); // FeedName --- FOR LED2
Adafruit_MQTT_Subscribe lampu5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu"); // FeedName --- FOR ALL LED
Adafruit_MQTT_Subscribe buzzer = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu5"); // FeedName --- FOR BUZZER
Adafruit_MQTT_Subscribe oled = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu6"); // FeedName --- FOR OLED

void MQTT_connect();

SSD1306Wire display(OLED_ADDR, OLED_SDA, OLED_SCL);
int counter = 0;

void initOLED() {
   pinMode(OLED_RST, OUTPUT);
   pinMode(OLED_VCC, OUTPUT);
   //Give a low to high pulse to the OLED display to reset it
   //This is optional and not required for OLED modules not containing a reset pin
   digitalWrite(OLED_RST, LOW);
   delay(20);
   digitalWrite(OLED_RST, HIGH);
}
void showOLEDMessage(String line1, String line2, String line3) {
   display.init();                        // clears screen
   display.setFont(ArialMT_Plain_16);
   display.drawString(0, 0, line1);       //  adds to buffer
   display.drawString(0, 20, line2);
   display.drawString(0, 40, line3);
   display.display();                     // displays content in buffer
} 

void setup() {
  Serial.begin(115200);

  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  initOLED();
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  myservo.attach(14); //Set servo pada pin PWM 14

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&lampu2);
  mqtt.subscribe(&lampu3);
  mqtt.subscribe(&lampu4);
  mqtt.subscribe(&lampu5);
  mqtt.subscribe(&buzzer);
  mqtt.subscribe(&oled);
}

void loop() {

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  if(distanceCm<=15) // Jarak (Cm) dapat anda sesuaikan
  {
   myservo.write(0); //Posisi servo pada 90 derajat
  }

  else{ //Jika jarak lebih dari yang ditentukan
   myservo.write(180); //Posisi servo pada 90 derajat
  }
  
  delay(1000);

 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {



    // mulai with button adafruit
    if (subscription == &lampu2) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu2.lastread);
      int Light1_State = atoi((char *)lampu2.lastread);
      digitalWrite(LED2, (Light1_State));
      
    }

    if (subscription == &lampu3) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu3.lastread);
      int Light2_State = atoi((char *)lampu3.lastread);
      digitalWrite(LED3, (Light2_State));
      
    }

    if (subscription == &lampu4) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu4.lastread);
      int Light3_State = atoi((char *)lampu4.lastread);
      digitalWrite(LED4, (Light3_State));
      
    }

    if (subscription == &lampu5) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu5.lastread);
      int Light4_State = atoi((char *)lampu5.lastread);
      digitalWrite(LED2, (Light4_State));
      digitalWrite(LED3, (Light4_State));
      digitalWrite(LED4, (Light4_State));
      
    }

    if (subscription == &buzzer) {
      Serial.print(F("Got: "));
      Serial.println((char *)buzzer.lastread);
      int Buzzer_State = atoi((char *)buzzer.lastread);
      digitalWrite(BUZZER, (Buzzer_State));
      
    }

    if (subscription == &oled) {
      Serial.print(F("Got: "));
      Serial.println((char *)oled.lastread);
      int OLED_State = atoi((char *)oled.lastread);
      if(OLED_State == 1){
        showOLEDMessage("Ilham", "Gennaro", "Alwan");
        digitalWrite(OLED_VCC, HIGH);
        delay(1000);
      }
      if(OLED_State == 0){
        digitalWrite(OLED_VCC, LOW);
        showOLEDMessage("Num seconds is: ", String(counter), " 02");
        delay(1000);
      }else {
        showOLEDMessage("Ilham", "Gennaro", "Alwan");
        digitalWrite(OLED_VCC, HIGH);
        delay(1000);
        continue;
      }

  
      
    }
    
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
