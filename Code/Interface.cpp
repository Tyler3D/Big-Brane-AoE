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
  uint8_t userInput;
  public:
  Buttons()
  {
    std::cout << "Buttons initialized" << std::endl;
  }
  uint8_t scanButtons() {
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
  	return userInput;
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
  uint8_t song[256]; 
  public:
  uint8_t temp = 0;
  SDCard()
  {
    std::cout << "SDCard initialized" << std::endl;
  }

  void openSong(uint8_t songNo)
  {
    
  }
  
  uint8_t nextTone()
  {
  	if (temp == 10)
    	temp = 0;
    temp++;
    return temp;
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
    ledState[3] = (1 << ENABLE_BIT) | nextTone;
    leds.setLED(3, nextTone);
  }
  
  void initialize()
  {
  	sdcard.openSong(songSelected);
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
      	if (nextTone == 10) // Signifying end of song
      	{
      		initialize();
      		std::cout << "Song is restarting... You Won!" << std::endl;
      		continue;
      	}
      	std::cout << unsigned(nextTone) << std::endl;
      
      	shiftLedsDown(nextTone);
      	// Loop is restarted if Buttons do not match leds
      	uint8_t user = buttons.scanButtons();
      	std::cout << "Input received... " << user << " Which is supposed to equal " << unsigned(ledState[0]) << std::endl;
      	
      	if (user != unsigned(ledState[0]))
      	{
      		// Secondary LED turns on to show failure
      		initialize();
      		std::cout << "Song is restarting... You Failed!" << std::endl;
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
