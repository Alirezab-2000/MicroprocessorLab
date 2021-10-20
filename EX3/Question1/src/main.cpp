#include <avr/io.h>
#include <avr/delay.h>

void display7seg(int num);

int main() {
    DDRC = 0xFF;
    PORTC = 0x00;

    TCCR1B = (1 << CS12);
    int number = 0;
    display7seg(number);

    while (1) {
        if(TCNT1 > 5000){
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
        PORTC = 0x3F;
        break;
    case 1:
        PORTC = 0x06;
        break;
    case 2:
        PORTC = 0x5B;
        break;
    case 3:
        PORTC = 0x4F;
        break;
    case 4:
        PORTC = 0x66;
        break;
    case 5:
        PORTC = 0x6D;
        break;
    case 6:
        PORTC = 0x7D;
        break;
    case 7:
        PORTC = 0x07;
        break;
    case 8:
        PORTC = 0x7F;
        break;    
    case 9:
        PORTC = 0x67;
        break; 
    default:
        PORTC = 0x00;
        break;
    }
}