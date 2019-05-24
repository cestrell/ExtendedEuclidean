// Carlos Estrella

#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <ctime>

using std::cout;

void generateRandomNumbers(mpz_t &num1, mpz_t &num2) {
	// Generate random state
	gmp_randstate_t state;
	unsigned long seed = time(NULL);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);

	// Generate two random 4096 bit integers
	mpz_urandomb(num1, state, 4096);
	mpz_urandomb(num2, state, 4096);
	
	gmp_printf("Random number 1:\n%Zd\n", num1);
	gmp_printf("\nRandom number 2:\n%Zd\n\n", num2);
	gmp_randclear(state);
} // generateRandomNumbers

void computeGCD(mpz_t &num1, mpz_t&num2, mpz_t &bezout_x, mpz_t &bezout_y) {
	mpz_t x, y, zero;
	mpz_inits(x, zero, NULL);
	mpz_init_set_ui(y, 1);

	// Extended Euclidean Algorithm
	while (mpz_cmp(num2, zero) != 0) {
		mpz_t quotient, remainder, placeholder;
		mpz_inits(quotient, remainder, placeholder, NULL);

		mpz_tdiv_q(quotient, num1, num2);
		mpz_tdiv_r(remainder, num1, num2);

		mpz_set(num1, num2);
		mpz_set(num2, remainder);

		mpz_t temp_x, temp_y;
		mpz_init_set(temp_x, x);
		mpz_init_set(temp_y, y);

		mpz_mul(placeholder, quotient, x);
		mpz_sub(placeholder, bezout_x, placeholder);
		mpz_set(x, placeholder);
		mpz_set(bezout_x, temp_x);

		mpz_mul(placeholder, quotient, y);
		mpz_sub(placeholder, bezout_y, placeholder);
		mpz_set(y, placeholder);
		mpz_set(bezout_y, temp_y);

		mpz_clears(quotient, remainder, placeholder, temp_x, temp_y, NULL);
	} // while
	mpz_clears(x, y, zero, NULL);
} // computeGCD

int main(int argc, char *argv[]) {
	// Initialize a and b in GCD(a,b)
	mpz_t num1, num2;
	mpz_inits(num1, num2, NULL);

	// Set numbers accordingly
	switch (argc) {
		case 1:
			generateRandomNumbers(num1, num2);
			break;

		case 3:
			mpz_set_str(num1, argv[1], 10);
			mpz_set_str(num2, argv[2], 10);
			break;

		default:
			cout << "Usage: " << argv[0] << " [NUM1] [NUM2]\n";
			cout << "If input is provided, two numbers are required.\n";
			cout << "Otherwise, computes GCD of two random numbers.\n";
			mpz_clears(num1, num2, NULL);
			exit(0);
	} // switch

	// Compute GCD using the GMP implementation
	mpz_t gmp_gcd_res;
	mpz_init(gmp_gcd_res);
	mpz_gcd(gmp_gcd_res, num1, num2);
	gmp_printf("GMP GCD:\n%Zd\n", gmp_gcd_res);
	mpz_clear(gmp_gcd_res);

	// Compute GCD using extended euclidean algorithm implementation
	mpz_t bezout_x, bezout_y;
	mpz_init_set_ui(bezout_x, 1);
	mpz_init(bezout_y);
	computeGCD(num1, num2, bezout_x, bezout_y);
	
	// Print computed results
	gmp_printf("\nComputed GCD:\n%Zd\n", num1);
	gmp_printf("\nBezout Coefficient for number 1:\n%Zd\n", bezout_x);
	gmp_printf("\nBezout Coefficient for number 2:\n%Zd\n\n", bezout_y);

	// Clear memory locations
	mpz_clears(num1, num2, bezout_x, bezout_y, NULL);
	return 0;
} // main