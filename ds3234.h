#ifndef __ds3234_h_
#define __ds3234_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

void DS3234_init(uint8_t pin, uint8_t creg);
void DS3234_set(uint8_t pin, uint8_t s, uint8_t mi, uint8_t h, uint8_t dw,
                uint8_t d, uint8_t mo, uint16_t y);
void DS3234_get(uint8_t pin, unsigned char type, char *buf, size_t len);

// control/status register
void DS3234_set_creg(uint8_t pin, uint8_t val);
void DS3234_set_sreg(uint8_t pin, uint8_t mask);
uint8_t DS3234_get_sreg(uint8_t pin);

// aging offset register
void DS3234_set_aging(uint8_t pin, int8_t value);
int8_t DS3234_get_aging(uint8_t pin);

// temperature register
float DS3234_get_treg(uint8_t pin);

// alarms
void DS3234_set_a1(uint8_t pin, uint8_t s, uint8_t mi, uint8_t h, uint8_t d,
                   boolean * flags);
void DS3234_get_a1(uint8_t pin, char *buf, size_t len);
void DS3234_set_a2(uint8_t pin, uint8_t mi, uint8_t h, uint8_t d,
                   boolean * flags);
void DS3234_get_a2(uint8_t pin, char *buf, size_t len);

// sram
void DS3234_set_sram_8b(uint8_t pin, uint8_t address, uint8_t value);
uint8_t DS3234_get_sram_8b(uint8_t pin, uint8_t address);

// helpers
uint8_t dectobcd(uint8_t val);
uint8_t bcdtodec(uint8_t val);

#endif
