#include <stdio.h>
#include "spimcore.c"

void printIns(unsigned instruction)
{
    printf("%u", instruction);
}

int main()
{
    Halt = instruction_fetch(PC,Mem,&instruction);

	if(!Halt)
	{
    printIns(&instruction);
	}

    return 0;
}


