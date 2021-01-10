#include "include/Interpreter.hpp"

namespace IBPCI{

Interpreter::Interpreter(ast::AST *tree){
    this->tree = tree;
    call_stack = cstk::CallStack(tree);
}

void Interpreter::interpret(){
    for(auto *a : tree->children){
        switch(a->id){        
            case ast::ASSIGN: assign(a); break;
        }
    }
    ast::delete_tree(tree);
    call_stack.test();
}   

void Interpreter::error(std::string message, ast::AST *leaf){
    std::cout << "SEMANTIC ERROR at line " << leaf->token.line
        << ": " << message << std::endl;
    exit(1);
}

void Interpreter::error(std::string message, tk::Token *token){
    std::cout << "RUN-TIME error at line " << token->line
        << ": " << message << std::endl;
    delete token;
    exit(1);
}

void Interpreter::method_decl(ast::AST *root){
    if(methods.find(root->token.val_str) == methods.end()){
        methods.insert(std::make_pair(root->token.val_str, root));
    }else{
        error("Duplicate method declaration", root);
    }
}

void Interpreter::assign(ast::AST *root){
    std::string var_name = root->children[0]->token.val_str;
    tk::Token *in;
    ast::AST *rn = root->children[1]; 
    switch(rn->id){
        case ast::NUM: in = binop(rn); break;
        case ast::STRING: in = binop(rn); break;
        case ast::ID: in = binop(rn); break;
        case ast::UN_MIN: in = binop(rn); break;
        case ast::BINOP: in = binop(rn); break;
    }
    call_stack.push(var_name, in);
    delete in;
}

tk::Token *Interpreter::binop(ast::AST *root){
    if(root == nullptr) return nullptr;
    switch(root->id){
        case ast::NUM: return new tk::Token(root->token);
        case ast::STRING: return new tk::Token(root->token);
        case ast::ID: return new tk::Token(call_stack.peek(root->token.val_str, root));
        case ast::UN_MIN: return negative(binop(root->children[0]));
        case ast::BINOP:
            switch(root->token.id){
                case tk::PLUS: 
                    return add(binop(root->children[0]), binop(root->children[1]));
                case tk::MINUS:
                    return subtract(binop(root->children[0]), binop(root->children[1]));
                case tk::MULT:
                    return multiply(binop(root->children[0]), binop(root->children[1]));
                case tk::DIV_WOQ:
                    return divide(binop(root->children[0]), binop(root->children[1]));
                case tk::DIV_WQ:
                    return int_div(binop(root->children[0]), binop(root->children[1]));
                case tk::MOD:
                    return modulo(binop(root->children[0]), binop(root->children[1]));
            }
    }
    return nullptr;
}

bool Interpreter::check_types(tk::Token *l, tk::Token *r){
    std::string error_message;
    if(l->id == r->id) return true;
    else{
        error_message = "Incompatible types: " + tk::id_to_str(l->id) + 
            " and " + tk::id_to_str(r->id);
        delete l; error(error_message, r);
    }
    return false;
}

tk::Token *Interpreter::add(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::STRING){
        out = new tk::Token(l->val_str + r->val_str);
    }else{
        out = new tk::Token(l->val_num + r->val_num);
    }
    delete l; delete r;
    return out;
}

tk::Token *Interpreter::subtract(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::NUM)
        out = new tk::Token(l->val_num - r->val_num);
    delete l; delete r;
    return out;
}

tk::Token *Interpreter::multiply(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::NUM)
        out = new tk::Token(l->val_num * r->val_num);
    delete l; delete r;
    return out;

}

tk::Token *Interpreter::divide(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::NUM){
        if(r->val_num == 0){
            delete l; 
            error("Division by 0 is illegal", r);
        }
        out = new tk::Token(l->val_num / r->val_num);
    }
    delete l; delete r;
    return out;
}

tk::Token *Interpreter::int_div(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::NUM){
        if(r->val_num == 0){
            delete l;
            error("Division by 0 is illegal", r);
        }
        out = new tk::Token((int)l->val_num / (int)r->val_num);
    }
    delete l; delete r;
    return out;

}

tk::Token *Interpreter::modulo(tk::Token *l, tk::Token *r){
    check_types(l,r);
    tk::Token *out;
    if(l->id == tk::NUM){
        out = new tk::Token((int)l->val_num % (int)r->val_num);
    }
    delete l; delete r;
    return out;
}

tk::Token *Interpreter::negative(tk::Token *val){
    if(val->id != tk::NUM) error(("Cannot make negative value from " + tk::id_to_str(val->id)), val);
    tk::Token *out = new tk::Token(-(val->val_num));
    delete val;
    return out;
}

}
