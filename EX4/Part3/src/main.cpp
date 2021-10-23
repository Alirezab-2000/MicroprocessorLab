#include <avr/io.h>
#include <util/atomic.h>
#include <avr/delay.h>
#include <stdio.h>
#include "lcd.h"

unsigned int overflows_count = 0;

signed int second = 0;
signed int minute = 0;
signed int hour = 0;

bool alarm_timer = false;

char time[10] = "";

void show_time()
{
    LCD_cmd(0x01); //clear lcd
    sprintf(time, "%d:%d:%d", hour, minute, second);
    LCD_Write_String(time);
}
void decrease_second()
{
    second--;

    if (second == -1)
    {
        minute--;
        second = 59;
    }
    if (minute == -1)
    {
        hour--;
        minute = 59;
    }
    if (hour == -1)
    {
        alarm_timer = false;
        PORTA = 0x01;

        LCD_cmd(0x01);
        LCD_Write_String("....END....");
    }
    else
    {
        show_time();
    }
}

ISR(INT0_vect) //External interrupt_zero ISR
{
    if ((PINC & 0x01) != 0)
    {
        alarm_timer = !alarm_timer;
    }
    else if ((PINC & 0x02) != 0)
    {
        second = second + 1;

        show_time();
    }
    else if ((PINC & 0x04) != 0)
    {
        minute = minute + 1;

        show_time();
    }
    else if ((PINC & 0x08) != 0)
    {
        hour = hour + 1;

        show_time();
    }
}

ISR(TIMER1_OVF_vect)
{
    if (alarm_timer)
    {
        overflows_count++;

        if (overflows_count % 15 == 0)
        {
            decrease_second();
        }
    }
}
int main()
{
    DDRC = 0x00; // start timer

    DDRA = 0x01; // set alarm light (first bit)

    DDRB = 0xFF;   // set LCD data port as output
    DDRD = 0xE0;   // set LCD signals (RS, RW, E) as out put
    init_LCD();    // initialize LCD
    LCD_cmd(0x0C); // display on, cursor off

    show_time(); // show the 0 time

    TIMSK = (1 << TOIE1); // enable timer2 overflow
    TCCR1B = (1 << CS10); // set timer2 prescaler to clock (no prescaling)

    GICR |= 1 << INT0;                    // Enable External Interrupts INT0 and INT1
    MCUCR |= (1 << ISC00) | (1 << ISC01); // set INT0 to trigger the intrupt on rasing edge

    sei(); // enable all intrupts

    while (1)
    {
    }
    return 0;
}