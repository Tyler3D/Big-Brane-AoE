Buttons b;
Audio audio;
Leds m;

void setup() {
}


void loop() {

//  // Play Song
//  uint8_t tones[] = {10, 10, 9, 9, 2, 5, 2, 2, 10, 10, 9, 9, 2, 10, 10, 9, 9, 2, 5, 2, 5, 7, 4, 5, 2
//  };
//  for (uint8_t i = 0; i < sizeof(tones); i++)
//  {
//    audio.playTone(tones[i] + 3);
//    delay(280);
//    audio.mute();
//    delay(20);
//  }

//  // Piano
//  uint8_t note_pressed = b.scanButtons();
//  if (note_pressed & BUTTONS_EMPTY_FLAG) {
//    ad9833.mute();
//    m.writeDigit(0, 0xff);
//  } 
//  else {
//    m.writeDigit(0, note_pressed);
//    ad9833.playTone(note_pressed);
//    while(note_pressed == b.scanButtons());
//  }

  // LEDs
  for (uint8_t row = 0; row < LEDS_ROWS; row++) {
    for (uint8_t note = 0; note < LEDS_NOTES; note++) {
     m.writeRow(row, note); 
     delay(100 * (row + 1));   
     m.clear();
    }
  }

}
