#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <ctime>

//using namespace std;
using std::cout;
using std::swap;

void generateRandomNumbers(mpz_t &num1, mpz_t &num2) {
	// Generate random state
	gmp_randstate_t state;
	unsigned long seed = time(0);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);

	// Generate two random 4096 bit integers
	mpz_urandomb(num1, state, 4096);
	mpz_urandomb(num2, state, 4096);
	
	gmp_printf("\nRandom number 1:\n%Zd\n", num1);
	gmp_printf("\nRandom number 2:\n%Zd\n", num2);
	gmp_randclear(state);
}

void computeGCD(mpz_t &num1, mpz_t&num2, mpz_t &bezout_x, mpz_t &bezout_y) {
	mpz_t x, y;
	mpz_init(x);
	mpz_init_set_ui(y, 1);

	mpz_t zero;
	mpz_init(zero);

	while (mpz_cmp(num2, zero) != 0) {
		mpz_t quotient, remainder;
		mpz_inits(quotient, remainder, NULL);

		mpz_tdiv_q(quotient, num1, num2);
		mpz_tdiv_r(remainder, num1, num2);

		mpz_set(num1, num2);
		mpz_set(num2, remainder);

		mpz_t temp;
		mpz_init(temp);

		mpz_t temp_x;
		mpz_init_set(temp_x, x);
		mpz_mul(temp, quotient, x);
		mpz_sub(temp, bezout_x, temp);
		mpz_set(x, temp);
		mpz_set(bezout_x, temp_x);

		mpz_t temp_y;
		mpz_init_set(temp_y, y);
		mpz_mul(temp, quotient, y);
		mpz_sub(temp, bezout_y, temp);
		mpz_set(y, temp);
		mpz_set(bezout_y, temp_y);

		mpz_clears(quotient, remainder, temp, temp_x, temp_y, NULL);
	}
	mpz_clears(x, y, zero, NULL);
}

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
			mpz_clears(num1, num2, NULL);
			cout << "Usage: " << argv[0] << " [NUM1] [NUM2]\n";
			cout << "If input is provided, two numbers are required.\n";
			cout << "Otherwise, computes GCD of two random numbers.\n";
			exit(0);
	}

	// Compute GCD using the GMP implementation
	mpz_t gmp_gcd_res;
	mpz_init(gmp_gcd_res);
	mpz_gcd(gmp_gcd_res, num1, num2);
	gmp_printf("\nGMP GCD:\n%Zd\n", gmp_gcd_res);
	mpz_clear(gmp_gcd_res);

	// Compute GCD using extended euclidean algorithm implementation
	short bez1 = 1;
	short bez2 = 2;
	if (mpz_cmp(num1, num2) < 0) {
		mpz_swap(num1, num2);
		swap(bez1, bez2);
	}

	mpz_t bezout_x, bezout_y;
	mpz_init_set_ui(bezout_x, 1);
	mpz_init(bezout_y);

	computeGCD(num1, num2, bezout_x, bezout_y);
	
	// Print computed results
	gmp_printf("\nComputed GCD:\n%Zd\n", num1);

	cout << "\nBezout Coefficient for number " << bez1 << ":\n";
	mpz_out_str(stdout, 10, bezout_x);

	cout << "\n\nBezout Coefficient for number " << bez2 << ":\n";
	mpz_out_str(stdout, 10, bezout_y);
	cout << '\n';

	// Clear memory locations
	mpz_clears(num1, num2, bezout_x, bezout_y, NULL);
	return 0;
}