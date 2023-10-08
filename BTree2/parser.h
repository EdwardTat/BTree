#ifndef PARSER_H
#define PARSER_H
#include "stringtokenclass.h"
#include "filetokenclass.h"
#include "myvector.h"
#include "mymultimap.h"
#include "mymap.h"
#include <cstdarg>

using namespace std;

typedef myVec<string> vectorstr;



class Parser
{
public:
    Parser();
    Parser(char everything[]);
    void setNewString(const char everything[]);
    MMap<string, string> parse_tree();
    bool get_parse_tree();
    bool fail();

    void add_table_name(string name);
    void add_rows(vectorstr fields);
    void add_columns(const vectorstr col);
private:
    void setupParserStateMachine();
    //    void get_column();

    void set_keywords();
    void set_sym(string sym);       // implement
    void set_sym(vectorstr sym);    // implement
    void setupTokens(const char arr[]);
    vectorstr tokens;
    int tokenspos;
    MMap<string, string> ptree;
    Map<string, int> keywords;
    bool failFlag;
private: const static int M_ROWS = 20, M_COLUMNS = 20;
    int _table[M_ROWS][M_COLUMNS];
    enum keys {
        ZERO, SELECT, FIELD, FROM, SYM, WHERE, OPERATOR, COMMA,
        MODS, TABLE_NAME, INSERT, INTO, MAKE, ASTERISK, WORD_TABLE,
        WORD_VALUES, WORD_FIELDS
    };

};

Parser::Parser()
{
    setupParserStateMachine();
    set_keywords();
    tokenspos = 1;
    tokens = vectorstr();
}

Parser::Parser(char everything[])
{
    setupParserStateMachine();
    setupTokens(everything);
    tokenspos = -1;
    set_keywords();
}

void Parser::setNewString(const char everything[])
{
    tokenspos = -1;
    tokens = vectorstr();
    setupTokens(everything);
    ptree = MMap<string, string>();
}

void Parser::add_columns(const vectorstr col)
{
    for(int i = 0; i < col.size(); i++)
    {
        for(int i = 0; i < col.size(); i++)
        {
            Pair<string, int> check (col[i], int());
            if(!keywords.contains(check))
                keywords.insert(col[i], SYM);

        }
    }
}

