# MEMU3

MEMU3 - Overwatch pixelbot, take three

Runs on Interception mouse filter driver and uses direct-x API for overlay (iirc)

Feel free to use in any way you like - that's just a POC.

## Controls

F2 - Change mode

Num-8 - Use different color settings (For version 1.0 I've included pre-calculated settings for red (enemy) and neon (allies) colors) (it's those obnoxious files 2mb files)

ALT - Toggle On/Off

Num-Del - Change Debug UI mode

Num-0 - Change UI mode

Num-Plus \ Num-Minus - Increase\decrease strength (or mode-specific value)

Num-9 \ Num-3 - Increase\decrease sensitivity (affects flickshots)


## About colors

Target colors are described in `.colorset` files, those are CSV (comma-separated values) : R,G,B,T where R stands for RED, G stands for GREEN, B stands for BLUE and T stands for TOLERANCE.

For example:
```csv
241,235,94,16
241,232,85,16
250,243,109,16
219,219,63,16
228,221,56,16
```
Means that cheat is going to search for color rgb(241,235,94) and everything "16" points around it like.

This is how it works:

Target color: `100,100,100,10`, scanning pixel (103,100,100):

sqrt((103-100)^2+(100-100)^2+(100-100)^2) = sqrt(9) = 3, which is lower than tolerance 10: color would pass the threshold.

Target color: `100,100,100,3`, scanning pixel (104,100,100):

sqrt((104-100)^2+(100-100)^2+(100-100)^2) = sqrt(16) = 4, which is higher than tolerance 3: color would not pass the threshold.

## Build

Build it with Cmake, should be straight-forward

## Contribute

Feel free to fork\open PR if there's anything to improve. Cheers o/
