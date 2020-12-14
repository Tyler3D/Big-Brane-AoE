#include <stdlib.h>
#include <stdio.h>

#define ENABLE_BIT 7
#define TONE_MASK 0x0f

class Audio {
  public:
  Audio()
  {
    Serial.print("Audio initialized \n");
  }
  void playTone(uint8_t tone) 
  {
    Serial.print("Playing tone ");
    Serial.println(unsigned(tone));
  }
  void mute() 
  {
    Serial.print("Speaker Muted \n");
  }
};

class Buttons {
  private:
  uint8_t song[0xff] = {4, 16, 1, 1, 5, 5, 6, 6, 5, 7, 4, 4, 3, 3, 2, 2, 1, 9, 4, 5, 5, 5, 0xff};
  uint8_t index;
  public:
  Buttons()
  {
    Serial.print("Buttons initialized \n");
    index = 0;
  }
  uint8_t scanButtons() // For now, cheat
  {
    if (song[index] == 0xff)
      index = 0;
    return song[index++];
  }
};

class LEDs {
  public:
  uint8_t leds[4];
  LEDs()
  {
    Serial.print("LEDs initialized \n");
  }

  void printLED()
  {
    for (int i = 0; i < 3; i++)
    {
      Serial.println(unsigned(leds[i]));
    }
  }

  void setLED(uint8_t row, uint8_t column) 
  {
      Serial.print("LED (");
      Serial.print(unsigned(row));
      Serial.print(", ");
      Serial.print(unsigned(column));
      Serial.println(")");
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
  uint8_t song[0xff] = {1, 1, 5, 5, 6, 6, 5, 7, 4, 4, 3, 3, 2, 2, 1, 9, 4, 5, 5, 5, 0, 0, 0, 0xff};
  uint8_t index;
  public:
  SDCard()
  {
    Serial.print("SDCard initialized \n");
    index = 0;
  }

  void openSong(uint8_t songNo)
  {
    index = 0;
    Serial.print("Selecting song: ");
    Serial.println(unsigned(songNo));
  }
  
  uint8_t nextTone()
  {
    return song[index++];
  }
};

class Piano {
  Audio audio;
  Buttons buttons;
  LEDs leds;
  SDCard sdcard;

  uint8_t ledState[4];
  uint8_t nextTone;
  uint8_t songSelected;
  uint8_t difficulty;

  public:

  void shiftLedsDown(uint8_t nextTone)
  {
    for (uint8_t i = 0; i < 3; i++)
    {
        ledState[i] = ledState[i + 1];
        leds.setLED(i, ledState[i] & TONE_MASK);
    }
    ledState[3] = ((1 << ENABLE_BIT) | nextTone) & 0x7f;
    leds.setLED(3, nextTone);
  }
  
  void initialize()
  {
    //while (!songSelected)
    //{
        songSelected = buttons.scanButtons();
    //}
    //songSelected &= TONE_MASK; // Drop BP bit

    //while (!difficulty)
    //{
      difficulty = buttons.scanButtons();
    //}
    
    sdcard.openSong(songSelected);
    Serial.print("Setting difficulty to: ");
    Serial.println(unsigned(difficulty));
    
    ledState[0] = 0;
    ledState[1] = 0;
    ledState[2] = 0;
    ledState[3] = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
      nextTone = sdcard.nextTone();
      Serial.println(unsigned(nextTone));
      shiftLedsDown(nextTone);
    }
  }
  
  Piano() 
  {
    initialize();
  }

  void next()
  {
    // Bring Leds Down
    nextTone = sdcard.nextTone();
    if (nextTone == 0xff) // Signifying end of song. Generally after three 0's
      {
        Serial.print("Song is restarting... You Won! \n");
        initialize();
        return;
      }
      Serial.println(unsigned(nextTone));
      delay(1000 * 8 / difficulty); // Will likely need balancing
      shiftLedsDown(nextTone);
      // Loop is restarted if Buttons do not match leds
      uint8_t user = buttons.scanButtons();
      Serial.print(unsigned(user));
      Serial.print(" Is the user input. Is supposed to equal: ");
      Serial.println(ledState[0]);
      if (user != (ledState[0]))
      {
        //Secondary LED turns on to show failure
        Serial.print("Song is restarting... You Failed! \n");
        initialize();
        return;
      }

      audio.playTone(ledState[0]);
  }

};

void setup() {
  Serial.begin(9600);
  Piano piano;
  while(true)
    piano.next();
}

void loop()
{
  
}
