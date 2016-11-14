#ifndef WAV_PARSER_H
#define WAV_PARSER_H


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#define MIN(A,B) ((A) < (B)) ? (A):(B)
#define MAX(A,B) ((A) > (B)) ? (A):(B)

typedef struct WAVE
{
	char riff_chunk_id[4]; 			// big
	uint32_t riff_chunk_size; 		// little
	char riff_format[4]; 			// big

	char fmt_chunk_id[4]; 			// big
	uint32_t fmt_chunk_size; 		// little
	uint16_t fmt_audio_format;		// little
	uint16_t fmt_num_channels;		// little
	uint32_t fmt_sample_rate;		// little
	uint32_t fmt_byte_rate; 		// little
	uint16_t fmt_block_align;		// little
	uint16_t fmt_bits_per_sample;	// little

	char data_chunk_id[4];			// big
	uint32_t data_chunk_size;		// little

}WAVE;

bool readWavFile(FILE * file_pointer, WAVE * wavHeader);
bool displayWavInfo(WAVE * wavHeader);
bool validateHeaders(WAVE * wavHeader);


#endif
