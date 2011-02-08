/*
Image is a linked list structure, that contains uint8_t blocks which are read from a file. Each block is stored in a ImageBlock element, and 
*/

#include "Image.h"

Image * ImageFromFile(char * file_path)
{
	Image * image = malloc(sizeof(Image));
	
	FILE * fp = fopen(file_path, "r");
	
	if(fp == NULL) {
		fprintf(stderr, "fopen: couldn't read file %s\n", file_path);
		return image;
	}
	
	image->start = NewImageBlock((uint8_t)getc(fp));
	
	ImageBlock * currentBlock = image->start;
	uint8_t currentByte = (uint8_t)getc(fp);
	
	do {
		// Set currentBlock->next to a new ImageBlock, and make currentBlock point to the new ImageBlock.
		currentBlock = (currentBlock->next = NewImageBlock(currentByte));
		
		if(currentBlock == NULL) {
			fprintf(stderr, "malloc: out of memory for image\n");
			break;
		}
		
		currentByte = (uint8_t)getc(fp);
	} while(!feof(fp));
	
	fclose(fp);
	
	return image;
}

ImageBlock * NewImageBlock(uint8_t val)
{
	ImageBlock * r = malloc(sizeof(ImageBlock));
	if(r != NULL) {
		r->val = val;
		r->next = NULL;
	}
	return r;
}

void FreeImage(Image * image)
{
	FreeImageBlocks(image->start);
	free(image);
}

/*
	Reccursive function that calls itself to free all linked blocks.
*/
void FreeImageBlocks(ImageBlock * block)
{
	if(block->next != NULL) {
		FreeImageBlocks(block->next);
	}
	
	free(block);
}
