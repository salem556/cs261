#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"
char *strdup(const char *s);
//struct definition for a call
struct Call {
    int ID;
    char *name;
    char *reason;
};

//struct definition for the call center
struct CallCenter {
    struct queue *callQueue;
    struct stack *answeredCalls;
    int callIDCounter;
};
// Function to receive a new call and add it to the queue
void receiveNewCall(struct CallCenter *center) {
    char name[100], reason[100];
    printf("Enter caller's name: ");
    scanf("%s", name);
    printf("Enter call reason: ");
    scanf("%s", reason);

    struct Call *newCall = (struct Call *)malloc(sizeof(struct Call));
    newCall->ID = ++(center->callIDCounter);
    newCall->name = strdup(name);
    newCall->reason = strdup(reason);

    queue_enqueue(center->callQueue, newCall);
    printf("The call has been successfully added to the queue!\n");
}


// Function to answer a call from the queue and add it to the stack
void answerCall(struct CallCenter *center) {
    if (queue_isempty(center->callQueue)) {
        printf("No more calls need to be answered at the moment!\n");
        return;
    }

    struct Call *answeredCall = (struct Call *)queue_dequeue(center->callQueue);
    stack_push(center->answeredCalls, answeredCall);
    printf("The following call has been answered and added to the stack!\n");
    printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n",
           answeredCall->ID, answeredCall->name, answeredCall->reason);
}



// Function to display the current state of the stack (answered calls)
void displayStack(struct CallCenter *center) {
    printf("Number of calls answered: %d\n", stack_size(center->answeredCalls));
    if (!stack_isempty(center->answeredCalls)) {
        struct Call *lastCall = (struct Call *)stack_top(center->answeredCalls);
        printf("Details of the last call answered:\n");
        printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n",
               lastCall->ID, lastCall->name, lastCall->reason);
    }
}



// Function to display the current state of the queue (calls to be answered)
void displayQueue(struct CallCenter *center) {
    printf("Number of calls to be answered: %d\n", queue_size(center->callQueue));
    if (!queue_isempty(center->callQueue)) {
        struct Call *firstCall = (struct Call *)queue_front(center->callQueue);
        printf("Details of the first call to be answered:\n");
        printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n",
               firstCall->ID, firstCall->name, firstCall->reason);
    }
}


int main() {
    struct CallCenter center;
    center.callQueue = queue_create();
    center.answeredCalls = stack_create();
    center.callIDCounter = 0;

    int option;
    do {
        printf("1. Receive a new call\n");
        printf("2. Answer a call\n");
        printf("3. Current state of the stack – answered calls\n");
        printf("4. Current state of the queue – calls to be answered\n");
        printf("5. Quit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                receiveNewCall(&center);
                break;
            case 2:
                answerCall(&center);
                break;
            case 3:
                displayStack(&center);
                break;
            case 4:
                displayQueue(&center);
                break;
            case 5:
                printf("Bye!\n");
                break;
            default:
                printf("Invalid option! Please choose again.\n");
        }
    } while (option != 5);

    // Free allocated memory
    while (!queue_isempty(center.callQueue)) {
        struct Call *call = (struct Call *)queue_dequeue(center.callQueue);
        free(call->name);
        free(call->reason);
        free(call);
    }
    queue_free(center.callQueue);

    while (!stack_isempty(center.answeredCalls)) {
        struct Call *call = (struct Call *)stack_pop(center.answeredCalls);
        free(call->name);
        free(call->reason);
        free(call);
    }
    stack_free(center.answeredCalls);

    return 0;
}