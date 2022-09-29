//Libraries
#include <Ethernet.h>//https://github.com/CisecoPlc/Arduino-W5100-W5200/tree/master/Ethernet
//Parameters
String request ;
const String EMPTY = "";
unsigned long refreshCounter  = 0;
byte mac [6] = {0x54, 0x34, 0x41, 0x30, 0x30, 0x31};
//Objects
EthernetServer server(80);
IPAddress ip(192, 168, 0, 10);
EthernetClient client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
 //Init Serial USB
 Serial.begin(9600);
 Serial.println(F("Initialize System"));
 //Init W5100
 Ethernet.begin(mac, ip);
 while (!Ethernet.begin(mac)) {
   Serial.println(F("failed. Retrying in 5 seconds."));
   delay(5000);
   Serial.print(F("Starting W5100..."));
 }
 
 server.begin();
 Serial.println(F("W5100 initialized"));
 Serial.print(F("IP Address: "));
 Serial.println(Ethernet.localIP());
 digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
 client = server.available();
 clientRequest();
 handleRequest();
}

void clientRequest( ) { /* function clientRequest */
 request = EMPTY;
 ////Get client request
 if (!client) {
   return;
 }
 // Wait until the client sends some data
 while (!client.available()) { // client.available() returns a number of bytes received, or 0 if no data available
   delay(1);
 }
 request = client.readStringUntil('\r'); // Read the first line of the request
 Serial.println(request);
 client.flush(); // dismiss all extra info of the request (like GET parameters)
}

void handleRequest( ) { 
  if(request == EMPTY) return;
  
 ////Handle web client request
 if (request.indexOf("GET") >= 0) 
 {
  if (request.indexOf("/dig2on") > 0) {

    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("turn led ON");
    redirect(client, F("/"));
    
  } else  if (request.indexOf("/dig2off") > 0) {

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("turn led Off");
    redirect(client, F("/"));
    
  } else if(request.indexOf("GET / ") >= 0) {

   webpage(client);

  } else {
  
    Serial.println("Unknown page");
    client.println(F("HTTP/1.1 404 Not Found"));
    
  }
 } else {// POST PUT DELETE ...
  
    Serial.println("Unknown action");
    client.println(F("HTTP/1.1 404 Not Found"));
    
  }
  
  client.stop();
  request = EMPTY;
}

void redirect(EthernetClient client, String location){
  client.println(F("HTTP/1.1 302 Found"));
  client.print("Location: ");
  client.println(location);
// example
//302 Found
//Location: http://www.iana.org/domains/example/
}

void webpage(EthernetClient client) { /* function webpage */
 ////Send webpage to client
 //output HTML data header
 client.println(F("HTTP/1.1 200 OK"));
 client.println(F("Content-Type: text/html"));
 client.println();
 //header
 client.print(F("<!DOCTYPE HTML><html><head><title>AranaCorp</title>"));
 client.print(F("<meta http-equiv='content-type' content='text/html; charset=UTF-8'>"));
 //meta-refresh page every x seconds
 client.print(F("<meta http-equiv='refresh' content='5'>"));
 client.print(F("</head><body bgcolor='black'><br>"));
 client.print(F("<hr/><hr>"));
 client.print(F("<h1 style='color : #3AAA35;'><center> AranaCorp - Arduino Web Controller </center></h1>"));
 client.print(F("<hr/><hr>"));
 client.print("<center><p style='color:white;'>");
 client.print(F("Page refresh number : "));
 client.print(refreshCounter); //current refresh count
 client.print("</p></center><br>");
 client.print(F("<h2 style='color:green;'>Arduino Inputs</h2>"));
 client.print("<p style='color:white;'>");
 client.print("<br><br>");
 //output analog input pin
 for (int i = 0; i < 6; i++) {
   client.print("<b>Input A");
   client.print(i);
   client.print(" : </b>");
   client.print(analogRead(14 + i)); //A0=14, A1=15 ,etc.
   client.print(F("<br>"));
 }
 client.print(F("</p><br>"));
 //digital output
 client.print(F("<h2 style='color:green;'>Arduino Outputs</h2>"));
 client.print(F("<p style='color:white;'>"));
 client.print(F("<br><br>"));
 client.print(F("<b>Digital output Pin 2 : </b>"));
 client.print("<input value=" + String(digitalRead(LED_BUILTIN)) + " readonly></input>");
 client.print(F("<a href='/dig2on'><button>Turn On </button></a>"));
 client.print(F("<a href='/dig2off'><button>Turn Off </button></a><br />"));
 client.print(F("</p><br>"));
 //file end
 client.print(F("<br></body></html>"));
 refreshCounter += 1;
}
