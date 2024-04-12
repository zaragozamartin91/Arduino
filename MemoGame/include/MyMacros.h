#ifndef MYMACROS_H
#define MYMACROS_H

#define DEBUG 1

#if DEBUG == 1
#define debug(x,y) Serial.print(x);Serial.println(y)
#else
#define debug(x,y)
#endif

#endif