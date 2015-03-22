
// during an alarm the INT pin of the RTC is pulled low
//
// this is handy for minimizing power consumption for sensor-like devices, 
// since they can be started up by this pin on given time intervals.


#include <SPI.h>
#include "ds3234.h"

#define BUFF_MAX 256

const int cs = 10;              // chip select pin
uint8_t sleep_period = 5;       // the sleep interval in minutes between 2 consecutive alarms

// how often to refresh the info on stdout (ms)
unsigned long prev = 5000, interval = 5000;

void set_next_alarm(void)
{
    struct ts t;
    unsigned char wakeup_min;

    DS3234_get(cs, &t);

    // calculate the minute when the next alarm will be triggered
    wakeup_min = (t.min / sleep_period + 1) * sleep_period;
    if (wakeup_min > 59) {
        wakeup_min -= 60;
    }

    // flags define what calendar component to be checked against the current time in order
    // to trigger the alarm
    // A2M2 (minutes) (0 to enable, 1 to disable)
    // A2M3 (hour)    (0 to enable, 1 to disable) 
    // A2M4 (day)     (0 to enable, 1 to disable)
    // DY/DT          (dayofweek == 1/dayofmonth == 0)
    uint8_t flags[4] = { 0, 1, 1, 1 };

    // set Alarm2. only the minute is set since we ignore the hour and day component
    DS3234_set_a2(cs, wakeup_min, 0, 0, flags);

    // activate Alarm2
    DS3234_set_creg(cs, DS3234_INTCN | DS3234_A2IE);
}

void setup()
{
    Serial.begin(9600);
    DS3234_init(cs, DS3234_INTCN);
    DS3234_clear_a2f(cs);
    set_next_alarm();
}

void loop()
{
    char buff[BUFF_MAX];
    unsigned long now = millis();
    struct ts t;

    // once a while show what is going on
    if ((now - prev > interval) && (Serial.available() <= 0)) {
        DS3234_get(cs, &t);

        // display current time
        snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
        Serial.println(buff);

        // display a2 debug info
        DS3234_get_a2(cs, &buff[0], 59);
        Serial.println(buff);

        if (DS3234_triggered_a2(cs)) {
            Serial.println(" -> alarm2 has been triggered");
            set_next_alarm();
            // clear a2 alarm flag and let INT go into hi-z
            DS3234_clear_a2f(cs);          
        }
        prev = now;
    }
}

