#include "mitsu.h"

// byte array containing default values
unsigned char template[] = {35, 203, 38, 1, 0, 255, 8, 255, 192, 64, 255, 0, 0, 0, 138, 0, 0, 255};

void setUpWiringPi();

void setTimeAndDate();

void setTemperature(unsigned char temp);

void setMode(unsigned char operation);

void setOnOff(unsigned char on);

void calculateChecksum();

unsigned int *calculateByte(unsigned int *u);

void printRawTimings(int size, const unsigned int *result);

void flashLED(int size, const unsigned int *result);

void pulse(unsigned int us) {
    float i = 0;
    while (i < us) {
        digitalWrite(LED, HIGH);
        delayMicroseconds(EMPIRICAL_DELAY);
        digitalWrite(LED, LOW);
        delayMicroseconds(EMPIRICAL_DELAY);
        i = i + 27.77; // 36 KHz
    }
}

void send(unsigned char temp, unsigned char on, unsigned char operation) {

    setUpWiringPi();

    setTimeAndDate();
    setTemperature(temp);
    setMode(operation);
    setOnOff(on);

    calculateChecksum();

    int size = 2 + ((sizeof(template) * 2) * 2 * 8) + 4 + 1;
    if (DEBUG) {
        printf("Size of data: %d\n", size);
    }

    unsigned int result[size];
    unsigned int *u = &result[0];
    u = calculateByte(u);
    *(u++) = P_H3;
    calculateByte(u);

    if (DUMP_RAW) {
        printRawTimings(size, result);
    }

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    flashLED(size, result);
    gettimeofday(&stop, NULL);

    long total = 0;
    int i;
    for (i = 0; i < size; i++) {
        total += result[i];
    }

    if (DEBUG) {
        printf("Total should be: %f\n", total / 1000000.0);
        printf("Elapsed %f\n",
               (double) (((stop.tv_sec - start.tv_sec) * 1000000ULL + (stop.tv_usec - start.tv_usec))) / 1000000.0);
    }
}

void flashLED(int size, const unsigned int *result) {
    int i;
    for (i = 0; i < size; i++) {
        if (i % 2 == 0) {
            pulse(result[i]);
        }
        else {
            delayMicroseconds(result[i]);
        }
    }
}

void printRawTimings(int size, const unsigned int *result) {
    printf("Raw timings:\n");
    int i;
    for (i = 0; i < size; i++) {
        if (i % 2 == 0) {
            printf("pulse ");
        }
        else {
            printf("space ");
        }
        printf("%d\n", result[i]);
    }
}

unsigned int *calculateByte(unsigned int *u) {
    int i, j, temp;
    *(u++) = P_H1;
    *(u++) = P_H2;
    unsigned char *t = &template[0];
    for (i = 0; i < sizeof(template); i++) {
        temp = *t;
        for (j = 0; j < 8; j++) {
            if ((temp & 1) == 1) {
                *(u++) = P_DEL;
                *(u++) = P_ONE;
            }
            else {
                *(u++) = P_DEL;
                *(u++) = P_ZERO;
            }
            temp = temp >> 1;
        }
        t++;
    }
    *(u++) = P_DEL;
    return u;
}

void calculateChecksum() {
    int summa = 0;
    unsigned char *t = &template[0];
    int i;
    for (i = 0; i < sizeof(template) - 1; i++) {
        summa = summa + (int) *(t++);
    }
    *t = (unsigned char) (summa & 0xff);
}

void setOnOff(unsigned char on) {
    if (DEBUG) {
        printf("ON: %d\n", on);
    }
    if (on == 0) {
        template[ON_OFF] = OFF;
    }
    else {
        template[ON_OFF] = ON;
    }
}

void setMode(unsigned char operation) {
    if (operation == 0) {
        template[MODE] = HEATING;
    } else {
        template[MODE] = COOLING;
        template[8] += 6;
    }
    if (DEBUG) {
        printf("Mode: %d\n", operation);
    }
}

void setTemperature(unsigned char temp) {
    if (DEBUG) {
        printf("Temperature: %d\n", temp);
    }
    if (temp == 10) {
        template[TEMP] = 0;
        template[TEMP2] = 32;
    }
    else {
        temp = (temp - 16) & 0xf;
        template[TEMP] = temp;
    }
}

void setTimeAndDate() {
    time_t timer;
    struct tm *timeinfo;
    timer = time(NULL);
    timeinfo = localtime(&timer);

    unsigned char time = (timeinfo->tm_hour * 6) + (timeinfo->tm_min / 10);
    template[TIME] = time;
    if (DEBUG) {
        printf("Time: %d\n", time);
    }

    unsigned char weekday = timeinfo->tm_wday;
    weekday = weekday - 1; // set monday to be 0 day of week
    if (weekday == -1) {
        weekday = 6;
    }
    if (weekday > 3) {
        weekday = (weekday - 4) | 0x80;
    }
    template[DAY] = weekday + 8;
}

void setUpWiringPi() {
    wiringPiSetup();
    // set RT priority
    piHiPri(99);
    pinMode(LED, OUTPUT);
}

