from PIL import Image

target_width = 320

for i in range(1, 9):
    im = Image.open('input/Landscape_{}.jpg'.format(i))
    
    width = target_width
    height = 10000000

    # Swap input width and height when rotating by 90 or 270 degrees.
    if i >= 5:
        width, height = height, width

    im.thumbnail((width,height), Image.LANCZOS)

    if i == 6: im = im.rotate(-90, expand=True)
    elif i == 8: im = im.rotate(90, expand=True)
    elif i == 3: im = im.rotate(180, expand=True)
    elif i == 2: im = im.transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 5: im = im.rotate(-90, expand=True).transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 7: im = im.rotate(90, expand=True).transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 4: im = im.rotate(180, expand=True).transpose(Image.FLIP_LEFT_RIGHT)

    im.save('output-patch/Landscape_{}.jpg'.format(i))

for i in range(1, 9):
    im = Image.open('input/Portrait_{}.jpg'.format(i))

    width = target_width
    height = 10000000

    # Swap input width and height when rotating by 90 or 270 degrees.
    if i >= 5:
        width, height = height, width

    im.thumbnail((width,height), Image.LANCZOS)

    if i == 6: im = im.rotate(-90, expand=True)
    elif i == 8: im = im.rotate(90, expand=True)
    elif i == 3: im = im.rotate(180, expand=True)
    elif i == 2: im = im.transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 5: im = im.rotate(-90, expand=True).transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 7: im = im.rotate(90, expand=True).transpose(Image.FLIP_LEFT_RIGHT)
    elif i == 4: im = im.rotate(180, expand=True).transpose(Image.FLIP_LEFT_RIGHT)

    im.save('output-patch/Portrait_{}.jpg'.format(i))
