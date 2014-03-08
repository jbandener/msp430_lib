/*
 * str_functions.c
 *
 *  Created on: 08.03.2014
 *      Author: jbandener
 */

//#include <signal.h>

static unsigned char out_array[8];

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



