#include <SPI.h>

#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_SHUTDOWN_NORMAL 1
#define MAX7219_REG_SHUTDOWN_SHUTDOWN 0

#define MAX7219_REG_DECODE 0x09
#define MAX7219_REG_DECODE_NONE 0

#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_INTENSITY_MAX 0xF

#define MAX7219_REG_SCANLIMIT 0x0B

#define MAX7219_REG_MODE 0x0F
#define MAX7219_REG_MODE_NORMAL 0
#define MAX7219_REG_MODE_TEST 1

#define MAX7219_SEG_A 0x40
#define MAX7219_SEG_B 0x20
#define MAX7219_SEG_C 0x10
#define MAX7219_SEG_D 0x08
#define MAX7219_SEG_E 0x04
#define MAX7219_SEG_F 0x02
#define MAX7219_SEG_G 0x01
#define MAX7219_SEGDP 0x80

#define MAX7219_SS 9

#define LEDS_ROWS 4
#define LEDS_NOTES 16
class Leds {
//  struct DigSegPair {
//    uint8_t dig : 4;
//    uint8_t seg : 4;
//    DigSegPair(uint8_t d, uint8_t s) : dig(d), seg(s) { }
//  };
  uint8_t digState[8];
  const static uint8_t segLookup[LEDS_ROWS][2];
public:
  Leds() : digState{0} {
    beginTransaction();
    setScanLimit(7);
    setIntensity(0x06);
    setDecode(MAX7219_REG_DECODE_NONE);
    setMode(MAX7219_REG_MODE_NORMAL);
    setShutdown(MAX7219_REG_SHUTDOWN_SHUTDOWN);
    setShutdown(MAX7219_REG_SHUTDOWN_NORMAL);
  }

  void writeRow(uint8_t row, uint8_t note) {
    uint8_t dig = note & 0b111; 
    writeDigit(dig, digState[dig] | segLookup[row][note > 7]);
  }

  void clear() {
    for(uint8_t i = 0; i < 8; ++i)
    {
      writeDigit(i, 0x00);
      digState[i] = 0;
    }    
  }
  
private:
  inline void beginTransaction() {
    SPI.begin();
    pinMode(MAX7219_SS, OUTPUT);
    digitalWrite(MAX7219_SS, HIGH);
    SPI.beginTransaction(SPISettings(140000, MSBFIRST, SPI_MODE0));
  }

  void setScanLimit(uint8_t val) {
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(MAX7219_REG_SCANLIMIT);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }
  
  void setIntensity(uint8_t val) {
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(MAX7219_REG_INTENSITY);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }

  void setDecode(uint8_t val) {
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(MAX7219_REG_DECODE);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }

  void setShutdown(uint8_t val) {
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(MAX7219_REG_SHUTDOWN);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }

  void setMode(uint8_t val) {
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(MAX7219_REG_MODE);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }

  void writeDigit(uint8_t dig, uint8_t val) {
    digState[dig] = val;
    beginTransaction();
    digitalWrite(MAX7219_SS, HIGH);
    digitalWrite(MAX7219_SS, LOW);
    SPI.transfer(dig+1);
    SPI.transfer(val);
    digitalWrite(MAX7219_SS, HIGH);
  }
};

const uint8_t Leds::segLookup[LEDS_ROWS][2] =
  {
    {
      MAX7219_SEG_A, MAX7219_SEG_B
    },
    {
      MAX7219_SEG_C, MAX7219_SEG_D
    },
    {
      MAX7219_SEG_E, MAX7219_SEG_F
    },
    {
      MAX7219_SEG_G, MAX7219_SEGDP
    },
  };
