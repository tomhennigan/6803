
#ifndef __IMAGE_H_INCLUDE
#define __IMAGE_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct _ImageBlock {
	uint8_t val;
	void * next;
} ImageBlock;

typedef struct _Image {
	ImageBlock * start;
} Image;

Image * ImageFromFile(char *);
void FreeImage(Image *);
ImageBlock * NewImageBlock(uint8_t);
void FreeImageBlocks(ImageBlock *);

#endif
