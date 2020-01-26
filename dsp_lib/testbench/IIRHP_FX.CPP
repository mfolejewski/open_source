#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
//sekcja bikwadratowa IIR 2-rzedu - Direct Form II
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
//format wspolczynnikow - signed long - staloprzecinkowy, ulamkowy system Q2.30
#define Q_FORMAT			30
//-----------------------------------------------------------------------------
//wspolczynniki B1, B2 - nalezy zmienic znak!
//-----------------------------------------------------------------------------
#define IIR1_COEFF_A0		0x40000000			// +1
#define IIR1_COEFF_A1		0x80000000			// -2
#define IIR1_COEFF_A2		0x40000000			// +1
#define IIR1_COEFF_B0		0x40000000			// +1
//-----------------------------------------------------------------------------
#define IIR1_COEFF_B1		0x7A510D82
#define IIR1_COEFF_B2		0xC5710932
//-----------------------------------------------------------------------------
#define IIR1_GAIN			0x3D380114
//-----------------------------------------------------------------------------
//wspolczynniki filtru IIR
static const unsigned long IIR1_Coeff[] =
{
	IIR1_COEFF_A0, IIR1_COEFF_A1, IIR1_COEFF_A2,
	IIR1_COEFF_B0, IIR1_COEFF_B1, IIR1_COEFF_B2,
	IIR1_GAIN
};
//-----------------------------------------------------------------------------
//dlugosc podstawowej zmiennej (int)
#define INT_BASE_SIZE		16
//ASTRO INT - zmienna astronomiczna
//dlugosc astronomicznych zmiennych - jako wielokrotnosc dlugosci podstawowej zmiennej (int)
//rozmiar n * m bit
#define SIZE_ASTRO_INT		6
//-----------------------------------------------------------------------------
//wspolczynniki filtru IIR - postac ASTRO INT
unsigned int TabCoeff[IIR_COEFF * SIZE_ASTRO_INT];
//-----------------------------------------------------------------------------
void InitCoeff(void)
{
	signed int i, j;
	for (i = 0; i < IIR_COEFF; i++)
	{
		unsigned long x;
		x = IIR1_Coeff[i];
		for (j = (SIZE_ASTRO_INT - 1); j >= 0; j--)
		{
			if (x & 0x80000000)
				TabCoeff[(i * SIZE_ASTRO_INT) + j] = 0xffff;
			else
				TabCoeff[(i * SIZE_ASTRO_INT) + j] = 0;
		}
		for (j = (SIZE_ASTRO_INT - 1); j >= (SIZE_ASTRO_INT - 2); j--)
		{
			TabCoeff[(i * SIZE_ASTRO_INT) + j] = x & 0xffff;
			x = x >> 16;
		}
	}
}
//-----------------------------------------------------------------------------
//wyzerowanie liczby
void clr_long(unsigned int *input, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
	{
		input[i] = 0;
	}
}
//-----------------------------------------------------------------------------
//kopiowanie liczb
void copy_long(unsigned int *input, unsigned int *output, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
	{
		output[i] = input[i];
	}
}
//-----------------------------------------------------------------------------
//przesuwanie arytmetyczne w prawo liczby o n bitow
//dane wejsciowe - liczby ze znakiem w kodzie U2
void shr_long(unsigned int *input, unsigned int shift, unsigned int size)
{
	unsigned int i, j;
	unsigned int x, y;
	for (j = 1; j <= shift; j++)
	{
		//kopiowanie znaku
		y = input[0] & 0x8000;
		//przesuwanie bitow
		for (i = 0; i < size; i++)
		{
			x = input[i];
			input[i] = (x >> 1) | y;
			y = 0;
			if (x & 0x0001)
				y = 0x8000;
		}
	}
//zaokraglanie
//	if (y && !(input[0] & 0x8000))
//		input[size - 1] |= 0x0001;
}
//-----------------------------------------------------------------------------
//przesuwanie arytmetyczne w lewo liczby o n bitow
//dane wejsciowe - liczby ze znakiem w kodzie U2
void shl_long(unsigned int *input, unsigned int shift, unsigned int size)
{
	signed int i, j;
	unsigned int x, y;
	for (j = 1; j <= shift; j++)
	{
		y = 0;
		//przesuwanie bitow
		for (i = (size - 1); i >= 0; i--)
		{
			x = input[i];
			input[i] = (x << 1) | y;
			y = 0;
			if (x & 0x8000)
				y = 0x0001;
		}
	}
}
//-----------------------------------------------------------------------------
//dodawanie 2 liczb o dlugosci (16 * n) bit
//wynik (output) moze byc zapisywany do jednej z danych (input1 lub input2)
//dane wejsciowe - liczby ze znakiem w kodzie U2
void add_long(unsigned int *input1, unsigned int *input2, unsigned int *output, unsigned int size)
{
	unsigned long x, y;
	x = 0;
	signed int i;
//dodaj liczby
	for (i = (size - 1); i >= 0; i--)
	{
		x = x >> 16;
		y = input1[i];
		x += y;
		y = input2[i];
		x += y;
		output[i] = x & 0xffff;
	}
}
//-----------------------------------------------------------------------------
//oblicz liczbe w kodzie U1
void u1_long(unsigned int *input, unsigned int *output, unsigned int size)
{
//negacja
	signed int i;
	for (i = 0; i < size; i++)
	{
		output[i] = ~input[i];
	}
}
//-----------------------------------------------------------------------------
//oblicz liczbe w kodzie U2
void u2_long(unsigned int *input, unsigned int *output, unsigned int size)
{
//negacja
	signed int i;
	for (i = 0; i < size; i++)
	{
		output[i] = ~input[i];
	}
//+1
	unsigned long x, y;
	x = 1;
	for (i = (size - 1); i >= 0; i--)
	{
		y = output[i];
		x += y;
		output[i] = x & 0xffff;
		x = x >> 16;
	}
}
//-----------------------------------------------------------------------------
//mnozenie 2 liczb o dlugosci (16 * n) bit
//dane wejsciowe - liczby ze znakiem w kodzie U2
void mul_long(unsigned int *input1, unsigned int *input2, unsigned int *output, unsigned int size)
{
	unsigned int number1[SIZE_ASTRO_INT], number2[SIZE_ASTRO_INT];
	signed int i, j;
//wyzeruj wynik
	clr_long((unsigned int *) output, SIZE_ASTRO_INT);
//skopiuj lokalnie 2 liczby
	copy_long((unsigned int *) input1, (unsigned int *) number1, SIZE_ASTRO_INT);
	copy_long((unsigned int *) input2, (unsigned int *) number2, SIZE_ASTRO_INT);
//sprawdz, czy liczby <> 0
	unsigned int sum1, sum2;
	sum1 = sum2 = 0;
	for (i = 0; i < size; i++)
	{
		sum1 |= number1[i];
		sum2 |= number2[i];
	}
//jedna z liczb = 0?
	if ((sum1 == 0) || (sum2 == 0))
	{
		return;
	}
	unsigned char sign1, sign2;
	sign1 = sign2 = 0;
//ustal znak i oblicz modul
	if (number1[0] & 0x8000)
	{
		sign1 = 1;
		u2_long((unsigned int *) number1, (unsigned int *) number1, size);
	}
//ustal znak i oblicz modul
	if (number2[0] & 0x8000)
	{
		sign2 = 1;
		u2_long((unsigned int *) number2, (unsigned int *) number2, size);
	}
//mnozenie
	unsigned long x;
	for (j = (size - 1); j >= 0; j--)
	{
		x = 0;
		for (i = (size - 1); i >= 0; i--)
		{
			x = x >> 16;
			unsigned long y, z;
			y = number1[j];
			z = number2[i];
			y *= z;
			x += y;
			signed int inx;
			inx = (size - 1) - j;
			inx = i - inx;
			if (inx >= 0)
			{
				unsigned int number3[SIZE_ASTRO_INT];
				unsigned int k;
				for (k = 0; k < size; k++)
				{
					number3[k] = 0;
				}
				number3[inx] = x & 0xffff;
				add_long((unsigned int *) number3, (unsigned int *) output, (unsigned int *) output, size);
			}
		}
	}
//oblicz znak koncowy
	sign1 += sign2;
	if (sign1 == 1)
	{
		u2_long((unsigned int *) output, (unsigned int *) output, size);
	}
}
//-----------------------------------------------------------------------------
void DebugPrint(unsigned int *input, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
	{
		printf("%04x", input[i]);
	}
	printf("  ");
}
//-----------------------------------------------------------------------------
unsigned int x[SIZE_ASTRO_INT], y[SIZE_ASTRO_INT], z[SIZE_ASTRO_INT];
//filtr IIR 2 rzedu
void IIR_BiquadFilter(unsigned int *input, unsigned int *output, unsigned int *Coeff, unsigned int *buffer)
{
//opoznienie
	copy_long((unsigned int *) &buffer[1 * SIZE_ASTRO_INT], (unsigned int *) &buffer[2 * SIZE_ASTRO_INT], SIZE_ASTRO_INT);
	copy_long((unsigned int *) &buffer[0 * SIZE_ASTRO_INT], (unsigned int *) &buffer[1 * SIZE_ASTRO_INT], SIZE_ASTRO_INT);
//1. stopien
// input * 1 (Q_FORMAT)
	copy_long((unsigned int *) input, (unsigned int *) x, SIZE_ASTRO_INT);
	shl_long((unsigned int *) x, Q_FORMAT, SIZE_ASTRO_INT);
//uwaga na zmieniony znak wspolczynnika B1
	mul_long((unsigned int *) &buffer[1 * SIZE_ASTRO_INT], (unsigned int *) &Coeff[COEFF_B1 * SIZE_ASTRO_INT], (unsigned int *) y, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) y, (unsigned int *) z, SIZE_ASTRO_INT);
