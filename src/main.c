#include "main.h"
#include "myritone.h"

int main(int argc, char** argv)
{
	if (wrong_args_check(argc))
		return 1;
	FILE* scale_in = fopen(argv[1], "r");
	char title[LINE_MAX];
	unsigned scale_length = 0;
	if (scale_in == NULL) {
		printf("Scale empty or invalid\n");
		return 1;
	}
	extract_header(scale_in, title, &scale_length);
	printf("%s\n", title);
	printf("%u\n!\n", scale_length);
	scalenote scale[scale_length];
	read_scale(scale_in, scale, scale_length);
	fclose(scale_in);
	print_scale(scale, scale_length);
	return 0;
}

bool wrong_args_check(int argc)
{
	if (argc < 2) {
		printf("Must specify the name of the input file\n");
		return true;
	}
	if (argc > 2) {
		printf("Too many arguments. Must be \"./myritone [filename]\"\n");
		return true;
	}
	return false;
}
