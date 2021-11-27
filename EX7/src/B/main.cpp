#include <avr/io.h>
#include <avr/delay.h>
#include <util/atomic.h>
#include <stdio.h>

unsigned int step = 0;
int temp = 0;

uint16_t adc_read(uint8_t ch);
void step_setter();
void rotate_motor(int step_number);

int main()
{
    DDRB = 0x0F;
    DDRA = 0xF0;

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // pre scaler 64 and adc enable

    PORTB = 0x00;

    while (1)
    {
       
    }
    return 0;
}

uint16_t adc_read(uint8_t ch)
{
    ch &= 0b00000111;            
    ADMUX = (ADMUX & 0xF8) | ch; //chooose channel

    ADCSRA |= (1 << ADSC); // start conversation
    
    while (ADCSRA & (1 << ADSC)); // wait for conversion to complete
    
    return (ADC); // read ADCL and ADCW
}
void step_setter()
{
    switch (step)
    {
    case 1:
        PORTB = 0b0100;
        break;
    case 2:
        PORTB = 0b1101;
        break;
    case 3:
        PORTB = 0b1011;
        break;
    case 4:
        PORTB = 0b0010;
        step = 0;
        break;
    default:
        PORTB = 0b0000;
        break;
    }
}
void rotate_motor(int step_number){
    for (int i = 0; i < step_number; i++)
    {
        step++;
        step_setter();
        _delay_ms(500);
    }
    
}