bool Parser::get_parse_tree()
{
    string temp;
    int val, lastState = 0;
//    cout << "tokenspos = " << tokenspos << endl;
    //        cout << "tokens.size() = " << tokens.size() << endl;
    //        cout << "tokenspos < tokens.size() = " << (tokenspos < static_cast<int>(tokens.size())) << endl;
//        cout << "tokens = " << tokens << endl;
    while(tokenspos < static_cast<int>(tokens.size()) - 1){
        //        cout << "while run\n";
        tokenspos++;
//        cout << "tokens[" << tokenspos << "] = " << tokens[tokenspos] << endl;
//        cout << "tokenspostokenspos = " << tokenspos << endl;
        //        if(tokens[tokenspos] == " "){
        //            val = SPACE;
        //            cout << "VAL IS SPACE\n";
        //        }
        //        else{
        //            cout << "token =|" << tokens[tokenspos] << "|\n";
        //                cout << "(val == MAKE) = " << (val == MAKE) << endl;
        //                cout << "val = " << val << endl;
        //                cout << "make = " << MAKE << endl;
        Pair<string, int> temp(tokens[tokenspos], int());
        if(keywords.contains(temp))
            val = keywords[tokens[tokenspos]];
        else{
            ptree =  MMap<string, string>();
            failFlag = true;
            return false;
        }


        //        }
        //        cout << "val = " << val << endl;


        //        MODS
        switch(val){
        case SELECT:
        case INSERT:
        case MAKE:
            //            cout << "called make\n";
            ptree["command"].push_back(tokens[tokenspos]);
            break;
        case FIELD:
            ptree["fields"].push_back(tokens[tokenspos]);
            break;
        case FROM:
        case INTO:
            break;
        case SYM:
            ptree["table"].push_back(tokens[tokenspos]);
            break;
        case WHERE:
            ptree["where"].push_back(tokens[tokenspos]);
            break;
        case OPERATOR:

            ptree["operators"].push_back(tokens[tokenspos]);
            if(tokens[tokenspos] != "=")
            {
            if(tokenspos == static_cast<int>(tokens.size()) - 1){
                ptree =  MMap<string, string>();
                failFlag = true;
                return false;
            }
            tokenspos++;

            ptree["table"].push_back(tokens[tokenspos]);

            lastState = 7;
            val = SYM;
            }

            break;
        case TABLE_NAME:
            ptree["table name"].push_back(tokens[tokenspos]);
            break;
        case MODS:
            ptree["mods"].push_back(tokens[tokenspos]);
            break;
        case ASTERISK:
            ptree["fields"].push_back(tokens[tokenspos]);
            break;
        case WORD_TABLE:
            if(tokenspos + 1 < static_cast<int>(tokens.size())){
                tokenspos++;
                ptree["table name"].push_back(tokens[tokenspos]);
            }
            break;
        case WORD_FIELDS:
            if(tokenspos + 1 >= static_cast<int>(tokens.size())){
                ptree =  MMap<string, string>();
                failFlag = true;
                return false;
            }
            tokenspos++;
            while(tokenspos < static_cast<int>(tokens.size())){
                if(tokens[tokenspos] != ","){
                    ptree["fields"].push_back(tokens[tokenspos]);
                }
                tokenspos++;
            }
            failFlag = false;
            return true;
        case WORD_VALUES:
            if(tokenspos + 1 >= static_cast<int>(tokens.size())){
                ptree =  MMap<string, string>();
                failFlag = true;
                return false;
            }
            tokenspos++;
            while(tokenspos < static_cast<int>(tokens.size())){
                if(tokens[tokenspos] != ","){
                    ptree["table"].push_back(tokens[tokenspos]);
                }
                tokenspos++;

            }
            //            cout << "called from parser\n";
            //            cout << "--------------ptree from parser--------------------\n\n";
            //            cout << ptree << endl;
            //            cout << "--------------ptree from end--------------------\n\n";
            failFlag = false;
            return true;

        }

        //        cout << "tokens = " << tokens << endl;
        //                cout << "tokens[i] = " << tokens[tokenspos] << endl;
        //                cout << "tokenpos = " << tokenspos << endl;
        if(_table[lastState][val] == -1){
            ptree =  MMap<string, string>();
            failFlag = true;
            return false;
        }
        lastState = _table[lastState][val];
        if(_table[lastState][0] == 1 && tokenspos == static_cast<int>(tokens.size()) - 1){
            //            cout << "success\n";
            failFlag = false;
            return true;
        }
        //        cout << "token pos " << tokenspos << " has finished\n";

    }
    ptree =  MMap<string, string>();
    failFlag = true;
    return false;
}

void Parser::setupTokens(const char s[])
{
    int counter = 0;
    STokenizer stk(s);
    string temp;
    Token t;
    while(stk.more())
    {
        counter++;
        temp = returnString(stk, t);
        if(t.type())
            tokens.push_back(temp);
        //        cout << "counter = " << counter << "\ntemp = " << temp << endl;
    }

    //        t = Token();
    //        temp = returnString(stk, t);
    //        if(t.type())
    //            tokens.push_back(temp);

}



MMap<string, string> Parser::parse_tree()
{
    return ptree;
}



void Parser::add_rows(vectorstr fields)
{
    for(int i = 0; i < fields.size(); i++)
    {
        keywords.insert(fields[i], FIELD);
    }
    //    cout << keywords << endl;
}

void Parser::add_table_name(string name)
{
    keywords.insert(name, TABLE_NAME);
}

