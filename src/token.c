#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void FileBufferInit(FileBuffer *this, const char *filePath)
{
	fopen_s(&this->file, filePath, "rt");
	this->buffer = fgetc(this->file);
	this->back = NULL;
}

void TokenStreamInit(TokenStream *this, const char *filePath, size_t tokenSize)
{
	this->fileBuffer = malloc(sizeof(*this->fileBuffer));
	FileBufferInit(this->fileBuffer, filePath);
	this->token = malloc(tokenSize);
}

void TokenStreamPush(TokenStream *this, const char *filePath)
{
	FileBuffer *newBuffer = malloc(sizeof(*newBuffer));
	FileBufferInit(newBuffer, filePath);
	newBuffer->back = this->fileBuffer;
	this->fileBuffer = newBuffer;
}

void TokenStreamNext(TokenStream *this)
{
popped_state:
	while (isspace(this->fileBuffer->buffer))
	{
		this->fileBuffer->buffer = fgetc(this->fileBuffer->file);
	}
	char *t = this->token;
	if (this->fileBuffer->buffer != EOF)
	{
		if (this->fileBuffer->buffer == '"')
		{
			do
			{
				*t++ = this->fileBuffer->buffer;
				this->fileBuffer->buffer = fgetc(this->fileBuffer->file);
			} while (this->fileBuffer->buffer != '"');
			this->fileBuffer->buffer = fgetc(this->fileBuffer->file);
		}
		else if (isalnum(this->fileBuffer->buffer))
		{
			do
			{
				*t++ = this->fileBuffer->buffer;
				this->fileBuffer->buffer = fgetc(this->fileBuffer->file);
			} while (isalnum(this->fileBuffer->buffer));
		}
		else
		{
			*t++ = this->fileBuffer->buffer;
			this->fileBuffer->buffer = fgetc(this->fileBuffer->file);
		}
	}
	else
	{
		if (this->fileBuffer->back)
		{
			this->fileBuffer = this->fileBuffer->back;
			goto popped_state;
		}
	}
	*t = 0;
}
