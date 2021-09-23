#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


const double MicroConst = 0.0001;

enum ROOTS {
	NO_ROOTS = 3,
	INFINITY_ROOTS = 4,
	ONE_ROOT = 1,
	TWO_ROOTS = 2
};

int IsEqual (double a, double b);
int LinearSolve (double b, double c, double* px);
int SquareSolve (double a, double b, double c, double* x1, double* x2);
void SolverTest ();
double InputTest ();



int main () {
	SolverTest ();
	return 0;

	printf ("enter arguments\n");
	printf ("\na = ");

	double a = InputTest ();
	printf ("\nb = ");

	double b = InputTest ();
	printf ("\nc = ");

	double c = InputTest ();
	printf ("\n");

	double x1 = NAN;
	double x2 = NAN;
	int n = SquareSolve (a, b, c, &x1, &x2);
	printf ("x1 = %f, x2 = %f\n", x1, x2);
	return 0;
}


int IsEqual (double a, double b) {
	if (fabs (a - b) <= MicroConst) return 1;
	return 0;
}

int LinearSolve (double b, double c, double* px) {
	if (!IsEqual (b, 0)) {
		*px = (-c / b);
		if (IsEqual (*px, 0)) *px = 0;
		return 1;
	}
	if (IsEqual (c, 0)) return 2;
	return 0;
}

int SquareSolve (double a, double b, double c, double* x1, double* x2) {
	if (IsEqual (a, 0)) {
		int error = LinearSolve (b, c, x1);
		if (error == 0) return NO_ROOTS;
		if (error == 2) return INFINITY_ROOTS;
		else {
			*x2 = NAN;
			return ONE_ROOT;
		}
	}
	double d = b * b - 4 * a * c;
	printf ("d = %lf\n", d);
	if (IsEqual (d, 0)) {
		*x1 = (-b / (2 * a));
		if (IsEqual (*x1, 0)) *x1 = 0;
		*x2 = *x1;
		return TWO_ROOTS;
	}
	if (d < 0) return NO_ROOTS;

	*x1 = (-b + sqrt (d)) / (2 * a);
	*x2 = (-b - sqrt (d)) / (2 * a);
	if (IsEqual (*x1, 0)) *x1 = 0;
	if (IsEqual (*x2, 0)) *x2 = 0;
	return TWO_ROOTS;
}

void SolverTest () {
	double  a[4] =     {0,   0,   1,     1};
	double  b[4] =     {0,   1,   1,     0};
	double  c[4] =     {0,   1,   0,     1};
	double TestX1[4] = {NAN, -1,  0,   NAN};
	double TestX2[4] = {NAN, NAN, -1,  NAN};
	int  n[4] =        {4,   1,   2,     3};
	double ResX1[4] =  {NAN, NAN, NAN, NAN};
	double ResX2[4] =  {NAN, NAN, NAN, NAN};


	
	int k = 0;
	for (int i = 0; i < 4; i++) {
		
		k = SquareSolve (a[i], b[i], c[i], &ResX1[i], &ResX2[i]);
		switch (k) {
			case ONE_ROOT:
				if (IsEqual (TestX1[i], ResX1[i]) && (n[i] == k)) printf ("test %d ok\n\n", i + 1);
				else printf ("test %d BAD :(\nn = %d, must be %d\nx1 = %f, must be %f\n\n", i + 1, k, n[i], ResX1[i], TestX1[i]);
				break;
			case TWO_ROOTS:
				if (IsEqual (TestX1[i], ResX1[i]) && IsEqual (TestX2[i], ResX2[i]) && (n[i] == k)) printf ("test %d ok\n\n", i + 1);
				else printf ("test %d BAD :(\nn = %d, must be %d\nx1 = %f, must be %f\nx2 = %f, must be %f\n\n", i + 1, k, n[i], ResX1[i], TestX1[i], ResX2[i], TestX2[i]);
				break;
			case NO_ROOTS:
				if (n[i] == k) printf ("test %d ok\n\n", i + 1);
				else printf ("test %d BAD :(\nn = %d, must be %d\n\n", i + 1, k, n[i]);
				break;
			case INFINITY_ROOTS:
				if (n[i] == k) printf ("test %d ok\n\n", i + 1);
				else printf ("test %d BAD :(\nn = %d, must be %d\n\n", i + 1, k, n[i]);
				break;
		}
	}
}

double InputTest () {
	//char* input_string = (char*) calloc (10, sizeof (char));
	char input_string[10];
	scanf ("%9s", input_string);
	printf ("%s\n", input_string);
	int i = 0;
	int count = 0;
	while ((input_string[i] != '\0') && (input_string[i] != '\n')) {
		if (input_string[i] == '.') count++;
		if (input_string[i] == ',') {
			input_string[i] = '.';
			count++;
		}
		if ((!isdigit (input_string[i])) && (input_string[i] != '.')) {
			printf ("please enter a number\n");
			return InputTest ();
		}
		i++;
	}
	if (count > 1) {
		printf ("\ntoo many arguments, try again:\n");
		return InputTest ();
	}
	double x = 0;
	sscanf (input_string, "%lf", &x);
	printf ("%lf\n", x);
	return x;

}