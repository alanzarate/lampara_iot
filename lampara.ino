int pulsadorPin = 19;
int ledPin = 26;
int pirPin = 16;
int stado= 0;
int ldrPin = 36;
bool flagLed = false;
#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000
void setup() {
  Serial.begin(115200);
  pinMode(pulsadorPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  stado = digitalRead(pulsadorPin);
  digitalWrite(ledPin, flagLed);
  ledcAttachPin(ledPin, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res); 
}
bool flagPulse = true;
void loop() {
  int valLdr = analogRead(ldrPin);
  int valPir = digitalRead(pirPin);
  if(stado != digitalRead(pulsadorPin)){
    stado = digitalRead(pulsadorPin);
     flagLed = !flagLed;

     
 // para prbar 
 ledcWrite(PWM1_Ch, a[0]);
     
  }
  if(flagLed){
    if(valLdr > 4000){
      digitalWrite(ledPin, HIGH);
    }else{
      digitalWrite(ledPin, LOW);
    }
//    if(valPir == 1){
//      digitalWrite(ledPin, HIGH);
//    }else{
//      digitalWrite(ledPin, LOW);
//    }
    //digitalWrite(ledPin, flagLed);
  }else{
    digitalWrite(ledPin, LOW);
  }
  
  Serial.print(digitalRead(pulsadorPin));
  Serial.print("..."+ String(flagLed));
  Serial.print("..."+String(valPir));
  Serial.print("..."+String(valLdr));
  Serial.println("");

}
