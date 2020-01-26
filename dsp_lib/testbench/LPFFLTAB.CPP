#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "sample.h"
//-----------------------------------------------------------------------------
//sekcja bikwadratowa IIR 2-rzedu - Direct Form II
//-----------------------------------------------------------------------------
//skalowanie wspolczynnikow do formatu staloprzecinkowego ulamkowego Qn.m
//0x20000000 => format Q2.30
#define QFORMAT_BASE		0x40000000
//-----------------------------------------------------------------------------
#define IIR_COEFF			7
//-----------------------------------------------------------------------------
#define COEFF_A0			0
#define COEFF_A1			1
#define COEFF_A2			2
#define COEFF_B0			3
#define COEFF_B1			4
#define COEFF_B2			5
#define COEFF_GAIN			6
//-----------------------------------------------------------------------------
//filtry IIR - wspolczynniki w formatacie zmiennoprzecinkowym
//-----------------------------------------------------------------------------
//sekcja F1
//-----------------------------------------------------------------------------
//wspolczynniki B1, B2 - nalezy zmienic znak!
//-----------------------------------------------------------------------------
#define IIR1_COEFF_A0		+1.0
#define IIR1_COEFF_A1		+2.0
#define IIR1_COEFF_A2		+1.0
//-----------------------------------------------------------------------------
#define IIR1_COEFF_B0		+1.0
#define IIR1_COEFF_B1		+1.9939453974366188
#define IIR1_COEFF_B2		-0.99400689732283354
//-----------------------------------------------------------------------------
#define IIR1_GAIN			+0.000015375204384326935
//-----------------------------------------------------------------------------
//sekcja F2
//-----------------------------------------------------------------------------
//wspolczynniki B1, B2 - nalezy zmienic znak!
//-----------------------------------------------------------------------------
#define IIR2_COEFF_A0		+1.0
#define IIR2_COEFF_A1		+2.0
#define IIR2_COEFF_A2		+1.0
//-----------------------------------------------------------------------------
#define IIR2_COEFF_B0		+1.0
#define IIR2_COEFF_B1		+1.9855311857536435
#define IIR2_COEFF_B2		-0.98559242580085993
//-----------------------------------------------------------------------------
#define IIR2_GAIN			+0.000015310011804103851
//-----------------------------------------------------------------------------
static const double IIR1_Coeff[] =
{
	IIR1_COEFF_A0, IIR1_COEFF_A1, IIR1_COEFF_A2,
	IIR1_COEFF_B0, IIR1_COEFF_B1, IIR1_COEFF_B2,
	IIR1_GAIN
};
//-----------------------------------------------------------------------------
static const double IIR2_Coeff[] =
{
	IIR2_COEFF_A0, IIR2_COEFF_A1, IIR2_COEFF_A2,
	IIR2_COEFF_B0, IIR2_COEFF_B1, IIR2_COEFF_B2,
	IIR2_GAIN
};
//-----------------------------------------------------------------------------
void DebugPrint(double data)
{
	signed long m;
	unsigned long mm;
	m = data;
	mm = m;
	printf("%04x", mm >> 16);
	printf("%04x  ", mm);
}
//-----------------------------------------------------------------------------
double mul64(double x, double y)
{
	double i, j;
	i = x;
	j = y;
	i *= j;
//skalowanie wyniku dla wspolczynnikow w formacie ulamkowym Qn.m
	//i /= QFORMAT_BASE;
	return i;
}
//-----------------------------------------------------------------------------
//filtr IIR 2 rzedu
double IIR_BiquadFilter(double input, double *Coeff, double *buffer)
{
	double i, sum1, sum2;
//opoznienie
	buffer[2] = buffer[1];
	buffer[1] = buffer[0];
//1. stopien
//uwaga na zmieniony znak wspolczynnika B1
	sum1 = +mul64(Coeff[COEFF_B1], buffer[1]);
//uwaga na zmieniony znak wspolczynnika B2
	sum2 = +mul64(Coeff[COEFF_B2], buffer[2]);
//sumowanie
	i = input + sum1 + sum2;
	buffer[0] = i;
//2. stopien
	i = mul64(Coeff[COEFF_A0], i);
	i += mul64(Coeff[COEFF_A1], buffer[1]);
	i += mul64(Coeff[COEFF_A2], buffer[2]);
/*
	DebugPrint(input);
	DebugPrint(Coeff[COEFF_B2]);
	DebugPrint(buffer[2]);
	DebugPrint(sum1);
	DebugPrint(sum2);
	DebugPrint(i);
	printf("\n");
*/
	i = mul64(Coeff[COEFF_GAIN], i);
	return i;
}
//-----------------------------------------------------------------------------
double iir1[3], iir2[3];
//-----------------------------------------------------------------------------
double input, output;
//-----------------------------------------------------------------------------
int main(void)
{
//wyswietl wspolczynniki
	unsigned int i;
	clrscr();
	printf("wspoczynniki filtru IIR: \n\n");
	for (i = 0; i < IIR_COEFF; i++)
	{
		if (IIR1_Coeff[i] >= 0.0)
		{
			printf(" ");
		}
		printf("%4.20f", IIR1_Coeff[i]);
		if (IIR1_Coeff[i] >= 0.0)
		{
			printf(" ");
		}
		printf("    %012.0f\n", (IIR1_Coeff[i] * QFORMAT_BASE));
	}
	printf("\nnastepna strona: nacisnij klawisz: 'n' ");
	if (getch() != 'n')
	{
		return 0;
	}
//-----------------------------------------------------------------------------
	//input = 0x40000000;
	long n, m;
	m = 0;
	for (n = 0; n < 100000; n++)
	{
		clrscr();
		for (i = 0; i < 16; i++)
		{
			input = (signed long) SampleTable[m++];
			output = IIR_BiquadFilter(input, (double *) IIR1_Coeff, (double *) &iir1[0]);
			output = IIR_BiquadFilter(output, (double *) IIR2_Coeff, (double *) &iir2[0]);
			printf("%4.0d   ", m);
			DebugPrint(output);
			printf("\n");
		}
		printf("\nnastepna strona: nacisnij klawisz: 'n' ");
		if (getch() != 'n')
		{
			return 0;
		}
	}
	printf("koniec!\n");
	return 0;
}
//-----------------------------------------------------------------------------
