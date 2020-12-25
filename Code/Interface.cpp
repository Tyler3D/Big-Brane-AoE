#define TONE_MASK 0x0f

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

  Audio audio;
  Buttons buttons;
  LEDs leds;
  SDCard sdcard;

  uint8_t ledState[4];
  uint8_t nextTone;
  uint8_t songSelected;
  uint8_t difficulty;
  uint8_t index;
  unsigned long timeStart;
  unsigned long timeEnd;

  void shiftLedsDown(uint8_t nextTone)
  {
    leds.clear();
    for (uint8_t i = 0; i < 3; i++)
    {
        ledState[i] = ledState[i + 1];
        leds.writeRow(i, ledState[i]);
    }
    ledState[3] = nextTone;
    leds.writeRow(3, nextTone);
  }

void setup() {
    leds.clear();
    while (songSelected != TONE_MASK)
    {
        songSelected = buttons.scanButtons();
    }

    while (difficulty != TONE_MASK)
    {
      difficulty = buttons.scanButtons();
    }

    // sdcard.openSong(songSelected); SD Card not Implemented
    
    ledState[0] = 0;
    ledState[1] = 0;
    ledState[2] = 0;
    ledState[3] = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
      nextTone = sdcard.nextTone();
      shiftLedsDown(nextTone);
      delay(1000 * 8 / difficulty);
    }
}

void loop()
{
    audio.mute();
    // Bring Leds Down
    nextTone = sdcard.nextTone();
    if (nextTone == 0xff) // Signifying end of song
      {
        setup();
        return;
      }
      shiftLedsDown(nextTone);
      // Loop is restarted if Buttons do not match leds
      timeStart = mills();
      do {
        uint8_t user = buttons.scanButtons();
        timeEnd = mills();
        
        if (user != TONE_MASK && user != ledState[0])
        {
          // Failure
          setup();
          break;
        } else if (user == ledState[0])
          break;
      } while (timeEnd - timeStart < 1000 * 8 / difficulty)

      if (timeEnd - timeStart > 1000 * 8 / difficulty)
      {
        // If they failed to press the button in time
        setup();
        return;
      }
      audio.playTone(ledState[0]);

}
