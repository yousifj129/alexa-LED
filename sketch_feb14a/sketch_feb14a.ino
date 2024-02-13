const char* ssid = "OPPO A74";
const char* password = "11111111";

String Device_1_Name = "1 Light";
String Device_2_Name = "2 Light";
String Device_3_Name = "3 Light";
String Device_4_Name = "4 Light";


#include <WiFi.h>
#include <Espalexa.h>
#include <Preferences.h>

Espalexa espalexa;
Preferences pref;

#define RelayPin1 23  
#define RelayPin2 22  
#define RelayPin3 19  
#define RelayPin4 18  



#define wifiLed   2

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;


boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);


boolean wifiConnected = false;

void firstLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      Serial.println("Device1 ON");
      toggleState_1 = 1;
    }
  else
  {
    digitalWrite(RelayPin1, LOW);
    Serial.println("Device1 OFF");
    toggleState_1 = 0;
  }
  pref.putBool("Relay1", toggleState_1);
}

void secondLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin2, HIGH);
      Serial.println("Device2 ON");
      toggleState_2 = 1;
    }
  else
  {
    digitalWrite(RelayPin2, LOW);
    Serial.println("Device2 OFF");
    toggleState_2 = 0;
  }
  pref.putBool("Relay2", toggleState_2);
}

void thirdLightChanged(uint8_t brightness)
{
  //Control the device  
  if (brightness == 255)
    {
      digitalWrite(RelayPin3, HIGH);
      Serial.println("Device3 ON");
      toggleState_3 = 1;
    }
  else
  {
    digitalWrite(RelayPin3, LOW);
    Serial.println("Device3 OFF");
    toggleState_3 = 0;
  }
  pref.putBool("Relay3", toggleState_3);
}

void fourthLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin4, HIGH);
      Serial.println("Device4 ON");
      toggleState_4 = 1;
    }
  else
  {
    digitalWrite(RelayPin4, LOW);
    Serial.println("Device4 OFF");
    toggleState_4 = 0;
  }
  pref.putBool("Relay4", toggleState_4);
}


void addDevices(){
  //devices
  espalexa.addDevice(Device_1_Name, firstLightChanged);
  espalexa.addDevice(Device_2_Name, secondLightChanged);
  espalexa.addDevice(Device_3_Name, thirdLightChanged);
  espalexa.addDevice(Device_4_Name, fourthLightChanged);


  espalexa.begin();
}

void sendFeedback(int relay, int value){
  EspalexaDevice* d = espalexa.getDevice(relay); 
  if(relay == 4){
    d->setPercent(map(value, 0, 4, 0, 100)); 
  }
  else{
    d->setPercent(value);
  }
}


void all_SwitchOn(){
  toggleState_1 = 1; digitalWrite(RelayPin1, HIGH); pref.putBool("Relay1", toggleState_1); sendFeedback(0, 100); delay(100);
  toggleState_2 = 1; digitalWrite(RelayPin2, HIGH); pref.putBool("Relay2", toggleState_2); sendFeedback(1, 100); delay(100);
  toggleState_3 = 1; digitalWrite(RelayPin3, HIGH); pref.putBool("Relay3", toggleState_3); sendFeedback(2, 100); delay(100);
  toggleState_4 = 1; digitalWrite(RelayPin4, HIGH); pref.putBool("Relay4", toggleState_4); sendFeedback(3, 100); delay(100);
}

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, LOW); pref.putBool("Relay1", toggleState_1); sendFeedback(0, 0); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, LOW); pref.putBool("Relay2", toggleState_2); sendFeedback(1, 0); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, LOW); pref.putBool("Relay3", toggleState_3); sendFeedback(2, 0); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, LOW); pref.putBool("Relay4", toggleState_4); sendFeedback(3, 0); delay(100);
}

boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void getRelayState()
{
  toggleState_1 = pref.getBool("Relay1", 0);
  digitalWrite(RelayPin1, toggleState_1); 
  (toggleState_1 == false) ? sendFeedback(0, 0) : sendFeedback(0, 100);
  delay(200);
  toggleState_2 = pref.getBool("Relay2", 0);
  digitalWrite(RelayPin2, toggleState_2); 
  (toggleState_2 == false) ? sendFeedback(1, 0) : sendFeedback(1, 100);
  delay(200);
  toggleState_3 = pref.getBool("Relay3", 0);
  digitalWrite(RelayPin3, toggleState_3); 
  (toggleState_3 == false) ? sendFeedback(2, 0) : sendFeedback(2, 100);
  delay(200);
  toggleState_4 = pref.getBool("Relay4", 0);
  digitalWrite(RelayPin4, toggleState_4); 
  (toggleState_4 == false) ? sendFeedback(3, 0) : sendFeedback(3, 100);
  delay(200); 
}

void setup()
{
  Serial.begin(115200);
  pref.begin("Relay_State", false);
  
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(5000);
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  digitalWrite(wifiLed, LOW);

  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    addDevices();
  }
  else
  {
    Serial.println("Cannot connect to WiFi. So in Manual Mode");
  }
  delay(1000);

  getRelayState(); //fetch data from NVS Flash Memory
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(wifiLed, LOW);
  }
  else
  {
    digitalWrite(wifiLed, HIGH);

    if (wifiConnected){
      espalexa.loop();
      delay(1);
    }
    else {
      wifiConnected = connectWifi();
      if(wifiConnected){
      addDevices();
      
      }
    }
  }
}