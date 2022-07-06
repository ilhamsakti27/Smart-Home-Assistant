// fan belum bisa, coba modifikasi rangkaiannya (gunakan relay) untuk memperkuat arus masuk ke motor dc.
// OLED belum bisa, coba untuk buat program di luar program ini krn terdapat masalah sambungan dengan adafruit saat melakukan tes on/off
// OLED pakek relay saja kaya fan

#include <WiFi.h>
#include <WebServer.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "SSD1306Wire.h"

#define Relay0            32                                // for LED0
#define Relay1            33                                // for LED1
#define Relay2            25                                // for LED2
#define Relay3            26                                // for LED3
#define Relay4            27                                // for LED4
#define Relay5            14                                // for LED5
#define Relay6            12                                // for LED6
#define Relay7            13                                // for LED7

/*********************** OLED related variables ******************************/
#define OLED_ADDR 0x3c
#define RelayOLED          15                                // for VCC OLED
#define OLED_SDA 21       //4 //TTGO board without SD Card has OLED SDA connected to pin 4 of ESP32
#define OLED_SCL 22       //15 //TTGO board without SD Card has OLED SCL connected to pin 15 of ESP32
#define OLED_RST 16       //Optional, TTGO board contains OLED_RST connected to pin 16 of ESP32

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
Adafruit_MQTT_Subscribe lampu0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu2"); // FeedName --- FOR LED0
Adafruit_MQTT_Subscribe lampu1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu3"); // FeedName --- FOR LED1
Adafruit_MQTT_Subscribe lampu2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu4"); // FeedName --- FOR LED2
Adafruit_MQTT_Subscribe lampu3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu5"); // FeedName --- FOR LED3
Adafruit_MQTT_Subscribe lampu4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu6"); // FeedName --- FOR LED4
Adafruit_MQTT_Subscribe lampu5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu7"); // FeedName --- FOR LED5
Adafruit_MQTT_Subscribe lampu6 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu8"); // FeedName --- FOR LED6
Adafruit_MQTT_Subscribe lampu7 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu9"); // FeedName --- FOR LED7

Adafruit_MQTT_Subscribe allLamp = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/lampu"); // FeedName --- FOR ALL LAMP

Adafruit_MQTT_Subscribe OLED = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Welcome Feed"); // FeedName --- FOR OLED

/******************************* define OLED ************************************/
SSD1306Wire display(OLED_ADDR, OLED_SDA, OLED_SCL);
int counter = 0;

void initOLED() {
   pinMode(OLED_RST, OUTPUT);
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
/******************************* define OLED ************************************/

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay0, OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);

  pinMode(RelayOLED, OUTPUT);
  initOLED();

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
  mqtt.subscribe(&lampu0);
  mqtt.subscribe(&lampu1);
  mqtt.subscribe(&lampu2);
  mqtt.subscribe(&lampu3);
  mqtt.subscribe(&lampu4);
  mqtt.subscribe(&lampu5);
  mqtt.subscribe(&lampu6);
  mqtt.subscribe(&lampu7);
  mqtt.subscribe(&allLamp);
  mqtt.subscribe(&OLED);
}

void loop() {
 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &lampu0) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu0.lastread);
      int Light0_State = atoi((char *)lampu0.lastread);
      digitalWrite(Relay0, (Light0_State));
      
    }

    if (subscription == &lampu1) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu1.lastread);
      int Light1_State = atoi((char *)lampu1.lastread);
      digitalWrite(Relay1, (Light1_State));
      
    }

    if (subscription == &lampu2) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu2.lastread);
      int Light2_State = atoi((char *)lampu2.lastread);
      digitalWrite(Relay2, (Light2_State));
      
    }

    if (subscription == &lampu3) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu3.lastread);
      int Light3_State = atoi((char *)lampu3.lastread);
      digitalWrite(Relay3, (Light3_State));
      
    }

    if (subscription == &lampu4) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu4.lastread);
      int Light4_State = atoi((char *)lampu4.lastread);
      digitalWrite(Relay4, (Light4_State));
      
    }

    if (subscription == &lampu5) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu5.lastread);
      int Light5_State = atoi((char *)lampu5.lastread);
      digitalWrite(Relay5, (Light5_State));
      
    }

    if (subscription == &lampu6) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu6.lastread);
      int Light6_State = atoi((char *)lampu6.lastread);
      digitalWrite(Relay6, (Light6_State));
      
    }

    if (subscription == &lampu7) {
      Serial.print(F("Got: "));
      Serial.println((char *)lampu7.lastread);
      int Light7_State = atoi((char *)lampu7.lastread);
      digitalWrite(Relay7, (Light7_State));
      
    }

    if (subscription == &allLamp) {
      Serial.print(F("Got: "));
      Serial.println((char *)allLamp.lastread);
      int All_Light_State = atoi((char *)allLamp.lastread);
      digitalWrite(Relay0, (All_Light_State));
      digitalWrite(Relay1, (All_Light_State));
      digitalWrite(Relay2, (All_Light_State));
      digitalWrite(Relay3, (All_Light_State));
      digitalWrite(Relay4, (All_Light_State));
      digitalWrite(Relay5, (All_Light_State));
      digitalWrite(Relay6, (All_Light_State));
      digitalWrite(Relay7, (All_Light_State));
      
    }

    if (subscription == &OLED) {
      Serial.print(F("Got: "));
      Serial.println((char *)OLED.lastread);
      int OLED_State = atoi((char *)OLED.lastread);
      
      showOLEDMessage("Num seconds is: ", String(counter), "");
      digitalWrite(RelayOLED, (OLED_State));
      delay(1000);
      counter = counter+1;
      
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
