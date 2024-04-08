#include "main.h"
#include "myritone.h"

void remove_whitespace(char* str)
{
	unsigned counter = 0, i;
	while (str[counter] == ' ' || str[counter] == '\t')
		++counter;
	if (counter) {
		for (i = 0; str[i] != '\0' && i + counter < LINE_MAX; ++i)
			str[i] = str[i + counter];
		while (str[i] != '\0' && i < LINE_MAX)
			str[i] = '\0';
	}
}

void read_thru_comment_lines(char* str, FILE* scale_in)
{
	do {
		fgets(str, LINE_MAX, scale_in);
		remove_whitespace(str);
	} while (!feof(scale_in) && *str == '!');
}

void extract_header(FILE* scale_in, char* title, unsigned* scale_len)
{
	char temp[LINE_MAX];
	read_thru_comment_lines(temp, scale_in);
	strncpy(title, temp, strnlen(temp, LINE_MAX) - 1);
	read_thru_comment_lines(temp, scale_in);
	sscanf(temp, "%u\n", scale_len);
}

void read_scale(FILE* scale_in, scalenote* scale, unsigned len)
{
	char temp[LINE_MAX];
	long a;
	unsigned long b, c, d;
	double e;
	char x;
	fgets(temp, LINE_MAX, scale_in);
	for (unsigned i = 0; i < len; ++i) {
		fgets(temp, LINE_MAX, scale_in);
		if (temp[strnlen(temp, LINE_MAX) - 1] == '\n') // remove newline
			temp[strnlen(temp, LINE_MAX) - 1] = '\0'; // from end of each line
		remove_whitespace(temp);
		if (sscanf(temp, "%lu/%lu", &c, &d) == 2) {
			scale[i].ji.n = c; // JI fraction
			scale[i].ji.d = d;
			scale[i].ed.n = 1;
			scale[i].ed.d = 1;
		} else if (sscanf(temp, "%lu\\%lu<%ld/%lu%c", &a, &b, &c, &d, &x) == 5
				&& x == '>') { // equal division of fraction
			scale[i].ji.n = c;
			scale[i].ji.d = d;
			scale[i].ed.n = a;
			scale[i].ed.d = b;
		} else if (sscanf(temp, "%ld\\%lu", &a, &b) == 2) {
			scale[i].ji.n = 2; // equal division of octave
			scale[i].ji.d = 1;
			scale[i].ed.n = a;
			scale[i].ed.d = b;
		} else if (sscanf(temp, "%ld.%lu", &a, &b) == 2) {
			scale[i].ji.n = 2; // decimal cents
			scale[i].ji.d = 1;
			sscanf(temp, "%lf", &e);
			scale[i].ed.n = round(e * 1.0e15);
			scale[i].ed.d = floor(1.2e18);
		} else if (sscanf(temp, "%ld%c", &a, &x) == 2 && x == '.') {
			scale[i].ji.n = 2; // whole cents
			scale[i].ji.d = 1;
			scale[i].ed.n = a;
			scale[i].ed.d = 1200;
		} else {
			scale[i].ji.n = 0;
			scale[i].ji.d = 0;
			scale[i].ed.n = 0;
			scale[i].ed.d = 0;
		}
	}
}
