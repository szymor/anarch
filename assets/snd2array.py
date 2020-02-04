# Python tool to convert a sound to C array.
#
# Pass sound file name to this sctipt as a parameter. The file must be in raw
# mono 8kHz 8bit PCM format.
#
# by drummyfish
# released under CC0 1.0.

import sys

even = True
val = 0

result = ""

first = True

count = 0

with open(sys.argv[1], "rb") as f:
  while True:
    byte = f.read(1)

    if byte == b"":
      break

    byte = ord(byte)

    quantized = byte / 16

    if even: 
      val = quantized << 4
    else:
      val = val | quantized

      count += 1

      if first:
        first = False
      else:
        result += ","

      if count % 20 == 0:
        result += "\n"

      result += str(val)

    even = not even

print("uint8_t sound[" + str(count) + "] = {\n" + result + "\n}")
