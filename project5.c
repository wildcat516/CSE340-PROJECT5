#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

#define ALLOC(t) (t*) calloc(1, sizeof(t))

struct StatementNode* parse_body();
struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();
struct AssignmentStatement* parse_assign_stmt();
struct PrintStatement* parse_print_stmt();
struct StatementNode* parse_generate_intermediate_representation();

#define VAR_NUM 100
struct ValueNode* varTable[VAR_NUM];
int var_length = 0;

struct ValueNode* fndvarTab(char* c)
{
    int k;
    for (k = 0; k < var_length; k++)
    {
        if (strcmp(varTable[k]->name, c) == 0)
        {
            return varTable[k];
        }
    }
    return NULL;
}

struct PrintStatement* parse_print_stmt()
{
    struct PrintStatement* print_stmt;
    print_stmt = ALLOC(struct PrintStatement);

    ttype = getToken();    
    if (ttype == PRINT)
    {
        ttype = getToken();
        
        if (ttype == ID)
        {
            print_stmt->id = fndvarTab(token);
            
            ttype = getToken();
            if (ttype == SEMICOLON)
            {
                return print_stmt;
            }
        }
    }
}

struct AssignmentStatement* parse_assign_stmt()
{
    struct AssignmentStatement* assign_stmt;
    assign_stmt = ALLOC(struct AssignmentStatement);
    
    struct AssignmentStatement* next;

    ttype = getToken();
    if (ttype == ID)
    {    
        assign_stmt->left_hand_side = fndvarTab(token);
       
        ttype = getToken();
        if (ttype == EQUAL)
        {
            ttype = getToken();
            if (ttype == ID)
            {
                assign_stmt->operand1 = fndvarTab(token);
//                assign_stmt->operand1->value = atoi()
            }
            else if (ttype == NUM)
            {
                struct ValueNode* numNode;
                numNode = ALLOC(struct ValueNode);
                assign_stmt->operand1 = numNode;
                numNode->value = atoi(token);
            }
            
            ttype = getToken();
            if (ttype == PLUS | ttype == MINUS | ttype == MULT | ttype == DIV)
            {
                assign_stmt->op = ttype;
                ttype = getToken();
                
                if (ttype == ID)
                {
                    assign_stmt->operand2 = fndvarTab(token);
                }
                else if (ttype == NUM)
                {
                    struct ValueNode* numNode;
                    numNode = ALLOC(struct ValueNode);
                    assign_stmt->operand2 = numNode;
                    numNode->value = atoi(token);                    
                }
                
                ttype = getToken();
                if (ttype == SEMICOLON)
                {
                    return assign_stmt;
                }
                
            }
            else
            {
                if (ttype == SEMICOLON)
                {
                    return assign_stmt;
                }
            }
        }
    }
    return assign_stmt;    
}

struct StatementNode* parse_stmt()
{
    struct StatementNode* st;
    
    st = ALLOC(struct StatementNode);
    
