//Os And Concurnacy part 1
// Jamie Fraser (H00216091)
// Stuart Cook  (H00247313)
// 

#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include <stdio.h>


//initialiases the global variables 
static int *stack;
static int sizeOfStack = DEFAULT_STACK_SIZE;
static int *stackTop;
//int test;

// Make the stack with its allocated memory loaction and sets the stackTop
void makeStack()
{	
stack = malloc(DEFAULT_STACK_SIZE * sizeof(int));
stackTop = stack;
}

//returns the size of the stack 
int getStackSize()
{
return sizeOfStack;
}


//resests the size of the stack
void setStackSize( int size)
{

//checks if the new size is smaller that the current stack
	
	if(size < sizeOfStack)
	{
	printf("Could not resize \n");

	//optional for reizing to a lower size
	//while(size < *stackTop)
	//{
	//removes elements in order to match the resizeing of the stack;
	//pop(stackTop);
	//}
	}	
	else{	
	//resizes the stack and reallocates the memory to compenste for the increase or decrease in size
	stack = (int *)realloc(stack, size * sizeof(int));
	//sets size of stack to perameter thats passed into the meothed 
	sizeOfStack = size;
	}
}

//frees alocated memory then allocates new memory for the default stack size
void deleteStack()
{
printf("deleteing stack...");
free(stack);
sizeOfStack =  DEFAULT_STACK_SIZE;
makeStack();

}


//returns top of stack
int top()
{	
	return *stackTop;
}

//check if the top of the stack is less than or equal to the bottom
int isEmpty()
{
	if(*stackTop <= 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//check if the top of the stack is equal to the max size of the stack
int isFull()
{
	if(*stackTop == sizeOfStack)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//checks if the stack is not empty, then pops the top element from the stack
int pop( int *val)
{
 
	if(!isEmpty())
	{

	*val = *stackTop;
	stackTop--;
	//printf("%d\n", test);
	//test++;
	return 1;
	}
	else
	{
	return 0;
	}
}

//check the if the stack is not full, then pushes elements onto the top of the stack 
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
	
	//for infinte stack un commnet the stuff below, and change the return to 1

	//printf("increasing stack size \n");
	//int newsize = getStackSize() + 1;
	//setStackSize(newsize);
	//stackTop++;
	//*stackTop = val; 

	return 0;
	}
}
