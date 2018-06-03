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
#define ADC_CLK     1600000  // SPI clock 1.6MHz


MCP3208 adc(ADC_VREF, SPI_CS);
SoftwareSerial BTSerial(3, 2); // RX | TX

void setup() {

  // configure PIN mode
  pinMode(SPI_CS, OUTPUT);

  // set initial PIN state
  digitalWrite(SPI_CS, HIGH);

  // initialize serial
  BTSerial.begin(115200);

  // initialize SPI interface for MCP3208
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}

uint16_t raw[8];
uint32_t t1;
uint32_t t2;


void loop() {
  // read single ended from channel 1 to 7
  t1 = micros();

  for (int i = 0; i <= 7; i++) {
    // this is a trick. please refer to the documentation of the library for explanation
    raw[i] = adc.read(i + 8);
  }
  t2 = micros();

  // get analog value
  //uint16_t val = adc.toAnalog(raw);
  for (int i = 0; i <= 7; i++) {
    BTSerial.println(raw[i]);
  }

  // sampling time
  BTSerial.print("Sampling time: ");
  double samp_time = static_cast<double>(t2 - t1) / 1000;
  BTSerial.print(samp_time, 4);
  BTSerial.println("ms");
  BTSerial.print("Sampling frequency: ");
  BTSerial.print(1 / samp_time, 4);
  BTSerial.println("kHz");

  BTSerial.println("");
  delay(2000);
}

