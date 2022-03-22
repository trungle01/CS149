#ifndef PROC_MANAGER_COMMANDNODE_C
#define PROC_MANAGER_COMMANDNODE_C

#include <string.h>
#include "CommandNode.h"

//create a new command node. usually nextCmd can be NULL and function InsertCommandAfter can be called to insert after head node.
void CreateCommandNode(CommandNode* thisNode, char cmd[20][20], int ind,  int count, CommandNode* nextCmd) {
    //this is useful if you store a string (char *): strcpy(thisNode->command, cmd);
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            thisNode->command[i][j] = cmd[i][j];
        }
    }
    thisNode->index = ind;
    thisNode->nextCommandPtr = nextCmd;
    thisNode->count = count;
    return;
}

//insert node newNode after thisNode
void InsertCommandAfter(CommandNode* thisNode, CommandNode* newNode) {
    CommandNode* tmpNext = NULL;

    tmpNext = thisNode->nextCommandPtr;
    thisNode->nextCommandPtr = newNode;
    newNode->nextCommandPtr = tmpNext;
    return;
}

//get next command node in linked list
CommandNode* GetNextCommand(CommandNode* thisNode) {
    return thisNode->nextCommandPtr;
}

//find a command based on the pid
CommandNode* FindCommand(CommandNode* cmd, int pid) {
    CommandNode* tmpNext = cmd;
    while (tmpNext != NULL) {
        if (tmpNext->PID == pid) { return tmpNext; }
        tmpNext = tmpNext->nextCommandPtr;
    }
    return NULL;
}

#endif //PROC_MANAGER_COMMANDNODE_C