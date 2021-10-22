#include <avr/io.h>
#include <util/atomic.h>
#include <avr/delay.h>
#include <stdio.h>
#include "lcd.h"

unsigned int overflows_count = 0;

unsigned int second = 0;
unsigned int minute = 0;
unsigned int hour = 0;

char time[10] = "";

void show_time()
{
    LCD_cmd(0x01); //clear lcd
    sprintf(time, "time: %d",second);
    LCD_Write_String(time);
}
void decrease_second()
{
    second--;

    if (second == -1)
    {
        PORTA = 0x01;

        LCD_cmd(0x01);
        LCD_Write_String("....END....");
    }
}
void change_timer_status(bool isStop)
{
    if (isStop)
    {
        TCCR1B &= ~(1 << CS12);
        TCCR1B &= ~(1 << CS11);
        TCCR1B &= ~(1 << CS10);
    }
    else
    {
        TCCR1B &= ~(0 << CS10);
    }
}





ISR(INT0_vect) //External interrupt_zero ISR
{
    if(TCCR1B && 0x01 == 0){
        change_timer_status(false);
    }else{
        change_timer_status(true);
    }
    
}
ISR(INT1_vect) //External interrupt_one ISR
{
    second = second + 1;

    show_time();
}
ISR(TIMER1_OVF_vect)
{
    overflows_count++;

    if (overflows_count % 15 == 0)
    {
        show_time();
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

    show_time();   // show the 0 time

    TIMSK = (1 << TOIE1); // enable timer2 overflow
    // TCCR1B = (1 << CS10); // set timer2 prescaler to clock (no prescaling)

    GICR |= (1 << INT0) | (1 << INT1);    //Enable External Interrupts INT0 and INT1
    MCUCR |= (1 << ISC00) | (1 << ISC01); // set INT0 to trigger the intrupt on rasing edge
    MCUCR |= (1 << ISC11) | (1 << ISC10); // set INT1 to trigger the intrupt on rasing edge

    sei(); // enable all intrupts

    while (1)
    {
    }
    return 0;
}