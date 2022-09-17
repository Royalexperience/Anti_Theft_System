//-----Librerie------//
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


//-----Costanti------//
const int TRIG_PIN = 22 ; // Pin Esp32 Connesso al Sensore Ultrasonico TRIG pin
const int ECHO_PIN = 23 ; // Pin Esp32 Connesso al Sensore Ultrasonico  ECHO pin
const int LED_PIN  = 21 ; // Pin Esp32 Connesso al Led verde
const int LED2 = 19 ;     // Pin Esp32 Connesso al Led Giallo
const int LED3 = 18 ;     // Pin Esp32 Connesso al Led Rosso 
const int Soglia_verde = 50; // Soglia di distanza massima per led verde
const int Soglia_gialla = 30; // Soglia di distanza massima per led gialla
const int Soglia_rossa = 10; // Soglia di distanza massima per led rosso
const int SUONO = 15 ;              // Pin connesso al buzzer attivo per allarme 
String server = "YOUR_SERVER"; //Indirizzo Server dove inviare dati
const char* ssid = "YOUR_WIFI_NAME";   // Il nome della mia Wi-Fi
const char* password = "YOUR_WIFI_PASSWORD";  // Password del mio Wi-Fi

// Variabili :
float durata, distanza_cm;


//-----Settaggi Bot Telegram------//
WiFiClientSecure client; // Oggetto necessario per connessione a Wi-Fi di Esp32
#define BOTtoken "YOUR_BOT_TOKEN"   // Mio Token Bot
#define CHAT_ID "427223998" // ID che riconduce alla mia chat di Telegram
UniversalTelegramBot bot(BOTtoken, client); //settaggio Bot 


//-----Funzioni utili------//

//-----Connessione alla Rete-----//
void connect()
{
  Serial.print("Connessione al Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Aggiunge certificato root per api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("Connesso al WiFi");
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());
 
}


//-----Funzioni per utilizzare tone e noTone anche nell' Esp 32-----//
void tone(uint8_t pin, unsigned int frequenza, unsigned long durata, uint8_t canale)
{
    if (ledcRead(canale)) {
        log_e("Tono Canale %d gia' usato", canale);
        return;
    }
    ledcAttachPin(pin, canale);
    ledcWriteTone(canale, frequenza);
    if (durata) {
        delay(durata);
        noTone(pin, canale);
    }    
}

void noTone(uint8_t pin, uint8_t canale)
{
    ledcDetachPin(pin);
    ledcWrite(canale, 0);
}


//-----Setup-----//
void setup() {
  Serial.begin (9600);      
  connect();
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  
  pinMode(LED_PIN, OUTPUT);  
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(SUONO,OUTPUT);
 
}

//-----Loop-----//
void loop() 
{
  // Genera un impulso di  10 microsecondi al pin TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // misura la durata di un impulso dal pin ECHO
  durata = pulseIn(ECHO_PIN, HIGH);
  
  // Calcola la distanza in Cm durata * Velocita' del suono / 2 (andata e ritorno)
  distanza_cm = durata * 0.034 / 2;

  //Se la distanza è tra la soglia massima del verde e del giallo 
  if(distanza_cm < Soglia_verde && distanza_cm > Soglia_gialla )
  {
    //accendo il Led Verde
    digitalWrite(LED_PIN, HIGH);
     
  }
    //Se la distanza è tra la soglia massima del giallo e del rosso
    if(distanza_cm < Soglia_gialla && distanza_cm > Soglia_rossa)
     {
      //accendo il Led giallo
       digitalWrite(LED2, HIGH);
       //spengo il Led Verde
       digitalWrite(LED_PIN, LOW);

       //Faccio partire la prassi per un get al server siccome misuro distanza Semi-Critica
       HTTPClient http;
       String datiADb =String(distanza_cm);
       String path = server + "?distanza=" + datiADb ;
       http.begin(path.c_str());
       http.GET();
       http.end();
     }

    //Se la distanza è minore della soglia massima della rossa
     if(distanza_cm < Soglia_rossa)
     {
      //Setto uno stringa con emoji e avvisi letterali
      String risposta ="\U000026A0 ALLARME \U000026A0 \n\n \U0001F534 AVVICINAMENTO A DISTANZA CRITICA \U0001F534 \n\n Distanza : ";
      String cm = " cm";
      
      //Setto alto il LED rosso e abbasso gli altri
       digitalWrite(LED3, HIGH);
       digitalWrite(LED_PIN, LOW);
       digitalWrite(LED2, LOW);

       //Suono dell'allarme del buzzer attivo
       tone(SUONO, 2093, 1500, 0);
       noTone(SUONO, 0);

       //Setto il messaggio che il bot deve mandare
       risposta.concat(distanza_cm);
       risposta.concat(cm);
       bot.sendMessage(CHAT_ID, risposta, "Markdown");
       
     }
     
     //Ulteriore management dei led 
     if(distanza_cm > Soglia_gialla)
     {
      digitalWrite(LED2, LOW);
     }
    
    if(distanza_cm > Soglia_verde){
    
     digitalWrite(LED_PIN, LOW);
    }
    if(distanza_cm > Soglia_rossa){
    
     digitalWrite(LED3, LOW);
    }
   
  // Stampo dati al monitor seriale
  Serial.print("distanza: ");
  Serial.print(distanza_cm);
  Serial.println(" cm");

  delay(500);
}
