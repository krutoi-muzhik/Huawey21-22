#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


const size_t BUFSIZE = 1024;
const size_t CMDSIZE = 16;
const size_t ARGSIZE = 16;

enum COMAND_ID {
	HLT = 0,
	PUSH = 1,
	POP = 2,
	IN = 3,
	OUT = 4,
	ADD = 5,
	SUB = 6,
	MUL = 7,
	DIV = 8,
	JMP = 9
};

enum BYTES {
	NUMB = 4.
	REGB = 5,
	OPEB = 6.
	PLUSB = 7
}

const char *PUSH_ = "push";
const char *POP_ = "pop";
const char *MUL_ = "mul";
const char *DIV_ = "div";
const char *SUB_ = "sub";
const char *ADD_ = "add";
const char *OUT_ = "out";
const char *IN_ = "in";
const char *HLT_ = "hlt";
const char *JMP_ = "jmp";

#define handle_error(comand) \
	do { printf (comand); printf (" !!! ERROR OCCURED CHECK LOGS !!!\n\n"); exit (EXIT_FAILURE); } while (0)

int
main () {
	char *in_buf = (char *) calloc (BUFSIZE, sizeof (char));
	char *out_buf = (char *) calloc (BUFSIZE, sizeof (char));
	FILE *fin, *fout;
	if ((fin = fopen ("input.txt", "r")) == NULL)
		handle_error ("fopen (input.txt)");
	fclose (fin);
	if ((fout = fopen ("output.txt", "wb")) == NULL)
		handle_error ("fopen output.txt");

	if (fread (in_buf, sizeof (char), BUFSIZE, fin) == 0)
		handle_error ("fread");	

	int id = 0;

	char *cmd = (char *) calloc (CMDSIZE, sizeof (char));
	char *arg = (char *) calloc (ARGSIZE, sizeof (char));
	// char *arg2 = (char *) calloc (ARGSIZE, sizeof (char));
	char reg = 0;
	int num = 0;
	size_t argsn;


	while (sscanf (in_buf, "%s", cmd) != 0) {
		argsn = 0;
		if (id > BUFSIZE) {
			printf ("not enough buffer size\n");
			exit (EXIT_FAILURE);
		}
		if (strcmp (cmd, PUSH_)) {
			sscanf (in_buf, "%d%n", &num, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB));
				out_buf[id + 1] = num;
				id += 2;
				continue;
			}
			sscanf (in_buf, "%cx%n", &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << REGB));
				out_buf[id + 1] = reg - 96;
				id += 2;
				continue;
			}
			sscanf (in_buf, "[%d]%n", &num, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB) | (1 << OPEB));
				out_buf[id + 1] = num;
				id += 2;
				continue;
			}
			sscanf (in_buf, "[%cx]%n", &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << REGB) | (1 << OPEB));
				out_buf[id + 1] = reg - 96;
				id += 2;
				continue;
			}
			sscanf (in_buf, "[%d + %cx]%n", &num, &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB) | (1 << REGB) | (1 << OPEB));
				out_buf[id + 1] = num;
				out_buf[id + 2] = reg - 96;
				id += 3;
				continue;
			}
			sscanf (in_buf, "%d + %cx%n", %num, &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB) | (1 << REGB));
				out_buf[id + 1] = num;
				out_buf[id + 2] = reg - 96;
				id += 3;
				continue;
			}
			sscanf (in_buf, "[%d - %cx]%n", &num, &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB) | (1 << REGB) | (1 << OPEB));
				out_buf[id + 1] = num;
				out_buf[id + 2] = reg - 96;
				id += 3;
				continue;
			}
			sscanf (in_buf, "%d - %cx%n", %num, &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (PUSH | (1 << NUMB) | (1 << REGB));
				out_buf[id + 1] = num;
				out_buf[id + 2] = reg - 96;
				id += 3;
				continue;
			}
			handle_error ("push assembling");
		}
		if (strcmp (cmd, POP_)) {
			sscanf (in_buf, "%cx%n", &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (POP | (1 << REGB));
				out_buf[id + 1] = (reg - 96);
				id += 2;
				continue;
			}
			sscanf (in_buf, "[%d]%n", &num, &argsn);
			if (argsn > 0) {
				out_buf[id] = (POP | (1 << NUMB) | (1 << OPEB));
				out_buf[id + 1] = num;
				id += 2;
			}
			sscanf (in_buf, "[%cx]%n", &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (POP | (1 << REGB) | (1 << OPEB));
				out_buf[id + 1] = (reg - 96);
				id += 2;
				continue;
			}
			sscanf (in_buf, "[%d + %cx]%n", &num, &reg, &argsn);
			if (argsn > 0) {
				out_buf[id] = (POP | (1 << NUMB) | (1 << REGB) | (1 << OPEB));
				out_buf[id + 1] = num;
				out_buf[id + 2] = (reg - 96);
				id += 3;
				continue;
			}
			handle_error ("pop assembling");
		}
		if (strcmp (cmd, MUL_)) {

			handle_error (" assembling");
		}
		if (strcmp (cmd, DIV_)) {

			handle_error ("push assembling");
		}
		if (strcmp (cmd, SUB_)) {

			handle_error ("push assembling");
		}
		if (strcmp (cmd, ADD_)) {

			handle_error ("push assembling");
		}
	}
	if (fwite (out_buf, sizeof (int), id + 1, fout) == 0)
		handle_error ("fwrite");
	fclose (fout);
	exit (EXIT_SUCCESS);
}

