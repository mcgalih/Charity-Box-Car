#define ledPin1 38
#define ledPin2 42

int i = 1;

// IR Sensor
#define IR_D_left 30
#define IR_D_right 50
#define IR_A_left A14
#define IR_A_right A15
unsigned int Dleft_ir, Dright_ir;
unsigned int Aleft_ir, Aright_ir;
// HIGH = tidak ada objek / objek hitam
// LOW = ada objek / objek putih

//ultrasonik
#include <NewPing.h>
#define FtrigPin 22
#define FechoPin 23
#define BtrigPin 24
#define BechoPin 25
NewPing Fsonar(FtrigPin, FechoPin, 50);
NewPing Bsonar(BtrigPin, BechoPin, 50);
unsigned int Fdistance;
unsigned int Bdistance;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  pinMode(IR_D_left, INPUT);
  pinMode(IR_D_right, INPUT);
  pinMode(IR_A_left, INPUT);
  pinMode(IR_A_right, INPUT);
}

void loop() {
  //LEDtest();

  //IR sensor
  Dleft_ir = digitalRead(IR_D_left);
  Aleft_ir = analogRead(IR_A_left);
  Dright_ir = digitalRead(IR_D_right);
  Aright_ir = analogRead(IR_A_right);
  ir_test();
  
  //Ultrasonic
  Fdistance = Fsonar.ping_cm();
  Bdistance = Bsonar.ping_cm();
  //ultrasonictest();

}

void LED(int LED1, int LED2){
  digitalWrite(ledPin1, LED1);
  digitalWrite(ledPin2, LED2);
}

void LEDtest(){
  LED(HIGH, LOW);
  delay(1000);
  LED(LOW, HIGH);
  delay(1000);
}

void ultrasonictest(){
  
  if(i <= 15){
    Serial.print(i);
    //Serial.print(") Depan = ");
    //Serial.print(Fdistance);
    Serial.print(") belakang = ");
    Serial.print(Bdistance);
    Serial.println(" cm");
    
    delay(500);
    
    i++;
  }
}
void ir_test(){
  float Aleft_volt = Aleft_ir * 5.0 / 1023.0;
  float Aright_volt = Aright_ir * 5.0 / 1023.0;
  if(i <= 15){
    Serial.print(i);
    Serial.println(")");
    Serial.print("Voltase kiri  = ");
    Serial.print(Aleft_volt);
    Serial.print(" Volt");
    Serial.print("\tnilai digital kiri  = ");
    Serial.println(Dleft_ir);
    Serial.print("voltase kanan = ");
    Serial.print(Aright_volt);
    Serial.print(" Volt");
    Serial.print("\tnilai digital kanan = ");
    Serial.println(Dright_ir);
    Serial.print("selisih = ");
    float selisih = Aleft_volt - Aright_volt;
    Serial.println(selisih);
    delay(500);
    i++;
  }
  
}
