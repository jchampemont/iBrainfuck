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

int main(int argc, char** argv)
{
	if(argc == 1 || argc > 2) /* Only accept 1 argument : filename or -v for version information. */
	{
		printf("Usage : \"ibrainfuck file.bf\"\n\"ibrainfuck -v\" for version information\n");
		return EXIT_SUCCESS;
	}
	if(strcmp(argv[1],"-v") == 0)
	{
		printf("ibrainfuck version 1.0b2\n");
	}
	else
	{
		FILE* sourceFile = NULL;
		BFmem mem;
		int brackets = 0;
		if((mem.memory = (char*) calloc(30000,sizeof(char))) == NULL) /* Brainfuck memory */
		{
			printf("Unable to allocate enough memory for BrainFuck memory...aborting\n");
			return EXIT_FAILURE;
		}
		mem.ptr = mem.memory;/* Initially, the pointer points to the leftmost memory value */
		mem.instructionPointer = 0;
		if((sourceFile = fopen(argv[1],"r")) == NULL) /* Openning file */
		{
			printf("Unable to open file...aborting\n");
			return EXIT_FAILURE;
		}
		fseek(sourceFile, 0, SEEK_END);
		mem.size = ftell(sourceFile);
		fseek(sourceFile, 0, SEEK_SET);
		if((mem.program = (char*) malloc(mem.size)) == NULL)
		{
			printf("Unable to allocate enough memory for source code...aborting\n");
			return EXIT_FAILURE;
		}
		fread(mem.program, mem.size, 1, sourceFile);
		fclose(sourceFile);
		while(mem.instructionPointer < mem.size)
		{
			if(mem.program[mem.instructionPointer] == '[')
				brackets++;
			else if(mem.program[mem.instructionPointer] == ']')
				brackets--;
			mem.instructionPointer++;
		}
		mem.instructionPointer = 0;
		if(brackets != 0)
		{
			printf("Unmatching bracket(s) found…aborting\n");
			return EXIT_SUCCESS;
		}
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
				if(mem.ptr > mem.memory + 30000)
				{
					printf("Memory pointer out of bound...aborting\n");
					return mem;
				}
				break;
			case '<':
				mem.ptr--;
				if(mem.ptr < mem.memory)
				{
					printf("Memory pointer out of bound...aborting\n");
					return mem;
				}
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
						else if(mem.program[mem.instructionPointer] == ']')
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
						else if(mem.program[mem.instructionPointer] == '[')
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
