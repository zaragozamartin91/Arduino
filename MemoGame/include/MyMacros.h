#ifndef MYMACROS_H
#define MYMACROS_H

#define MZ_DEBUG 1

#if MZ_DEBUG == 1
#define debugPrint(x,y) Serial.print(x);Serial.println(y)
#else
#define debugPrint(x,y)
#endif

#endif