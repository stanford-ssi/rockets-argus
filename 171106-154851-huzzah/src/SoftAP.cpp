#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>


// Name of WiFi and password
const char* ssid = "ESP_LORDZ";
const char* password = "johnbean";

const int LEDpin = 13; // pin of LED

WiFiUDP Udp;

unsigned int localUdpPort = 4210;
int numPackets = 0;
bool receivedXbee = false;

SoftwareSerial mySerial(14, 15); // RX, TX


IPAddress localIP(192,168,4,1);
//IPAddress stationIP(192,168,1,22);
IPAddress stationIP(0,0,0,0);
IPAddress gateway(192,168,1,9);
IPAddress subnet(255,255,255,0);

void setup(void) {
  Serial.begin(115200); //baudrate for Serial
  Serial.println();

  int xbeeBaudrate = 9600;
  mySerial.begin(xbeeBaudrate);


  pinMode(LEDpin,OUTPUT); // sets pin 13 to be an output pin
  digitalWrite(LEDpin, HIGH);

  Serial.printf("Making Soft Access Point with ssid: %s\n", ssid);
  bool check1, check2 = false; //bool if connected

  while(!check1 || !check2) {
    check2 = WiFi.softAPConfig(localIP, gateway, subnet);
    check1 = WiFi.softAP(ssid, password); //make access point until you've made one

  };
  delay(2000);
  Serial.printf("\nSoftAP is set up!\n\n");
  Serial.println(WiFi.softAPIP()); //print IP address connected to


  Serial.println("begin UDP port");
  Udp.begin(localUdpPort);
  Serial.print("local UDP port: ");
  Serial.println(localUdpPort);
};

void loop() {
  // checks to see which stations are connected and prints them
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(1000);

  Serial.println();

  mySerial.write("Hello Test\n");

  //int packetSize = Udp.parsePacket();

  while (mySerial.available()) {
    // read the incoming byte:
    char incomingByte = mySerial.read();

    // say what you got:
    Serial.print(incomingByte);
    receivedXbee = true;
  }


  if (WiFi.softAPgetStationNum() && receivedXbee) {
    char number[4];

    numPackets++;

    itoa (numPackets,number,10);


    int check = Udp.beginPacket(stationIP, 4210);
    //Serial.printf("Sent to IP %s at port %d", Udp.remoteIP().toString().c_str(), 4210);
    Serial.printf("check = %d", check);

    if (check == 0) {
      Serial.printf("\nWhat the fuck\n");
    };

    Udp.write(number);
    Udp.endPacket();
    Serial.printf("\nSent packet to station\n");
    if (digitalRead(LEDpin) == LOW) {
      digitalWrite(LEDpin,HIGH);
    } else {
      digitalWrite(LEDpin,LOW);
    };
  };
};
