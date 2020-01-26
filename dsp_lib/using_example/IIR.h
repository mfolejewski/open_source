#ifndef _IIRASM_h_
#define _IIRASM_h_

//-----------------------------------------------------------------------------
//A/D converter (ADC): channel number
#define ADC_CHANNEL_NUM		4
//-----------------------------------------------------------------------------
//ADC channels
#define ADC_CHANNEL_0		0
#define ADC_CHANNEL_1		1
#define ADC_CHANNEL_2		2
#define ADC_CHANNEL_3		3
//-----------------------------------------------------------------------------
//IIR biquad filter, 2nd-order - direct form II
//-----------------------------------------------------------------------------
//low-pass filter LPF: coefficients A0 = +1, A1 = +2, A2 = +1, B0 = +1
//high-pass filter HPF: coefficients A0 = +1, A1 = -2, A2 = +1, B0 = +1
//-----------------------------------------------------------------------------
#define IIR_COEFF			3
//-----------------------------------------------------------------------------
#define COEFF_B1			0
#define COEFF_B2			1
#define COEFF_GAIN			2
//-----------------------------------------------------------------------------
//coefficient format - signed long - fixed-point, fractional system: Q2.30
#define Q_FORMAT			30
//-----------------------------------------------------------------------------
//B1, B2 coefficients - you must change the sign!
//-----------------------------------------------------------------------------
//LPF filter 0.1 Hz @ 1000 Hz sample rate
//F1 section
#define IIR0_COEFF_B1		0x7FF1710A
#define IIR0_COEFF_B2		0xC00E8D4E
#define IIR0_GAIN			0x0000006A
//-----------------------------------------------------------------------------
//HPF filter 0.1 Hz @ 1000 Hz sample rate
//F1 section
#define IIR1_COEFF_B1		0x7FF1710A
#define IIR1_COEFF_B2		0xC00E8D4E
#define IIR1_GAIN			0x3FF8B8EF
//-----------------------------------------------------------------------------
//LPF filter 10 Hz @ 1000 Hz sample rate
//F1 section
#define IIR2_COEFF_B1		0x7CBFF44A
#define IIR2_COEFF_B2		0xC300E720
#define IIR2_GAIN			0x000FC925
//F2 section
#define IIR3_COEFF_B1		0x78BE361E
#define IIR3_COEFF_B2		0xC704AC7C
#define IIR3_GAIN			0x000F4759
//-----------------------------------------------------------------------------
//LPF filter 50 Hz @ 1000 Hz sample rate
//F1 section
#define IIR4_COEFF_B1		0x6CDC9980
#define IIR4_COEFF_B2		0xCD893863
#define IIR4_GAIN			0x01668B87
//F2 section
#define IIR5_COEFF_B1		0x5EB2FB7D
#define IIR5_COEFF_B2		0xDC6D6B7A
#define IIR5_GAIN			0x0137E642
//-----------------------------------------------------------------------------
//LPF filter 100 Hz @ 1000 Hz sample rate
//F1 section
#define IIR6_COEFF_B1		0x5489D87D
#define IIR6_COEFF_B2		0xD7813526
#define IIR6_GAIN			0x04FD3C97
//F2 section
#define IIR7_COEFF_B1		0x431C4166
#define IIR7_COEFF_B2		0xED0C0950
#define IIR7_GAIN			0x03F5ED52
//-----------------------------------------------------------------------------
extern const unsigned long iir_coeff_0[];
extern const unsigned long iir_coeff_1[];
extern const unsigned long iir_coeff_2[];
extern const unsigned long iir_coeff_3[];
extern const unsigned long iir_coeff_4[];
extern const unsigned long iir_coeff_5[];
extern const unsigned long iir_coeff_6[];
extern const unsigned long iir_coeff_7[];
//-----------------------------------------------------------------------------
//buffer size for single IIR section
#define IIR_BUFFER_SEC_SZ	6
//-----------------------------------------------------------------------------
//IIR: single section buffer
extern unsigned long iir_buffer_0[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
extern unsigned long iir_buffer_1[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
extern unsigned long iir_buffer_2[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
extern unsigned long iir_buffer_3[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
extern unsigned long iir_buffer_4[ADC_CHANNEL_NUM * IIR_BUFFER_SEC_SZ];
//-----------------------------------------------------------------------------
//IIR biquad filter - low-pass filter (LPF)
void iir_lpf(unsigned long *coeff, unsigned long *buffer, unsigned long *sample, unsigned long number);
//IIR biquad filter - high-pass filter (HPF)
void iir_hpf(unsigned long *coeff, unsigned long *buffer, unsigned long *sample, unsigned long number);
//DC offset adding
void set_offset(unsigned long *coeff, unsigned long *sample, unsigned long data_size);
//adjustable gain amplifier
void adj_gain(unsigned long *coeff, unsigned long *sample, unsigned long number);
//-----------------------------------------------------------------------------
#endif
