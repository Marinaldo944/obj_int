#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHT_SENSOR_PIN  21
#define DHT_SENSOR_TYPE DHT22 

#define TOPICO_SUBSCRIBE_STATUS1 "OBJ_INT/STATUS_1" 
#define TOPICO_SUBSCRIBE_STATUS2 "OBJ_INT/STATUS_2"
#define TOPICO_SUBSCRIBE_STATUS3 "OBJ_INT/STATUS_3"

#define TOPICO_PUBLISH_TEMP   "OBJ_INT/TEMPERATURA"
#define TOPICO_PUBLISH_UMIAR   "OBJ_INT/UMIDADE_AR"
#define TOPICO_PUBLISH_UMISOLO   "OBJ_INT/UMIDADE_SOLO"

#define TOPICO_PUBLISH_BOMB_1   "OBJ_INT/BOMBA_1"
#define TOPICO_PUBLISH_BOMB_2   "OBJ_INT/BOMBA_2"
#define TOPICO_PUBLISH_BOMB_3   "OBJ_INT/BOMBA_3"
      
#define ID_MQTT  "OBJ_INT_CONEC1" 

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const char* SSID = "CLARO_C044F4-IOT"; 
const char* PASSWORD = "RckycpY2Ka"; 
  
const char* BROKER_MQTT = "test.mosquitto.org"; 
int BROKER_PORT = 1883;

boolean nivel_1 = false;
boolean nivel_2 = false;
boolean nivel_3 = false;
 
 
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
    init_dht();
    init_serial();
    init_wifi();
    init_mqtt();
}


void init_dht() 
{
    dht_sensor.begin();
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
    Serial.print(":");
    Serial.println(msg);

    
    if (msg == "0")
      {
        nivel_1 = false;
      } 
      
    if (msg == "1")
       {
        nivel_1 = true;
       } 
     
          
      Serial.println(nivel_1);
      
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
            MQTT.subscribe(TOPICO_SUBSCRIBE_STATUS1);
            MQTT.subscribe(TOPICO_SUBSCRIBE_STATUS2);
            MQTT.subscribe(TOPICO_SUBSCRIBE_STATUS3);
             
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
    int val = analogRead(35);
    int umisolo = (2700 - val)*100 / 1700;
    float umiar  = dht_sensor.readHumidity();
    float tempC = dht_sensor.readTemperature();
    if (umisolo < 32)
      {
        MQTT.publish(TOPICO_PUBLISH_BOMB_1, "1");
      } else 
         {
           MQTT.publish(TOPICO_PUBLISH_BOMB_1, "0");
         }
    verifica_conexoes_wifi_mqtt();
    MQTT.publish(TOPICO_PUBLISH_TEMP, String(tempC).c_str());
    MQTT.publish(TOPICO_PUBLISH_UMIAR, String(umiar).c_str());
    MQTT.publish(TOPICO_PUBLISH_UMISOLO, String(umisolo).c_str());
    MQTT.loop();
    delay(1000);   
}