//uwaga na zmieniony znak wspolczynnika B2
	mul_long((unsigned int *) &buffer[2 * SIZE_ASTRO_INT], (unsigned int *) &Coeff[COEFF_B2 * SIZE_ASTRO_INT], (unsigned int *) x, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) z, (unsigned int *) z, SIZE_ASTRO_INT);

	DebugPrint((unsigned int *) z, SIZE_ASTRO_INT);

	shr_long((unsigned int *) z, Q_FORMAT, SIZE_ASTRO_INT);
	copy_long((unsigned int *) z, (unsigned int *) &buffer[0 * SIZE_ASTRO_INT], SIZE_ASTRO_INT);
//2. stopien
	mul_long((unsigned int *) &buffer[0 * SIZE_ASTRO_INT], (unsigned int *) &Coeff[COEFF_A0 * SIZE_ASTRO_INT], (unsigned int *) x, SIZE_ASTRO_INT);
	mul_long((unsigned int *) &buffer[1 * SIZE_ASTRO_INT], (unsigned int *) &Coeff[COEFF_A1 * SIZE_ASTRO_INT], (unsigned int *) y, SIZE_ASTRO_INT);
	mul_long((unsigned int *) &buffer[2 * SIZE_ASTRO_INT], (unsigned int *) &Coeff[COEFF_A2 * SIZE_ASTRO_INT], (unsigned int *) z, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) y, (unsigned int *) x, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) z, (unsigned int *) output, SIZE_ASTRO_INT);
	shr_long((unsigned int *) output, Q_FORMAT, SIZE_ASTRO_INT);
