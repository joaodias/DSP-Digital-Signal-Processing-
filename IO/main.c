#include "stdio.h"
#include "conf.h"

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

 	while(1)
 	{

    //sine wave from 50 Hz to 8kHz
     if(i<8000){
    	left_output =  generate_sinewave_L(i, 10000);
    	right_output = generate_sinewave_R(i, 10000);
    	i++;
     }
     else{
    	 i=50;
    	 left_output =  generate_sinewave_L(i, 10000);
    	 right_output = generate_sinewave_R(i, 10000);
    	 i++;
     }
     //write to the output
     codec_write(left_output, right_output);
 	}
}

