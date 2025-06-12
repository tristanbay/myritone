#include "myritone.h"

int main(int argc, char** argv)
{
	FILE* scale_in = fopen(argv[1], "r"); // input file
	char title[LINE_MAX]; // scale title
	unsigned scale_length = 0; // length of scale

	// use getopt here

	if (scale_in == NULL) {
		printf("Scale empty or invalid\n");
		return EXIT_FAILURE;
	}
	extract_header(scale_in, title, &scale_length);
	printf("%s\n", title);
	printf("%u\n!\n", scale_length);
	scalenote scale[scale_length];
	read_scale(scale_in, scale, scale_length);
	fclose(scale_in);
	print_scale(scale, scale_length);
	return EXIT_SUCCESS;
}
