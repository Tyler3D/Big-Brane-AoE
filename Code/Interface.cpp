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
    std::cout << "Playing tone " << unsigned(tone) << std::endl;
  }
  void mute() 
  {
    std::cout << "Speaker Muted" << std::endl;
  }
};

class Buttons {
  private:
  uint8_t timesPressed;
  uint8_t userInput;
  public:
  Buttons()
  {
    std::cout << "Buttons initialized" << std::endl;
  }
  uint8_t scanButtons() 
  {
  	/* For now, works on user input
    timesPressed++;
    if (timesPressed <= 10)
    {
      return 0;
    }
    else {
        return (1 << ENABLE_BIT) | (timesPressed & TONE_MASK);
    }
    */
  	std::cin >> userInput;
  	return userInput - 0x30; // Since std::cin returns a char, we need to convert from ASCII
  	// return userInput;
  }
};

class LEDs {
  public:
  uint8_t leds[4];
  LEDs()
  {
    std::cout << "LEDs initialized" << std::endl;
  }

  void printLED()
  {
    for (int i = 0; i < 3; i++)
    {
      std::cout << unsigned(leds[i]) << std::endl;
    }
  }

  void setLED(uint8_t row, uint8_t column) 
  {
      std::cout << "LED (" << unsigned(row) << ", " << unsigned(column) << ")" << std::endl;
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
  uint8_t song[256] = {1, 1, 5, 5, 6, 6, 5, 7, 4, 4, 3, 3, 2, 2, 1, 9, 4, 5, 5, 5, 0, 0, 0, 0xff};
  uint8_t index = 0;
  public:
  SDCard()
  {
    std::cout << "SDCard initialized" << std::endl;
  }

  void openSong(uint8_t songNo)
  {
  	index = 0;
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
  	sdcard.openSong(songSelected);
  	ledState[0] = 0;
  	ledState[1] = 0;
  	ledState[2] = 0;
  	ledState[3] = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
    	nextTone = sdcard.nextTone();
    	std::cout << unsigned(nextTone) << std::endl;
    	shiftLedsDown(nextTone);
    }
  }
  
  Piano() 
  {
    while (!songSelected)
    {
        songSelected = buttons.scanButtons();
    }
    songSelected &= TONE_MASK; // Drop BP bit
  
    initialize();

  }
  
    void loop()
    {
      // Probably should include some delay between instructions
      // Bring Leds Down
      // Should consider end of song
      while(true)
      {
      	nextTone = sdcard.nextTone();
      	if (nextTone == 0xff) // Signifying end of song
      	{
      		std::cout << "Song is restarting... You Won!" << std::endl;
      		initialize();
      		continue;
      	}
      	std::cout << unsigned(nextTone) << std::endl;
      
      	shiftLedsDown(nextTone);
      	// Loop is restarted if Buttons do not match leds
      	uint8_t user = buttons.scanButtons();
      	std::cout << "Input received... " << unsigned(user) << " Which is supposed to equal " << unsigned(ledState[0]) << std::endl;
      	if (user != (ledState[0]))
      	{
      		// Secondary LED turns on to show failure
      		std::cout << "Song is restarting... You Failed!" << std::endl;
      		initialize();
      		continue;
      	}

      	audio.playTone(ledState[0]);
      }
	}
  };


int main() {
    Piano piano;
    //for (int i = 0; i < 5; i++)
    	piano.loop();
}
