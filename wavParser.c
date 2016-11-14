#include "wavParser.h"

static bool isLoaded(WAVE * wavHeader)
{
	return (wavHeader->riff_chunk_id[0] = 'W') && (wavHeader->riff_chunk_id[1] = 'A') &&
			(wavHeader->riff_chunk_id[2] = 'V') && (wavHeader->riff_chunk_id[3] = 'E');
}
static void printCharArr(char *arr, uint8_t size)
{
	if ((arr == NULL) || (size < 1)) return;

	int i;
	for (i = 0; i < size; i ++)
	{
		printf("%c",arr[i]);
	}
	printf("\n");
}

bool readWavFile(FILE * fp, WAVE * wavHeader)
{
	fread(wavHeader, sizeof(WAVE), 1, fp);

	return isLoaded(wavHeader);

}

bool displayWavInfo(WAVE * wavHeader)
{
	if (!isLoaded(wavHeader))	return false;

	float playTime = ((float)wavHeader->riff_chunk_size);
	playTime *= 8;
	playTime /= (float)wavHeader->fmt_sample_rate;
	playTime /= (float)wavHeader->fmt_bits_per_sample;
	playTime /= (float)wavHeader->fmt_num_channels;	

	printf("--------WAVE HEADER INFO--------\n");
	printf("RIFF_CHUNK_ID: "); printCharArr(wavHeader->riff_chunk_id, 4);
	printf("RIFF_CHUNK_SIZE: %u\n", wavHeader->riff_chunk_size);
	printf("RIFF_FORMAT: "); printCharArr(wavHeader->riff_format, 4);
	printf("\n");
	printf("FMT_CHUNK_ID: "); printCharArr(wavHeader->fmt_chunk_id, 4);
	printf("FMT_CHUNK_SIZE: %u\n", wavHeader->fmt_chunk_size);
	printf("FMT_AUDIO_FMT: %u\n", wavHeader->fmt_audio_format);
	printf("FMT_NUM_CHANNELS: %u\n", wavHeader->fmt_num_channels);
	printf("FMT_SAMPLE_RATE: %u\n", wavHeader->fmt_sample_rate);
	printf("FMT_BYTE_RATE: %u\n", wavHeader->fmt_byte_rate);
	printf("FMT_BLOCK_ALIGN: %u\n", wavHeader->fmt_block_align);
	printf("FMT_BITS_PER_SAMPLE: %u\n", wavHeader->fmt_bits_per_sample);
	printf("\n");
	printf("DATA_CHUNK_ID: "); printCharArr(wavHeader->data_chunk_id, 4);
	printf("DATA_CHUNK_SIZE: %u\n", wavHeader->data_chunk_size);
	printf("\n");
	printf("File Duration: %0.2f\n", playTime);
	printf("--------------------------------\n");

	return true;

}

bool validateHeaders(WAVE * wavHeader)
{
	if (!isLoaded(wavHeader))	return false;
	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("USAGE: <file.wav>");
		exit(0);
	}

	WAVE wavHeader;
	char * fileName = argv[1];

	FILE * file_pointer = fopen(fileName, "rb");
	if (file_pointer == NULL)
	{
		printf("File Open Error!\n");
		exit(0);
	}

	if (!readWavFile(file_pointer, &wavHeader))
	{
		printf("File Read Error!\n");
		exit(0);
	}

	if (!displayWavInfo(&wavHeader)) printf("File Display Error!\n");
	if (!validateHeaders(&wavHeader)) printf("File Validation Error!\n");

	return 0;
}
