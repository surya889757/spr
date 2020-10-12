double pulse;
void setup()
{
pinMode(11,INPUT);
pinMode(13,OUTPUT);
pinMode(8,OUTPUT);
Serial.begin(9600);
Serial.print("metal detector");
}
void loop()
{
digitalWrite(13,HIGH);
delayMicroseconds(5000);
digitalWrite(13,LOW); 
delayMicroseconds(100);
pulse = pulseIn(11,HIGH,5000);
if(pulse > 920)
{ Serial.print("metal detected be safe");
}
}
