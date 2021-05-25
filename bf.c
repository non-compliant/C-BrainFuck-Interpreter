#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define LENGTH 65535

uint8_t is_operator(int c)
{
	switch(c)
	{
		case '+': return 1;
		case '-': return 1;
		case '>': return 1;
		case '<': return 1;
		case '[': return 1;
		case ']': return 1;
		case ',': return 1;
		case '.': return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc < 2) return -1;

	FILE *srccode;
	int *opcodes,
	operator,
	numopcodes,
	bracketcount;
	
	opcodes = (int *)malloc(1);
	numopcodes = 0;
	operator = 0;
	bracketcount = 0;
	srccode = fopen(argv[1], "r");

	if(srccode == NULL)
	{
		fprintf(stderr, "Unable to open file %s", argv[1]);
		return -1;
	}
	
	// Parser
	while(operator != EOF)
	{
		operator = fgetc(srccode);
		if(is_operator(operator))
		{
			opcodes[numopcodes++] = operator;
			opcodes = (int *)reallocarray(opcodes, numopcodes+1, sizeof(int));
		}
		if(operator == '[' || operator == ']') ++bracketcount;
		
	}

	if((bracketcount % 2) == 1)
	{
		fputs("Unbalanced brackets.\r\n", stderr);
		return -1;
	}
	
	// Evaluation
	int i, *ptr, count,
	tape[LENGTH], *loopstart;

	ptr = &tape[0];
	count = 0;

	memset(tape, 0, sizeof(tape)/sizeof(tape[0]));

	for(i = 0; i < numopcodes; ++i)
	{

		if(opcodes[i] == '+') ++*ptr;
		else if(opcodes[i] == '-') --*ptr;
		else if(opcodes[i] == '>')
		{
			if(ptr == &tape[LENGTH-1]) ptr = &tape[0];
			else ptr++;
		}
		else if(opcodes[i] == '<')
		{
			if(ptr == &tape[0]) ptr = &tape[LENGTH-1];
			else ptr--;
		}
		else if(opcodes[i] == '[')
		{
			if(*ptr == 0)
			{
				int brackets = 1;
				for(; brackets > 0; ++i)
				{
					if(opcodes[i] == '[') ++brackets;
					if(opcodes[i] == ']') --brackets;
				}
			}
			else
			{
				loopstart = (int *)reallocarray(loopstart, count+1, sizeof(int));
				loopstart[count++] = i;
			}
		}
		else if(opcodes[i] == ']')
		{
			if(*ptr != 0) i = loopstart[count-1];
			else loopstart = (int *)reallocarray(loopstart, --count, sizeof(int));
		}
		else if(opcodes[i] == ',') *ptr = getchar();
		else if(opcodes[i] == '.') putchar(*ptr);
	}
	
	return 0;
}
