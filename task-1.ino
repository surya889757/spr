
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h> 

#define motor1_A 10
#define motor1_B 9
#define Switch 6
#define ir_sensor_1 A0
#define ir_sensor_2 A1
#define metal_sensor A2
// Declare the Servo pin 
int servoPin = 7;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
unsigned int count=0;
bool ir_flag=0;
bool metal_flag=0;
String api_key="6O8OV4C6K09SSB97";
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Create a servo object 
Servo Servo1; 
void setup() 
{
// We need to attach the servo to the used pin number 
Servo1.attach(servoPin);
Serial.begin(9600);
lcd.begin(16, 2);

pinMode(LED_BUILTIN, OUTPUT);

pinMode(Switch,INPUT_PULLUP);
pinMode(ir_sensor_1,INPUT);
pinMode(ir_sensor_2,INPUT);
pinMode(metal_sensor,INPUT);

pinMode(motor1_A,OUTPUT);
pinMode(motor1_B,OUTPUT);

digitalWrite(motor1_A, LOW);
digitalWrite(motor1_B, LOW);

lcd.clear();
delay(10);
lcd.print(" SMART METAL ");
delay(10);
lcd.setCursor(0, 1);
delay(10);
lcd.print("BOTTLE COUNTER ");
delay(2000);
//give the sensor some time to calibrate
lcd.clear();
delay(10);
lcd.print("calibrating ");
lcd.setCursor(0, 1);
delay(10);
lcd.print("Sensors");
 for(int i = 0; i < 6; i++){
      lcd.print(".");
      delay(500);
      }
lcd.clear();
delay(10);
lcd.print("SENSORS ACTIVE");
delay(1000);
lcd.clear();
delay(10);
lcd.print("SETTING UP GSM ");
delay(1000);
lcd.setCursor(0, 1);
delay(10);
lcd.print("     DONE     ");
delay(1000);
}



void loop() 
{
lcd.clear();
delay(10);
lcd.print("Status:");
delay(10);
lcd.setCursor(0, 1);
delay(10);
lcd.print("count: "+String(count));
if(digitalRead(Switch)==LOW)  //////// SWITCH TO UPLOAD TO THINKSPEAK
{
 digitalWrite(motor1_A, LOW);
digitalWrite(motor1_B, LOW);
 lcd.clear();
delay(100);
lcd.print("Uploading Data..");
delay(1000);
GPRS_send(count);
// count=0;
}
else
{
 if(digitalRead(ir_sensor_1)==LOW)  //  BOTTLE ENTRY DETECTING CONDITION
 {
  ir_flag=!(ir_flag);
  delay(100); 
  ////////////////// FRESH BOTTLE //////
  if(ir_flag==1)
  {
lcd.setCursor(7, 0);
delay(10);
lcd.print("New Entry");
  digitalWrite(motor1_A, HIGH);
digitalWrite(motor1_B, LOW);
  }
  else    ////////// RETURNED BOTTLE
  {
    lcd.setCursor(7, 0);
delay(10);
lcd.print("Exiting");
digitalWrite(motor1_A, LOW);
digitalWrite(motor1_B, LOW); 
  }
 delay(100); 
 }
 if((ir_flag==1) && (digitalRead(metal_sensor)==LOW) && (digitalRead(ir_sensor_2)==LOW)) ////METAL BOTTLE DETECTING CONDITION
 {
  Servo1.write(180); 
 lcd.setCursor(7, 0);
delay(10);
lcd.print("Accepted  ");
  ir_flag=0;
//digitalWrite(motor1_A, LOW);
//digitalWrite(motor1_B, LOW);

 count++;
 delay(100);
 }
 else if((ir_flag==1) &&(digitalRead(metal_sensor)==HIGH) && (digitalRead(ir_sensor_2)==LOW))  /// NON-METALLIC BOTTLE
 {
  ir_flag=1;
  digitalWrite(motor1_A, LOW);
digitalWrite(motor1_B, LOW);
lcd.setCursor(7, 0);
delay(10);
lcd.print("Rejected  ");
  Servo1.write(90);
  delay(500);
digitalWrite(motor1_A, LOW);
digitalWrite(motor1_B, HIGH);
 }
  
}
delay(100); 
}

//void sendsms(const char *message)
//{
//   lcd.setCursor(0, 2);
//  delay(10);
//  lcd.print("SENDING SMS      ");
//  delay(500);
//  
// mySerial.println("AT\r\n");
//  delay(2000);
// mySerial.println("ATE0\r\n");
//  delay(2000);
//mySerial.println("AT+CMGF=1\r\n");
// delay(2000);
// mySerial.println("AT+CMGS=\"num\"");
//  delay(1000);
//  mySerial.print(message);
//  delay(1000);
// mySerial.println((char)26);
// 
//  delay(1000);
//}

void GPRS_send(float a)
{
Serial.println("AT");
delay(1000);
Serial.println("AT+CPIN?");//check for sim
delay(1000);
Serial.println("AT+CREG?"); // checking sim registeration
delay(1000); 
Serial.println("AT+CGATT?");//checking if MS is connected to GPRS
delay(1000);
Serial.println("AT+CIPSHUT");
delay(1000); 
Serial.println("AT+CIPSTATUS"); // current connection status
delay(2000);
Serial.println("AT+CIPMUX=0");// start multiconnection
delay(2000);
Serial.println("AT+CSTT=\"airtelgprs.com\"");// APN of the sim
delay(1000);
Serial.println("AT+CIICR ");// start wireless connection with GPRS
delay(3000);
Serial.println("AT+CIFSR ");//get local IP address
delay(1000); 
Serial.println("AT+CIPSPRT=0");
delay(3000); 
Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");// start TCP connection AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update\"
delay(5000);  
Serial.println("AT+CIPSEND");// send data through TCP/UDP connection
delay(5000);
//String str="GET https://api.thingspeak.com/update?api_key=PESQLADSZ6S92RM0&field6=" +String(a)+"&field7=" +String(b); 
String str="GET https://api.thingspeak.com/update?api_key="+api_key+"&field1=" +String(a); 

Serial.print(str);
Serial.println();
delay(3000);
Serial.write(26);
delay(1000); 
Serial.println();
Serial.println("AT+CIPSHUT");
delay(1000); 
lcd.setCursor(0, 1);
  delay(10);
  lcd.print("   Uploaded ");
  delay(500);
}





                          
