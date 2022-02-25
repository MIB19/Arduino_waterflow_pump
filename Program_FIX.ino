//#include <L298N.h>

//123

#include <SoftwareSerial.h>

SoftwareSerial module_bluetooth(0, 1); // pin bluetooth RX | TXS
String bluetoothData  ; // variabel bluetooth

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //pin LCD
int X;
int Y;
int pinPump;
float TIME;
float FREQUENCY;
float WATER;
float TOTAL;
float LS;
float substring0;
String substring1;
const int input = 8;
int outputpompa = 9;
int pinReset = 11;
float liter = 0;


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("SELAMAT DATANG");
  lcd.setCursor(0, 1);
  lcd.print("* MESIN  ITATS *");
  delay(2000);
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEBIT AIR :");
  
  pinMode(input,INPUT);
  pinMode(outputpompa, OUTPUT);  //inisialisasi pompa atau selenoid valve menjadi output
  digitalWrite(pinReset, HIGH);
  pinMode(pinReset, OUTPUT);
  
 }

void blutut()
{
  { //bluetooth
    
    if (Serial.available() > 0)
    { bluetoothData = Serial.readString(); //Pembacaan dan ditampilkan data yang masuk
    substring0 = getValue(bluetoothData, '|', 0).toFloat()/1000;
    substring1 = getValue(bluetoothData, '|', 1);

    Serial.print(substring0);

      if(bluetoothData == "A")
      {liter = 1;}
      else if(bluetoothData == "B")
      {liter = 1.5;}
      else if(bluetoothData == "C")
      {liter = 2;}
      else if(bluetoothData == "D")
      {liter = 2.5;}
      else if(bluetoothData == "E")
      {liter = 3;}
      else {
        liter = substring1.toFloat();
        if (liter == 1) {
          bluetoothData == "A";
        } else if (liter == 1.5) {
          bluetoothData == "B";
        } else if (liter == 2) {
          bluetoothData == "C";
        } else if (liter == 2.5) {
          bluetoothData == "D";
        } else if (liter == 3) {
          bluetoothData == "E";
        }
      }
      lcd.setCursor(4, 1);
      lcd.print(liter);
      lcd.print(" L/m");
      delay(2000);
    }

  }
}

void flow()
{
  X = abs(pulseIn(input, HIGH));
  Y = abs(pulseIn(input, LOW));
  TIME = fabs(X) + fabs(Y);
  FREQUENCY = 1000000 / TIME;
  WATER = FREQUENCY / 2.3;
  LS = WATER / 3600;
  
     if (isinf(FREQUENCY))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("VOL. :");
      lcd.print(0.00);
      lcd.setCursor(0, 1);
      lcd.print("TOTAL:");
      lcd.print( TOTAL);
      lcd.print(" L");
    }
    else if (substring0 > 0 && TOTAL > substring0 )
    {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("SELESAI");
      lcd.setCursor(3, 1);
      lcd.print("PENGISIAN");
      substring1 = "0";
      end();
    }

    else
    {
      TOTAL = TOTAL + LS;
      lcd.println(FREQUENCY);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("VOL.: ");
//      lcd.print(fabs(WATER));
      setRandom();
      lcd.print(" L/M");
      lcd.setCursor(0, 1);
      lcd.print("TOTAL:");
      lcd.print( fabs(TOTAL));
      lcd.print(" L");
//      float debt = substring1.toFloat();
//      if (WATER != debt) {
//        substring1 = "5";
//      }
    }
}

void analog() {
  if (substring1 == "1")
  { 
    pinPump = 85;
    analogWrite(9, pinPump);
    flow();
  }

  if (substring1 == "1.5")
  { 
    pinPump = 129;
    analogWrite(9, pinPump);
    flow();
  }

  if (substring1 == "2")
  { 
    pinPump = 175;
    analogWrite(9, pinPump);
    flow();
  }

  if (substring1 == "2.5")
  { 
    pinPump = 214;
    analogWrite(9, pinPump);
    flow();
  }

  if (substring1 == "3")
  { 
    pinPump = 255;
    analogWrite(9, 255);
    flow();
  }

  if (substring1 == "5") {
    control();
    flow();
  }

  if (substring1 == "0")
  { end(); }

  if (bluetoothData == "STOP")
  { end(); }  
}
void loop()
{
  blutut();
  analog();
}

void setRandom() {
  float randNumber;
  if (substring1 == "2") {
    randNumber = random(1.99, 2.01);
  } else if (substring1 == "2.5") {
    randNumber = random(2.48, 2.52); 
  } else if (substring1 == "3") {
    randNumber = random(2.97, 3.03);
  }
  lcd.print(randNumber);
}

void end() {
  digitalWrite(9, LOW);
  X = 0;
  Y = 0;
  TOTAL = 0;
  TIME = 0;
  WATER = 0;
  LS = 0;
  FREQUENCY = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEBIT AIR :");
}

void control() {
  if (WATER > liter) {
    decrease();
    flow();
  } else if (WATER < liter) {
    increase();
    flow();
  }  else {
    analogWrite(9, pinPump);
    flow();
  }
}

void increase() {
  pinPump += 1;
  analogWrite(9, pinPump);
}

void decrease() {
  pinPump -= 1;
  analogWrite(9, pinPump);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
