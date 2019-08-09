#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

int stack[DEFAULT_STACK_SIZE];
static int *stackTop = &stack[0];




int getStackSize()
{
  return 0;
}

void setStackSize( int size)
{
}

void deleteStack()
{
}

int top()
{	
	return *stackTop;
}

int isEmpty()
{
	if(stackTop == &stack[0])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isFull()
{
	if(stackTop == &stack[DEFAULT_STACK_SIZE])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int pop( int *val)
{
	if(!isEmpty())
	{
	*val = *stackTop;
	stackTop--;
	return 1;
	}
	else
	{
	return 0;
	}
}

int push( int val)
{
	if(!isFull())
	{
	stackTop++;
	*stackTop = val; 
	return 1;
	}
	else
	{
	return 0;
	}
}
