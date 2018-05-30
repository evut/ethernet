#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x85,0x8f,0x6e,0x2D,0x30,0x65 };
static byte myip[] = { 169,254,25,150 };

byte Ethernet::buffer[500];
BufferFiller bfill;

void setup () {
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
    Serial.begin(9600);
    pinMode(A0,INPUT);
  ether.staticSetup(myip);
}
int prehoz (char a[7])
{
  int potenciometr=analogRead(A0);
  char potemax[]= {'M','A','X','I','M','U','M'}; 
  char poteprubeh[] {'P','R','U','B','E','H',' '};
  char potemin[]= {'M','I','N','I','M','U','M'}; 
    if (potenciometr >683)
    {
      int number;
      for(number=0;number<7;number++)
         {
          a[number]=potemax[number];
         }}
  else
         {
          if(potenciometr == 0 )
          {
             int number1;
      for(number1=0;number1<7;number1++)
         {
          a[number1]=potemin[number1];
          }
          }
          else{
          int number1;
      for(number1=0;number1<7;number1++)
         {
          a[number1]=poteprubeh[number1];
         }}
    }
    return(a[7]);
}

static word homePage() {
  int potenciometr=analogRead(A0);
    char a[7];
  prehoz(a); 
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>Ethernet komunikace</title>" 
    "<body><h2>hodnota a poloha potenciometru</h2>"
    "<p>A0: $D $S </p></body>"
    "<h4><center>CAS OD SPUSTENI<h4>"
    "<p>$D$D:$D$D:$D$D</p></center>"),
     potenciometr,a, h/10, h%10, m/10, m%10, s/10, s%10);
       
  return bfill.position();
    
}

void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data
}
