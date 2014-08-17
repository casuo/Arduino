#include <dht.h>
#include <Wire.h>
#include <SeeedOLED.h>
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

static unsigned char fontDatas[11][32] PROGMEM={
  //超
  {
    0x40,0x48,0x48,0xFF,0x48,0x48,0x82,0x42,
    0x3E,0x02,0x22,0x42,0x3F,0x02,0x00,0x00,
    0x40,0x3F,0x10,0x1F,0x22,0x42,0x40,0x5F,
    0x51,0x51,0x51,0x51,0x5F,0x60,0x20,0x00
  }
  ,
  //声
  {
    0x04,0x14,0xD4,0x54,0x54,0x54,0x54,0xDF,  
    0x54,0x54,0x54,0x54,0xF4,0x56,0x04,0x00,  
    0x80,0x60,0x1F,0x02,0x02,0x02,0x02,0x03,  
    0x02,0x02,0x02,0x02,0x07,0x00,0x00,0x00     
  }
  ,
  //波
  {
    0x20,0x42,0x04,0x0C,0x80,0xF8,0x88,0x88,
    0x88,0xFF,0x88,0x88,0xA8,0x18,0x00,0x00,
    0x04,0x04,0xFE,0x41,0x30,0x8F,0x40,0x22,
    0x14,0x08,0x14,0x22,0x41,0xC0,0x40,0x00
  }
  ,
  //距
  {
    0x00,0x3E,0x22,0xE2,0x22,0x3E,0x00,0xFE,
    0x22,0x22,0x22,0x22,0xF3,0x22,0x00,0x00,
    0x20,0x7F,0x20,0x3F,0x12,0x12,0x00,0x7F,
    0x42,0x42,0x42,0x42,0x47,0x60,0x40,0x00
  }
  ,
  //离
  {
    0x04,0x04,0x04,0xF4,0x04,0x8C,0x55,0x26,
    0x54,0x8C,0x04,0xF4,0x04,0x06,0x04,0x00,
    0x00,0xFC,0x04,0x05,0x25,0x35,0x2D,0x27,
    0x25,0x35,0x25,0x45,0x84,0x7E,0x04,0x00
  }
  ,
  //探
  {
    0x10,0x10,0x10,0xFF,0x90,0x58,0x06,0x22,
    0x12,0x0A,0xC2,0x0A,0x12,0xAA,0x06,0x00,
    0x02,0x42,0x81,0x7F,0x00,0x00,0x21,0x11,
    0x09,0x05,0xFF,0x05,0x09,0x31,0x11,0x00
  }
  ,
  //测
  {
    0x10,0x22,0x6C,0x00,0x80,0xFC,0x04,0xF4,
    0x04,0xFE,0x04,0xF8,0x00,0xFE,0x00,0x00,
    0x04,0x04,0xFE,0x01,0x40,0x27,0x10,0x0F,
    0x10,0x67,0x00,0x47,0x80,0x7F,0x00,0x00
  }
  ,
  //仪
  {
    0x80,0x40,0x20,0xF8,0x07,0x00,0x38,0xC0,
    0x02,0x04,0x0C,0x80,0x60,0x1C,0x00,0x00,
    0x00,0x00,0x00,0xFF,0x80,0x40,0x20,0x10,
    0x0B,0x04,0x0A,0x11,0x60,0xC0,0x40,0x00
  },
  //wen
  {
    0x10,0x22,0x64,0x0C,0x80,0x00,0xFE,0x92,
    0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00,
    0x04,0x04,0xFE,0x01,0x40,0x7E,0x42,0x42,
    0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00
  },
  //shi
  {
    0x10,0x22,0x64,0x0C,0x80,0xFE,0x92,0x92,
    0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00,
    0x04,0x04,0xFE,0x41,0x44,0x48,0x50,0x7F,
    0x40,0x40,0x7F,0x50,0x48,0x64,0x40,0x00
  },
  //du
  {
    0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0xA5,
    0xA6,0xA4,0xFC,0x24,0x34,0x26,0x04,0x00,
    0x40,0x20,0x9F,0x80,0x42,0x42,0x26,0x2A,
    0x12,0x2A,0x26,0x42,0x40,0xC0,0x40,0x00
  }
};

