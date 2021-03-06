#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <RCSwitch.h>

// Relays e pinos
#define relay01 A0
#define relay02 3
#define relay03 4
#define relay04 5
#define relay05 6
#define relay06 7
#define relay07 8
#define relay08 9

// RF433 pinos
#define rf_transmitter A5

// MQTT Server
const char* BROKER_MQTT = "192.168.1.250";    // IP do MQTT Server
int BROKER_PORT = 1883;                       // Porta MQTT server
#define ID_MQTT "uno01"                       // ID do MQTT
#define TOPIC_RELAY_MQTT "relays/pincmd"      // Topico para Relays
#define TOPIC_ALARM_MQTT "alarm/portas/sala"  // Topico para Alarmes
#define USER_MQTT "bruno"                     // Usuario do MQTT
#define PWD_MQTT "dnakfg"                     // Senha MQTT

// Variavel de tempo para o Atualizador de status de MQTT
unsigned long startTime;

// RF433 things
RCSwitch mySwitch = RCSwitch();

// Configuracoes de Ethernet
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168, 1, 241);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Declaracao de funcoes
void mantemConexoes();
void atualizaMQTT();
void conectaMQTT();
void recebePayload(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient MQTT(ethClient);

long lastReconnectAttempt = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(relay01, OUTPUT);
  digitalWrite(relay01, HIGH);
  pinMode(relay02, OUTPUT);
  digitalWrite(relay02, HIGH);
  pinMode(relay03, OUTPUT);
  digitalWrite(relay03, HIGH);
  pinMode(relay04, OUTPUT);
  digitalWrite(relay04, HIGH);
  pinMode(relay05, OUTPUT);
  digitalWrite(relay05, HIGH);
  pinMode(relay06, OUTPUT);
  digitalWrite(relay06, HIGH);
  pinMode(relay07, OUTPUT);
  digitalWrite(relay07, HIGH);
  pinMode(relay08, OUTPUT);
  digitalWrite(relay08, HIGH);
  
  mySwitch.enableTransmit(rf_transmitter);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
    
  Ethernet.begin(mac, ip, gateway, subnet);
  // Note - the default maximum packet size is 128 bytes. If the
  // combined length of clientId, username and password exceed this,
  // you will need to increase the value of MQTT_MAX_PACKET_SIZE in
  // PubSubClient.h
  delay(1000); // Delay para garantir a conexão Ethernet.
    
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(recebePayload); 
  
  Serial.print("Endereço IP do Arduino: ");
  Serial.println(Ethernet.localIP());
}

//int tellstate = 0;

void loop()
{  
  mantemConexoes();
  MQTT.loop();

/* RF Receiver things */
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    Serial.println(value);                    //Exibe o Valor recebido pelo receptor RF433
    if (value == 0) {
      Serial.print("Unknown encoding");
    }  
    if ( mySwitch.getReceivedValue() == 6137257 ) { 
      Serial.print( mySwitch.getReceivedValue() );
      Serial.println( " / Alarme da Porta da Sala Acionado" );
      MQTT.publish(TOPIC_ALARM_MQTT,"alarm01");
    }

    mySwitch.resetAvailable();
  }

   if ( (millis() - startTime) > 30000 ) {
      atualizaMQTT();
      startTime = millis();
   }
   
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
   // Serial.println("MQTT Conectado."); // Descomente para testar Conexao MQTT  
}

