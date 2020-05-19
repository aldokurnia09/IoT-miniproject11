#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

const char* ssid = "Wifiku";
const char* password = "Passwordku";
const char* mqtt_server = "IPku";


const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 5, 4, 14, 12); // IN1, IN3, IN2, IN4
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // set the speed at 80 rpm:
  myStepper.setSpeed(80);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

void setup_wifi() {
   delay(3000);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  int p =(char)payload[0]-'0';
  // step one revolution  in one direction:
  if(p==1) 
  {
    myStepper.step(stepsPerRevolution);
    Serial.print("  clockwise" );
   }
  // step one revolution in the other direction:
  else if(p==2)
  {
    myStepper.step(-stepsPerRevolution);
    Serial.print("  counterclockwise" );
   }
   Serial.println();
}
 
//  Serial.println();
 //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.publish("state", "connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("putar");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} //end reconnect()
