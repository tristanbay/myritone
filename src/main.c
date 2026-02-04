#include "myritone.h"
#include "output.h"

int main(int argc, char** argv)
{
	FILE* input;
	myri_scale_t scale;
	if (argc != 2) { // # of args check
		printf("Usage: \"./myritone <input file>\"\n");
		return EXIT_FAILURE;
	}
	input = fopen(argv[1], "r");
	if (input == NULL) { // valid file check
		printf("Could not open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	scale = read_scale(input);
	fclose(input);
	print_scale(scale);
	return EXIT_SUCCESS;
}
