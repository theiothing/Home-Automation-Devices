/*  MQTT LIGHT with BRIGHTNESS control with ESP8266 (NODEMCU - WEMOS) and MQTT

    - WiFi INITIALISAZION: as described embedded
    - MQTT standard pubsubclient LIBRARY.
        - reconnect() connect to the broker and SUBSCRIBE to topics
        - callback() recieve the messages this client is subscribed to
    - turnON/turnOFF function
    - brightness control

    this sketch is intended to be used in a SMART HOME ENVIRONMENT like:
    HOME ASSISTANT
    NODERED
    OPEN HAB
    DOMOTICZ
    ....etc....

    find more info @:
    https://github.com/theiothing/Home-Automation-Devices
    
    if you like share! (because sharing is caring)

    Copyright and stuff - The IO Thing - www.theiothing.com
    Marco P. - v 1.3 - 02/2017

*/

#include <ESP8266WiFi.h>      //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
#include <PubSubClient.h>     //https://github.com/knolleary/pubsubclient
WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define MQTT_VERSION MQTT_VERSION_3_1_1                                   //say this to the libarary 

/************ WIFI and MQTT AUTENTICATION ******************/
                                                                          /****************    CUSTOMIZE THIS    *********************/
// WiFi Parameters
const PROGMEM char* wifi_ssid = "[YOUR_NETWORK_NAME]";                    //your WiFi name
const PROGMEM char* wifi_password = "[YOUR_PASSWORD]";                    //your WiFi password

// MQTT Connection Parameters
const PROGMEM char* mqtt_server = "[YOUR_MQTT_BROKER]";                   //your MQTT broker ip address or host name
const PROGMEM uint16_t mqtt_port = 1883;                                  //your MQTT broker port
const PROGMEM char* mqtt_user = "[super_cool_username]";                  //your MQTT username (if not set the parameter is ignored by the broker)
const PROGMEM char* mqtt_password = "[SuperSecretPSWD]";                  //your MQTT password (if not set the parameter is ignored by the broker)


// MQTT topics
#define MAX_TOPIC_LENGTH 64
char topicHeader[MAX_TOPIC_LENGTH] = "myDimmableLight";                   //this will create a myESPrelay/{topic identifier}

// TURN ON/OFF
char powerSub[MAX_TOPIC_LENGTH] = "setPower";                             //  turn ON turn OFF
char powerPub[MAX_TOPIC_LENGTH]  = "powerSet";                            //  and confirm

//  BRIGHTNESS
char brightnessSub[MAX_TOPIC_LENGTH] = "setBrightness";                   //  change brightness
char brightnessPub[MAX_TOPIC_LENGTH] = "brightnessSet";                   //  and confirm

// ON OFF PAYLOADS
const PROGMEM char* p_lightOn = "ON";
const PROGMEM char* p_lightOff = "OFF";
//
#define ledPin D5
boolean   ledState = false;                                               //defalut status @ BOOT - change to "true" if you want it to turn on @ boot
uint8_t   t_brightness = 128;                                             //target brightness @ BOOT
uint8_t   c_brightness = 0;                                               //current brightness

// TRANSITION                                                             change this if you want a faster or smoother transition
#define UPDATES_PER_SECOND 30
#define TRANSITION_TIME 300
uint8_t steps = TRANSITION_TIME / UPDATES_PER_SECOND;

/****************  END OF CUSTOMISATION  *********************/

#define BUFFER_SIZE 50                                                    //buffer used to send/receive data with MQTT
char msg_buff[BUFFER_SIZE];                                               //storage for incoming/outcoming messages

