/*
 * BreastMammogramROIExtractionCpuCode.c
 *
 *  Created on: 23 Apr 2015
 *      Author: imilankovic
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <MaxSLiCInterface.h>
#include "Maxfiles.h"

void loadImage(char *filename, uint **dest, uint *width, uint *height, uint p3)
{
	char buffer[80];
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error opening file %s.", filename);
		exit(1);
	}
	uint i;

	fgets(buffer, 80, file); // Type
	fgets(buffer, 80, file); // Comment
	fscanf(file, "%d %d\n", width, height);
	fgets(buffer, 80, file); // Max intensity

	*dest = malloc((*width) * (*height) * sizeof(int) * ((p3)? 3: 1));

	for(i = 0; i < (*width) * (*height) * ((p3)? 3: 1); i++) {
		int v;
		fscanf(file, "%d", &v);
		(*dest)[i] = v;
	}

	fclose(file);
}

void writeImage(char *filename, uint *data, uint width, uint height, uint p3)
{
	FILE *file = fopen(filename, "w");
	uint i;

	fprintf(file, (p3) ? "P3\n" : "P2\n");
	fprintf(file, "#generated\n");
	fprintf(file, "%d %d\n", width, height);
	fprintf(file, "255\n");

	for(i = 0; i < (width * height) * ((p3)? 3 : 1); i++)
		fprintf(file, "%d\n", (int)data[i]);

	fclose(file);
}

int main()
{

	uint *in_stream1;
	uint *out_stream1;
	uint width1, height1;
	uint data_size;
	uint black = 10, threshold = 170;

	struct timeval start, end;
	long mtime, seconds, useconds;

	printf("Generating data to send to FPGA.\n");

	// Load the images
	loadImage("input/mdb012.pgm", &in_stream1, &width1, &height1, 0);

	data_size = width1 * height1 * sizeof(int);

	// Allocate a buffer for the edge data to go into.
	out_stream1 = malloc(data_size);

	printf("Streaming data to FPGA.\n");


	BreastMammogramROIExtraction(black, height1, width1 * height1,threshold, width1, in_stream1, out_stream1);

	gettimeofday(&start, NULL);

	//for (int i = 0; i < 1000; i++)
		BreastMammogramROIExtraction(black, height1, width1 * height1,threshold, width1, in_stream1, out_stream1);

//for (int i = 0; i < 1000; i++)
//		max_run(device,
//				max_input("image_pixel1", in_stream1, data_size),
//
//				max_output("output1", out_stream1, data_size),
//
//				max_runfor("RoiKernel1", width1 * height1 * loop_length),
//
//				max_end());

	gettimeofday(&end, NULL);

	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;

	mtime = ((seconds) * 1000 + useconds / 1000.0);

	printf("Elapsed time: %ld milliseconds \n", mtime);

	printf("Outputting data read from FPGA.\n");

	writeImage("output/output1.pgm", out_stream1, width1, height1, 0);

	printf("Shutting down\n");

	return 0;

}
