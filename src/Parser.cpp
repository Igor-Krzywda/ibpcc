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

ast::AST *Parser::parse(){
    ast::AST *root = expr();
    return root;
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
        case tk::ID_VAR: assignment(); break;
        case tk::METHOD: method(); break;
        case tk::INT: expr(); break;
        case tk::FLOAT: expr(); break;
        case tk::IF: if_statement(); break;
        case tk::LOOP: loop(); break;
        case tk::RETURN: return_statement(); break;
        case tk::ID_METHOD: method_call(); break;
        case tk::INPUT: input(); break;
        case tk::OUTPUT: output(); break;
        default: std::cout << "unexpected token: " <<
                 *tk::id_to_str(current_token->id); 
                 exit(1);
    }
    return 0;
}

ast::AST *Parser::input(){
    eat(tk::INPUT);
    eat(tk::ID_VAR);
    return 0;
}

ast::AST *Parser::output(){
    eat(tk::OUTPUT);
    factor();
    while(current_token->id == tk::COMMA){
        eat(tk::COMMA);
        factor();
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
    expr();
    eat(tk::TO);
    expr();
    return 0;
}

ast::AST *Parser::loop_while(){
    eat(tk::WHILE);
    comparison_list();
    return 0;
}

ast::AST *Parser::return_statement(){
    eat(tk::RETURN);
    expr();
    return 0;
}

ast::AST *Parser::method_call(){
    eat(tk::ID_METHOD);
    eat(tk::LPAREN);
    factor();
    while(current_token->id != tk::RPAREN){
        eat(tk::COMMA);
        factor();
    }
    eat(tk::RPAREN);
    return 0;
}

ast::AST *Parser::assignment(){
    eat(tk::ID_VAR);
    if(current_token->id == tk::LSQBR){
        array_element();
        if(current_token->id != tk::EQ){
            return 0;
        }
    }
    eat(tk::EQ);
    if(current_token->id == tk::LSQBR){
        array_initialization();
    }else{
        expr();
    }
    return 0;
}

ast::AST *Parser::array_initialization(){
    array_argument();
    while(current_token->id == tk::COMMA){
        eat(tk::COMMA);
        array_argument();
    }
    return 0; 
}

ast::AST *Parser::array_argument(){
    switch(current_token->id){
        case tk::INT: eat(tk::INT); break;
        case tk::FLOAT: eat(tk::FLOAT); break;
        case tk::STRING: eat(tk::STRING); break;
        case tk::LSQBR:
            eat(tk::LSQBR);
            array_initialization(); 
            eat(tk::RSQBR);
            break;
    }
    return 0;
}

ast::AST *Parser::array_element(){
    while(current_token->id == tk::LSQBR){
        eat(tk::LSQBR);
        factor();
        eat(tk::RSQBR);
    }
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
    ast::AST *node = term();
    tk::Token *token;
    if(current_token->id == tk::PLUS 
            || current_token->id == tk::MINUS){
        token = current_token;
        eat(current_token->id);
    }
    return new ast::AST(ast::BIN_OP, node, token, term());
}

ast::AST *Parser::term(){
    ast::AST *node = factor();
    tk::Token *token;
    if(current_token->id == tk::MULT ||
            current_token->id == tk::DIV_WQ ||
            current_token->id == tk::DIV_WOQ ||
            current_token->id == tk::MOD){
        token = current_token;
        eat(current_token->id);   
    }
        return new ast::AST(ast::BIN_OP, node, token, factor());
}

ast::AST *Parser::factor(){
    ast::AST *node;
    tk::Token *token = current_token;
    switch(current_token->id){
        case tk::INT:
            eat(tk::INT);
            return new ast::AST(token);
        case tk::FLOAT:
            eat(tk::FLOAT);
            return new ast::AST(token);
        case tk::STRING:
            eat(tk::STRING);
            return new ast::AST(token);
        case tk::ID_VAR:
            eat(tk::ID_VAR);
            if(current_token->id == tk::LSQBR){
                array_element();
            }else if(current_token->id == tk::DOT){
                eat(tk::DOT);
                eat(tk::STANDARD_METHOD);
                eat(tk::LPAREN);
                eat(tk::RPAREN);
            }
            break;
        case tk::LPAREN:
            eat(tk::LPAREN);
            node = expr();
            eat(tk::RPAREN);
            return node;
        case tk::ID_METHOD:
            method_call();
            break;
        default: break;
    }
    return 0; 
}

}
