//Os And Concurnacy part 1
// Jamie Fraser (H00216091)
// Stuart Cook  (H00247313)
// 

#define DEFAULT_STACK_SIZE 12

//new
extern void makeStack();

extern void setStackSize( int size); 
extern int getStackSize();
extern void deleteStack();
extern int top();

//new
extern int isEmpty();	
//new
extern int isFull();

extern int pop(int* val);
extern int push(int val);
