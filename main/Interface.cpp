#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define ENABLE_BIT 7
#define TONE_MASK 0x0f

class Audio {
  public:
  Audio()
  {
    std::cout << "Audio initialized" << std::endl;
  }
  void playTone(uint8_t tone) 
  {
    std::cout << "Playing tone " << tone << std::endl;
  }
  void mute() 
  {
    std::cout << "Speaker Muted" << std::endl;
  }
};

class Buttons {
  private:
  uint8_t timesPressed;
  public:
  Buttons()
  {
    std::cout << "Buttons initialized" << std::endl;
  }
  uint8_t scanButtons() {
    timesPressed++;
    if (timesPressed <= 10)
    {
      return 0;
    }
    else {
        return (1 << ENABLE_BIT) | (timesPressed & TONE_MASK);
    }
  }
};

class LEDs {
  public:
  uint16_t leds[4];
  LEDs()
  {
    std::cout << "LEDs initialized" << std::endl;
  }

  void printLED()
  {
    for (int i = 0; i < 3; i++)
    {
      std::cout << leds[i] << std::endl;
    }
  }

  void setLED(uint8_t row, uint8_t column) 
  {
      std::cout << "LED (" << row << ", " << column << std::endl;
      leds[row] = column;
  }

  void clearRow(uint8_t row) 
  {
    leds[row] = 0;
  }
};

class SDCard { 
  private:
  // Allocate space for 512 individual notes.
  uint8_t song[256]; 
  public:

  SDCard()
  {
    std::cout << "SDCard initialized" << std::endl;
  }

  void openSong(uint8_t songNo)
  {
    
  }
  
  uint8_t nextTone()
  {
    
  }
};

class Piano {
  Audio audio;
  Buttons buttons;
  LEDs leds;
  SDCard sdcard;

  uint8_t ledState[4];
  
  public:

  void shiftLedsDown(uint8_t nextTone) 
  {
    for (uint8_t i = 0; i < 3; i++) 
    {
        ledState[i] = ledState[i + 1];
        leds.setLED(i, ledState[i] & TONE_MASK);
    }
    ledState[3] = (1 << ENABLE_BIT) | nextTone;
    leds.setLED(3, nextTone);
  }
  
  
  Piano() 
  {
    uint8_t songSelected = buttons.scanButtons();
    while (!songSelected)  
    {
        songSelected = buttons.scanButtons();
    }
    songSelected &= TONE_MASK; // Drop BP bit
  
    sdcard.openSong(songSelected);
  }
  
    void loop()
    {
      uint8_t nextTone = sdcard.nextTone();

      std::cout << nextTone << std::endl;
      audio.playTone(nextTone);
      // Probably should include some delay between instructions
      // Bring Leds Down
      shiftLedsDown(nextTone);
    
    }
  };  


int main() {
    Piano piano;
    piano.loop();
}
