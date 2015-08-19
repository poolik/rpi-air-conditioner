


#include "wiringPi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define LED              3 // IR LED wiringPi pin
#define EMPIRICAL_DELAY 12 // to achieve 36 KHz
#define ON               32
#define OFF              0

// position of interesting values within byte array
#define TEMP    7
#define TIME   10
#define DAY    14
#define ON_OFF  5
#define MODE    6

// timings
#define P_DEL    450
#define P_ZERO   420
#define P_ONE   1300
#define P_H1    3400
#define P_H2    1750
#define P_H3   17100

#define DEBUG 1
#define DUMP_RAW 1

#define HEATING    72 // with i-see on
#define COOLING    88 // with i-see on

void send(unsigned char temp, unsigned char on, unsigned char operation);