//0-9 :
static unsigned char fontDigital[11][32] PROGMEM = {
  //0
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x0C,0x04,
    0x04,0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x1F,0x30,0x20,
    0x20,0x20,0x30,0x1F,0x0F,0x00,0x00,0x00
  }
  ,
  //1
  {
    0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xF8,
    0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,
    0x3F,0x20,0x00,0x00,0x00,0x00,0x00,0x00
  }
  ,
  //2
  {
    0x00,0x00,0x00,0x00,0x30,0x38,0x0C,0x04,
    0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x20,0x30,0x38,0x2C,
    0x26,0x23,0x21,0x38,0x00,0x00,0x00,0x00
  }
  ,
  //3
  {
    0x00,0x00,0x00,0x00,0x10,0x18,0x0C,0x84,
    0x84,0xCC,0x78,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x18,0x30,0x20,
    0x20,0x31,0x1F,0x0E,0x00,0x00,0x00,0x00 
  }
  ,
  //4
  {
    0x00,0x00,0x00,0x00,0x80,0xC0,0x70,0x18,
    0xFC,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x06,0x07,0x05,0x04,0x24,
    0x3F,0x3F,0x24,0x04,0x00,0x00,0x00,0x00
  }
  ,
  //5
  {
    0x00,0x00,0x00,0x00,0xFC,0xFC,0xC4,0x44,
    0x44,0xC4,0x84,0x04,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x18,0x30,0x20,
    0x20,0x30,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //6
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x8C,0x44,
    0x44,0xCC,0x98,0x10,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x1F,0x30,0x20,
    0x20,0x30,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //7
  {
    0x00,0x00,0x00,0x00,0x1C,0x0C,0x0C,0x0C,
    0xCC,0xEC,0x3C,0x1C,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3F,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  }
  ,
  //8
  {
    0x00,0x00,0x00,0x00,0x30,0x78,0xCC,0x84,
    0x84,0xCC,0x78,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0E,0x1F,0x31,0x20,
    0x20,0x31,0x1F,0x0E,0x00,0x00,0x00,0x00
  }
  ,
  //9
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x0C,0x04,
    0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x19,0x33,0x22,
    0x22,0x33,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //:
  {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x33,0x33,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  }
};

dht DHT;
#define DHT11_PIN 4 //put the sensor in the digital pin 4
SoftwareSerial SoftSerial(0, 1);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 

