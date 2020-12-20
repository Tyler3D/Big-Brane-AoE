import re

lookup = dict((key, val) for val, key in enumerate(['1', '1.5', '2', '3', '3.5', '4', '4.5', '5', '6', '6.5', '7', '7.5',  '8', '8.5', '9', '10', '10.5', '11', '11.5', '12']))

rasputin = '''2 – 2 – 6.5, – 6.5, – 6.5, – 6.5– – 6.5
7 – 8 – 7 – 6.5, – 6.5–
3.5 – 4 – 5 – 5 – 5
3.5 – 4 – 5 – 5 – 4 – 3.5 – 2–

6.5, – 6.5, – 6.5, – 6.5, – 6.5, – 6.5-
6.5 – 7 – 8 – 7 – 6.5, – 6.5–
3.5, – 4 – 5 – 5 – 5
3.5, – 4 – 5 – 5 – 4 – 3.5, – 2–

2– – 3.5, – 4– – 5 – 6.5, – 7, – 6.5, – 5, – 4– – 2,
3.5, – 3.5, – 3.5– – 3.5, – 4, – 2, – 2, – 3.5–
2– – 3.5, – 4– – 5 – 6.5, – 7, – 6.5, – 5, – 4– – 2,
3.5– – 5, – 4— – 3.5, – 2—'''

spooky = '''7 – 7 – 6.5 – 6.5 – 2– – 4 – 2– – 2–
7, – 7- – 6.5 – 6.5 – 2—
7 – 7 – 6.5 – 6.5 – 2 – 4 – 2–
4 – 5 – 3.5 – 4 – 2–'''

song = spooky # Spits out conversion to play Sooky Scary Skeletons

song = re.sub(r'[^\d.]', ' ', song)

notes = (str(lookup[stylophone_note]) for stylophone_note in song.split())
print('{' + ', '.join(notes) + '};')