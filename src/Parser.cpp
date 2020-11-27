#include "include/Parser.hpp"

namespace prs{
    
Parser::Parser(const lxr::Lexer &lexer) : lex(lexer){
    current_token = lex.get_next_token();
}

void Parser::eat(int token_id){
    if(current_token->id == token_id){
        current_token = lex.get_next_token();
        }else{
        std::cout << "unexpected token: " << 
            *tk::id_to_str(current_token->id) <<
            ", expected token: " << *tk::id_to_str(token_id) <<
            std::endl;
            exit(1);
    }   
}

void Parser::parse(){
    while(current_token->id != tk::END_FILE){
        switch(current_token->id){
            case tk::METHOD: method(); break;
        }   
    }
    eat(tk::END_FILE);
}

ast::AST *Parser::method(){
    eat(tk::METHOD);
    eat(tk::ID_METHOD);
    eat(tk::LPAREN);
    if(current_token->id == tk::ID_VAR){
        eat(tk::ID_VAR);
        while(current_token->id != tk::RPAREN){
            eat(tk::COMMA);
            eat(tk::ID_VAR);
        }
    }
    eat(tk::RPAREN);
    while(current_token->id != tk::END){
        statement();
    }
    eat(tk::END);
    eat(tk::METHOD);
    return 0;
}

ast::AST *Parser::statement(){
    switch(current_token->id){
        case tk::ID_VAR:
            eat(tk::ID_VAR);
            if(current_token->id == tk::EQ) assignment();
            else expr();
            break;
        case tk::INT: expr(); break;
        case tk::FLOAT: expr(); break;
        case tk::IF: if_statement(); break;
        case tk::LOOP: loop(); break;
        case tk::ID_METHOD: break;
        default: std::cout << "unexpected token: " <<
                 *tk::id_to_str(current_token->id); 
                 exit(1);
    }
    return 0;
}

ast::AST *Parser::if_statement(){
    eat(tk::IF);
    comparison_list();
    eat(tk::THEN);
    while(current_token->id != tk::END){
        if(current_token->id == tk::ELSE){
            eat(tk::ELSE);
            if(current_token->id == tk::IF){
                eat(tk::IF);
                comparison_list();
                eat(tk::THEN);
            }
        }
        statement();

    }
    eat(tk::END);
    eat(tk::IF);
    return 0;
}

ast::AST *Parser::loop(){
    eat(tk::LOOP);
    switch(current_token->id){
        case tk::ID_VAR: loop_for(); break;
        case tk::WHILE: loop_while(); break;
    }
    while(current_token->id != tk::END){
        statement();
    }
    eat(tk::END);
    eat(tk::LOOP);
    return 0;
}

ast::AST *Parser::loop_for(){
    eat(tk::ID_VAR);
    eat(tk::FROM);
    factor();
    eat(tk::TO);
    factor();
    return 0;
}

ast::AST *Parser::loop_while(){
    eat(tk::WHILE);
    comparison_list();
    return 0;
}

ast::AST *Parser::assignment(){
    eat(tk::EQ);
    expr();
    return 0;
}

ast::AST *Parser::comparison_list(){
    comparison();
    while(current_token->id == tk::AND ||
            current_token->id == tk::OR){
        if(current_token->id == tk::AND){
            eat(tk::AND);
            comparison();
        }else if(current_token->id == tk::OR){
            eat(tk::OR);
            comparison();
        }
    }
   return 0; 
}

ast::AST *Parser::comparison(){
    term();
    if(current_token->id == tk::LT ||
            current_token->id == tk::GT ||
            current_token->id == tk::LEQ ||
            current_token->id == tk::GEQ ||
            current_token->id == tk::IS){
        switch(current_token->id){
            case tk::LT: eat(tk::LT); term(); break;
            case tk::GT: eat(tk::GT); term(); break;
            case tk::LEQ: eat(tk::LEQ); term(); break;
            case tk::GEQ: eat(tk::GEQ); term(); break;
            case tk::IS: eat(tk::IS); term(); break;
        }
    }
    return 0;
}

ast::AST *Parser::expr(){
    term();
    while(current_token->id == tk::PLUS ||
            current_token->id == tk::MINUS){
        if(current_token->id == tk::PLUS){
            eat(tk::PLUS);
            term();
        }else if(current_token->id == tk::MINUS){
            eat(tk::MINUS);
            term();
        }
    }
    return 0;
}

ast::AST *Parser::term(){
    factor();
    while(current_token->id == tk::MULT ||
            current_token->id == tk::DIV_WQ ||
            current_token->id == tk::DIV_WOQ ||
            current_token->id == tk::MOD){
        switch(current_token->id){
            case tk::MULT: eat(tk::MULT); factor(); break;
            case tk::DIV_WQ: eat(tk::DIV_WQ); factor(); break;
            case tk::DIV_WOQ: eat(tk::DIV_WOQ); factor(); break;
            case tk::MOD: eat(tk::MOD); factor(); break;
            default: break;
        }
    }
    return 0;
}

ast::AST *Parser::factor(){
    switch(current_token->id){
        case tk::INT:
            eat(tk::INT);
            break;
        case tk::FLOAT:
            eat(tk::FLOAT);
            break;
        case tk::ID_VAR:
            eat(tk::ID_VAR);
            break;
        case tk::LPAREN:
            eat(tk::LPAREN);
            expr();
            eat(tk::RPAREN);
            break;
        default: break;
    }
    return 0; 
}

}
