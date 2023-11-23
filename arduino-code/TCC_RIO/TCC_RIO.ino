/*
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("A");
  delay(1000);
  Serial.print("B");
  delay(1000);
}
 */

#include <stdint.h>

int verde =11;
int ama =12;
int verme =13;
int rele = 10; 
int chuva = A0;
int pluvio = A1;
int boia1 = 9; 
int boia2 = 8;

void setup(){
  Serial.begin(9600);
  pinMode(verme, OUTPUT);
  pinMode(ama, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rele,OUTPUT);
  pinMode(chuva,INPUT);
  pinMode(pluvio,INPUT);
  pinMode(boia1,INPUT);
  pinMode(boia2,INPUT);
  digitalWrite(rele, HIGH);
}
void loop(){
  //status chuva
  int32_t Chovendo = analogRead(chuva);// valor do sensor de chuva   
  //Serial.println(Chovendo);
  //Serial.print("Tempo: ");// 1º if
  /*
  if (Chovendo <=600) { // esta chovendo?
    digitalWrite(rele, LOW);               
    //Serial.println("CHUVOSO"); // sim 
  }
  else {
    //Serial.println("NORMAL"); // não  
    digitalWrite(rele, HIGH);
  }
  */
  Chovendo = (1024-Chovendo)/1;
  //Serial.print("Sending data ");
  //Serial.println(Chovendo);
  Serial.write((char*)&Chovendo, sizeof(int32_t));
  // Fim chuva*******************
  
  // INÍCIO volume de chuva
  int32_t Volchu = analogRead(pluvio); // valor do sensor volume de chuva  
  /*
  //pausa motor durante a chuva
  if (Volchu<240){
    digitalWrite(rele, HIGH);
    delay(3000);
  }
  // FIM volume de chuva
  */
  
  // inicio Status do RIO
  int estadoB1 = digitalRead(boia1); /*ler sensor boia1*/
  //Serial.print("Alerta : "); 
  //Serial.println(estadoB1); 
  int estadoB2 = digitalRead(boia2); /*ler sensor boia2*/
  //Serial.print("Perigo : "); 
  //Serial.println(estadoB2); 
  
  // fim status do RIO
  if (estadoB1==0 and estadoB1==0 ){
    digitalWrite(verde, HIGH);} 
  else{ 
    digitalWrite(verde, LOW);
    //digitalWrite(verme, LOW);
  }
  if (estadoB1!=0){
    digitalWrite(verde, LOW);
    digitalWrite(verme, LOW);
    digitalWrite(ama, HIGH);
  }
  else{
    digitalWrite(ama, LOW);
  }
  if (estadoB2!=0){
    digitalWrite(verde, LOW);
    digitalWrite(ama, LOW);
    digitalWrite(verme, HIGH);
  }
  else{
    digitalWrite(verme, LOW);
  }
  delay(5000);
}
