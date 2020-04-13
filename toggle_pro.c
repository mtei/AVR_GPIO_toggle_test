/* Pro Micro   PORTB, PINB  bit 1,2,3,4,5,6 toggle test */

//          ====
//  +-------====------+
//  | d3/txo      raw |
//  | d2/rxi      gnd |
//  | gnd         rst |
//  | gnd         vcc |
//  | d1/sda       f4 |
//  | d0/scl       f5 |
//  | d4           f6 |
//  | c6           f7 |
//  | d7       sck/B1 |
//  | e6      miso/B3 |
//  | B4      mosi/B2 |
//  | B5           B6 |
//  +-----------------+

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define DELAY _delay_ms

void run(void)
{
    PORTB = 0;
    for( uint8_t i = 2; i < 0x80; i <<= 1 ) {
        _delay_ms(100); PORTB = i;
    }
    _delay_ms(250); PORTB = 0;
    for( uint8_t i = 0x40; i > 1; i >>= 1 ) {
        _delay_ms(100); PORTB = i;
    }
    _delay_ms(250);
}

void bit3_toggle_1(void)
{
    PINB |= (1<<3);
    // with -O1  OK
    //    00000132 <bit3_toggle_1>:
    //     132:   1b 9a           sbi     0x03, 3 ; 3
    //     134:   08 95           ret
    // with -O0  NG
}

void bit3_toggle_2(void)
{
    PINB = (1<<3);
    // with -O1  OK
    //    00000136 <bit3_toggle_2>:
    //     136:   88 e0           ldi     r24, 0x08       ; 8
    //     138:   83 b9           out     0x03, r24       ; 3
    //     13a:   08 95           ret
    // with -O0  OK
}

void bit3_toggle_3(void)
{
    PORTB ^= (1<<3);
    // with -O1  OK
    //    0000013c <bit3_toggle_3>:
    //     13c:   85 b1           in      r24, 0x05       ; 5
    //     13e:   98 e0           ldi     r25, 0x08       ; 8
    //     140:   89 27           eor     r24, r25
    //     142:   85 b9           out     0x05, r24       ; 5
    //     144:   08 95           ret
    // with -O0  OK
}

// RESULT ********************************************
//   |                  | optimize on | optimize off | object size (optimize on) |
//   | PINB |= (1<<3)   |    OK       |     NG       |    1 step                 |
//   | PINB  = (1<<3)   |    OK       |     OK       |    2 step                 |
//   | PORTB ^= (1<<3)  |    OK       |     OK       |    4 step                 |

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
    DDRB = 0x7E; // bit 6,5,5  3,2,1
    while(1) {
        run();
        PORTB = 0x7E;
        bit3_toggle_test();
        PORTB = 0x42;
        bit3_toggle_test();
    }
}
