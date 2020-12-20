Buttons buttons;
Audio audio;
Leds leds;

//SDCard sd;

uint8_t ledState[4] = {0x03};

const uint8_t tones[] = {10, 10, 9, 9, 2, 5, 2, 2, 10, 10, 9, 9, 2, 10, 10, 9, 9, 2, 5, 2, 5, 7, 4, 5, 2};
uint8_t currentToneIdx = 0;

uint8_t shiftLeds(const uint8_t nextTone) {
  leds.clear();
  // Returns tone that the user should play next.
  ledState[0] = ledState [1];
  ledState[1] = ledState [2];
  ledState[2] = ledState [3];
  ledState[3] = nextTone;
  
  for (uint8_t i = 0; i < 4; ++i)
  {
    leds.writeRow(i, ledState[i]);
  }

  return ledState[0];
}

void setup() {
  while (currentToneIdx < 3) // Load in first 4 tones.
  {
    shiftLeds(tones[currentToneIdx++]);
  }
}


void loop() {
  uint8_t nextTone = shiftLeds(tones[currentToneIdx++]);
  while (buttons.scanButtons() != nextTone);  // Wait for the user to press the correct button.
  audio.playTone(nextTone);
  while (buttons.scanButtons() == nextTone);  // Wait until the user releases the button.
  audio.mute();

  if (currentToneIdx == sizeof(tones))
  {
    currentToneIdx = 0;
  }
}

//  // Play Song
//  for (uint8_t i = 0; i < sizeof(tones); i++)
//  {
//    audio.playTone(tones[i] + 3);
//    delay(180);
//    audio.mute();
//    delay(20);
//  }

//  // Piano
//  uint8_t note_pressed = b.scanButtons();
//  if (note_pressed & BUTTONS_EMPTY_FLAG) {
//    audio.mute();
//  } 
//  else {
//    audio.playTone(note_pressed);
//    while(note_pressed == b.scanButtons());
//  }

//  // LEDs
//  for (uint8_t row = 0; row < LEDS_ROWS; row++) {
//    for (uint8_t note = 0; note < LEDS_NOTES; note++) {
//     m.writeRow(row, note); 
//     delay(100 * (row + 1));   
//     m.clear();
//    }
//  }
