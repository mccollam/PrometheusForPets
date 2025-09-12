#ifndef __FACES_H
#define __FACES_H

#include <FastLED.h>

extern CRGB FACE_SAD[], FACE_HAPPY[], FACE_CONNECTING[], FACE_NONE[];

void draw_face(CRGB leds[25], CRGB face[25]);

#endif /* __FACES_H */