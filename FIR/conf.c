#include "stdio.h"
#include "conf.h"
#include "tms320.h"
#include "dsplib.h"

Int16 counter1; // Counters for monitoring real-time operation.
Int16 counter2;


/****************************************************************
*                       Write Data to Codec
*****************************************************************/
void codec_write(Int16 left_output, Int16 right_output){

    counter2 = 0;
    
    while( !(IR & 0x20) )
    {
        counter2++; // Wait for transmit interrupt
    }   
    W0_MSW_W = left_output;         // Left output       
    W0_LSW_W = 0;
    W1_MSW_W = right_output;        // Right output
    W1_LSW_W = 0;
}

/****************************************************************
*                    Simple delay functions
*****************************************************************/
void wait( Uint32 delay ){
    volatile Uint32 i;
    for ( i = 0 ; i < delay ; i++ ){ };
}

void waitusec( Uint32 usec ){
    wait( (Uint32)usec * 8 );
}

/****************************************************************
*              Generate a sine wave (using TI's dsplib)
*****************************************************************/
signed int generate_sinewave_R( signed short int frequency, signed short int amplitude){
 
 short int sinusoid;
 signed long result;
 static short int count = 0;
 
 /* Multiply frequency by scaling factor of 32767 / 48000 */
 
 result = ( (long)frequency * 22368 ) >> 14 ;
 
 if ( result > 32767)
   {
     result = 32767; /* Maximum value for highest frequency */
   }
 else if ( 0 == result)  
   {
     result = 1;     /* Minimum value for lowest fequency */
   }
 else if ( result < -32767)
   {
     result = -32767;
   }  
 
 count += (short int) result; 
 
 /* Obtain sine of input */

 sine ( &count, &sinusoid, 1);

 if ( amplitude > 32767 )
   {
     amplitude = 32767; /* Range limit amplitude */
   } 

 /* Scale sine wave to have maximum value set by amplitude */

 result =  ( (long) sinusoid * amplitude ) >> 15;
 
 return ( (signed int ) result );
}

signed int generate_sinewave_L( signed short int frequency, signed short int amplitude){
 
 short int sinusoid;
 signed long result;
 static short int count = 0;
 
 /* Multiply frequency by scaling factor of 32767 / 48000 */
 
 result = ( (long)frequency * 22368 ) >> 14 ;
 
 if ( result > 32767)
   {
     result = 32767; /* Maximum value for highest frequency */
   }
 else if ( 0 == result)  
   {
     result = 1;     /* Minimum value for lowest fequency */
   }
 else if ( result < -32767)
   {
     result = -32767;
   }  
 
 count += (short int) result; 
 
 /* Obtain sine of input */

 sine ( &count, &sinusoid, 1);

 if ( amplitude > 32767 )
   {
     amplitude = 32767; /* Range limit amplitude */
   } 

 /* Scale sine wave to have maximum value set by amplitude */

 result =  ( (long) sinusoid * amplitude ) >> 15;
 
 return ( (signed int ) result );
}

