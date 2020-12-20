#define BUTTONS_ROWS 4
#define BUTTONS_COLS 4
#define BUTTONS_EMPTY_FLAG (1 << 7)

class Buttons {
public:
  static const uint8_t ROW_PINS[];
  static const uint8_t COL_PINS[];
  static const uint8_t TONE_LOOKUP[BUTTONS_COLS][BUTTONS_ROWS];
public:
  Buttons() {
    for (uint8_t i = 0; i < 4; ++i)
    {
      pinMode(COL_PINS[i], OUTPUT);
      digitalWrite(COL_PINS[i], HIGH);
      pinMode(ROW_PINS[i], INPUT_PULLUP);
    }
  }
  uint8_t scanButtons() {
    for (uint8_t col = 0; col < BUTTONS_COLS; ++col)
    {
      digitalWrite(COL_PINS[col], LOW);
      for (uint8_t row = 0; row < BUTTONS_ROWS; ++row) {
        delay(.01);
        if (!digitalRead(ROW_PINS[row])) {
          // Make sure to bring this back high for next run.
          digitalWrite(COL_PINS[col], HIGH); 
          return TONE_LOOKUP[col][row];
        }
      }
      digitalWrite(COL_PINS[col], HIGH);
    }
    return BUTTONS_EMPTY_FLAG;
  }
};

const uint8_t Buttons::ROW_PINS[] = {0, 1, 2, 3};
const uint8_t Buttons::COL_PINS[] = {4, 5, 6, 7};
const uint8_t Buttons::TONE_LOOKUP[BUTTONS_COLS][BUTTONS_ROWS] =
  {
    {0, 4, 9, 12},
    {1, 5, 8, 13},
    {2, 7, 10, 14},
    {3, 6, 11, 15},
  };
