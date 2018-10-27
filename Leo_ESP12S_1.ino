/*
------------------------------------------
THIS WORK FROM THE AT control
AT+CIPSTART="TCP","api.thingspeak.com",80
AT+CIPSEND=49
GET /update?api_key=FXH3AE6AZNZGKYVG&field1=124

Below the process!
AT+CIPSTART="TCP","api.thingspeak.com",80
CONNECT

OK
AT+CIPSEND=49

OK
> 
Recv 49 bytes

SEND OK

+IPD,2:10CLOSED
----------------------------------
THIS WORK FROM THE BROWSER
https://api.thingspeak.com/update?api_key=FXH3AE6AZNZGKYVG&field1=2
----------------------------------
*/


#include <SoftwareSerial.h> // needed to connect via serial port
SoftwareSerial EspSerial(10, 11); // Rx,  Tx
String errore =""; //string for error check across the libraries
String statusChWriteKey = "FXH3AE6AZNZGKYVG";  // Thing speak Status authorization code
String ThingSpeakString = ""; //variable to contain the data to be sent
bool debug = false; // debug off

void setup() {
Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
while (!Serial) {
    ;               // wait for serial port to connect. Needed for native USB port only
  }
EspSerial.begin(9600); // starting the communication with the ESP
}


void loop() {

StartTSstring(); //generate the general part of the string to be sent
AddTSstring("field1","80"); // add the data for the sensor TEST
//Serial.print(ThingSpeakString);
//Serial.println(),
SendTSstring();
delay (20000); //delay to repeat the command

}

/*Leo_ESP8266-1, 
ESP 8266 wifi communication with thingspeak ver 29-9-2018

Previously the ESP need to be set in:
AT+CIOBAUD=9600  to get the 9600 baud speed connection which seems the best one for this application
AT+CWMODE=1 to be a sta, used to send command 
AT+CWJAP="network_name","password" in case is not yet configured


Need the :                  
#include <SoftwareSerial.h> // needed to connect via serial port
SoftwareSerial EspSerial(10, 11); // Rx,  Tx
String errore =""; //string for error check across the libraries
String statusChWriteKey = "FXH3AE6AZNZGKYVG";  // Thing speak Status authorization code
String ThingSpeakString = ""; //variable to contain the data to be sent

Errors :
Not included errors for now.

Debug:
bool debug = false; // debug off
if debug = true  will print on the serial print all the data and errors.
*/ 

void StartTSstring(void)
{
 ThingSpeakString = "";                        //Clean it
 ThingSpeakString = "GET /update?api_key=";
 ThingSpeakString += statusChWriteKey;
}

void AddTSstring(String field, String data)
{
 ThingSpeakString += "&";
 ThingSpeakString += field;
 ThingSpeakString += "=";
 ThingSpeakString += data;
}

void SendTSstring(void)
{
EspSerial.flush();//wait that everything has been sent before starting again.
int count=0; // internal counter
  
String cmd = "AT+CIPSTART=\"TCP\",\"";
cmd += "api.thingspeak.com"; // Adress IP of api.thingspeak.com
cmd += "\",80"; // port
  
EspSerial.println(cmd); 
Serial.println(cmd);

do{       //await that the buffer is full with the answer
  delay(500);
  count++;
    if(count==10){
    Serial.print("AT+CIPSTART no communication");
    return;
    }
  Serial.print("AT+CIPSTART waiting to 10: "); 
  Serial.println(count);  
  }while (EspSerial.available() <=0);     
  
if(EspSerial.find("OK")){
  Serial.println("AT+CIPSTART OK");
  } else {
         Serial.println("AT+CIPSTART error");
         EspSerial.println("AT+CIPCLOSE");  //close the communication
         return;
  }
 
cmd = "AT+CIPSEND=";
cmd += String(ThingSpeakString.length()+2);
EspSerial.println(cmd);
Serial.println(cmd);
count=0; //reset the counter

do{       //await that the buffer is full with the answer
  delay(500);
  count++;
    if(count==10){
    Serial.print("AT+CIPSEND no communication");
    return;
    }
  Serial.print("AT+CIPSEND waiting to 10: "); 
  Serial.println(count);  
  }while (EspSerial.available() <=0);

if(EspSerial.find("OK")){
  Serial.println("AT+CIPSEND OK");
  } else {
          Serial.println("AT+CIPSEND error");
          EspSerial.println("AT+CIPCLOSE");  //close the communication
          return;
  }

delay(2000);
EspSerial.println(ThingSpeakString);
Serial.println(ThingSpeakString);

count=0; //reset the counter

do{       //await that the buffer is full with the answer
  delay(500);
  count++;
    if(count==10){
    Serial.print("GET communication not working");
    return;
    }
  Serial.print("GET waiting to 10: "); 
  Serial.println(count);  
  }while (EspSerial.available() <=0);

if(EspSerial.find("SEND")){
  Serial.println("GET OK");
  } else {
          Serial.println("GET error");
          EspSerial.println("AT+CIPCLOSE");  //close the communication
          return;
  } 

EspSerial.println("AT+CIPCLOSE");

}

