/*
  Ping Example
 
 This example sends an ICMP pings every 500 milliseconds, sends the human-readable
 result over the serial port. 

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 30 Sep 2010
 by Blake Foster
 
 */
#include <ICMPPing.h>
#include <util.h>
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // max address for ethernet shield
byte ip[] = {192,168,0,3}; // ip address for ethernet shield
byte subnet[] = {255,255,255,0};
IPAddress pingAddr(192,168,0,2); // ip address to ping

SOCKET pingSocket = 0;

char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

int falha = 0;
const int limite = 5;
const int saida = 9; 
int deligado = 4000;
int religa = 2000;


void setup() 
{
  // start Ethernet
  Ethernet.begin(mac, ip, subnet);
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop()
{
  digitalWrite(9, HIGH);
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
  {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply.data.seq,
            echoReply.addr[0],
            echoReply.addr[1],
            echoReply.addr[2],
            echoReply.addr[3],
            REQ_DATASIZE,
            millis() - echoReply.data.time,
            echoReply.ttl);
            falha = 0;
  }
  else
  {
    if(falha == limite){
      digitalWrite(saida, LOW);    // turn the LED off by making the voltage LOW
      delay(deligado); 
      digitalWrite(saida, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(religa);              // wait for a second
      falha = 0;
      }
      else{
        sprintf(buffer, "Echo request failed; %d", echoReply.status);
        falha++;
        }
  }
  Serial.println(buffer);
  delay(500);
}










