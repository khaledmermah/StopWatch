#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
/* declaring 2 arrays num & ov */
unsigned char num[6]= {0}; // each element in the array used to describe the num in different seven segment
unsigned char ov[5]={0},execute=0; /*each element in the array used to know if over flow was happen \
in this seven segment or not */
ISR (INT0_vect){ // interrupt ser routine of the first interr ( make all 7 seg = 0 )
	num[0]=0;
	num[1]=0;
	num[2]=0;
	num[3]=0;
	num[4]=0;
	num[5]=0;

	TCNT1=0;
}
void INT0_Init(void) // declaring first interrupt (reset button)
{
	DDRD &=~(1<<PD2); // make pin 2 in D input pin
	PORTD |=(1<<2);  //external interrupt
		GICR |= (1<<6); // enable to the interrupt 0
		MCUCR |= (1<<ISC01);
		SREG  |= (1<<7);
} //(isr 2 ( pause the stop watch by make no clock reach to the timer )
// by make CSx =0 (no prescalling)
ISR (INT1_vect){
	   TCCR1B &= 0xF8;

}
void INT1_Init(void) // declaring secound interrupt
{ // the same like INT0 but some diff in data sheet
	DDRD &=~(1<<PD3);
			GICR |= (1<<7);
			MCUCR |=  (1<<ISC11) | (1<<ISC10) ;
			SREG  |= (1<<7);


}
ISR (INT2_vect){ // isr 3 ( resume the stop watch by make the clock reach to the timer again )

	TCCR1B |= (1<<CS10) | (1<<CS11);
}
void INT2_Init(void) // declaring third interrupt
{
	// the same like INT0 but some diff in data sheet
	        DDRB &=~(1<<2);
	        PORTB |= (1<<2);
			GICR |= (1<<5);
			MCUCSR &= ~(1<<ISC2);
			SREG  |= (1<<7);
}
ISR(TIMER1_COMPA_vect){
	execute=1; /* give 1 to this var make the stop watch increase by 1 sec by make if condition
	true in the main */
}

void timer1_Init(void){ //declaring timer 1 with prescaler 64
	TCNT1=0;
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS11);
	OCR1A =15625;
	TIMSK = (1<<4);
}
int main(){ // calling the three interrupts
	    INT0_Init();
		INT1_Init();
		INT2_Init();

	DDRC |= 0x0F; // declaring first 4 ports in C as outpot pins ( decoder )
	PORTC &= 0xF0; // intialy = 0
	DDRA |= 0x3F; // declaring first 6 ports in A as outpot pins ( enables )
	PORTA &= 0xC0; // intialy = 0
	SREG|=(1<<7); //enable iterrupts
	timer1_Init();
while(1){

	if (execute==1){
		if( num[0] != 9){ // first seven segment
				num[0]++;
				PORTC = (PORTC & 0xF0) | (num[0] & 0x0F); //inserting num0 in C
				ov[0]=0;
				}
				else { // over flow happen
					num[0]=0;
					PORTC = (PORTC & 0xF0) | (num[0] & 0x0F) ; //inserting num0 in C
					ov[0]=1;
				}
		if (ov[0]==1) // if over flow happen in the last one increace next seven seg
		{

				if(num[1] != 5){
				num[1]++;
				}
				else { // over flow happen
					num[1]=0;
							ov[1]=1;
			}
		}

			if (ov[1]==1){ // if over flow happen in the last one increace next seven seg

				if(num[2] != 9){
				num[2]++;
				}
				else { // over flow happen
					num[2]=0;
							ov[2]=1;
			}
				ov[1]=0; //reset over flow
		}
			if (ov[2]==1){ // if over flow happen in the last one increace next seven seg

				if(num[3] != 5){
				num[3]++;

				}
				else { // over flow happen
					num[3]=0;
							ov[3]=1;
			}
				ov[2]=0; //reset over flow
		}
			if (ov[3]==1){ // if over flow happen in the last one increace next seven seg

				if(num[4] != 9){
				num[4]++;

				}
				else { // over flow happen
					num[4]=0;
					PORTA = 0x01;

							ov[4]=1;
			}
				ov[3]=0; //reset over flow
		}
		if (ov[4]==1){ // if over flow happen in the last one increace next seven seg

				if(num[5] != 5){
				num[5]++;

				}
				else { // over flow happen
					num[5]=0;
			}
		}
		ov[4]=0; //reset over flow
		execute =0  ;
	}
PORTA = 0x01; //enable first seven seg and disable others
PORTC = ( PORTC & 0xF0 ) | ( num[0] & 0x0F); //inserting num0 in C
_delay_ms(1);
	PORTA = 0x02; //enable sec seven seg and disable others
	PORTC = ( PORTC & 0xF0 ) | ( num[1] & 0x0F); //inserting num1 in C
		_delay_ms(1);

		PORTA = 0x04; //enable third seven seg and disable others
		PORTC = ( PORTC & 0xF0 ) | ( num[2] & 0x0F); //inserting num2 in C
			_delay_ms(1);

			PORTA = 0x08; //enable fourth seven seg and disable others
			PORTC = ( PORTC & 0xF0 ) | ( num[3] & 0x0F); //inserting num3 in C
				_delay_ms(1);

				PORTA = 0x10; //enable fifth seven seg and disable others
				PORTC = ( PORTC & 0xF0 ) | ( num[4] & 0x0F); //inserting num4 in C
					_delay_ms(1);
					PORTA = 0x20; //enable sixth seven seg and disable others
					PORTC = ( PORTC & 0xF0 ) | ( num[5] & 0x0F); //inserting num5 in C
						_delay_ms(1);


}
}



