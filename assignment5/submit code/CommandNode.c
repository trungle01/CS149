/*
 * Assignment 5
 * By: Trung Le and Roshini Malempati
 * Group 30
 */
#include <string.h>
#include "CommandNode.h"

void CreateCommandNode(CommandNode *thisNode, char *content, int ind, CommandNode *nextCommand){
    thisNode->content = content;
    thisNode->index = ind;
    thisNode->nextCommandPtr = nextCommand;
    return;
}

void InsertCommand(CommandNode* newNode, CommandNode *headNode){
    CommandNode  *tmp;
    tmp = headNode;
    newNode->nextCommandPtr = tmp;
}

// print the contents of the linked list
void PrintHeadNode(CommandNode* thisNode) {
    if (thisNode != NULL) {
        printf("Head Node is changed. Content = : %s\n", thisNode->content);
    }

    return;
}