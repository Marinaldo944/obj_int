#include <WiFi.h>
#include <PubSubClient.h>
#include <HCSR04.h>

#define rele 33
#define pino_trigger 26
#define pino_echo 27

#define TOPICO_SUBSCRIBE_BOMB_3 "OBJ_INT/BOMBA_3"
#define TOPICO_PUBLISH_NIVEL3   "OBJ_INT/NIVEL_RES_3"
#define TOPICO_PUBLISH_STATUS3   "OBJ_INT/STATUS_3"  
    
#define ID_MQTT  "OBJ_INT_CONEC4" 

UltraSonicDistanceSensor distanceSensor(pino_trigger, pino_echo);

const char* SSID = "CLARO_C044F4-IOT"; 
const char* PASSWORD = "RckycpY2Ka"; 
  
const char* BROKER_MQTT = "test.mosquitto.org"; 
int BROKER_PORT = 1883;
 
 
WiFiClient espClient;
PubSubClient MQTT(espClient);
  
void init_serial(void);
void init_wifi(void);
void init_mqtt(void);
void reconnect_wifi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void verifica_conexoes_wifi_mqtt(void);
 

void setup() 
{
    pinMode(rele, OUTPUT);
    digitalWrite(rele, LOW);
    init_serial();
    init_wifi();
    init_mqtt();
}


void init_serial() 
{
    Serial.begin(9600);
}
 
void init_wifi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconnect_wifi();
}
  
void init_mqtt(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); 
    MQTT.setCallback(mqtt_callback);            
}
  
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.print(topic);
    Serial.print(" : ");
    Serial.println(msg);

    if (msg == "0") {
       digitalWrite(rele, LOW);
    }

    if (msg == "1") {
       digitalWrite(rele, HIGH);
    }
}
  
void reconnect_mqtt(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE_BOMB_3);
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
void reconnect_wifi() 
{
    
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD);
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
void verifica_conexoes_wifi_mqtt(void)
{
    reconnect_wifi(); 
    if (!MQTT.connected()) 
        reconnect_mqtt(); 
} 
 
void loop() 
{   
    int nivel = distanceSensor.measureDistanceCm();
    verifica_conexoes_wifi_mqtt();
    MQTT.publish(TOPICO_PUBLISH_NIVEL3, String(nivel).c_str());
    if (nivel>20)
    {
      MQTT.publish(TOPICO_PUBLISH_STATUS3, "0");
    } else {
      MQTT.publish(TOPICO_PUBLISH_STATUS3, "1");
    }
    MQTT.loop();
    delay(1000);   
}
