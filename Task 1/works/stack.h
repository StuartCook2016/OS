#define DEFAULT_STACK_SIZE 20

extern void setStackSize( int size); 
extern int getStackSize();
extern void deleteStack();
extern int top();
extern int isEmpty();	
extern int isFull();
extern int pop(int* val);
extern int push(int val);
