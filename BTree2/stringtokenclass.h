#ifndef STRINGTOKENCLASS_H
#define STRINGTOKENCLASS_H
#include "myvector.h"
#include <iostream>
#include <cstring>
using namespace std;

class STokenizer;

typedef myVec<string> vectorstr;

const int MAX_ROWS = 8, MAX_COLUMNS = 128, MAX_BUFFER = 301;
class Token
{
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    friend STokenizer& operator >> (STokenizer& s, Token& t);
    friend string returnString(STokenizer& s, Token& t);
    // Returns token type
    int type();
    // returns token type as a string
    string type_string();
    // returns token
    string token_str();
private:
    string _token;
    int _type;
};

class STokenizer
{
public:
    STokenizer();
    STokenizer(const char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);

    int get_current_pos();

    bool char_null_at(int index);
    friend string returnString(STokenizer& s, Token& t);


private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    int get_token(int start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};


#endif // STRINGTOKENCLASS_H
