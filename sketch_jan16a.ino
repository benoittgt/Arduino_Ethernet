// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,1,203 };
int lightPin = 0;

byte Ethernet::buffer[500];
BufferFiller bfill;

void setup () {
  pinMode(lightPin, INPUT);
  Serial.begin(9600);
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip);
}

static word homePage() {
  
  int lightlevel = analogRead(lightPin);
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>Light sensor server</title>"
    "<h2>Light level is : $D</h2>"),
      lightlevel);
  return bfill.position();
}

void loop () {
  word len = ether.packetReceive();
  Serial.println(ether.packetReceive());
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data
}

