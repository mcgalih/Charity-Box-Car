#define ledPin1 38 // LED kiri
#define ledPin2 42 // LED kanan

// IR Sensor
#define IR_mid 31
#define IR_left 30
#define IR_right 50
int mid_ir, left_ir, right_ir;
bool detection = false;
int total_count = 0;
bool belok = false;
bool DONE = false;
bool counting = true;
unsigned long time_now = 0;
int delaying = 500;

#include <AFMotor.h>
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
int vmaju = 100;
int vbelok = 180;

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
bool us_back = true;
bool us_front = false;

//push button
#define button 36
int val_button;
bool condition = true;
bool pushed = false;
#define ubin_terang true
#define ubin_gelap false

///=====================================================///
///=====================MAIN PROGRAM====================///
///=====================================================///

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  pinMode(IR_mid, INPUT);
  pinMode(IR_left, INPUT);
  pinMode(IR_right, INPUT);
  pinMode(button, INPUT_PULLUP);
}


void loop() {
  //push_button
  val_button = digitalRead(button);
  button_pushed(val_button, condition);
  
  //Ultrasonic
  Fdistance = Fsonar.ping_cm();
  Bdistance = Bsonar.ping_cm();
  
  //IR sensor
  mid_ir = digitalRead(IR_mid);
  left_ir = digitalRead(IR_left);
  right_ir = digitalRead(IR_right);
  unsigned long t = millis();
  
  if(counting == true && DONE == false){
    ir_control(mid_ir, total_count, t, belok, DONE);
  }
  
  //Motor driver
  if(belok == true){
    counting = false;
    if(t <= time_now + 1000) berhenti();
    else if(t > time_now + 1000){
      line_follower(left_ir, right_ir, condition);
      if(mid_ir == LOW){
        maju();
        Serial.println("maju dikit");
        delay(delaying);
        Serial.println("exit line follower");
        counting = true;
        belok = false;
      }
    }
  } else{
    if(DONE == true) berhenti();
    else ultrasonic_control(us_back, us_front, Bdistance, Fdistance);
  }
}

///==================================================///
///=====================FUNCTION=====================///
///==================================================///

void LED(int LED1, int LED2){
  digitalWrite(ledPin1, LED1);
  digitalWrite(ledPin2, LED2);
}

void mset(const int leftmotor, const int rightmotor, int v){
  motor1.run(leftmotor);  motor1.setSpeed(v);
  motor2.run(rightmotor); motor2.setSpeed(v);
  motor3.run(rightmotor); motor3.setSpeed(v);
  motor4.run(leftmotor);  motor4.setSpeed(v);
}

void maju(){
  LED(LOW, LOW);
  //mset(FORWARD, FORWARD, vmaju);
  motor1.run(FORWARD);  motor1.setSpeed(94);
  motor2.run(FORWARD); motor2.setSpeed(vmaju);
  motor3.run(FORWARD); motor3.setSpeed(vmaju);
  motor4.run(FORWARD);  motor4.setSpeed(94);
}

void bkiri(){
  LED(LOW, LOW);
  mset(BACKWARD, FORWARD, vbelok);
}

void bkanan(){
  LED(LOW, LOW);
  mset(FORWARD, BACKWARD, vbelok);
}

void berhenti(){
  LED(HIGH, HIGH);
  mset(RELEASE, RELEASE, 0);
}

void button_pushed(int val_button, bool &condition){ 
  if (pushed == false) {
    if (val_button == LOW) pushed = true;  // val_tombol = LOW -> tombol ditekan
  }
  
  if (pushed == true) {
    if (val_button == HIGH) {               // val_tombol = HIGH -> tombol dilepas
      if (condition == ubin_gelap) condition = ubin_terang;
      else if (condition == ubin_terang) condition = ubin_gelap;
      pushed = false;
    }
  }
}

void line_follower(int val_ir1, int val_ir2, bool condition){
  if(condition == ubin_terang){
    if (val_ir1 == HIGH && val_ir2 == LOW) bkiri();
    else if (val_ir1 == LOW && val_ir2 == HIGH) bkanan();
    else maju();
  } else if(condition == ubin_gelap){
    if (val_ir1 == LOW && val_ir2 == HIGH) bkiri();
    else if (val_ir1 == HIGH && val_ir2 == LOW) bkanan();
    else maju();
  }
}

void ir_control(int val_ir, int &total_count, unsigned long &t, bool &turn, bool &done){ 
  if(detection == false) {
    if(total_count == 0){
      if(val_ir == LOW){ // deteksi objek gelap
        detection = true;
      }
    } else if (total_count >= 1){
      if(t <= time_now + delaying){                // menunggu deteksi berikutnya setelah deteksi awal
        if(val_ir == LOW) detection = true;
      } else if (t > time_now + delaying){         // selesai counting
        if(total_count == 1){
          turn = true; done = false;
        } else if(total_count == 2){
          turn = false; done = true;
        }
        total_count = 0;   // reset
        time_now = t;
      }
    }
  }
  
  if (detection == true) {
    if (val_ir == HIGH) {
      total_count += 1;
      time_now = t;
      detection = false;
    }
  }
}

void ultrasonic_control(bool &back, bool &front, int back_d, int front_d){
  if(back == true){
    //mode maju
    maju();
    if(front_d >= 3 && front_d <= 15){
      back = false;
      front = true;
    }
  }
  if(front == true){
    // mode berhenti
    berhenti();
    if(back_d >= 3 && back_d <= 15){
      back = true;
      front = false;
    }
  }
}
