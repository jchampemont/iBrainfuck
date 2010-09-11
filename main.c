/*
 * Copyright (C) 2010 Jean Champemont (jean.champemont@gmail.com)
 *
 * This file is part of iBrainfuck.
 * 
 * iBrainfuck is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * iBrainfuck is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with iBrainfuck.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHUNK 1024

typedef struct {
	char *ptr; /* Brainfuck pointer */
	char *memory; /* Brainfuck memory */
	char *program; /* Brainfuck source code */
	int size; /* Byte-size of source code */
	int instructionPointer; /*  */
} BFmem;

BFmem parseBF(BFmem mem);
/*
 * This function parse brainfuck recursively
 *
 * The parameter state is used with the following rule :
 * 0 for the initial call
 * 1 if the call is made after encountering a [
 * -1 if the call is made after encountering a ]
 *
 * This parameter is for internal use only.
 * Non recursive call of this function should set this parameter to 0
 *
 */

int read_file(FILE *fp, char **buf);
/* 
 * Read the contents of a file into a buffer.  Return the size of the file 
 * and set buf to point to a buffer allocated with malloc that contains  
 * the file contents.
 *
 */


int main(int argc, char** argv)
{
	if(argc == 1 || argc > 2) /* Only accept 1 argument : filename or -v for version information. */
	{
		printf("Usage : \"ibrainfuck file.bf\"\n\"ibrainfuck -v\" for version information\n");
		return EXIT_SUCCESS;
	}
	if(strcmp(argv[1],"-v") == 0)
	{
		printf("ibrainfuck version 1.0b1\n");
	}
	else
	{
		FILE* sourceFile = NULL;
		BFmem mem;
		mem.memory = (char*) calloc(30000,sizeof(char)); /* Brainfuck memory */
		mem.ptr = mem.memory;/* Initially, the pointer points to the leftmost memory value */
		mem.instructionPointer = 0;
		if((sourceFile = fopen(argv[1],"r")) == NULL) /* Openning file */
		{
			printf("Unable to open file...aborting\n");
			return EXIT_FAILURE;
		}
		mem.size = read_file(sourceFile, &(mem.program));
		parseBF(mem); /* Parsing */
	}
	return EXIT_SUCCESS;
}

BFmem parseBF(BFmem mem)
{
	int bfChar; /* Char readed from source code */
	while(mem.instructionPointer < mem.size) /* Parsing until end of source code buffer */
	{
		bfChar = mem.program[mem.instructionPointer];
		mem.instructionPointer++;
		switch(bfChar)
		{
			case '>':
				mem.ptr++;
				break;
			case '<':
				mem.ptr--;
				break;
			case '+':
				*(mem.ptr) = *(mem.ptr) + 1;
				break;
			case '-':
				*(mem.ptr) = *(mem.ptr) - 1;
				break;
			case '.':
				putchar((int)*(mem.ptr));
				break;
			case ',':
				*(mem.ptr) = getchar();
				break;	
			case '[':
				if(*(mem.ptr) == 0)
				{
					int bracketCount = 0;
					while(bracketCount != -1)
					{
						if(mem.program[mem.instructionPointer] == '[')
						{
							bracketCount++;
						}
						if(mem.program[mem.instructionPointer] == ']')
						{
							bracketCount--;
						}
						mem.instructionPointer++;
					}
				}
				break;
			case ']':
				if(*(mem.ptr) != 0)
				{
					int bracketCount = 0;
					mem.instructionPointer -= 2;
					while(bracketCount != -1)
					{
						if(mem.program[mem.instructionPointer] == ']')
						{
							bracketCount++;
						}
						if(mem.program[mem.instructionPointer] == '[')
						{
							bracketCount--;
						}
						mem.instructionPointer--;
					}
					mem.instructionPointer += 2;
				}
				break;
		}
	}
	return mem;
}

int read_file(FILE *fp, char **buf) 
{
	int n, np, r;
	char *b, *b2;
	n = 0;
	np = CHUNK;
	b = malloc(sizeof(char)*n);
	while ((r = fread(b, sizeof(char), CHUNK, fp)) > 0)
	{
		n += r;
		if (np - n < CHUNK)
		{
			np *= 2;                      /* buffer is too small, the next read could overflow! */
			b2 = malloc(np*sizeof(char));
			memcpy(b2, b, n * sizeof(char));
			free(b);
			b = b2;
		}
	}
	*buf = b;
	return n;
}
