#include "stdio.h"
#include "conf.h"

#define SAMPLES_PER_SECOND 48000
#define ADCgain 30

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;
unsigned int i = 50;

//FFT related
complex input[FFTPOINTS];
complex intermediate[FFTPOINTS];
complex output[FFTPOINTS];
Int16 index = 0;


void main( void ) 
{
	I2C_init();
	Sampling(SAMPLES_PER_SECOND, ADCgain);
	init_buffer(input);
    init_buffer(intermediate);
    init_buffer(output);

	while(1){

	 left_input = generate_sinewave_L(1000, 10000);
	 wait_buffer();
	 rotate_buffer();
	 fft(intermediate, 512);

	 while(index != 0){
		 right_input = output[index].real;
		 left_output = left_input;
		 right_output = right_input;
		 codec_write(left_output, right_output);
		 index++;
	 }
	 index = 0;
	}
}

//fill input buffer
void wait_buffer(void){
	while(index != FFTPOINTS){
		input[index].real = left_input;
		input[index].imag = 0.0;
		index++;
	}
	index = 0;
}

//rotate buffers
void rotate_buffer(void){
	memcpy(input, output, sizeof(complex*));
	memcpy(intermediate, input, sizeof(complex*));
	memcpy(output, intermediate, sizeof(complex*));
}

