#pragma once

#include <stdio.h>

typedef struct FileBuffer
{
	FILE *file;
	int buffer;
	struct FileBuffer *back;
} FileBuffer;

typedef struct TokenStream
{
	FileBuffer *fileBuffer;
	char *token;
} TokenStream;

void TokenStreamInit(TokenStream *this, const char *filePath, size_t tokenSize);
void TokenStreamNext(TokenStream *this);
