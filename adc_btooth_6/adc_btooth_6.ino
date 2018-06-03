/*
   MCP3208 ADC over bluetooth module HC-05
   Using Arduino Nano with CH341 serial chip

   Pin connections:

   Arduino --> MCP3208
   -------------------
   D13 (SCK) -->  pin 13
   D12 (MISO) --> pin 12
   D11 (MOSI) --> pin 11
   D10  (CS) --> pin 10

   2018 Xaratustrah

*/

#include <SPI.h>
#include <Mcp3208.h>
#include <SoftwareSerial.h>


#define SPI_CS      10        // SPI slave select
#define ADC_VREF    3300     // 3.3V Vref
#define ADC_CLK     1000000  // SPI clock 1.0MHz

#define PGA_CS      9        // SPI slave select

#define BUFFLEN     8 * 64 // 512 data points

uint32_t t1;
uint32_t t2;

byte incomingByte;
uint16_t raw[BUFFLEN];

MCP3208 adc(ADC_VREF, SPI_CS);
SoftwareSerial BTSerial(3, 2); // RX | TX

void setup() {

  // configure PIN mode
  pinMode(SPI_CS, OUTPUT);
  pinMode(PGA_CS, OUTPUT);

  digitalWrite(PGA_CS, HIGH);

  // initialize bluetooth
  BTSerial.begin(115200);

  // initialize SPI interface for MCP3208
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  digitalWrite(SPI_CS, LOW);
  SPI.begin();
  SPI.beginTransaction(settings);
}


void loop() {

  if (BTSerial.available()) {
    incomingByte = BTSerial.read();

    // wait for the character "a"
    if (incomingByte == 97)
    {

      // set initial PIN state
      // digitalWrite(SPI_CS, HIGH);

      t1 = micros();
      for (int i = 0; i < BUFFLEN; i = i + 8)
      {
        raw[i + 0] = adc.read(8);
        raw[i + 1] = adc.read(9);
        raw[i + 2] = adc.read(10);
        raw[i + 3] = adc.read(11);
        raw[i + 4] = adc.read(12);
        raw[i + 5] = adc.read(13);
        raw[i + 6] = adc.read(14);
        raw[i + 7] = adc.read(15);
      }
      t2 = micros();
      // digitalWrite(SPI_CS, LOW);

      //t1 = micros();
      for (int i = 0; i < BUFFLEN; i++) {
        BTSerial.println(raw[i]);
      }
      //t2 = micros();
      // time
      BTSerial.print("Time: ");
      double samp_time = static_cast<double>(t2 - t1);
      BTSerial.print(samp_time, 1);
      BTSerial.println("us");

      BTSerial.println("");
      //  delay(2000);
    }
  }
}

