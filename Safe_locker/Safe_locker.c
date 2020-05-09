/*
 * Safe_locker.c
 *
 * Created: 03/05/2020 5:43:27 AM
 *  Author: Mohamed Elsayed
 */ 


#include "LCD.h"
#include "EEPROM.h"
#include "keypad.h"
uint8_t d1,d2,d3,d4; // password digits
uint8_t c1,c2,c3,c4;
uint8_t n1,n2,n3,n4; // new password
uint8_t state=0;
uint8_t count=0;
uint8_t get_password (uint8_t state);
int main(void)
{
	SETBIT(DDRC,4);
	LCD_init();
	keypad_init();
	LCD_write_command(0x1);
	LCD_write_string("Ur 4-digit pass.");
	_delay_ms(500);
	uint8_t check_state;
	while(1){
	d1=EEPROM_read(10); // first digit of stored password 
	d2=EEPROM_read(11); // second digit of stored password 
	d3=EEPROM_read(12); // third digit of stored password 
	d4=EEPROM_read(13);	// forth digit of stored password  
	check_state=get_password (state);
	state =check_state;
	
	}	
	}
	
uint8_t get_password (uint8_t state)
{
	
	
	int8_t key=keypad_read();
	if(key != -1 && key!='N'){
		switch (count)
		{
			case 0 :
			LCD_write_command(0xc3);
			LCD_write_char(key);
			LCD_write_char('.');
			_delay_ms(200);
			c1=key-48;   // first digit of entered password from user
			count++;
			break;
			case 1 :
			LCD_write_char(key);
			LCD_write_char('.');
			c2=key-48;  // second digit of entered password from user
			_delay_ms(200);
			count++;
			break;
			case 2 :
			LCD_write_char(key);
			LCD_write_char('.');
			c3=key-48;   // third digit of entered password from user
			_delay_ms(200);
			count++;
			break;
			case 3 :
			LCD_write_char(key);
			c4=key-48;  // forth digit of entered password from user
			_delay_ms(200);
			if (state==0 && (c1==d1&&c2==d2&&c3==d3&&c4==d4))
			{	LCD_write_command(0x1);
				LCD_write_string("o to open");
				LCD_write_command(0xc0);
				LCD_write_string("c to change");
				count++;
			}
			else if (state==0 &&(c1!=d1||c2!=d2||c3!=d3||c4!=d4)){
				LCD_write_command(0x1);
				LCD_write_string("wrong try again");
				count=0;
			}
			else if (state==1){
				 n1=c1;
				 n2=c2;
				 n3=c3;
				 n4=c4;
				 LCD_write_command(0x1);
				 LCD_write_string("plz confirm pass");
				 state=2;
				 count=0;
				 
			 }
			 else if (state==2 && (c1==n1&&c2==n2&&c3==n3&&c4==n4))
			 {
				  /*to change stored value of password with new one*/
				 EEPROM_write(10,n1); 
				 EEPROM_write(11,n2);
				 EEPROM_write(12,n3);
				 EEPROM_write(13,n4);
				 LCD_write_command(0x1);
				 LCD_write_string("password changed");
				 _delay_ms(500);
				 LCD_write_command(0x1);
				 LCD_write_string("Ur 4-digit pass.");
				 state=0;
				 count=0;
				 
			 }
			 else if (state==2 && (c1!=n1||c2!=n2||c3!=n3||c4!=n4)){
				 LCD_write_command(0x1);
				 LCD_write_string("wrong try again");
				 state=1;
				 count=0;
			 }

			break;
			case 4 :
			if(key=='C'){
				LCD_write_command(0x1);
				LCD_write_string("write new pass");
				state=1;
				_delay_ms(200);
				count=0;}
			else if (key=='O'){
				SETBIT(PORTC,4);
				LCD_write_command(0x1);
				LCD_write_command(0x82);
				LCD_write_string("safe is open");
				_delay_ms(1000);
				CLRBIT(PORTC,4);
				LCD_write_command(0x1);
				LCD_write_string("ur 4-digit pass.");
				count=0;}
			else{
			LCD_write_command(0x1);
			LCD_write_string("Error retry");
			_delay_ms(200);
				count=0;}
			break;		
			
						
}	
		}
		return state;
			}		