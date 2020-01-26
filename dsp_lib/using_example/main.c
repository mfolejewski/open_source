//-----------------------------------------------------------------------------
//example of using IIR biquad low-pass and high-pass filters
//-----------------------------------------------------------------------------
#include "IIR.h"
//-----------------------------------------------------------------------------
//coefficients gain table
unsigned long gain_coeff[ADC_CHANNEL_NUM];
//ADC raw data
unsigned long adc_raw_data[ADC_CHANNEL_NUM];
//-----------------------------------------------------------------------------
int main(void)
{

[...]

//low-pass filter, #1
//section F1
iir_lpf((unsigned long *) &iir_coeff_2[0], (unsigned long *) &iir_buffer_0[ADC_CHANNEL_0 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
iir_lpf((unsigned long *) &iir_coeff_2[0], (unsigned long *) &iir_buffer_0[ADC_CHANNEL_1 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
iir_lpf((unsigned long *) &iir_coeff_2[0], (unsigned long *) &iir_buffer_0[ADC_CHANNEL_2 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
iir_lpf((unsigned long *) &iir_coeff_2[0], (unsigned long *) &iir_buffer_0[ADC_CHANNEL_3 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);
//section F2
iir_lpf((unsigned long *) &iir_coeff_3[0], (unsigned long *) &iir_buffer_1[ADC_CHANNEL_0 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
iir_lpf((unsigned long *) &iir_coeff_3[0], (unsigned long *) &iir_buffer_1[ADC_CHANNEL_1 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
iir_lpf((unsigned long *) &iir_coeff_3[0], (unsigned long *) &iir_buffer_1[ADC_CHANNEL_2 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
iir_lpf((unsigned long *) &iir_coeff_3[0], (unsigned long *) &iir_buffer_1[ADC_CHANNEL_3 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);

//low-pass filter, #2
//section F1
iir_lpf((unsigned long *) &iir_coeff_6[0], (unsigned long *) &iir_buffer_2[ADC_CHANNEL_0 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
iir_lpf((unsigned long *) &iir_coeff_6[0], (unsigned long *) &iir_buffer_2[ADC_CHANNEL_1 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
iir_lpf((unsigned long *) &iir_coeff_6[0], (unsigned long *) &iir_buffer_2[ADC_CHANNEL_2 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
iir_lpf((unsigned long *) &iir_coeff_6[0], (unsigned long *) &iir_buffer_2[ADC_CHANNEL_3 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);
//section F2
iir_hpf((unsigned long *) &iir_coeff_7[0], (unsigned long *) &iir_buffer_3[ADC_CHANNEL_0 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
iir_hpf((unsigned long *) &iir_coeff_7[0], (unsigned long *) &iir_buffer_3[ADC_CHANNEL_1 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
iir_hpf((unsigned long *) &iir_coeff_7[0], (unsigned long *) &iir_buffer_3[ADC_CHANNEL_2 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
iir_hpf((unsigned long *) &iir_coeff_7[0], (unsigned long *) &iir_buffer_3[ADC_CHANNEL_3 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);

//high-pass filter, #3
iir_hpf((unsigned long *) &iir_coeff_1[0], (unsigned long *) &iir_buffer_4[ADC_CHANNEL_0 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
iir_hpf((unsigned long *) &iir_coeff_1[0], (unsigned long *) &iir_buffer_4[ADC_CHANNEL_1 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
iir_hpf((unsigned long *) &iir_coeff_1[0], (unsigned long *) &iir_buffer_4[ADC_CHANNEL_2 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
iir_hpf((unsigned long *) &iir_coeff_1[0], (unsigned long *) &iir_buffer_4[ADC_CHANNEL_3 * IIR_BUFFER_SEC_SZ], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);

//set gain
adj_gain((unsigned long *) &gain_coeff[0], (unsigned long *) &adc_raw_data[ADC_CHANNEL_0], 1);
adj_gain((unsigned long *) &gain_coeff[1], (unsigned long *) &adc_raw_data[ADC_CHANNEL_1], 1);
adj_gain((unsigned long *) &gain_coeff[2], (unsigned long *) &adc_raw_data[ADC_CHANNEL_2], 1);
adj_gain((unsigned long *) &gain_coeff[3], (unsigned long *) &adc_raw_data[ADC_CHANNEL_3], 1);

[...]

//-----------------------------------------------------------------------------
	while(1)
	{
	}
}
//-----------------------------------------------------------------------------
