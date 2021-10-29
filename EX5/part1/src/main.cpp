#include <avr/io.h>
#include <util/atomic.h>

unsigned int overflow_cout = 0;
unsigned int step = 0;

void step_setter(int step){
    int mode = step % 8;

    switch (mode)
    {
    case 0:
        PORTA = 0b010;
        break;
    case 1:
        PORTA = 0b0110;
        break;
    case 2:
        PORTA = 0b0100;
        break;
    case 3:
        PORTA = 0b0101;
        break;
    case 4:
        PORTA = 0b1101;
        break;
    case 5:
        PORTA = 0b1001;
        break;
    case 6:
        PORTA = 0b1011;
        break;
    case 7:
        PORTA = 0b1010;
        break; 
    default:
        PORTA = 0b0000;
        break;
    }
}

ISR(TIMER0_OVF_vect){
    if(PINC != 0x04){
        if(overflow_cout % 390 == 0){
            // the Clock is 1MHz and, 390 timer0 overflow with clock prescaler cause 100ms delay
            step_setter(step);

            step++;
        }
    }
    overflow_cout++;
}
int main() {
    DDRC = 0x00;
    DDRA = 0x0F;

    PORTA = 0x00;

    TCCR0 = (1 << CS00);
    TIMSK |= (1 << TOIE0);
  
    sei();
 
    while (1) {
        
    }
    return 0;
}