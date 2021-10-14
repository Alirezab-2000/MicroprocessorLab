#include <avr/io.h>
#include <avr/delay.h>

void display7seg(int num);

int main() {
    DDRD = 0xFF;
    DDRC = 0xFF;

    PORTC = 0x00;
    PORTD = 0x00;

    TCCR1B = (1 << CS12);

    TCCR0 = (1 << CS00) | (1 << CS02);
    TCNT0 = 0;

    int number = 0;
    display7seg(number);

    while (1) {
       PORTC = 1 << (TCNT0 % 4); 
       if(TCNT1 > 5000 && number < 9){
            TCNT1 = 0;
            number = number + 1; 
            display7seg(number);   
        }
    }
    return 0;
}

void display7seg(int num){
    switch (num)
    {
    case 0:
        PORTD = 0x3F;
        break;
    case 1:
        PORTD = 0x06;
        break;
    case 2:
        PORTD = 0x5B;
        break;
    case 3:
        PORTD = 0x4F;
        break;
    case 4:
        PORTD = 0x66;
        break;
    case 5:
        PORTD = 0x6D;
        break;
    case 6:
        PORTD = 0x7D;
        break;
    case 7:
        PORTD = 0x07;
        break;
    case 8:
        PORTD = 0x7F;
        break;    
    case 9:
        PORTD = 0xEF;
        break; 
    default:
        PORTD = 0x00;
        break;
    }
}