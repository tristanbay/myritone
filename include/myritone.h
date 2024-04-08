#define LINE_MAX 256

typedef struct ratio
{
	long n;
	unsigned long d;
} ratio;

typedef struct scalenote
{ // cents can be stored with ji = 2/1 and ed = x/1200
	ratio ji, ed; // ji for just ratio, ed to logarithmically divide it
} scalenote;

void remove_whitespace(char* str);

void read_thru_comment_lines(char* str, FILE* scale_in);

void extract_header(FILE* scale_in, char* title, unsigned* scale_len);

void read_scale(FILE* scale_in, scalenote* scale, unsigned len);

void print_scale(scalenote* scale, unsigned len);
