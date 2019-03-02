#include <FirebaseArduino.h> 
#include <ESP8266WiFi.h>             //wifi library
#define WIFI_SSID "Soiyeah"             //replace SSID with your wifi username
#define WIFI_PASSWORD "2657091123"          //replace PWD with your wifi password
#define WIFI_LED D5                  //connect a led to any of the gpio pins of the board and replace pin_number with it eg. D4                      
#define SWITCH1 D1
#define SWITCH2 D2
#define SWITCH3 D3
#define SWITCH4 D4

#define FIREBASE_HOST "smart-home-f7bee.firebaseio.com"           //link of api
#define FIREBASE_AUTH "gVR1Lr2rLhySQ9W76pFmCRMIPxlU1rWclpqUbLPi"  //database secret

void setup() {
  Serial.begin(9600);
  pinMode(WIFI_LED,OUTPUT);                         //define pinmodes
  pinMode(SWITCH1,OUTPUT);                         //switch1
  pinMode(SWITCH2,OUTPUT);                         //switch2
  pinMode(SWITCH3,OUTPUT);                         //switch3
  pinMode(SWITCH4,OUTPUT);                         //switch4  

  digitalWrite(SWITCH1, HIGH);                    // to keep the switches off in case of a powercut
  digitalWrite(SWITCH2, HIGH);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {           //wait till connected to WiFi
    delay(100);  
    digitalWrite(WIFI_LED,LOW);                     //Blink the light till connected to WiFi
    delay(100);
    digitalWrite(WIFI_LED,HIGH);
    Serial.print("."); }
    
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(WIFI_LED,LOW);   //------------Connected led------------//
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);             //connect to Database
  Firebase.stream("/devices");  
  delay(1000);

}

void loop() 
{

  if(Firebase.failed())
  {
    Serial.println("firebase failed!");
    Serial.println(Firebase.error());
  }
  else
  {

    if (Firebase.available()) 
    {
       FirebaseObject event = Firebase.readEvent();
       String eventType = event.getString("type");
       eventType.toLowerCase();
       //Serial.println(eventType);
       
       if (eventType == "put") 
       {
          String path = event.getString("path");
          String data = event.getString("data");

          Serial.println(data);
          
          if (path.equals("/switch1")) 
          {
              if (data.equals("on")) 
              {
                digitalWrite(SWITCH1, LOW);                
              } 
              else 
              {                
                digitalWrite(SWITCH1, HIGH);                 
              }
          } 
          else if (path.equals("/switch2")) 
          {
              if (data.equals("on")) 
              { 
                 digitalWrite(SWITCH2, LOW);                 
              } 
              else 
              {
                  digitalWrite(SWITCH2, HIGH);  
              }
          }
          else if (path.equals("/switch3")) 
          {
              if (data.equals("on")) 
              {
                 fadeIn(SWITCH3);  
              } 
              else 
              {
                  fadeOut(SWITCH3);  
              }
          }
          else if (path.equals("/switch4")) 
          {
              if (data.equals("on")) 
              {
                 digitalWrite(SWITCH4, HIGH);
              } 
              else 
              {
                  digitalWrite(SWITCH4, LOW);
              }
          }
       }
    } 
 
  }
   
} 

void fadeIn(int led)
{
  for(int i=0; i<1023; i++)
  {
    analogWrite(led,i);
    delay(2);
  }
}

  void fadeOut(int led)
{
  for(int i=1023; i>0; i--)
  {
    analogWrite(led,i);
    delay(2);
  }
}

