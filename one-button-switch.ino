#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Parameters to modify for WiFi and MQTT 

const char* ssid = "TMNL-935A51"; // Enter the WiFi name
const char* password =  "JEY63L53AC8DTWNQ"; // Enter WiFi password
const char* mqttServer = "192.168.1.110"; // ip address of the MQTT server
const int mqttPort = 1883; // port MQTT server
const char* mqttUser = "mqttserver";  
const char* mqttPassword = "mqttserver";
char main_topic[] = "Luz_Blanca";
char aux_topic[] = "uno_aux";

// Pin of the relay is 5 on R1D1 WEMOS

int light = 5;



WiFiClient espClient;
PubSubClient client(espClient);

char recv_payload[] = " ";  //
 
void setup() {
 
  Serial.begin(115200);

  pinMode(light , OUTPUT) ;
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  client.publish("log", "Luz blanca connected"); //Topic name
  
  client.subscribe(main_topic);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {

  recv_payload[0] = (char)payload[0];
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();

  if(recv_payload[0] == '1'){
    digitalWrite(light, HIGH);
    Serial.println("Luz Blanca encendida");
    Serial.println(recv_payload);
    client.publish(aux_topic, "1");
    }
    else if(recv_payload[0] == '0'){
      digitalWrite(light,LOW);
      Serial.println("Luz Blanca apagada");
      Serial.println(recv_payload);
      client.publish(aux_topic, "0");
      }
      else if(recv_payload[0] != '1' && recv_payload[0] != '0' && recv_payload[0] != ' '){
        Serial.println("Input no conocido");
        Serial.println(recv_payload);
        }

  recv_payload[0] = ' ';
}
