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
  Serial.begin(9600);
  BTSerial.begin(115200);

  // initialize SPI interface for MCP3208
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}

void loop() {

  uint32_t t1;
  uint32_t t2;

  // start sampling
  Serial.println("Reading...");

  t1 = micros();
  uint16_t raw = adc.read(MCP3208::SINGLE_1);
  t2 = micros();

  // get analog value
  uint16_t val = adc.toAnalog(raw);

  // read value
  Serial.print("value: ");
  Serial.print(raw);
  Serial.print(" (");
  Serial.print(val);
  BTSerial.write(val);
  Serial.println(" mV)");

  // sampling time
  Serial.print("Sampling time: ");
  double samp_time = static_cast<double>(t2 - t1) / 1000;
  Serial.print(samp_time, 4);
  Serial.println("ms");
  Serial.print("Sampling frequency: ");
  Serial.print(1 / samp_time, 4);
  Serial.println("kHz");

  delay(500);
}

