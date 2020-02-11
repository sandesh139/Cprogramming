#include <stdio.h>
#include <stdlib.h>


char *okVeritas = "My Computer Backup";
char *okSuccess = "completed";



/**
 * Type for individual stack entry
 */
struct stack_entry {
  char *data;
  struct stack_entry *next;
};

/**
 * Type for stack instance
 */
struct stack_t
{
  struct stack_entry *head;
  size_t stackSize;  // not strictly necessary, but
                     // useful for logging
};

/**
 * Create a new stack instance
 */
struct stack_t *newStack(void)
{
  struct stack_t *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stackSize = 0;
  }
  return stack;
}

/**
 * Make a copy of the string to be stored (assumes
 * strdup() or similar functionality is not
 * available
 */
char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

/**
 * Push a value onto the stack
 */
void push(struct stack_t *theStack, char *value)
{
  struct stack_entry *entry = malloc(sizeof *entry);
  if (entry)
  {
    entry->data = copyString(value);
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stackSize++;
  }
  else
  {
    // handle error here
  }
}

/**
 * Get the value at the
 of the stack
 */
char *getFirst(struct stack_t *theStack)
{
  if (theStack && theStack->head)
    return theStack->head->data;
  else
    return NULL;
}

/**
 * Pop the top element from the stack; this deletes both
 * the stack entry and the string it points to
 */
void pop(struct stack_t *theStack)
{
  if (theStack->head != NULL)
  {
    struct stack_entry *tmp = theStack->head;
    theStack->head = theStack->head->next;
    free(tmp->data);
    free(tmp);
    theStack->stackSize--;
  }
}


    void findVeritas(){
    struct stack_t myStack = {NULL, 0};
    FILE * fp;
    fp = fopen("Veritas System Recovery.log.txt" , "r");
    char singleLines[1000];
    int day,month,year;
    if(fp == NULL)
    {
        printf("\n Error in opening the file");
        exit(1);
    } else {
        printf("\n successfully read the file");
        }

    while (fgets(singleLines, 1000, fp) != NULL){

        push (&myStack, singleLines);
    }

    /*char *data;
    data = getFirst(&myStack);
    printf("this is what i want %s\n \n",data);
    pop(&myStack);
    data = getFirst(&myStack);
    pop(&myStack);
    */
    int boolValue = 1;

    while(boolValue){

        char *data;
        data = getFirst(&myStack);

        if(strstr(data, okVeritas) && strstr(data, okSuccess)){
            printf("\n \n Got it \n %s \n ", data);
            boolValue =0;
            int boolDate =1;
            int counter =0;
            int date =0;
            int dateCounter =0;
            int yearCounter =0;

            while(boolDate){
                char c = data[dateCounter];
                printf("\nthis is char = %c \n", c);
                if(data[dateCounter] == 47 || yearCounter ==4){
                    switch(counter){
                    case 0:
                        month =date;
                        date =0;
                        printf("this is month");
                        break;
                    case 1:
                        day = date;
                        date =0;
                        printf("this is day");

                        break;
                    case 2:
                        year = date;
                        date =0;
                        printf("this is date %d  %d  %d", month ,day, year);
                        boolDate = 0;
                        break;
                    }
                    counter++;
                }
                else {
                    date = date *10 +(c -'0');
                    if(counter ==2){
                        yearCounter++;
                    }

                }
                dateCounter++;
            }

        }
        pop(&myStack);
    }

    fclose(fp);
    }

int main()
    {
     findVeritas();
     return 0;
    }
