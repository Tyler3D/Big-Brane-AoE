#include <SPI.h>

#define AD9833_WRITE_FREQ0_LSB 0x4000
#define AD9833_REG_PHASE0 0xC000
#define AD9833_REG_CONTROL 0
#define AD9833_REG_CONTROL_RESET 0x0100
#define AD9833_SS 10
class Audio {

private:
  const static uint16_t TONE_COMMAND_LOOKUP[];
  
  static const SPISettings SETTINGS;
  inline void beginTransaction() {
    SPI.begin();
    SPI.beginTransaction(SETTINGS);
    pinMode(AD9833_SS, OUTPUT);
  }

  inline void resetOn() {
    SPI.transfer16(AD9833_REG_CONTROL | AD9833_REG_CONTROL_RESET);
  }

  inline void resetOff() {
    SPI.transfer16(AD9833_REG_CONTROL);
  }

public:
  Audio()
  {
    beginTransaction();
    digitalWrite(AD9833_SS, LOW);
    SPI.transfer16(0x2100);
    SPI.transfer16(0x4000);
    SPI.transfer16(0x4000);
    SPI.transfer16(0xC000);
    SPI.transfer16(0x2000);
    digitalWrite(AD9833_SS, HIGH);
  }
  inline void mute() {
    beginTransaction();
    digitalWrite(AD9833_SS, LOW);
    SPI.transfer16(AD9833_WRITE_FREQ0_LSB | 0x0000);
    digitalWrite(AD9833_SS, HIGH);
  }
  inline void playTone(uint8_t tone) {
      beginTransaction();
      digitalWrite(AD9833_SS, LOW);
      SPI.transfer16(TONE_COMMAND_LOOKUP[tone]);
      SPI.transfer16(0x4000);
      digitalWrite(AD9833_SS, HIGH);
    }
};
const SPISettings Audio::SETTINGS(000, MSBFIRST, SPI_MODE2);
const uint16_t Audio::TONE_COMMAND_LOOKUP[] {
  0x0751 | AD9833_WRITE_FREQ0_LSB, // C4
  0x07C0 | AD9833_WRITE_FREQ0_LSB, // C#4
  0x0836 | AD9833_WRITE_FREQ0_LSB, // D4
  0x08B3 | AD9833_WRITE_FREQ0_LSB, // D#4
  0x0938 | AD9833_WRITE_FREQ0_LSB, // E4
  0x09C4 | AD9833_WRITE_FREQ0_LSB, // F4
  0x0A59 | AD9833_WRITE_FREQ0_LSB, // F#4
  0x0AF6 | AD9833_WRITE_FREQ0_LSB, // G4
  0x0B9D | AD9833_WRITE_FREQ0_LSB, // G#4
  0x0C4E | AD9833_WRITE_FREQ0_LSB, // A4
  0x0D09 | AD9833_WRITE_FREQ0_LSB, // A#4
  0x0DCF | AD9833_WRITE_FREQ0_LSB, // B4
  0x0EA2 | AD9833_WRITE_FREQ0_LSB, // C5
  0x0F80 | AD9833_WRITE_FREQ0_LSB, // C#5
  0x106C | AD9833_WRITE_FREQ0_LSB, // D5
  0x1166 | AD9833_WRITE_FREQ0_LSB, // D#5
  0x126F | AD9833_WRITE_FREQ0_LSB, // E5
  0x1388 | AD9833_WRITE_FREQ0_LSB, // F5
  0x14B1 | AD9833_WRITE_FREQ0_LSB, // F#5
  0x15EC | AD9833_WRITE_FREQ0_LSB, // G5
  0x173A | AD9833_WRITE_FREQ0_LSB, // G#5
  0x189B | AD9833_WRITE_FREQ0_LSB, // A5
  0x1A12 | AD9833_WRITE_FREQ0_LSB, // A#5
  0x1B9F | AD9833_WRITE_FREQ0_LSB  // B5
};
