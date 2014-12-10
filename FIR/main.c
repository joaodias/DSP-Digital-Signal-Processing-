#include "stdio.h"
#include "conf.h"
#include "FIR_filters_asm.h"
#include "coeff.h"

#define SAMPLES_PER_SECOND 48000
#define ADCgain 30

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;
unsigned int i = 50;

void main( void ) 
{
	I2C_init();
	Sampling(SAMPLES_PER_SECOND, ADCgain);

	while(1){

     left_input = 	generate_sinewave_L(250, 10000);
     right_input = generate_sinewave_R(250, 10000);

     //filter input
 	 left_output = FIR_filter_asm(&hp[0], left_input);
     right_output = FIR_filter_asm_2(&hp[0], right_input);
     codec_write(left_output, right_output);
	}
}

