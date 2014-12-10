
#ifndef IIR_FILTERS_H
#define IIR_FILTERS_H


/* Second order low pass filter 300 Hz */
const signed int IIR_lp300[6]  = {    13,     13,    13, 
                                            32767, -31857,  30997 };

/* Second order high pass filter 300 Hz */
const signed int IIR_hp300[6]  = { 31870, -31870, 31870, 
                                             32767, -31857, 30997   };

#endif
