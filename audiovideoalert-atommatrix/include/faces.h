#ifndef __FACES_H
#define __FACES_H

#include <FastLED.h>

extern CRGB FACE_SAD[], FACE_HAPPY[];

void draw_face(CRGB* leds, CRGB* face);

#endif /* __FACES_H */