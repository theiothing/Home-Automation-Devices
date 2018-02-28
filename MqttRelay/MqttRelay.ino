/*  MQTT light/switch (relay control) with ESP8266 (NODEMCU - WEMOS) and MQTT

    - WiFi INITIALISAZION: as described embedded
    - MQTT standard pubsubclient LIBRARY.
        - reconnect() connect to the broker and SUBSCRIBE to topics
        - callback() recieve the messages this client is subscribed to
    - turnON/turnOFF function

    The IO Thing - www.theiothing.com
    Marco P. - v 1.4 - 07/2017

    find more info @:
    https://github.com/theiothing/Home-Automation-Devices

    if you like share (because sharing is caring)
    
*/

#include <ESP8266WiFi.h>      //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
#include <PubSubClient.h>   //https://github.com/knolleary/pubsubclient


/************ WIFI and MQTT AUTENTICATION ******************/
/****************    CUSTOMIZE THIS    *********************/
/*                       ||||||
                         ||||||
                        \||||||/
                         \||||/
                          \||/
                           \/                             */
// WiFi Parameters
const PROGMEM char* wifi_ssid = "[YOUR_NETWORK_NAME]";     //your WiFi name
const PROGMEM char* wifi_password = "[YOUR_PASSWORD]";     //your WiFi password

// MQTT Connection Parameters
const PROGMEM char* mqtt_server = "[YOUR_MQTT_BROKER]";    //your MQTT broker ip address or host name
const PROGMEM uint16_t mqtt_port = 1883;                   //your MQTT broker port
const PROGMEM char* mqtt_user = "[super_cool_username]";   //your MQTT username (if not set the parameter is ignored by the broker)
const PROGMEM char* mqtt_password = "[SuperSecretPSWD]";   //your MQTT password (if not set the parameter is ignored by the broker)

// MQTT topics
#define MAX_TOPIC_LENGTH 64
char topicHeader[MAX_TOPIC_LENGTH] = "myESPrelay";    //this will create a myESPrelay/{topic identifier}

char relayPowerSub[MAX_TOPIC_LENGTH] = "setPower";    //  turn ON turn OFF
char relayPowerPub[MAX_TOPIC_LENGTH]  = "powerSet";   //  and confirm

#define relayPin D1  //define relay pin
bool relayState = false; //defalut status @ boot - change to "true" if you want it to turn on @ boot

const char* p_relayOn = "ON";
const char* p_relayOff = "OFF";

/****************  END OF CUSTOMISATION  *********************/

WiFiClient espClient;                     //initialise a wifi client
PubSubClient client(espClient);           //creates a partially initialised client instance for MQTT connection
char msg_buff[50];                        //for storing incoming/outcoming messages
String topic;
String payload;

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

void publishRelay() {
  if (relayState) {
    client.publish(relayPowerPub, p_relayOn, true);
  } else {
    client.publish(relayPowerPub, p_relayOff, true);
  }
}

void setRelay() {
  if (relayState) {
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay on");
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay off");
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected. Make you things talk");
      // Once (re)connected (re)subscribe
      client.subscribe(relayPowerSub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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
  
  if (topic == String(relayPowerSub)) {
    if (payload == String(p_relayOn)) {
      if (relayState != true) {
        relayState = true;
        setRelay();
        publishRelay();
      }
    } else if (payload == String(p_relayOff)) {
      if (relayState != false) {
        relayState = false;
        setRelay();
        publishRelay();
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  digitalWrite(relayPin, LOW);
  Serial.print("Your relay Status topic: ");
  topicComposer(relayPowerPub);
  Serial.print("Your relay Command topic: ");  
  topicComposer(relayPowerSub);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);  //client is now ready for use
  client.setCallback(callback);              //client is now listening for incoming messages
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void topicComposer(char actionTopic[]) {
  //                                    TOPICHEADER/ACTION_TOPIC
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

/****** END OF FILE ******/
