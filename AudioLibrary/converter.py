#!/usr/bin/env python3

import csv

with open('tones.csv') as tone_file:
   tone_data = list(csv.reader(tone_file))

for row in tone_data[1:]:
    note_name = row[1].strip()
    hex_code = '0x' + row[4].strip()
    print(f'{hex_code} | AD9833_WRITE_FREQ0_LSB, // {note_name}')


