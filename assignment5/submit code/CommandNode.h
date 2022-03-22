/*
 * Assignment 5
 * By: Trung Le and Roshini Malempati
 * Group 30
 */
#include <stdbool.h>

typedef struct command_struct {
    char *content;
    int index;
    struct command_struct* nextCommandPtr;
} CommandNode;

// basic linked list structure (one used per list)
typedef struct list_t{
    CommandNode* head;
} list_t;

void CreateCommandNode(CommandNode *thisNode, char *content, int ind, CommandNode *nextCommand);
void InsertCommand(CommandNode* newNode, CommandNode *headNode);
void PrintHeadNode(CommandNode* head);