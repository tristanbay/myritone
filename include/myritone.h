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
	myri_note_t data[CHANNEL_SIZE][CHANNEL_COUNT]; // scale notes, up to 16 note channels
	uint8_t used_notes; // how many notes are used per channel
	uint8_t used_channels; // how many channels are used
} myri_scale_t;

void trim_beginning(char* str)
{ // trims leading whitespace
	while (isspace(str[0]))
		++str; // trim first char off by advancing pointer
}

void comment_check(char* buf, FILE* scale_in)
{ // trims leading whitespace and checks for comments
	do { // if leading ! or just whitespace then go to next line
		buf = fgets(buf, STR_MAX, scale_in);
		trim_beginning(buf);
	} while ((buf[0] == '\0') || (buf[0] == '!'));
}

void trim_ending(char* str)
{ // trims trailing whitespace from string
	uint8_t new_len = strlen(str);
	char* end = str[new_len - 1];
	while ((new_len > 1) && (isspace(*(end - 1)))) // assumes \0 at end of string
		--end;
	*end = '\0'; // trim end by changing pointed-to byte to \0
}

void notes_and_channels(char* buf, char* tok, uint8_t* nt, uint8_t* ch)
{ // buf will already have the relevant data in it
	tok = strtok(buf, ",");
	int64_t temp_nt, temp_ch;
	if (tok) {
		trim_ending(tok); // trim whitespace from tok
		trim_beginning(tok);
		temp_nt = atoi(tok);	
		if ((temp_nt < 1) || (temp_nt > 255)) { // check for note count out of bounds
			printf("Invalid number of notes per channel\n");
			exit(EXIT_FAILURE);
		}
		*nt = (uint8_t)temp_nt;
		trim_ending(buf); // trim whitespace from buf
		trim_beginning(buf);
		temp_ch = atoi(buf);
		if ((temp_ch < 1) || (temp_ch > 255)) { // check for channel count out of bounds
			printf("Invalid number of channels\n");
			exit(EXIT_FAILURE);
		}
		*ch = (uint8_t)temp_ch;
	} else {
		trim_ending(tok); // trim whitespace from tok
		trim_beginning(tok);
		temp_nt = atoi(buf);	
		if ((temp_nt < 1) || (temp_nt > 255)) { // check for note count out of bounds
			printf("Invalid number of notes per channel\n");
			exit(EXIT_FAILURE);
		}
		*nt = (uint8_t)temp_nt;
		*ch = 1;
	}
}

void get_notes(FILE* scale_in, myri_note_t** data)
{

}

myri_scale_t read_scale(FILE* scale_in)
{
	myri_scale_t scale;
	char* buf[STR_MAX];
	char* tok[STR_MAX];
	comment_check(buf, scale_in);
	trim_ending(buf);
	strcpy(scale.title, buf); // get title
	comment_check(buf, scale_in);
	notes_and_channels(buf, tok, &scale.used_notes, &scale.used_channels);
		// get number of notes and channels ("a, b" or if only "a" then b = 1)
	get_notes(scale_in, scale.data); // get notes
}

#endif