    ttype = getToken();
    
//    debug("ttype: %d\n", ttype);
    if (ttype == IF)
    {
        st->type = IF_STMT;

        struct IfStatement* if_st;
        if_st = ALLOC(struct IfStatement);
//  st->if_stmt = parse_if_stmt();
        st->if_stmt = if_st;
        
        ttype = getToken();
        if (ttype == NUM)
        {
            struct ValueNode* numNode;
            numNode = ALLOC(struct ValueNode);
            if_st->condition_operand1 = numNode;
            if_st->condition_operand1->value = atoi(token);
        }
        else if (ttype == ID)
        {
            if_st->condition_operand1 = fndvarTab(token);
        }
        
        ttype = getToken();
        if (ttype == GREATER | ttype == LESS | ttype == NOTEQUAL)
        {
            if_st->condition_op = ttype;
        }
        
        ttype = getToken();
        if (ttype == NUM)
        {
            struct ValueNode* numNode;
            numNode = ALLOC(struct ValueNode);
            if_st->condition_operand2 = numNode;
            if_st->condition_operand2->value = atoi(token);
        }
        else if (ttype == ID)
        {
            if_st->condition_operand2 = fndvarTab(token);
        }

//  create no_op node        
        struct StatementNode* no_op;
        no_op = ALLOC(struct StatementNode);
        no_op->type = NOOP_STMT;

//  expected to be an iteration to find the lastNode in parse_body
        struct StatementNode* firstNode;
        firstNode = parse_body();
        if_st->true_branch = firstNode;
        struct StatementNode* lastNode = firstNode;
        while (lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = no_op;
        if_st->false_branch = no_op;
        
        st->next = no_op;
    }
    else if (ttype == ID)
    {
//    debug("ttype: %d\n", ttype);
        ungetToken();
        st->type = ASSIGN_STMT;
        
        st->assign_stmt = parse_assign_stmt();
    }
    else if (ttype == PRINT)
    {
        ungetToken();
        st->type = PRINT_STMT;
        
        st->print_stmt = parse_print_stmt();
    }
    else if (ttype == WHILE)
    {
        st->type = IF_STMT;
        
        struct IfStatement* if_st;
        if_st = ALLOC(struct IfStatement);
//  st->if_stmt = parse_if_stmt();
        st->if_stmt = if_st;
        
        ttype = getToken();
        if (ttype == NUM)
        {
            struct ValueNode* numNode;
            numNode = ALLOC(struct ValueNode);
            if_st->condition_operand1 = numNode;
            if_st->condition_operand1->value = atoi(token);
        }
        else if (ttype == ID)
        {
            if_st->condition_operand1 = fndvarTab(token);
        }
        
        ttype = getToken();
        if (ttype == GREATER | ttype == LESS | ttype == NOTEQUAL)
        {
            if_st->condition_op = ttype;
        }
        
        ttype = getToken();
        if (ttype == NUM)
        {
            struct ValueNode* numNode;
            numNode = ALLOC(struct ValueNode);
            if_st->condition_operand2 = numNode;
            if_st->condition_operand2->value = atoi(token);
        }
        else if (ttype == ID)
        {
            if_st->condition_operand2 = fndvarTab(token);
        }

//  create no_op node        
        struct StatementNode* no_op;
        no_op = ALLOC(struct StatementNode);
        no_op->type = NOOP_STMT;

//  expected to be an iteration to find the lastNode in parse_body
        struct StatementNode* firstNode;
        firstNode = parse_body();
        if_st->true_branch = firstNode;
        struct StatementNode* lastNode = firstNode;
        while (lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        
        struct StatementNode* gt;
        gt = ALLOC(struct StatementNode);

        lastNode->next = gt;
        
        struct GotoStatement* gt_stmt;
        gt_stmt = ALLOC(struct GotoStatement);
        
        gt->type = GOTO_STMT;
        gt->goto_stmt = gt_stmt;
        
        gt_stmt->target = st;
        
        if_st->false_branch = no_op;
        
        st->next = no_op;
    }
    
    return st;
}

struct StatementNode* parse_stmt_list()
{
    struct StatementNode* st;
    struct StatementNode* stl;
    
    ttype = getToken();
    
    debug("ttype1: %d\n", ttype);
    if (ttype == ID | ttype == PRINT | ttype == WHILE | ttype == IF | ttype == SWITCH)
    {
        ungetToken();    
        st = parse_stmt();
        
        ttype = getToken();
    debug("ttype2: %d\n", ttype);
        if (ttype == ID | ttype == PRINT | ttype == WHILE | ttype == IF | ttype == SWITCH)
        {
            ungetToken();
            stl = parse_stmt_list();
            
            if(st->type == IF_STMT)
            {
                st->next->next = stl;
            }
            else
            {
                st->next = stl;
            }
            return st;
        }
        else
        {
            ungetToken();
            return st;
        }
    }
}

struct StatementNode* parse_body()
{
    struct StatementNode* stl;
    
    ttype = getToken();
    if (ttype == LBRACE)
    {
        stl = parse_stmt_list();
    }
    ttype = getToken();
    if (ttype == RBRACE)
    {
        return stl;
    }
}

struct StatementNode* parse_generate_intermediate_representation()
{
    struct StatementNode* prog;
    prog = ALLOC(struct StatementNode);
    
    ttype = getToken();
//    printf("%d", ttype);
    while (ttype == ID)
    {
        if (varTable[var_length] == NULL)
        {
            varTable[var_length] = ALLOC(struct ValueNode);
        }
        varTable[var_length]->name = strdup(token);
        varTable[var_length]->value = 0;
        var_length++;
        
//        printf("%d", var_length);
        
        ttype = getToken();
        if (ttype == COMMA)
        {
            ttype = getToken();
        }
        else if (ttype == SEMICOLON)
        {
            break;
        }
    }
    ttype = getToken();
    if (ttype == LBRACE)
    {
        ungetToken();
        
        prog = parse_body();
        return prog;
    }
}


