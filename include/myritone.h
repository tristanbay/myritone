#ifndef MYRITONE_H
#define MYRITONE_H

#include <stdio.h>
#include <stdlib.h>
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
	uint8_t color[COLOR_CH]; // first element is red channel, second is green, third is blue
} myri_note_t;

typedef struct myri_scale_s
{
	char title[STR_MAX]; // scale title
	uint8_t used_notes; // how many notes are used per channel
	uint8_t used_channels; // how many channels are used
	myri_ratio_t equave_ji, equave_ed; // what interval to repeat the scale at
	myri_note_t data[CHANNEL_SIZE][CHANNEL_COUNT]; // scale notes, up to 16 note channels
} myri_scale_t;

char* trim_beginning(char* str)
{ // trims leading whitespace
	while (isspace(str[0]))
		++str; // trim first char off by advancing pointer
	return str;
}

void trim_ending(char* str)
{ // trims trailing whitespace from string
	uint8_t new_len = strlen(str);
	char* end = &str[new_len - 1];
	while (new_len > 1 && isspace(*end)) {
		--end;
		--new_len;
	}
	*(end + 1) = '\0'; // trim end by changing pointed-to byte to \0
}

void comment_check(char* buf, FILE* scale_in)
{ // trims leading whitespace and checks for comments
	char temp_str[STR_MAX];
	do { // if leading ! or just whitespace then go to next line
		if (!fgets(buf, STR_MAX, scale_in)) {
			printf("File ended earlier than expected\n");
			exit(EXIT_FAILURE);
		}
		strncpy(temp_str, trim_beginning(buf), STR_MAX);
		strncpy(buf, temp_str, STR_MAX);
	} while ((buf[0] == '\0') || (buf[0] == '!'));
}

void get_note_ratios(char* input, myri_ratio_t* ji, myri_ratio_t* ed)
{
	int64_t a;
	uint64_t b, c, d;
	double e;
	uint64_t log_e;
	char x;
	char temp_str[STR_MAX];
	trim_ending(input);
	strncpy(temp_str, trim_beginning(input), STR_MAX);
	strncpy(input, temp_str, STR_MAX);
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
	} else if (sscanf(input, "%ld.%lu", &a, &b) == 2
			|| sscanf(input, ".%lu", &a) == 1
			|| sscanf(input, "-.%lu", &a) == 1) {
		ji->n = 2; // decimal cents
		ji->d = 1;
		sscanf(input, "%lf", &e);
		log_e = ceil(log10(abs(e)));
		ed->n = round(e * pow(10, 15 - log_e));
		ed->d = floor(1.2 * pow(10, 18 - log_e)); // 1200 * 10^(15-log_e)
		while (ed->n % 10 == 0 && ed->d % 10 == 0) { // take off any unnecessary zeros
			ed->n /= 10;
			ed->d /= 10;
		}
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
	char temp_str[STR_MAX];
	char* delim = NULL;

	strncpy(used_notes_str, buf, STR_MAX);
	delim = strchr(used_notes_str, ',');
	if (!delim) { // only number of notes
		trim_ending(used_notes_str);
		strncpy(temp_str, trim_beginning(used_notes_str), STR_MAX);
		strncpy(used_notes_str, temp_str, STR_MAX);
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
		strncpy(temp_str, trim_beginning(used_notes_str), STR_MAX);
		strncpy(used_notes_str, temp_str, STR_MAX);
		used_notes_temp = atoi(used_notes_str);
		if (used_notes_temp < 1 || used_notes_temp > CHANNEL_SIZE) {
			printf("Invalid number of notes\n");
			exit(EXIT_FAILURE);
		}
		trim_ending(used_channels_str);
		strncpy(temp_str, trim_beginning(used_channels_str), STR_MAX);
		strncpy(used_channels_str, temp_str, STR_MAX);
		used_channels_temp = atoi(used_channels_str);
		if (used_channels_temp < 1 || used_channels_temp > CHANNEL_COUNT) {
			printf("Invalid number of channels\n");
			exit(EXIT_FAILURE);
		}
		trim_ending(equave_str);
		strncpy(temp_str, trim_beginning(equave_str), STR_MAX);
		strncpy(equave_str, temp_str, STR_MAX);
	}
	scale->used_notes = (uint8_t)used_notes_temp;
	scale->used_channels = (uint8_t)used_channels_temp;
	get_note_ratios(equave_str, &(scale->equave_ji), &(scale->equave_ed));
}

void get_notes(FILE* input, char* buf, myri_scale_t* scale)
{
	char note[STR_MAX], name[STR_MAX], color[STR_MAX];
	uint8_t ch_ct = scale->used_channels;
	uint8_t nt_ct = scale->used_notes;
	char temp_str[STR_MAX];
	char* delim = NULL;
	for (uint8_t ch = 0; ch < ch_ct; ++ch) {
		for (uint8_t nt = 0; nt < nt_ct; ++nt) {
			comment_check(buf, input);
			strncpy(note, buf, STR_MAX); // copy note interval
			delim = strchr(buf, '"');
			if (!delim) {
				name[0] = '\0'; // default to empty
			} else {
				strncpy(name, delim + 1, STR_MAX); // copy note name
			}
			delim = strchr(buf, '#');
			if (!delim) {
				for (uint8_t i = 0; i < COLOR_CH * 2; ++i) // default to white
					color[i] = 'f';
				color[COLOR_CH * 2] = '\0';
			} else {
				strncpy(color, delim + 1, STR_MAX); // copy note color
			}
			delim = strchr(note, '"'); // prepare note interval string
			if (delim)
				*delim = '\0'; // trim to just before beginning double quote for name
			trim_ending(note);
			strncpy(temp_str, trim_beginning(note), STR_MAX);
			strncpy(note, temp_str, STR_MAX);
			if (strnlen(name, STR_MAX)) { // if string not empty
				delim = strchr(name, '"'); // prepare note name string
				if (!delim) {
					printf("Note name should be enclosed in 1 pair of double quotes\n");
					exit(EXIT_FAILURE);
				}
				*delim = '\0'; // trim to just before ending double quote
				trim_ending(name);
				strncpy(temp_str, trim_beginning(name), STR_MAX);
				strncpy(name, temp_str, STR_MAX);
			} // vv add scale note data to note struct vv
			get_note_ratios(note, &(scale->data[nt][ch].ji), &(scale->data[nt][ch].ed));
			strncpy(scale->data[nt][ch].name, name, STR_MAX); // add scale name to note struct
			sscanf(color, "%2x%2x%2x", &(scale->data[nt][ch].color[0]),
									   &(scale->data[nt][ch].color[1]), // add data from strings to
									   &(scale->data[nt][ch].color[2])); // note structure
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
	return scale;
}

#endif