void setup_wifi() {
  randomSeed(micros());
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void dimLEDS(int cBright, int tBright) {                                  // function called to modify brightness and turn on/off with transition
  Serial.print(cBright);
  Serial.println(tBright);
  int deltaBrightness = tBright - cBright;
  int brightnessSteps = deltaBrightness / steps;
  for (int count = 0;  count < steps; count++) {
    cBright += brightnessSteps;
    analogWrite(ledPin, cBright);
    delay(1000 / UPDATES_PER_SECOND);
  }
  analogWrite(ledPin, tBright);
  c_brightness = tBright;
}

void publishState() {                                                     // function called to publish the state of the led (on/off)
  Serial.print("publish state:");
  Serial.println(!ledState);
  if (ledState) {
    client.publish(powerPub, p_lightOn, true);
  } else {
    client.publish(powerPub, p_lightOff, true);
  }
}

void publishBrightness() {                                                // function called to publish the brightness of the led
  itoa(c_brightness, msg_buff, 10);
  Serial.print("publish brightness:");
  Serial.println(msg_buff);
  client.publish(brightnessPub, msg_buff, true);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "aDimmableLight-";                                  // Create a random client ID
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {     // Attempt to connect
      Serial.println("connected. Make you things talk");
      publishBrightness();
      publishState();
      client.subscribe(powerSub);                                         // Once (re)connected (re)subscribe
      client.subscribe(brightnessSub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);                                                        // Wait 5 seconds before retrying
    }
  }
}

void callback(char* c_topic, byte* c_payload, unsigned int c_length) {

  for (int i = 0; i < c_length; i++) {
    msg_buff[i] = c_payload[i];
  }
  msg_buff[c_length] = '\0';
  String payload = String(msg_buff);
  String topic = String(c_topic);
  Serial.println();
  Serial.println("Message arrived: ");
  Serial.print("topic: [");
  Serial.print(topic);
  Serial.println("]");
  Serial.print("payload: [");
  Serial.print(payload);
  Serial.println("]");

  // handle message topic
  if (topic == String(powerSub)) {
    // test if the payload is equal to "ON" or "OFF"
    if (payload == p_lightOn) {                                           //turn ON
      if (ledState != true) {                                             //only if it was OFF
        ledState = true;
        dimLEDS(0, t_brightness);
        publishState();
        publishBrightness();
      }
    } else if (payload == p_lightOff) {
      if (ledState != false) {
        ledState = false;
        dimLEDS(c_brightness, 0);
        publishState();
      }
    }
  } else if (topic == brightnessSub) {
    uint8_t brightness = payload.toInt();
    if (brightness > 0 && brightness <= 255) {
      t_brightness = brightness;
      dimLEDS(c_brightness, t_brightness);
      publishBrightness();
      if (!ledState) {                                                    //if it was off publishes "ON"
        ledState = true;
        publishState();
      }
    } else if (brightness == 0) {
      ledState = false;
      dimLEDS(c_brightness, 0);
      publishState();
    }
  }
}

void topicComposer(char actionTopic[]) {
  //                            TOPICHEADER      /           ACTION_TOPIC
  String stringComposer = String(topicHeader) + "/" + String(actionTopic);
  if ( stringComposer.length() >= MAX_TOPIC_LENGTH) {
    Serial.print(" ERROR!! \nlength of topic exceed the max value of ");
    Serial.print(MAX_TOPIC_LENGTH - 1);
    Serial.print(" characters. Impossible to create such a topic: ");
    Serial.println(stringComposer);
    Serial.print("your topic is: ");
    Serial.println(actionTopic);
  } else {
    stringComposer.toCharArray(actionTopic, stringComposer.length() + 1);
    Serial.println(actionTopic);
  }
}

void setup() {
  Serial.begin(115200);                                                   // init the serial (choose the BAUD rate you prefer)
  pinMode(ledPin, OUTPUT);
  analogWriteRange(255);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  analogWrite(ledPin, LOW);
  if (ledState) {                                                         // if it should turn on @ boot
    dimLEDS(0, t_brightness);
  }
  setup_wifi();
  Serial.print("Your light Status topic: ");
  topicComposer(powerPub);
  Serial.print("Your light Command topic: ");
  topicComposer(powerSub);
  Serial.print("Your light Status brightness topic: ");
  topicComposer(brightnessPub);
  Serial.print("Your light Command brightness topic: ");
  topicComposer(brightnessSub);
  //init the MQTT connection
  client.setServer(mqtt_server, mqtt_port);                               //client is now ready for use
  client.setCallback(callback);                                           //client is now listening for incoming messages
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

/****** END OF FILE ******/
