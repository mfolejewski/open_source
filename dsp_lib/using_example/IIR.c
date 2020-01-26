//-----------------------------------------------------------------------------
#include "IIR.h"
//-----------------------------------------------------------------------------
//IIR0 filter coefficients
const unsigned long iir_coeff_0[] = {IIR0_GAIN, IIR0_COEFF_B1, IIR0_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR1 filter coefficients
const unsigned long iir_coeff_1[] = {IIR1_GAIN, IIR1_COEFF_B1, IIR1_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR2 filter coefficients
const unsigned long iir_coeff_2[] = {IIR2_GAIN, IIR2_COEFF_B1, IIR2_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR3 filter coefficients
const unsigned long iir_coeff_3[] = {IIR3_GAIN, IIR3_COEFF_B1, IIR3_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR4 filter coefficients
const unsigned long iir_coeff_4[] = {IIR4_GAIN, IIR4_COEFF_B1, IIR4_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR5 filter coefficients
const unsigned long iir_coeff_5[] = {IIR5_GAIN, IIR5_COEFF_B1, IIR5_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR6 filter coefficients
const unsigned long iir_coeff_6[] = {IIR6_GAIN, IIR6_COEFF_B1, IIR6_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR7 filter coefficients
const unsigned long iir_coeff_7[] = {IIR7_GAIN, IIR7_COEFF_B1, IIR7_COEFF_B2};
//-----------------------------------------------------------------------------
//IIR: single section buffer
unsigned long iir_buffer_0[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
unsigned long iir_buffer_1[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
unsigned long iir_buffer_2[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
unsigned long iir_buffer_3[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
//-----------------------------------------------------------------------------
