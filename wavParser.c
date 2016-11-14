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
	fseek(fp, 0, SEEK_SET);
	fread(wavHeader, sizeof(WAVE), 1, fp);

	return isLoaded(wavHeader);

}

bool displayWavInfo(WAVE * wavHeader)
{
	if (!isLoaded(wavHeader))	return false;

	// uint32_t num = wavHeader->riff_chunk_size;
 //    uint32_t swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
 //                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
 //                    ((num>>8)&0xff00) | // move byte 2 to byte 1
 //                    ((num<<24)&0xff000000);
 //        wavHeader->riff_chunk_size = swapped;	

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
	printf("File Duration: %0.2f sec, %0.2f min\n", playTime, playTime/60);
	printf("--------------------------------\n");

	return true;

}

static uint32_t findDataSize(FILE * file_pointer)
{
	const int  dataOffset = 44;

	fseek(file_pointer, dataOffset, SEEK_SET);

	uint32_t size = 0;

	while(!feof(file_pointer))
	{
		getc(file_pointer);
		size++;
	}
	printf("Data CALC: %u\n", size);
	return size;

}

bool validateHeaders(WAVE * wavHeader, FILE * file_pointer)
{
	if (!isLoaded(wavHeader))	return false;

	uint32_t dataSize = findDataSize(file_pointer);
	uint8_t errCnt = 0;

	if (wavHeader->riff_chunk_size != dataSize + 36)
	{
		printf("Riff Chunk Size: ERROR\n");
		errCnt++;
	}

	if (wavHeader->data_chunk_size != dataSize)
	{
		printf("Data Chunk Size: ERROR\n");
		errCnt++;
	}

	if (wavHeader->fmt_chunk_size != 16)
	{
		printf("FMT Chunk Size: ERROR\n");
		errCnt++;
	}

	printf("Total Errors: %u\n", errCnt);

	return (errCnt == 0);

}

static bool fixHeaders(WAVE * wavHeader, FILE * file_pointer)
{
	uint32_t dataSize = findDataSize(file_pointer);

	fseek(file_pointer, 40, SEEK_SET);

	fwrite(&dataSize, sizeof(uint32_t), 1, file_pointer);

	fseek(file_pointer, 4, SEEK_SET);

	uint32_t riffSize  = dataSize + 36;

	fwrite(&riffSize, sizeof(uint32_t), 1, file_pointer);

	if(!readWavFile(file_pointer,wavHeader)) return false;
	if(!displayWavInfo(wavHeader)) return false;

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

	FILE * file_pointer = fopen(fileName, "rb+");
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
	if (!validateHeaders(&wavHeader, file_pointer))
	{
		 printf("File Validation Error!\n");
		 printf("Fixing Headers...\n");
		 if(!fixHeaders(&wavHeader, file_pointer)) printf("FIX ERROR\n");
	}
	fclose(file_pointer);

	return 0;
}
