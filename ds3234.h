#ifndef __ds3234_h_
#define __ds3234_h_

//#if ARDUINO >= 100
#include <Arduino.h>
//#else
//#include <WProgram.h>
//#endif

#include "config.h"

#define SECONDS_FROM_1970_TO_2000 946684800

// control register bits
#define DS3234_A1IE     0x1
#define DS3234_A2IE     0x2
#define DS3234_INTCN    0x4

// status register bits
#define DS3234_A1F      0x1
#define DS3234_A2F      0x2
#define DS3234_OSF      0x80

struct ts {
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t mday;        /* day of the month */
    uint8_t mon;         /* month */
    int year;            /* year */
    uint8_t wday;        /* day of the week */
    uint8_t yday;        /* day in the year */
    uint8_t isdst;       /* daylight saving time */
    uint8_t year_s;      /* year in short notation*/
#ifdef CONFIG_UNIXTIME
    uint32_t unixtime;   /* seconds since 01.01.1970 00:00:00 UTC*/
#endif    
};

void DS3234_init(const uint8_t pin, const uint8_t creg);
void DS3234_set(const uint8_t pin, struct ts t);
void DS3234_get(const uint8_t pin, struct ts *t);

void DS3234_set_addr(const uint8_t pin, const uint8_t addr, const uint8_t val);
uint8_t DS3234_get_addr(const uint8_t pin, const uint8_t addr);

// control/status register
void DS3234_set_creg(const uint8_t pin, const uint8_t val);
void DS3234_set_sreg(const uint8_t pin, const uint8_t mask);
uint8_t DS3234_get_sreg(const uint8_t pin);

// aging offset register
void DS3234_set_aging(const uint8_t pin, const int8_t value);
int8_t DS3234_get_aging(const uint8_t pin);

// temperature register
float DS3234_get_treg(const uint8_t pin);

// alarms
void DS3234_set_a1(const uint8_t pin, const uint8_t s, const uint8_t mi, const uint8_t h, const uint8_t d,
                   const uint8_t * flags);
void DS3234_get_a1(const uint8_t pin, char *buf, const uint8_t len);
void DS3234_clear_a1f(const uint8_t pin);
uint8_t DS3234_triggered_a1(const uint8_t pin);

void DS3234_set_a2(const uint8_t pin, const uint8_t mi, const uint8_t h, const uint8_t d,
                   const uint8_t * flags);
void DS3234_get_a2(const uint8_t pin, char *buf, const uint8_t len);
void DS3234_clear_a2f(const uint8_t pin);
uint8_t DS3234_triggered_a2(const uint8_t pin);

// sram
void DS3234_set_sram_8b(const uint8_t pin, const uint8_t address, const uint8_t value);
uint8_t DS3234_get_sram_8b(const uint8_t pin, const uint8_t address);

// helpers
uint32_t get_unixtime(struct ts t);
uint8_t dectobcd(const uint8_t val);
uint8_t bcdtodec(const uint8_t val);
uint8_t inp2toi(const char *cmd, const uint16_t seek);

#endif
