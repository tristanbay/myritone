#include "myritone.h"

void print_note(myri_note_t data) { // function for printing each note
	printf("%-4u\\%-4lu<%-3u/%-3lu>\t%-10s\t#%02x%02x%02x\n",
		data.ed.n, data.ed.d, data.ji.n, data.ji.d, data.name,
		data.color[0], data.color[1], data.color[2]);
}

void print_scale(myri_scale_t scale) { // display contents of scale data structure
	printf("%s\n\n", scale.title);
	printf("Notes per channel: %u\nChannels used:     %u\n\n",
		scale.used_notes, scale.used_channels);
	for (uint8_t i = 0; i < scale.used_channels; ++i) {
		for (uint8_t j = 0; j < scale.used_notes; ++j) {
			printf("Ch %02d Nt %03d:\t", i + 1, j + 1);
			print_note(scale.data[j][i]);
		}
		printf("\n");
	}
}
