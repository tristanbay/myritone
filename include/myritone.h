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

typedef struct myri_ratio_s {
	int64_t n;
	uint64_t d;
} myri_ratio_t;

typedef struct myri_note_s { // arbitrary cent values stored as ji = 2/1 and ed = x/1200
	myri_ratio_t ji, ed; // ji for just ratio, ed to logarithmically divide it
	char name[STR_MAX]; // note name
	int8_t color[COLOR_CH]; // first element is red channel, second is green, third is blue
} myri_note_t;

typedef struct myri_scale_s {
	char title[STR_MAX]; // scale title
	myri_note_t data[CHANNEL_SIZE][CHANNEL_COUNT]; // scale notes, up to 16 note channels
	uint8_t used_notes; // how many notes are used per channel
	uint8_t used_channels; // how many channels are used
} myri_scale_t;

void comment_check(char* buf, FILE* scale_in) { // trims whitespace and checks for comments
	do { // if leading ! or just whitespace then go to next line
		buf = fgets(buf, STR_MAX, scale_in);
		while (isspace(buf[0]))
			++in; // trim first char off by advancing pointer
	} while ((buf[0] == '\0') || (buf[0] == '!'));
}

myri_scale_t read_scale(FILE* scale_in) {
	myri_scale_t scale;
	char* buf[STR_MAX];
	char* tok[STR_MAX];
	comment_check(buf, scale_in);
	strcpy(scale.title, buf); // get title
	comment_check(buf, scale_in);
	// get number of notes and channels ("a, b" or if only "a" then b = 1)
	// get notes
}

#endif
