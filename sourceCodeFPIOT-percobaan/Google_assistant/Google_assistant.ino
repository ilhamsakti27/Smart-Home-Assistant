#include <WiFi.h>
#include <WebServer.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED2            32                                // for 1st lamp
#define LED3            33                                // for 2nd lamp
#define LED4            25                                // for 3rd lamp
#define BUZZER          26                                // for Buzzer

#define WLAN_SSID       "ITS-ASRAMA-E"                              // Your SSID
#define WLAN_PASS       "itssurabaya"           // Your password

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

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUZZER, OUTPUT);

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
}

void loop() {
 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
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
//      delay(2000);
     
//      digitalWrite(BUZZER, LOW);
//      while(Buzzer_State == 1){
//          digitalWrite(BUZZER, (Buzzer_State));
//          delay(1000);
//          digitalWrite(BUZZER, LOW);
//          delay(1000);
//      }
      
      
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