void atualizaMQTT() {

   
    if ( digitalRead(relay01) ) {
       MQTT.publish("relays/stats/relay01","off");
    } else {
       MQTT.publish("relays/stats/relay01","on");
    }
    if ( digitalRead(relay02) ) {
       MQTT.publish("relays/stats/relay02","off");
    } else {
       MQTT.publish("relays/stats/relay02","on");
    }
    if ( digitalRead(relay03) ) {
       MQTT.publish("relays/stats/relay03","off");
    } else {
       MQTT.publish("relays/stats/relay03","on");
    }
    if ( digitalRead(relay04) ) {
       MQTT.publish("relays/stats/relay04","off");
    } else {
       MQTT.publish("relays/stats/relay04","on");
    }
    if ( digitalRead(relay05) ) {
       MQTT.publish("relays/stats/relay05","off");
    } else {
       MQTT.publish("relays/stats/relay05","on");
    }
    if ( digitalRead(relay06) ) {
       MQTT.publish("relays/stats/relay06","off");
    } else {
       MQTT.publish("relays/stats/relay06","on");
    }
    if ( digitalRead(relay07) ) {
       MQTT.publish("relays/stats/relay07","off");
    } else {
       MQTT.publish("relays/stats/relay07","on");
    }
    if ( digitalRead(relay08) ) {
       MQTT.publish("relays/stats/relay08","off");
    } else {
       MQTT.publish("relays/stats/relay08","on");
    }
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_RELAY_MQTT, 1);
        } 
        else {
            Serial.println("Nao foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void recebePayload(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
       Serial.println( msg );        // Exibe a entrada do MQTT Subscribe
    }
    
    if (msg == "rl1-0") {
       MQTT.publish("relays/stats/relay01","off");
       digitalWrite(relay01, HIGH);
    }

    if (msg == "rl1-1") {
       MQTT.publish("relays/stats/relay01","on");
       digitalWrite(relay01, LOW);
    }
    
    if (msg == "rl2-0") {
       MQTT.publish("relays/stats/relay02","off");
       digitalWrite(relay02, HIGH);
    }

    if (msg == "rl2-1") {
       MQTT.publish("relays/stats/relay02","on");
       digitalWrite(relay02, LOW);
    }
    
    if (msg == "rl3-0") {
       MQTT.publish("relays/stats/relay03","off");
       digitalWrite(relay03, HIGH);
    }

    if (msg == "rl3-1") {
       MQTT.publish("relays/stats/relay03","on");
       digitalWrite(relay03, LOW);
    }

    if (msg == "rl4-0") {
       MQTT.publish("relays/stats/relay04","off");
       digitalWrite(relay04, HIGH);
    }

    if (msg == "rl4-1") {
       MQTT.publish("relays/stats/relay04","on");
       digitalWrite(relay04, LOW);
    }

    if (msg == "rl5-0") {
       MQTT.publish("relays/stats/relay05","off");
       digitalWrite(relay05, HIGH);
    }

    if (msg == "rl5-1") {
       MQTT.publish("relays/stats/relay05","on");
       digitalWrite(relay05, LOW);
    }

    if (msg == "rl6-0") {
       MQTT.publish("relays/stats/relay06","off");
       digitalWrite(relay06, HIGH);
    }

    if (msg == "rl6-1") {
       MQTT.publish("relays/stats/relay06","on");
       digitalWrite(relay06, LOW);
    }

    if (msg == "rl7-0") {
       MQTT.publish("relays/stats/relay07","off");
       digitalWrite(relay07, HIGH);
    }

    if (msg == "rl7-1") {
       MQTT.publish("relays/stats/relay07","on");
       digitalWrite(relay07, LOW);
    }

    if (msg == "rl8-0") {
       MQTT.publish("relays/stats/relay08","off");
       digitalWrite(relay08, HIGH);
    }

    if (msg == "rl8-1") {
       MQTT.publish("relays/stats/relay08","on");
       digitalWrite(relay08, LOW);
    }

// Acionadores por RF433
    if (msg == "rf1-0") {
       mySwitch.switchOn(11111, 00010);
    }
    
    if (msg == "rf1-1") {
       mySwitch.switchOn(11111, 00011);
    }

    if (msg == "rf2-0") {
       mySwitch.switchOn(11112, 00010);
    }

    if (msg == "rf2-1") {
       mySwitch.switchOn(11112, 00010);
    }

    if (msg == "rf3-0") {
       mySwitch.switchOn(11113, 00010);
    }

    if (msg == "rf3-1") {
       mySwitch.switchOn(11113, 00010);
    }

}
