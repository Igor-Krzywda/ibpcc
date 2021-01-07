#ifndef AST_HPP
#define AST_HPP

#include "Token.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <list>

namespace ast{

enum ast_id{
    START,
    METHOD, METHOD_CALL, PARAM, RETURN,
    WHILE, FOR,
    IF, ELSE, COND, CMP,
    ASSIGN,
    BINOP, UN_MIN,
    NUM, STRING, 
    ID,
    ARR, ARR_DYN, ARR_ACC,
    STACK, QUEUE,
    STANDARD_METHOD, INPUT, OUTPUT
};

class AST{
public:
    int id;
    unsigned line_num;
    double val_num;
    std::string val_str;
    std::list<AST*> children;
    AST(tk::Token &token, int node_id, unsigned ln);
    AST(int node_id);
    void push_child(AST *child);
};

void print_tree(AST *root, int offset);

void delete_tree(AST *root);

std::string id_to_str(int id);

}
#endif
