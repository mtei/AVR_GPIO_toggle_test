/* Arduino UNO   PORTD, PIND  bit 2,3,4,5,6,7 toggle test */

// Arduinoe UNO
// DIGITAL 2 --> PD2
// DIGITAL 3 --> PD3
// ......
// DIGITAL 6 --> PD6
// DIGITAL 7 --> PD7
// http://i.stack.imgur.com/b3YjT.jpg

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void run(void)
{
    PORTD = 0;
    for( uint8_t i = 4; i != 0; i <<= 1 ) {
        _delay_ms(100); PORTD = i;
    }
    _delay_ms(250); PORTD = 0;
    for( uint8_t i = 0x80; i != 2; i >>= 1 ) {
        _delay_ms(100); PORTD = i;
    }
    _delay_ms(250);
}

void bit3_toggle_1(void)
{
    PIND |= (1<<5);
    // with -O1  OK
    //    000000ee <bit3_toggle_1>:
    //      ee:   4d 9a           sbi     0x09, 5 ; 9
    //      f0:   08 95           ret
    // with -O0  NG
}

void bit3_toggle_2(void)
{
    PIND = (1<<5);
    // with -O1  OK
    //    000000f2 <bit3_toggle_2>:
    //      f2:   80 e2           ldi     r24, 0x20       ; 32
    //      f4:   89 b9           out     0x09, r24       ; 9
    //      f6:   08 95           ret
    // with -O0  OK
}

void bit3_toggle_3(void)
{
    PORTD ^= (1<<5);
    // with -O1  OK
    //    000000f8 <bit3_toggle_3>:
    //      f8:   8b b1           in      r24, 0x0b       ; 11
    //      fa:   90 e2           ldi     r25, 0x20       ; 32
    //      fc:   89 27           eor     r24, r25
    //      fe:   8b b9           out     0x0b, r24       ; 11
    //     100:   08 95           ret
    // with -O0  OK
}

// RESULT ********************************************
//   |                  | optimize on | optimize off | object size (optimize on) |
//   | PIND |= (1<<5)   |    OK       |     NG       |    1 step                 |
//   | PIND  = (1<<5)   |    OK       |     OK       |    2 step                 |
//   | PORTD ^= (1<<5)  |    OK       |     OK       |    4 step                 |

void bit3_toggle_test(void)
{
    for( uint8_t i = 0; i < 8; i++ ) {
        _delay_ms(500);
#ifdef TEST_1
        bit3_toggle_1();
#endif
#ifdef TEST_2
        bit3_toggle_2();
#endif
#ifdef TEST_3
        bit3_toggle_3();
#endif
    }
    _delay_ms(500);
}

int main(void)
{
    DDRB = 0xFC;
    while(1) {
        run();
        PORTD = 0xFC;
        bit3_toggle_test();
        PORTD = 0x84;
        bit3_toggle_test();
    }
}

