# Python tool to convert font image to C bit array.
#
# by drummyfish
# released under CC0 1.0.

from PIL import Image

imageArray = []
paletteColors = []
paletteArray = []

image = Image.open("font.png").convert("RGB")
pixels = image.load()

column = 0
value = 0
index = 0

result = ""

chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,!?0123456789/-+()%"

for x in range(image.size[0]):

  if column == 4:
    column = 0
    value = 0
    index += 1
    continue 

  for y in range(image.size[1]):
    value = value * 2 + (1 if pixels[x,y][0] < 128 else 0)

  if column == 3:
    result += "  0x" + hex(value)[2:].zfill(4) + ", // " + str(index) + " \"" + chars[index] + "\"\n"

  column += 1

result = "{\n" + result + "}";

print(result)
