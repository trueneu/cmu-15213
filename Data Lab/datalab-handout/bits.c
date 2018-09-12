/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif

/*
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    return ~(~x | ~y);
}

/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
    int n_times_8 = n << 3;
    return (x >> n_times_8) & 0xff;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
    /* 15 operations
     * The biggest problem here is that we can't << to the width of operand
     * So we have to somehow make an "if" (n == 0) and (n > 0) without actually using an if
     * So to do that we're using property of !n == 0 for n > 0, and !n == 1 for n == 0,
     * consequentially ~!n + 1 == -1 for n == 0, ~!n + 1 == 0 for n > 0 (due to overflow)
     * so for n > 0, we're performing -1 << (width - n), ~
     * for n == 0, it's 0 << 0, ~
     * To get first zero for this special case, we do ~!!n + 1,
     * to get the second one, we do (width - n) - (width - n), or (width - n) + (n - width),
     * to nullify the second (n - width), we bitwise AND it against -1 for n == 0, and against 0 for n > 0
     */
    int x_arithm_shift = x >> n;
    int _32_minus_n = 32 + 1 + ~n;
    int _n_minus_32 = n + 1 + ~32;
    int n_HO_zeroes = ~( ~!!n + 1 << (_32_minus_n + (_n_minus_32 & (~!n + 1) ) ) );
    return (x_arithm_shift & n_HO_zeroes);
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    /*
     * Absolutely no ideas how to it in 40 ops without loops. c += x & 1; x >>= 1; takes 3 * 32 = 96 operations.
     * "Optimized" x & (x - 1) until x == 0 requires conditions.
     */
    int c = 0;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;
    c += x & 1; x >>= 1;

    return c;
}

/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    /* Fold the number to the right
     * So if there is at least one 1 in binary representation, it stays there
     * Otherwise it's a zero.
     */

    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;
    return ~(x & 1) & 1;
}

/*
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    /*
     * Follow the definition
     */
    return 1 << 31;
}

/*
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    /*
     * Straightforwardly find tmax and tmin, using the fact that -n = ~n + 1,
     * then if either of tmax - x < 0 and x - tmin < 0 is true,
     * then number doesn't fit
     *
     */
    int tmax = (0x01 << (n + ~0x0)) + ~0x0;
    int sign_bit = 0x01 << 31;
    int x_neg = x & sign_bit;
    int tmax_minus_x = (tmax -x );
    int x_minus_tmin = (x - ~tmax);

    return !(x ^ tmax) | !(x ^ ~tmax) | !(x_neg & x_minus_tmin) | !(~!x_neg & tmax_minus_x);
}

/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    /*
     * To do this, we must add a bias of 2^n, but only when we're dealing with a negative number AND
     * we're suffering precision loss when performing the division, e.g.
     * -15 >> 2 == -4, should be -3, so we add 2^2, and get -11 >> 2 == -3
     * but -16 >> 2 == -4 and should be -4, so we don't add anything
     */
    int sign_x = x >> 31;
    int precision_loss = !!(x & ~(~0x0 << n));
    return (x + (sign_x & precision_loss << n)) >> n;
}

/*
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    /*
     * We already did this a number of times.
     */
    return ~x + 1;
}

/*
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    /*
     * The only quirk here is to return 0 for 0, as it is not greater than 0
     * The only value !!x == 0 for is 0 itself.
     */
    int sign_x = x >> 31;
    return !(1 & sign_x) & !!x;
}

/*
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    /*
     * Fails on corner cases like MAX_INT vs MIN_INT and vice versa.
     */
    int x_minus_y = x + ~y + 1;
    int x_minus_y_neg = !!(x_minus_y & (1 << 31));
    int x_neg = !!(x & (1 << 31));
    int diff_signs = !!((x ^ y) & (1 << 31));
    return (diff_signs & x_neg) | (!diff_signs & x_minus_y_neg) | !(x ^ y);
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    int acc = 0;
    x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1); x >>= 1;
    acc = acc + (!x ^ 1);
    return acc;
}

/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    /*
     * Pretty straightforward
     */

    int exponent = (uf & (0xff << 23)) >> 23;
    int significand = uf & (0x7fffff);
    if ((exponent == 0xff) && (significand != 0x0)) { // NaN test
        return uf;
    } else {
        return (uf ^ (0x80 << 24));
    }
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    /*
     * Gives errors by one for some reason
     */

    if (x == 0 ) return x;

    int neg = x < 0;
    if (neg) x = -x;
    int log_x = 31;
    unsigned check = 1 << 31;
    while (!(x & check)) {
        log_x--;
        check >>= 1;
    };
    x ^= check;

    unsigned sign = neg;
    unsigned exponent = log_x + 127;
    unsigned significand;
    if (log_x > 23) {
        significand = x >> (log_x - 23);
    } else {
        significand = x << (23 - log_x);
    }

    return (sign << 31) | (exponent << 23) | significand;
}

/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    /*
     * Pretty straightforward solution
     */
    if (uf == 0 || uf == 0x80000000) return uf;

    int sign = uf & (0x01 << 31);
    int exponent = (uf & (0xff << 23)) >> 23;
    int significand = uf & (0x7fffff);
    if (exponent == 0xff) { // NaN/inf test
        return uf;
    }

    if (exponent == 0) {
        significand <<= 1;
    } else {
        exponent++;
    }

    uf ^= exponent << 23;
    if (exponent == 0xff) {
        return sign | (exponent << 23);
    }
    return sign | (exponent << 23) | significand;
}
