this is my open source dsp library for cortex-m3 architecture.
source code was created and tested in 2010-09 to 2010-10 in my free time.
I used gcc toolchain (codesourcery) and Luminary Micro microcontrollers (US startup accuired by Texas Instruments).

in folder /lib you can find my dsp library - pure ASM code that contain iir biquad filters - low- and high-pass.
in folder /using_example you can find some source code for implementation in C language.

iir filters are using fixed-point, fractional 1.31 (or 2.30) arithmetic.
