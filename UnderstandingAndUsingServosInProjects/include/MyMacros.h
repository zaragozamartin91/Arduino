#ifndef MYMACROS_H
#define MYMACROS_H

#define MZ_DEBUG 1

#if MZ_DEBUG == 1
#define debugPrint(x) Serial.println(x)
#define debugPrintArgs(x,y) Serial.print(x);Serial.println(y)
#else
#define debugPrint(x)
#define debugPrintArgs(x,y)
#endif

// Max number of buttons and leds currently supported
#define MAX_BUTTONS_AND_LEDS 3

#endif