void setup()
{
  Wire.begin();	
  SeeedOled.init();  //initialze SEEED OLED display  
  DDRB|=0x21;        
  PORTB |= 0x21;
  SoftSerial.begin(9600);               // the SoftSerial baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  //init display
  SeeedOled.init();                       //initialze SEEED OLED display
  SeeedOled.clearDisplay();               // clear the screen and set start position to top left corner
  SeeedOled.deactivateScroll();           // deactivete Scroll (might be activated by previous test case)
  SeeedOled.setNormalDisplay();           // Non-inverted Display 
  //SeeedOled.setHorizontalMode();                // Page mode to start with

  //print title
  SeeedOled.setBitmapRect(0,1,0,127);
  //p0
  for(int i = 0 ;i< 8;i++){
    SeeedOled.drawBitmap(fontDatas[i],16);
  }
  //p1
  for(int i = 0 ;i< 8;i++){
    SeeedOled.drawBitmap(fontDatas[i] + 16,16);
  }
  //wendu
  SeeedOled.setBitmapRect(3,4,16,47);
  SeeedOled.drawBitmap(fontDatas[8],16);
  SeeedOled.drawBitmap(fontDatas[10],16);
  SeeedOled.drawBitmap(fontDatas[8] + 16,16);
  SeeedOled.drawBitmap(fontDatas[10] + 16,16);
  
  //shidu  
  SeeedOled.setBitmapRect(5,6,16,47);
  SeeedOled.drawBitmap(fontDatas[9],16);
  SeeedOled.drawBitmap(fontDatas[10],16);
  SeeedOled.drawBitmap(fontDatas[9] + 16,16);
  SeeedOled.drawBitmap(fontDatas[10] + 16,16);
  
  SeeedOled.setBitmapRect(7,7,0,23);
  SeeedOled.putString("ID:");
}
void loop()
{
  testTemperature();
  delay(2000);
  testHumidity();
  delay(2000);
  rfid();
  delay(2000);
}
void testTemperature(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.print("OK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
  }
 
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
 
  //display in lcd page 5,6 col 16
  SeeedOled.setBitmapRect(3,4,64,111);
  unsigned char char_buffer[3]="";
  unsigned char i = 0;
  unsigned char f = 0;
  long long_num = DHT.temperature - 2;//modify for offset

  //SeeedOled.sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
  //clear digi zone 3
  for(i=0;i<96;i++){
    SeeedOled.setHorizontalMode();
    SeeedOled.sendData(0x00);
  }
  
  i=0;

  if (long_num == 0) {
    f=1;    
    SeeedOled.setBitmapRect(3,4,64,79);
    SeeedOled.drawBitmap(fontDigital[0],32);
  } 
  else{
    while (long_num > 0) 
    {
      char_buffer[i++] = long_num % 10;
      long_num /= 10;
    }
    f=f+i;
    for(; i > 0; i--)
    {
      //Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(3,4,112-i*16,128-i*16);
      SeeedOled.drawBitmap(fontDigital[char_buffer[i-1]],32);      
      //putChar('0'+ char_buffer[i - 1]);
    }
  }
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

void testHumidity(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.print("OK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
  }
 
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
 
  //display in lcd page 5,6 col 16
  SeeedOled.setBitmapRect(5,6,64,111);
  unsigned char char_buffer[3]="";
  unsigned char i = 0;
  unsigned char f = 0;
  long long_num = DHT.humidity;

  //SeeedOled.sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
  //clear digi zone 3
  for(i=0;i<96;i++){
    SeeedOled.setHorizontalMode();
    SeeedOled.sendData(0x00);
  }
  
  i=0;

  if (long_num == 0) {
    f=1;    
    SeeedOled.setBitmapRect(5,6,64,79);
    SeeedOled.drawBitmap(fontDigital[0],32);
  } 
  else{
    while (long_num > 0) 
    {
      char_buffer[i++] = long_num % 10;
      long_num /= 10;
    }
    f=f+i;
    for(; i > 0; i--)
    {
      //Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(5,6,112-i*16,128-i*16);
      SeeedOled.drawBitmap(fontDigital[char_buffer[i-1]],32);      
    }
  }
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

void rfid()
{
  //delay(5000);
  Serial.print("Begin RFID Module:");
  Serial.println(millis());
  if (SoftSerial.available())              // if date is comming from softwareserial port ==> data is comming from SoftSerial shield
  {    
  Serial.println("Begin RFID Read Module");
    //set text page mode and clear display
    SeeedOled.setBitmapRect(7,7,24,127);
    for(int i=1;i<=13;i++){
      SeeedOled.putChar(' ');
    }
    SeeedOled.setBitmapRect(7,7,24,127);
    while(SoftSerial.available())          // reading data into char array 
    {
      buffer[count++]=SoftSerial.read();     // writing data into array    
      if(count == 64)break;
    }
    SeeedOled.putString((char *)buffer);
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    Serial.println();
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
  }
  //if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
    //SoftSerial.write(Serial.read());       // write it to the SoftSerial shield
}

void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
  { 
    buffer[i]=NULL;
  }                  // clear all index of array with command NULL
}

