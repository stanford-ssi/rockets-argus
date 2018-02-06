#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Wifi to connect to and password
const char* ssid = "ESP_LORDZ";
const char* password = "johnbean";

const int LEDpin = 13; // pin of LED
const int ONpin = 14;

WiFiUDP Udp;

unsigned int localUdpPort = 4210;
char incomingPacket[255];
char replyPacket[] = "Message Received";

IPAddress staticIP(192,168,1,22);
IPAddress gateway(192,168,1,9);
IPAddress subnet(255,255,255,0);

void setup(void) {
  Serial.begin(115200); //baudrate for Serial
  Serial.println();

  pinMode(LEDpin,OUTPUT); // sets pin 13 to be an output pin
  digitalWrite(LEDpin,HIGH); // outputs on pin 13 to turn off LED
  pinMode(ONpin, OUTPUT);
  digitalWrite(ONpin,LOW); // outputs on pin 14 to keep the raspberry pi off

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password); //connect to access point (Wifi)

  while (WiFi.status() != WL_CONNECTED) // while not connected to wifi
  {
    delay(500);
    Serial.print("."); // keep printing dots
  };

  WiFi.config(staticIP, gateway, subnet);

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); //print IP address connected to

  Serial.println("begin UDP port");
  Udp.begin(localUdpPort);
  Serial.print("local UDP port: ");
  Serial.println(localUdpPort);
}

void loop(void) {
  delay(1);
  int packetSize = Udp.parsePacket(); // Keep Parsing for packets
  //Serial.printf("%d\n",packetSize);

  // if packetSize > 0 (which means there is something received)
  if (packetSize) {
    Serial.printf("Received packets from %s", Udp.remoteIP().toString().c_str());

    int len = Udp.read(incomingPacket, 255); // put the packet into variable incomingPacket
    if (len > 0) {
      incomingPacket[len] = 0; // set the character after the end of the message to 0
    };
    Serial.printf("UDP packet contents: %s\n", incomingPacket); // print the contents of the package

    //digitalWrite(LEDpin,LOW);
    Serial.printf("Turning on Raspberry Pi\n\n");
    digitalWrite(ONpin,HIGH); // Turn on pin to turn on Raspberry Pi

    if(digitalRead(LEDpin) == LOW) {
      digitalWrite(LEDpin,HIGH); // outputs on pin 13 to turn on LED
      digitalWrite(ONpin,HIGH);
    } else {
      digitalWrite(LEDpin,LOW);
    }
    delay(3000);

  };
}
