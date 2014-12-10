/****************************************************************
*						Configuration Header
****************************************************************/

#ifndef CONF_H
#define CONF_H

/****************************************************************
*				      Some Data types macros
****************************************************************/
#define Uint32  unsigned long
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

#define GPIO26  0x1A

/****************************************************************
*				        System Registers
****************************************************************/
#define EBSR      		    *(volatile ioport Uint16*)(0x1c00)
#define PCGCR1         		*(volatile ioport Uint16*)(0x1c02)
#define PCGCR2         		*(volatile ioport Uint16*)(0x1c03)
#define PSRCR         		*(volatile ioport Uint16*)(0x1c04)
#define PSRCR       		*(volatile ioport Uint16*)(0x1c05)
#define ODSCR		       	*(volatile ioport Uint16*)(0x1c16)
#define PDINHIBR1        	*(volatile ioport Uint16*)(0x1c17)

/****************************************************************
*				      	GPIO Registers
****************************************************************/
#define IODIR1      		*(volatile ioport Uint16*)(0x1c06)
#define IODIR2          	*(volatile ioport Uint16*)(0x1c07)
#define IOINDATA1		   	*(volatile ioport Uint16*)(0x1c08)
#define IOINDATA2		   	*(volatile ioport Uint16*)(0x1c09)
#define IODATAOUT1		  	*(volatile ioport Uint16*)(0x1c0a)
#define IODATAOUT2  		*(volatile ioport Uint16*)(0x1c0b)

/****************************************************************
*				      	I2C Registers
****************************************************************/
#define IER    	       		*(volatile ioport Uint16*)(0x1A04)
#define STR    	       		*(volatile ioport Uint16*)(0x1A08)
#define CLKL           		*(volatile ioport Uint16*)(0x1A0C)
#define CLKH           		*(volatile ioport Uint16*)(0x1A10)
#define CNT    		   		*(volatile ioport Uint16*)(0x1A14)
#define DRR    		   		*(volatile ioport Uint16*)(0x1A18)
#define SAR    	       		*(volatile ioport Uint16*)(0x1A1C)
#define DXR    	       		*(volatile ioport Uint16*)(0x1A20)
#define MDR            		*(volatile ioport Uint16*)(0x1A24)
#define EDR    	       		*(volatile ioport Uint16*)(0x1A2C)
#define PSC    	       		*(volatile ioport Uint16*)(0x1A30)

/***************************************************************
*				      	I2S Registers
****************************************************************/
#define CR            		*(volatile ioport Uint16*)(0x2800)
#define SRGR          		*(volatile ioport Uint16*)(0x2804)
#define W0_LSW_W      		*(volatile ioport Uint16*)(0x2808)
#define W0_MSW_W      		*(volatile ioport Uint16*)(0x2809)
#define W1_LSW_W      		*(volatile ioport Uint16*)(0x280C)
#define W1_MSW_W      		*(volatile ioport Uint16*)(0x280D)
#define IR            		*(volatile ioport Uint16*)(0x2810)
#define ICMR          		*(volatile ioport Uint16*)(0x2814)
#define W0_LSW_R      		*(volatile ioport Uint16*)(0x2828)
#define W0_MSW_R      		*(volatile ioport Uint16*)(0x2829)
#define W1_LSW_R      		*(volatile ioport Uint16*)(0x282C)
#define W1_MSW_R      		*(volatile ioport Uint16*)(0x282D)

/****************************************************************
*				      		  FFT
****************************************************************/
#define FFTPOINTS 512

/****************************************************************
*				      Function Prototypes
****************************************************************/
typedef struct {

	float real; //real part of the compex number
	float imag; //imaginary part of the compex number
}complex;

void GPIO_direction( Uint16 number, Uint16 direction );
void GPIO_dataout( Uint16 number, Uint16 output );

void codec_write(Int16 left_output, Int16 right_output);
void wait(Uint32 delay);
void waitusec(Uint32 usec);
signed int singen1( signed short int frequency, signed short int amplitude);
signed int singen2( unsigned short int frequency, unsigned short int amplitude);  

unsigned long Sampling(unsigned long SamplingFrequency, unsigned long ADCgain);
Int16 codec_rset( Uint16 regnum, Uint16 regval );
Int16 I2C_write(Uint16 i2c_addr, Uint8* data, Uint16 len);
void I2C_reset(void);
void I2C_init(void);

void wait_buffer(void);
void init_buffer(complex *p);
void free_buffer(complex *p);
void rotate_buffer(void);


#endif
