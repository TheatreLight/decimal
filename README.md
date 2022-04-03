# Decimal

My own implementation of decimal library in C.

make s21_decimal.a - build a library;

make test - build and run unit-tests (need a check.h lib);

make gcov_report - build an html-report (need an lcov).



## Information

The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.
When the result of the division and multiplication is passed to the Round method, the result suffers no loss of precision.
A decimal number is a floating-point value that consists of a sign, a numeric value where each digit in the value ranges from 0 to 9, and a scaling factor that indicates the position of a floating decimal point that separates the integral and fractional parts of the numeric value.
The binary representation of a Decimal value consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the 96-bit integer and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28. Therefore, the binary representation of a Decimal value the form, ((-2^96 to 2^96) / 10^(0 to 28)), where -(2^96-1) is equal to MinValue, and 2^96-1 is equal to MaxValue.
The scaling factor also can preserve any trailing zeros in a Decimal number. Trailing zeros do not affect the value of a Decimal number in arithmetic or comparison operations.

Binary representation
The binary representation of a Decimal number consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the integer number and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28.
Decimal number can be implemented as a four-element array of 32-bit signed integers (int bits[4];).
bits[0], bits[1], and bits[2] contain the low, middle, and high 32 bits of the 96-bit integer number accordingly.
bits[3] contains the scale factor and sign, and consists of following parts:

Bits 0 to 15, the lower word, are unused and must be zero.
Bits 16 to 23 must contain an exponent between 0 and 28, which indicates the power of 10 to divide the integer number.
Bits 24 to 30 are unused and must be zero.
Bit 31 contains the sign; 0 meaning positive, and 1 meaning negative.

Note that the bit representation differentiates between negative and positive zero. These values can be treated as being equal in all operations.


This library include itself next functions.
1. Arithmetic operators:
    Addition;
    Substraction;
    Multiplication;
    Division;
    Remainder;
2. Comparison operators:
    Less then;
    Less then or equal to;
    Greater then;
    Greater then or equal to;
    Equal to;
    Not equalto;
3. Convertors:
    From integer to decimal;
    From float to decimal;
    From decimal to integer;
    Freom decimal to float;
4. Another functions:
    Rounds a specified Decimal number to the closest integer toward negative infinity;
    Rounds a decimal value to the nearest integer;
    Returns the integral digits of the specified Decimal (any fractional digits are discarded, including trailing zeroes);
    Returns the result of multiplying the specified Decimal value by negative one.
