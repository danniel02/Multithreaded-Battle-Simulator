#include "defs.h"



/*
  Function:  initDeque
  Purpose:   Initializes an empty Deque.
       in:   an Uninitialized Deque
*/
void initDeque(DequeType **deq){
    (*deq) = malloc(sizeof(DequeType));
    (*deq)->head = NULL;
    (*deq)->tail = NULL;
}


/*
  Function:  addLast
  Purpose:   Adds a fighter to the end of a specified deque
       in:   FighterType.
*/
void addLast(DequeType *deq, FighterType *fighter){
    //Dynamically allocate a new node
    NodeType *newNode = malloc(sizeof(NodeType));
    newNode->data = fighter;

    //If deque is empty set head and tail to newNode
    if(deq->tail == NULL){
        deq->head = newNode;
        deq->tail = newNode;
    //Else add to tail.next and update the tail
    }else{
        deq->tail->next = newNode;
        deq->tail = newNode;
        deq->tail->next = NULL;
    }
}


/*
  Function:  addFirst
  Purpose:   Adds a fighter to the front of a specified deque
       in:   FighterType.
*/
void addFirst(DequeType *deq, FighterType *fighter){
    //Dynamically allocate a new node
    NodeType *newNode = malloc(sizeof(NodeType));
    newNode->data = fighter;

    //If deque is empty, set head and tail to new node
    if(deq->head == NULL){
        deq->head = newNode;
        deq->tail = newNode;
    //else add before head and update head
    }else{
        newNode->next = deq->head;
        deq->head = newNode;
    }
}


/*
  Function:  removeFirst
  Purpose:   Removes a fighter from the front of a deque
       out:   FighterType.
*/
void removeFirst(FighterType *fighter, DequeType *deq){
    //If the deque is empty do nothing
    if(deq->head == NULL){
    //If only one node exists in the deque, set head and tail to NULL
    }else if(deq->tail == deq->head){
        fighter = deq->head->data;
        deq->head = NULL;
        deq->tail = NULL;
    //Else update head and remove old head
    }else{
        NodeType *newNode = malloc(sizeof(NodeType));
        fighter = deq->head->data;
        newNode = deq->head;
        deq->head = deq->head->next;
        free(newNode->data);
        free(newNode);
    }
}


/*
  Function:  removeLast
  Purpose:   Removes a fighter from the end of a deque
       out:   FighterType.
*/
void removeLast(FighterType *fighter, DequeType *deq){
    //If deque is empty do nothing
    if(deq->tail == NULL){
    //if the deque only has one node, remove it and set head and tail to NULL
    }else if(deq->tail == deq->head){
        fighter = deq->tail->data;
        deq->head = NULL;
        deq->tail = NULL;
    //find second to last node, set to new tail, remove old tail
    }else{
        NodeType *newNode = malloc(sizeof(NodeType));
        fighter = deq->tail->data;
        newNode = deq->head;
        while(1){
            if(newNode->next == deq->tail){
                deq->tail = newNode;
                deq->tail->next = NULL;
                free(newNode->next->data);
                free(newNode->next);
                break;
            }
            newNode = newNode->next;
        }
    }
}


/*
  Function:  deepCopy
  Purpose:   creates a deep copy of the input source deque and outputs the new deque.
       in:   Source deque to be copied.
       out:   newDeque with copied contents of the source deque.
*/
void deepCopy(DequeType *source, DequeType *newDeque){
    NodeType *curNode = malloc(sizeof(NodeType));
    curNode = source->head;
    //Iterate through the source deque and copy every element to new Deque
    while(curNode != NULL){
        addLast(newDeque, curNode->data);
        curNode = curNode->next;
    }
}


/*
  Function:  printDeque
  Purpose:   prints a deque, Used for TESTING purposes only.
       out:   Printed deque.
*/
void printDeque(DequeType *deq){
    if(deq->head == NULL){
        printf("Deque is empty\n");
    }else{
        NodeType *curNode;
        curNode = deq->head;
        int counter = 1;
        printf("\nPrinting Deque:\n");
        while(curNode != NULL){
            printf("Pirate %d: strength: %d armour: %d health: %d\n",counter, curNode->data->strength, curNode->data->armour, curNode->data->health);
            counter++;
            curNode = curNode->next;
        }
    }
}


/*
  Function:  cleanupDeque
  Purpose:   Deallocates all allocated memory from a deque and frees the deque
       in:   Deque.
*/
void cleanupDeque(DequeType *deq){
    NodeType *curNode;
    NodeType *temp;
    curNode = deq->head;
    while(curNode != NULL){
        temp = curNode;
        curNode = curNode->next;
        free(temp->data);
        free(temp);
    }
    free(deq);
}


/*
  Function:  isEmpty
  Purpose:   Checks if the given deque is empty, returns 1 if it is, 0 if not.
       in:   deque to check if empty or not.
       out:   int 1 or 0 depedning on if empty.
*/
int isEmpty(DequeType *deq){
    if(deq->head == NULL){
        return 1;
    }
    return 0;
}