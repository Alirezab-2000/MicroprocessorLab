#include <avr/io.h>
#include <avr/delay.h>

int get_digit(int number , int timer0status);
void enable_display(int timer0status);
void display7seg(int num);

int main() {
    DDRD = 0xFF;
    DDRC = 0xFF;

    PORTC = 0xFF;
    PORTD = 0x00;

    TCCR1B = (1 << CS12);

    TCCR0 = (1 << CS00) | (1 << CS02);
    TCNT0 = 0;

    int number = 0;
    int digit = 0;
    int timer0Status = 0;
    display7seg(number);

    while (1) {
       timer0Status = TCNT0 % 4;
       digit = get_digit(number , timer0Status);

       enable_display(timer0Status);
       display7seg(digit);

       if(TCNT1 > 2000 && number <= 9999){
            TCNT1 = 0;
            number = number + 1; 
        }
           
    }
    return 0;
}
int get_digit(int number , int timer0status){
    int result = number;

    if(timer0status == 0) return result % 10;
    else if(timer0status == 1) return (result / 10) %10;
    else if(timer0status == 2) return (result / 100) %10;
    else return ( result / 1000 ) %10;
}
void enable_display(int timer0status){
    switch (timer0status)
    {
    case 3:
        PORTC = 0x0E;
        break;
    case 2:
        PORTC = 0x0D;
        break;    
    case 1:
        PORTC = 0x0B;
        break;
    case 0:
        PORTC = 0x07;
        break;

    default:
        PORTC = 0x00;
        break;
    }
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