//wersja optymalna: A0 = +1, A1 = -2, A2 = +1
/*
	u2_long((unsigned int *) &buffer[1 * SIZE_ASTRO_INT], (unsigned int *) x, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) x, (unsigned int *) x, SIZE_ASTRO_INT);
	add_long((unsigned int *) x, (unsigned int *) z, (unsigned int *) z, SIZE_ASTRO_INT);
	add_long((unsigned int *) z, (unsigned int *) &buffer[2 * SIZE_ASTRO_INT], (unsigned int *) output, SIZE_ASTRO_INT);
*/
//output * gain
	copy_long((unsigned int *) output, (unsigned int *) x, SIZE_ASTRO_INT);
	mul_long((unsigned int *) x, (unsigned int *) &Coeff[COEFF_GAIN * SIZE_ASTRO_INT], (unsigned int *) output, SIZE_ASTRO_INT);
	shr_long((unsigned int *) output, Q_FORMAT, SIZE_ASTRO_INT);

	DebugPrint((unsigned int *) &buffer[0 * SIZE_ASTRO_INT], SIZE_ASTRO_INT);
	DebugPrint((unsigned int *) output, SIZE_ASTRO_INT);
	printf("\n");
}
//-----------------------------------------------------------------------------
unsigned int input[SIZE_ASTRO_INT], output[SIZE_ASTRO_INT];
#define IIR_DELAY_STAGE			3
unsigned int iir1[IIR_DELAY_STAGE * SIZE_ASTRO_INT];
//-----------------------------------------------------------------------------
int main(void)
{
	unsigned int i;
	clrscr();
	printf("Dlugosc zmiennej astronomicznej = %d bitow.\n", (SIZE_ASTRO_INT * INT_BASE_SIZE));
//inicjalizacja wspolczynnikow
	InitCoeff();
//wyswietl wspolczynniki
	printf("\n\nwspoczynniki filtru IIR: \n\n");
	for (i = 0; i < IIR_COEFF; i++)
	{
		DebugPrint((unsigned int *) &TabCoeff[i * SIZE_ASTRO_INT], SIZE_ASTRO_INT);
		printf("\n");
	}
	printf("\nnastepna strona: nacisnij klawisz: 'n' ");
	if (getch() != 'n')
	{
		return 0;
	}
//-----------------------------------------------------------------------------
//filtr IIR
//-----------------------------------------------------------------------------
//inicjalizacja danych
	clr_long((unsigned int *) input, SIZE_ASTRO_INT);
	clr_long((unsigned int *) iir1, (IIR_DELAY_STAGE * SIZE_ASTRO_INT));
//input = 500000;
	//input[SIZE_ASTRO_INT - 2] = 0x0007;
	//input[SIZE_ASTRO_INT - 1] = 0xa120;
//input = 0x7fff ffff ffff
	//input[SIZE_ASTRO_INT - 3] = 0x7fff;
	//input[SIZE_ASTRO_INT - 2] = 0xffff;
	//input[SIZE_ASTRO_INT - 1] = 0xffff;
//input = 0x4000 0000
	input[SIZE_ASTRO_INT - 2] = 0x4000;
	input[SIZE_ASTRO_INT - 1] = 0x0000;
//-----------------------------------------------------------------------------
	unsigned long n;
	for (n = 0; n < 100000; n++)
	{
		clrscr();
		for (i = 0; i < 16; i++)
		{
			IIR_BiquadFilter((unsigned int *) input, (unsigned int *) output, (unsigned int *) TabCoeff, (unsigned int *) iir1);
			//DebugPrint((unsigned int *) output, SIZE_ASTRO_INT);
			//printf("\n");
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
