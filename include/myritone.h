#ifndef MYRITONE_H
#define MYRITONE_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define STR_MAX 256
#define CHANNEL_SIZE 128
#define CHANNEL_COUNT 16
#define COLOR_CH 3

typedef struct myri_ratio_s
{
	int64_t n;
	uint64_t d;
} myri_ratio_t;

typedef struct myri_note_s
{ // arbitrary cent values stored as ji = 2/1 and ed = x/1200
	myri_ratio_t ji, ed; // ji for just ratio, ed to logarithmically divide it
	char name[STR_MAX]; // note name
	int8_t color[COLOR_CH]; // first element is red channel, second is green, third is blue
} myri_note_t;

typedef struct myri_scale_s
{
	char title[STR_MAX]; // scale title
	uint8_t used_notes; // how many notes are used per channel
	uint8_t used_channels; // how many channels are used
	myri_ratio_t equave_ji, equave_ed; // what interval to repeat the scale at
	myri_note_t data[CHANNEL_SIZE][CHANNEL_COUNT]; // scale notes, up to 16 note channels
} myri_scale_t;

void trim_beginning(char* str)
{ // trims leading whitespace
	while (isspace(str[0]))
		++str; // trim first char off by advancing pointer
}

void trim_ending(char* str)
{ // trims trailing whitespace from string
	uint8_t new_len = strlen(str);
	char* end = str[new_len - 1];
	while ((new_len > 1) && (isspace(*(end - 1)))) // assumes \0 at end of string
		--end;
	*end = '\0'; // trim end by changing pointed-to byte to \0
}

void comment_check(char* buf, FILE* scale_in)
{ // trims leading whitespace and checks for comments
	do { // if leading ! or just whitespace then go to next line
		buf = fgets(buf, STR_MAX, scale_in);
		trim_beginning(buf);
	} while ((buf[0] == '\0') || (buf[0] == '!'));
}

void get_note_ratios(char* input, myri_ratio_t* ji, myri_ratio_t* ed)
{
	int64_t a;
	uint64_t b, c, d;
	double e;
	char x;
	trim_ending(input);
	trim_beginning(input);
	if (sscanf(input, "%lu/%lu", &c, &d) == 2) {
		ji->n = c; // JI fraction
		ji->d = d;
		ed->n = 1;
		ed->d = 1;
	} else if (sscanf(input, "%lu\\%lu<%ld/%lu%c", &a, &b, &c, &d, &x) == 5 && x == '>') {
		ji->n = c; // equal division of fraction
		ji->d = d;
		ed->n = a;
		ed->d = b;
	} else if (sscanf(input, "%ld\\%lu", &a, &b) == 2) {
		ji->n = 2; // equal division of octave
		ji->d = 1;
		ed->n = a;
		ed->d = b;
	} else if (sscanf(input, "%ld.%lu", &a, &b) == 2) {
		ji->n = 2; // decimal cents
		ji->d = 1;
		sscanf(input, "%lf", &e);
		ed->n = round(e * 1.0e15);
		ed->d = floor(1.2e18); // 1200 * 1.0e15
	} else if (sscanf(input, "%ld%c", &a, &x) == 2 && x == '.') {
		ji->n = 2; // whole cents
		ji->d = 1;
		ed->n = a;
		ed->d = 1200;
	} else {
		printf("Invalid note ratio format\n");
		exit(EXIT_FAILURE);
	}
}

void get_rest_of_header(char* buf, myri_scale_t* scale)
{ // buf will already have the relevant data in it
	int32_t used_notes_temp = 0;
	int32_t used_channels_temp = 0;
	char used_notes_str[STR_MAX];
	char used_channels_str[STR_MAX];
	char equave_str[STR_MAX] = "0\\0";
	char* delim;

	strncpy(used_notes_str, buf, STR_MAX);
	delim = strchr(used_notes_str, ',');
	if (!delim) { // only number of notes
		trim_ending(used_notes_str);
		trim_beginning(used_notes_str);
		used_notes_temp = atoi(used_notes_str);
		if (used_notes_temp < 1 || used_notes_temp > CHANNEL_SIZE) {
			printf("Invalid number of notes\n");
			exit(EXIT_FAILURE);
		}
		used_channels_temp = 1;
	} else { // note count per channel, number of channels, and equave
		strncpy(used_channels_str, delim + 1, STR_MAX);
		*delim = '\0';
		delim = strchr(used_channels_str, ',');
		if (!delim) {
			printf("Invalid formatting or missing equave\n");
			exit(EXIT_FAILURE);
		}
		strncpy(equave_str, delim + 1, STR_MAX);
		*delim = '\0';
		delim = strchr(equave_str, '!');
		if (delim)
			*delim = '\0';
		trim_ending(used_notes_str);
		trim_beginning(used_notes_str);
		used_notes_temp = atoi(used_notes_str);
		if (used_notes_temp < 1 || used_notes_temp > CHANNEL_SIZE) {
			printf("Invalid number of notes\n");
			exit(EXIT_FAILURE);
		}
		trim_ending(used_channels_str);
		trim_beginning(used_channels_str);
		used_channels_temp = atoi(used_channels_str);
		if (used_channels_temp < 1 || used_channels_temp > CHANNEL_COUNT) {
			printf("Invalid number of channels\n");
			exit(EXIT_FAILURE);
		}
		trim_ending(equave_str);
		trim_beginning(equave_str);
	}
	scale->used_notes = (uint8_t)used_notes_temp;
	scale->used_channels = (uint8_t)used_channels_temp;
	get_note_ratios(equave_str, &(scale->equave_ji), &(scale->equave_ed));
}

void get_notes(FILE* input, char* buf, myri_scale_t* scale)
{
	for (uint8_t ch = 0; ch < ch_ct; ++ch) {
		for (uint8_t nt = 0; nt < nt_ct; ++nt) {
		}
	}
}

myri_scale_t read_scale(FILE* scale_in)
{
	myri_scale_t scale;
	char buf[STR_MAX];
	comment_check(buf, scale_in);
	trim_ending(buf);
	strncpy(scale.title, buf, STR_MAX); // get title
	comment_check(buf, scale_in);
	get_rest_of_header(buf, &scale); // get # of notes and channels, and equave
	get_notes(scale_in, buf, &scale); // get notes
}

#endif
