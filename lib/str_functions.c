/*
 * str_functions.c
 *
 *  Created on: 08.03.2014
 *      Author: jbandener
 */

//#include <signal.h>

static unsigned char out_array[8];
static unsigned char hex_array[5];
static unsigned char character;
/*
 * creates an 8 digit char array for printing an integer number on the pollin display
 * */


unsigned char *int2charArray(unsigned int integer_value) {
	unsigned int tmp_counter = 0;
	unsigned char tmp_out[8];
	unsigned int tmp;
	unsigned int toConvert = 0;
	*out_array = "";

	tmp = integer_value;
	if(integer_value > 0) {
		while(tmp > 0){
			toConvert = tmp%10;
			tmp = (tmp-toConvert)/10;
			tmp_out[tmp_counter] = 48+toConvert; // for numbers 0 to 9
			if (tmp != 0) {
				tmp_counter = tmp_counter + 1;
			}
		}
	}
	else {
		out_array[0] = "0";
	}

	// resort array
	unsigned int i = 0;
	while(i<=(tmp_counter)){
		out_array[i] = tmp_out[tmp_counter-i];
		i++;
	}

	return out_array;
}

unsigned char getHexDigit(unsigned int integer_value) {
	if(integer_value==0) character='0';
	else if(integer_value==1) character='1';
	else if(integer_value==2) character='2';
	else if(integer_value==3) character='3';
	else if(integer_value==4) character='4';
	else if(integer_value==5) character='5';
	else if(integer_value==6) character='6';
	else if(integer_value==7) character='7';
	else if(integer_value==8) character='8';
	else if(integer_value==9) character='9';
	else if(integer_value==10) character='A';
	else if(integer_value==11) character='B';
	else if(integer_value==12) character='C';
	else if(integer_value==13) character='D';
	else if(integer_value==14) character='E';
	else if(integer_value==15) character='F';
	else {
		character='?';
	}
	return character;
}

unsigned char *int2HEXcharArray(unsigned int integer_value) {
	unsigned char tmp;
	//hex_array=" ";
	int tmp_int;
	hex_array[0]='0';
	hex_array[1]='x';
	tmp_int=((integer_value-(integer_value%16)))/16;
	hex_array[2]= getHexDigit(tmp_int);
	tmp_int = integer_value%16;
	hex_array[3] = getHexDigit(tmp_int);


	return hex_array;
}