/****************************************************************
*                      Set Sampling Frequency
*****************************************************************/
unsigned long Sampling(unsigned long SamplingFrequency, unsigned long ADCgain){

    unsigned int PLLPR = 0x91; // Default to 48000 Hz   
    unsigned int gain;
    unsigned long output;

    if ( ADCgain >= 48)
     {
      gain = 95;      //  Limit gain to 47.5 dB
      ADCgain = 48;   // For display using printf()
     }
    else 
    {
     gain = (ADCgain << 1); // Convert 1dB steps to 0.5dB steps
    }
     
    switch (SamplingFrequency)
    {
     case 48000:
        PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
        printf("Sampling frequency 48000 Hz Gain = %2d dB\n", ADCgain);
        output = 48000;
     break;

     
     case 6857:
        PLLPR = 0xF1; //1111 0001b. PLL on. P = 7, R = 1.
        printf("Sampling frequency 6857 Hz Gain = %2d dB\n", ADCgain);  
        output = 6857;    
     break;
     

     default:
        PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
        printf("Sampling frequency not recognised. Default to 48000 Hz Gain = %2d dB\n", ADCgain);
        output = 48000;  
     break;
    } 
 
    /* Configure Serial Bus */
    EBSR|= 0x0100;  // Configure Serial bus 0 for I2S0
 
     
    /* Configure codec */ 
    //From codec datasheet -> http://www.eas.uccs.edu/wickert/ece5655/code/codecs_lab/tlv320aic3204.pdf
    // Pag 97

    codec_rset( 0, 0 );      // Select page 0
    codec_rset( 1, 1 );      // Reset codec
    codec_rset( 0, 1 );      // Point to page 1
    codec_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    codec_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    codec_rset( 0, 0 );
    /* PLL and Clocks config and Power Up  */
    codec_rset( 27, 0x1d );  // BCLK and WCLK is set as o/p to codec(Master)
    codec_rset( 28, 0x00 );  // Data ofset = 0
    codec_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    codec_rset( 6, 7 );      // PLL setting: J=7
    codec_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D)
    codec_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D)
    codec_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    codec_rset( 5, PLLPR );   //PLL setting: Power up PLL, P=1 and R=1
    codec_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    codec_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    codec_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    codec_rset( 11, 0x87 );  // Power up NDAC and set NDAC value to 7
    codec_rset( 12, 0x82 );  // Power up MDAC and set MDAC value to 2
    codec_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    codec_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    /* DAC ROUTING and Power Up */
    codec_rset( 0, 1 );      // Select page 1
    codec_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    codec_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    codec_rset( 0, 0 );      // Select page 0
    codec_rset( 64, 2 );     // Left vol=right vol
    codec_rset( 65, 0 );     // Left DAC gain to 0dB VOL; Right tracks Left
    codec_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    codec_rset( 0, 1 );      // Select page 1
    codec_rset( 0x10, 10 );  // Unmute HPL , 10dB gain
    codec_rset( 0x11, 10 );  // Unmute HPR , 10dB gain
    codec_rset( 9, 0x30 );   // Power up HPL,HPR
    codec_rset( 0, 0 );      // Select page 0
    wait( 100 );    // wait
    /* ADC ROUTING and Power Up */
    codec_rset( 0, 1 );      // Select page 1
    codec_rset( 0x34, 0x10 );// STEREO 1 Jack
                               // IN2_L to LADC_P through 0 kohm
    codec_rset( 0x37, 0x10 );// IN2_R to RADC_P through 0 kohmm
    codec_rset( 0x36, 1 );   // CM_1 (common mode) to LADC_M through 0 kohm
    codec_rset( 0x39, 0x40 );// CM_1 (common mode) to RADC_M through 0 kohm
    codec_rset( 0x3b, gain );   // MIC_PGA_L unmute
    codec_rset( 0x3c, gain );   // MIC_PGA_R unmute
    codec_rset( 0, 0 );      // Select page 0
    codec_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    codec_rset( 0x52, 0 );   // Unmute Left and Right ADC
    
    codec_rset( 0, 0 );    
    wait( 100 );  // Wait
    
    /* I2S settings */
    SRGR = 0x0;     
    CR = 0x8010;    // 16-bit word, slave, enable I2C
    ICMR = 0x3f;    // Enable interrupts

    return(output);
}

/****************************************************************
*           This function builds a vector with address, 
*               data and its leght to write via I2C
*****************************************************************/
Int16 codec_rset( Uint16 regnum, Uint16 regval ){

    Uint8 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Device Address
    cmd[1] = regval;                // 8-bit Register Data

    return I2C_write(0x18, cmd, 2);
}

/****************************************************************
*                       Write data I2C
*****************************************************************/
Int16 I2C_write(Uint16 i2c_addr, Uint8* data, Uint16 len){

    Int32 timeout, i;

        CNT = len;                    // Set length
        SAR = i2c_addr;               // Set I2C slave address
        MDR = 0x2000                  // Set for Master Write
                  | 0x0200
                  | 0x0400
                  | 0x0020
                  | 0x4000;

        wait(10);              // Short delay

        for ( i = 0 ; i < len ; i++ )
        {
            DXR = data[i];            // Write

            timeout = 0x7fff;
            do
            {
                if ( timeout-- < 0  )
                {
                    I2C_reset();
                    return -1;
                }
            } while ( ( STR & 0x0010 ) == 0 );// Wait for Tx Ready
        }

        MDR |= 0x0800;             // Generate STOP

        waitusec(100);
}

/****************************************************************
*                          Reset I2C
*****************************************************************/
void I2C_reset(void){
    MDR = 0;
    I2C_init();
}

/****************************************************************
*                      Inititalize I2C
*****************************************************************/
void I2C_init(void){

    MDR         = 0x0400;           // Reset I2C
    PSC         = 20;               // Config prescaler for 12MHz
    CLKL        = 20;               // Config clk LOW for 20kHz
    CLKH        = 20;               // Config clk HIGH for 20kHz    
    MDR         = 0x0420;           // Release from reset; Master, Transmitter, 7-bit address
}
