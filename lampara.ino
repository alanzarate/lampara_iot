#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "config.h"
#include "pitches.h"
//pines
int pin_buzzer = 18;  
int pin_pir = 25;
int pin_redLed = 21;
int pin_greenLed = 22;
int pin_blueLed = 23;
int pin_poten = A0;
int pin_button = 13;
int pin_ldr = A3;
int pin_relay = 19;

// variables especiales
JSONVar readings;

// variables globales to send

String s_rssi = "";
String s_ip = "";
String s_hostname = "";
String s_wifiStatus ="";
String s_ssid ="";
String s_psk = "";
String s_bssid = ""; 
int s_foco = 0;


int page_current = 0;
int value_pwm = 0;
int value_ldr = 0;
int limit_ldr = 0;
int value_pir = 0;
// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

String getValues(){
  
  readings["rssi"] = String(s_rssi);
  readings["ip"] = s_ip;
  readings["hostname"] = s_hostname;
  readings["wifiStatus"] = s_wifiStatus;
  readings["ssid"] = s_ssid;
  readings["psk"] = s_psk;
  readings["bssid"] = s_bssid;
  readings["statusFoco"] = s_foco;
  readings["pwm"] = value_pwm;
  readings["ldr"] = value_ldr;
  readings["limldr"] = limit_ldr;
  readings["pir"] = value_pir;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Inicializando LittleFS
void initFS() {
 // Iniciamos  SPIFFS
  if(!SPIFFS.begin())
     { Serial.println("ha ocurrido un error al montar SPIFFS");
       return; }
}

// Inicializando WiFi
void initWiFi() {
// conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      // Esperamos un segundo
      Serial.println("Conectando a la red WiFi..");
    }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());
  
  s_ip = WiFi.localIP().toString().c_str();
  s_rssi = String(String(WiFi.RSSI()).toInt()*-1);
  s_hostname = WiFi.getHostname();
  s_wifiStatus =WiFi.status();
  s_ssid =WiFi.SSID().c_str();
  s_psk = WiFi.psk().c_str();
  s_bssid = WiFi.BSSIDstr().c_str(); 
  
}
String getLDR(){
  value_ldr = analogRead(pin_ldr);
  return String(value_ldr); 
}
void sound(){
  // begin: variables
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  // end variables 
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin_buzzer, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pin_buzzer);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pin_pir, INPUT);
  pinMode(pin_redLed, OUTPUT);
  pinMode(pin_greenLed, OUTPUT);
  pinMode(pin_blueLed, OUTPUT);
  pinMode(pin_poten, INPUT);
  pinMode(pin_button, INPUT);
  pinMode(pin_ldr, INPUT);
  pinMode(pin_relay, OUTPUT);

  ledcAttachPin(pin_greenLed, 0);  // PIN, CHANNEL
  ledcSetup(0, 1000, 8);  // CHANNEL, Frequency , RESOLUTION

  initWiFi();
  initFS();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
 
  server.serveStatic("/", SPIFFS, "/");
  
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getValues();
    request->send(200, "application/json", json);
    json = String();
  });

  server.on("/TURN", HTTP_PUT, [](AsyncWebServerRequest *request) {
    String val = request->arg("VALUE");
    String val2 = request->arg("PAGE");
    
    
  Serial.println("val get: " +val);
  Serial.println(val2);
   s_foco = val.toInt();
   page_current = val2.toInt();
   if(s_foco == 1){
    digitalWrite(pin_relay, HIGH);
   }else{
    digitalWrite(pin_relay, LOW);
   }
   Serial.println(s_foco);
   Serial.println(page_current);
    });

    server.on("/PWM", HTTP_PUT, [](AsyncWebServerRequest *request) {
    String val = request->arg("VALUE");
    String val2 = request->arg("PAGE");
    
    
  Serial.println("val get: " +val);
  Serial.println(val2);
  value_pwm = val.toInt();
   page_current = val2.toInt();
    });

    server.on("/LDRLIM", HTTP_PUT, [](AsyncWebServerRequest *request) {
    String val = request->arg("VALUE");
    String val2 = request->arg("PAGE");
    
    
  Serial.println("val get: " +val);
  Serial.println(val2);
  float valint = val.toFloat(); 
  float res = valint/100;
   
    limit_ldr = round(res*4095);
    Serial.println(limit_ldr);
    page_current = val2.toInt();
    });

    server.on("/LDR", HTTP_GET, [](AsyncWebServerRequest *request){
     String json = getValues(); 
     request->send(200, "application/json", json);
     json = String();
  });

  

  // Start server
  server.begin();
}

int sts_light = 0;
int sts_button = 0;
void loop() { 
   
  value_pir = analogRead(pin_pir);
  
  
  s_rssi = String(String(WiFi.RSSI()).toInt()*-1);
  value_ldr = analogRead(pin_ldr);
  if(s_foco == 0){
    digitalWrite(pin_relay, LOW);
  }else{
    digitalWrite(pin_relay, HIGH);
  }
  if(page_current == 1){
    if(s_foco == 1){
      ledcWrite(0, value_pwm);
    }else{
      ledcWrite(0, 0);
    }
    
  }else{
    if(page_current == 2){
      if(s_foco == 1){
        value_pwm = 255 - round(value_ldr/ 16.05);
      ledcWrite(0,value_pwm);
      }else{
        ledcWrite(0, 0);
      }
    }else{
      if(page_current == 3){
        
      }else{
        if(page_current == 4){
           if(value_ldr > limit_ldr){
            s_foco = 0;
           }else{
            s_foco = 1;
           }
        }else{
          s_foco  = value_pir;
          if(s_foco == 1){
      ledcWrite(0, value_pwm);
    }else{
      ledcWrite(0, 0);
    }if(value_ldr > limit_ldr){
            s_foco = 0;
           }else{
            s_foco = 1;
           }
        }
      }
    }
  }
  /*
  //ledcWrite(0, 15); // CHANNEL, value(0-255)

  // control button manual
  int r_btn = digitalRead(pin_button);
  if(sts_button != r_btn){
    sts_button = r_btn;
    sts_light = r_btn;
    digitalWrite(pin_relay, sts_light);
  }


  // control led by potentiometer
  
 * para PWM 
 * value:0 -> potentiometer:0
 * value:255 -> potentiometer:4095
 * factor de conversion: value = round(pottentiometer / 16.05)
 

  int r_poten = digitalRead(pin_poten);
  ledcWrite(0, round(r_poten/16.05));

  control by LDR
 *  
 *  ldr max = 4095; lightest
 *  ldr min = 0; darkest
 *  and for same value in led user the sample 
 

 // PWM
  int r_ldr = analogRead(pin_ldr);
  ledcWrite(0, round(r_ldr/ 16.05));
  
  // turn on by ldr

  if(r_ldr < 2047){
    sts_light = 1;
    digitalWrite(pin_relay, sts_light);
  }else{
    sts_light = 0;
    digitalWrite(pin_relay, sts_light);
  }
  */
  
}