void Parser::set_keywords()
{
    // TEST INSERTS
    //-------------------------------------------------------

    //    keywords.insert("employee", TABLE_NAME);


    //    keywords.insert("students", TABLE_NAME);
    //    keywords.insert("lname", FIELD);

    //    keywords.insert("authors", SYM);

    //    keywords.insert("Jones", SYM);
    //    keywords.insert("and", MODS);
    //    keywords.insert("fname", FIELD);

    //    keywords.insert("John", SYM);
    //    keywords.insert("Jim Bob", SYM);

    //    keywords.insert("first", FIELD);
    //    keywords.insert("second", FIELD);
    //    keywords.insert("third", FIELD);
    //-------------------------------------------------------
    keywords.insert("and", MODS);
    keywords.insert("or", MODS);

    keywords.insert(">", OPERATOR);
    keywords.insert(">=", OPERATOR);
    keywords.insert("<", OPERATOR);
    keywords.insert("<=", OPERATOR);
    keywords.insert("=", OPERATOR);

    keywords.insert("select", SELECT);

    keywords.insert("make", MAKE);

    keywords.insert("from", FROM);
    keywords.insert("insert", INSERT);
    keywords.insert("where",WHERE);

    keywords.insert(",", COMMA);

    keywords.insert("table", WORD_TABLE);
    keywords.insert("values", WORD_VALUES);
    keywords.insert("fields", WORD_FIELDS);
    keywords.insert("into", INTO);
    keywords.insert("*", ASTERISK);

}


//void Parser::get_column()
//{
//    //    keywords["select"] = SELECT;

//    keywords.insert("students", TABLE_NAME);
//    keywords.insert("lname", FIELD);

//    keywords.insert("authors", SYM);

//    keywords.insert("Jones", SYM);
//    keywords.insert("and", MODS);
//    keywords.insert("fname", FIELD);

//    keywords.insert("John", SYM);

//    keywords.insert("first", FIELD);
//    keywords.insert("second", FIELD);
//    keywords.insert("third", FIELD);


//}

//void Parser::get_column(const string key[], const int value[], const int& size)
//{
//    for(int i = 0; i < size; i++){
//        keywords.insert(key[i], value[i]);
//    }
//}

//void Parser::get_column(myVec<string> key, myVec<int> value)
//{
//    assert(key.size() == value.size());

//    for(int i = 0, j = static_cast<int>(key.size()); i < j; i++){
//        keywords.insert(key[i], value[i]);
//    }
//}



void Parser::setupParserStateMachine()
{

    for(int i = 0; i < M_ROWS; i++){
        for(int j = 0; j < M_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
    for(int i = 0; i < M_ROWS; i++)
    {
        //        cout << "run " << i << "\n";
        _table[i][0] = 0;
    }

    _table[0][SELECT] = 1;
    _table[1][FIELD] = 2;
    _table[1][ASTERISK] = 9;
    _table[9][FROM] = 3;
    _table[10][FIELD] = 2;
    _table[2][COMMA] = 10;
    _table[2][FROM] = 3;
    _table[3][TABLE_NAME] = 4;

    _table[4][0] = 1;

    //    for(int i = 0; i < M_ROWS; i++){
    //        _table[i][SPACE] = i;
    //    }

    _table[4][WHERE] = 5;
    _table[5][FIELD] = 6;
    _table[6][OPERATOR] = 7;
    _table[7][SYM] = 8;

    _table[8][0] = 1;

    _table[8][MODS] = 5;

    _table[0][MAKE] = 11;
    _table[11][WORD_TABLE] = 12;
    _table[12][WORD_FIELDS] = 13;

    _table[0][INSERT] = 14;
    _table[14][INTO] = 15;
    _table[15][TABLE_NAME] = 16;
    _table[16][WORD_VALUES] = 17;
    _table[17][0] = 1;



    //    for(int i = 0; i < M_ROWS; i++){
    //        for(int j = 0; j < M_COLUMNS; j++){
    //            cout << _table[i][j] << " ";
    //        }
    //        cout << endl;
    //    }




    //    cout << "max rows = " << M_ROWS << endl;
    //     cout << "M_COLUMNS = " << M_COLUMNS << endl;
    //     cout << "_table[1][0] = " << _table[1][0] << endl;
}


#endif // PARSER_H
