#include "stdio.h"
#include "conf.h"
#include "IIR_filters.h"

#define SAMPLES_PER_SECOND 48000
#define ADCgain 20

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;
unsigned int i=50;

void main( void ) 
{

    I2C_init();
    Sampling(SAMPLES_PER_SECOND, ADCgain);
   
 	while(1)
 	{
 	 left_input = 	generate_sinewave_L(2000, 10000);
     right_input = generate_sinewave_R(2000, 10000);

     left_output = IIR_direct_form_II(&IIR_hp300[0], left_input);
     right_output = IIR_direct_form_II(&IIR_hp300[0], right_input);

     codec_write(left_output, right_output);

 	}
}

