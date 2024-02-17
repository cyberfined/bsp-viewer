#include "../time.h"
#include "syscall.h"

#define LEAPOCH (946684800LL + 86400*(31+29))

#define DAYS_PER_400Y (365*400 + 97)
#define DAYS_PER_100Y (365*100 + 24)
#define DAYS_PER_4Y   (365*4   + 1)

void get_localtime(time_spec_t *time) {
    long long days, secs, years;
    int remdays, remsecs, remyears;
    int qc_cycles, c_cycles, q_cycles;
    int months;
    int wday, yday, leap;
    static const char days_in_month[] = {31,30,31,30,31,31,30,31,30,31,31,29};

    long t;
    syscall1(__NR_TIME, (long)&t);

    secs = t - LEAPOCH;
    days = secs / 86400;
    remsecs = secs % 86400;
    if(remsecs < 0) {
        remsecs += 86400;
       days--;
    }

    wday = (3+days)%7;
    if(wday < 0)
        wday += 7;

    qc_cycles = days / DAYS_PER_400Y;
    remdays = days % DAYS_PER_400Y;
    if(remdays < 0) {
        remdays += DAYS_PER_400Y;
        qc_cycles--;
    }

    c_cycles = remdays / DAYS_PER_100Y;
    if(c_cycles == 4)
        c_cycles--;
    remdays -= c_cycles * DAYS_PER_100Y;

    q_cycles = remdays / DAYS_PER_4Y;
    if(q_cycles == 25)
        q_cycles--;
    remdays -= q_cycles * DAYS_PER_4Y;

    remyears = remdays / 365;
    if(remyears == 4)
        remyears--;
    remdays -= remyears * 365;

    leap = !remyears && (q_cycles || !c_cycles);
    yday = remdays + 31 + 28 + leap;
    if(yday >= 365+leap)
        yday -= 365+leap;

    years = remyears + 4*q_cycles + 100*c_cycles + 400LL*qc_cycles;

    for(months=0; days_in_month[months] <= remdays; months++)
        remdays -= days_in_month[months];

    if(months >= 10) {
        months -= 12;
        years++;
    }

    time->year = years + 2000;
    time->month = months + 3;
    time->day = remdays + 1;
    time->hour = remsecs / 3600;
    time->min = remsecs / 60 % 60;
    time->sec = remsecs % 60;
}
