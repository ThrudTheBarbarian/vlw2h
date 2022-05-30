# vlw2h
Convert VLW fonts (created by Processing) into header file format

The idea is to use '[Processing](https://processing.org/download)' software to create fonts in this VLW format, which can then be processed into alpha-values for display on an LCD, using an RP2040 (though anything that drives the display will do)

Steps to use:

- Download Processing from the above link (it is pretty cross-platform)
- Go to 'Tools' -> 'Create Font' and choose any of the fonts on your OS
- Optionally choose which unicode character-set to use 
- Go to 'File' -> 'Save' and it will save the sketch.

In the sketch 'data' directory, there will be a .vlw file, named after the font you chose. This is the input to 'vlw2h'

> vlw2h -f [path to font file] -h [header filename]

And that's it.