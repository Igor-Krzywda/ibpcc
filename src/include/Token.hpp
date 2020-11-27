#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <map>

namespace tk{

enum id{
    END_FILE,
    PLUS,
    MINUS,
    MULT,
    DIV_WOQ,
    DIV_WQ,
    MOD,
    LSQBR,
    RSQBR,
    LPAREN,
    RPAREN,
    QTMARK,
    LT,
    GT,
    LEQ,
    GEQ,
    EQ,
    IS,
    DOT,
    COMMA,
    INT,
    FLOAT,
    STRING,
    ID_VAR,
    ID_METHOD,
    AND,
    OR,
    NOT,
    METHOD,
    RETURN,
    LOOP,
    FROM,
    TO,
    WHILE,
    UNTIL,
    IF,
    ELSE,
    THEN,
    END,
    OUTPUT,
    ADD_ITEM,
    GET_NEXT,
    RESET_NEXT,
    HAS_NEXT,
    PUSH,
    POP,
    ENQUEUE,
    DEQUEUE,
    IS_EMPTY,
    LENGTH,
};

class Token{
    public:
        std::string *attr;
        int id;
        Token(int id, std::string *attr);
};

const std::map<std::string, int> RESERVED_KEYWORDS = {
    {"div", DIV_WQ},
    {"mod", MOD},
    {"AND", AND},
    {"OR", OR},
    {"NOT", NOT},
    {"method", METHOD},
    {"return", RETURN},
    {"loop", LOOP},
    {"from", FROM},
    {"to", TO},
    {"while", WHILE},
    {"until", UNTIL},
    {"if", IF},
    {"else", ELSE},
    {"then", THEN},
    {"end", END},
    {"output", OUTPUT},
    {"addItem", ADD_ITEM},
    {"getNext", GET_NEXT},
    {"resetNext", RESET_NEXT},
    {"hasNext", HAS_NEXT},
    {"push", PUSH},
    {"pop", POP},
    {"enqueue", ENQUEUE},
    {"dequeue", DEQUEUE},
    {"isEmpty", IS_EMPTY},
    {"length", LENGTH}
};

int lookup_keyword(std::string lexeme);

std::string *tok_to_str(Token *token);
   
std::string *id_to_str(int id);


}
#endif
