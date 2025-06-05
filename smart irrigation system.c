#include<pic.h>
#define _XTAL_FREQ 20000000

#define rs RB3
#define en RB4

#define forward RB0
#define reverse RB1
#define enable  RB2

#define red   RC0
#define green RC1

int adc,adcval;

void cmd(unsigned char a);
void data(unsigned char a);
void lcd();
void string (const unsigned char *str);

void delay(int n);

void main()
{
	GIE = 1;
	PEIE = 1;
	ADIE = 1;

	TRISB = 0x00;
	TRISD = 0x00;
	TRISC = 0x00;
	PORTB = PORTD = PORTC = 0x00;

	lcd();
	cmd(0xc0);
	string("MOISTURE :");
	
	ADCON0 = 0x41; // Fosc/8 ,AN0 selected, ADC ON
	ADCON1 = 0x8E; // right adjusted,AN0 analog
	
	forward = reverse = enable = 0;
	green = red =0;

	while(1)
{
	ADCON0 |= 0x04; // ADC start conversion
	while(ADCON0 &0x04);
	adc =  ((unsigned int)ADRESH <<8) + ADRESL;
	cmd(0xca);
	adcval = adc;
    data((adcval/1000)+48);
    data(((adcval/100)%10)+48);
    data(((adcval/10)%10)+48);
    data((adcval%10)+48);
	if (adc <= 500)
{
	forward = 1;
	enable  = 1;
	red     = 1;
	green   = 0;
	cmd(0x80);
	string("MOTOR ON ");
	delay(500);
}else
{
	forward = 0;
	enable  = 0;
	red     = 0;
	green   = 1;
	cmd(0x80);
	string("MOTOR OFF");
}
}
}

void data(unsigned char a)
{
	PORTD = a;
	rs = 1;
	en = 1;
	delay(1);
	en = 0; 
}

void cmd(unsigned char a)
{
	PORTD = a;
	rs = 0;
	en = 1;
	delay(1);
	en = 0;
}

void string(const unsigned char *str)
{
	while(*str)
{
	data(*str);
	str++;
}
}

void lcd()
{
	cmd(0x38);
	cmd(0x06);
	cmd(0x0c);
	cmd(0x01);
}

void delay(int n)
{
	for(int i=0;i<n;i++)
{
	for (int j=0;j<1000;j++);
}
}