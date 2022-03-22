#ifndef PROC_MANAGER_COMMANDNODE_H
#define PROC_MANAGER_COMMANDNODE_H

#include <stdbool.h>

typedef struct command_struct {
    char command[20][20];
    int index;
    int PID;
    int starttime;
    int finishtime;
    bool active;
    int count;
    struct command_struct* nextCommandPtr;
} CommandNode;


void CreateCommandNode(CommandNode* thisNode, char cmd[20][20], int ind, int count, CommandNode* nextCmd);
void InsertCommandAfter(CommandNode* thisNode, CommandNode* newNode);
CommandNode* GetNextCommand(CommandNode* thisNode);
CommandNode* FindCommand(CommandNode* cmd, int pid);

#endif //PROC_MANAGER_COMMANDNODE_H
