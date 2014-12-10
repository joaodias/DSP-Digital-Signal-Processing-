
/* Numerator coefficients */
#define B0 0
#define B1 1
#define B2 2

/* Denominator coefficients */
#define A0 3
#define A1 4
#define A2 5

//IIR direct form II implementation with two stages

signed int IIR_direct_form_II ( const signed int * coefficients, signed int input){
  long temp;
  static short int delay[2][3] = { 0, 0, 0, 0, 0, 0};
  unsigned int stages;

  /* Copy input to temp for temporary storage */

  temp = (long) input; 

  for ( stages = 0 ; stages < 2 ; stages++)
    {
      /* Process denominator coefficients */

     delay[stages][0] = (signed int) temp;

     temp = (( (long) coefficients[A0] * delay[stages][0] ) >> 7); /* Divide by 128 */
  
     temp -= ( (long) coefficients[A1] * delay[stages][1] );  /* A1/2 */

     temp -= ( (long) coefficients[A1] * delay[stages][1] );  /* A1/2 */  

     temp -= ( (long) coefficients[A2] * delay[stages][2] );
  
     temp >>= 15;  /* Divide temp by coefficients[A0] */

     if ( temp > 32767)
       {
         temp = 32767;
       }
     else if ( temp < -32767)
       {
         temp = -32767;
       }  

     delay[stages][0] = ( signed int ) temp;

     /* Process numerator coefficients */

     temp = ((long) coefficients[B0] * delay[stages][0] );

     temp += ((long) coefficients[B1] * delay[stages][1] ) ;  /* B1/2 */ 

     temp += ((long) coefficients[B1] * delay[stages][1] ) ;  /* B1/2 */

     temp += ((long) coefficients[B2] * delay[stages][2] ) ;  

     delay[stages][2] = delay[stages][1];
     delay[stages][1] = delay[stages][0];

     /* Divide temp by coefficients[A0] then multiply by 128 */

     temp >>= ( 15 - 7 );

     /* Range limit temp between maximum and minimum */

     if ( temp > 32767)
       {
         temp = 32767;
       }
     else if ( temp < -32767)
       {
         temp = -32767;
       }  

     /* Temp will be fed into input of filter next time through */
    }

  return ( (short int) temp ); 
}

