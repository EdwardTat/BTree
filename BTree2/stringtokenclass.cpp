//currrent

#include "stringtokenclass.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

bool STokenizer::char_null_at(int index)
{
    return (_buffer[index] == '\0');
}

STokenizer::STokenizer()
{
    _buffer[0] = NULL;
    _pos = 0;
    make_table(_table);
}

STokenizer::STokenizer(const char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
    make_table(_table);
}

void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
}

int STokenizer::get_current_pos()
{
    return _pos;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
    for(int i = 0; i < MAX_ROWS; i++){
        _table[i][0] = 0;
    }
    // These for loops will handle letters.
    // 1 is the state for letters and is a save state.
    _table[1][0] = 1;
    for(int i = 0; i < 26; i++){
        _table[0][i + 'A'] = 1;
        _table[0][i + 'a'] = 1;
        _table[1][i + 'A'] = 1;
        _table[1][i + 'a'] = 1;
    }
    for(int i = 0; i < 10; i++){
        _table[0]['0' + i] = 1;
        _table[1]['0' + i] = 1;
    }
    _table[0]['\"'] = 2;
    _table[0]['*'] = 4;


    for(int j = 1; j < MAX_COLUMNS; j++){
        _table[2][j] = 2;
    }
    _table[2]['\"'] = 3;

    _table[0][','] = 4;

    _table[0]['>'] = 5;
    _table[0]['<'] = 5;
    _table[5]['='] = 4;
    _table[0]['='] = 4;
    // These loops are for numbers.
    // Numbers from State 0 will take you to state 2.
    // State 2 is a save state. Numbers from 2 will take you to
    //  state 4 which is also a save state. A period will
    //  take you to state 3 which is not a save state.
    // State 3 is not a save state. Periods in state 3
    //  will cause you to fail. Only numbers will allow
    //  you to move to state 4 which is a save state.
    _table[1][0] = 1;
    _table[3][0] = 1;
    _table[4][0] = 1;
    _table[5][0] = 1;

    // This is to handle punctuation


}



//true: there are no more tokens
bool STokenizer::done()
{
    return _pos > strlen(_buffer);
}

//true: there are more tokens
bool STokenizer::more()
{
    return _pos < strlen(_buffer) + 1;
}

Token::Token()
{
    _type = -1;
    _token = "EMPTY";
}


Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}



ostream& operator <<(ostream& outs, const Token& t)
{
    return outs << t._token;
}


int Token::type()
{
    return _type;
}


string Token::type_string()
{
    switch (_type){
    case 0:
        return "UNKNOWN";
    case 1:
        return "ALPHA";
    case 2:
    case 3:
    case 4:
        return "NUMBER";
    case 5:
        return "SPACE";
    case 7:
        return "PUNCTUATION";
    default:
        return "ERROR";
    }
}



string Token::token_str()
{
    return _token;
}

int STokenizer::get_token(int start_state, string& token)
{
    token.clear();
    string temp = "", tempOut = "";
    int currentState = start_state, saveState = start_state, tempPos = _pos;
    while(more()){
        // Eject, does not affect output unless input is invalid at the start.
        if(_buffer[tempPos] <= 0 || static_cast<int>(_buffer[tempPos]) >= MAX_COLUMNS){
            // handles first token being fail state
            if(currentState == start_state){
                token += _buffer[tempPos];
                return 0;
            }
            else{
                // For if fail state is not next token
                token += tempOut;
            }
            return saveState;
        }
        // handles if the next input is a failstate
        if(_table[currentState][_buffer[tempPos]] == -1){
            // Handles first token being a fail state
            if(currentState == start_state){
                token += _buffer[tempPos];
                return 0;
            }
            // Handles token besides first being fail state
            else
                token += tempOut;
            return saveState;
        }
        else{
            // Handles current value being a save state
            if(_table[_table[currentState][_buffer[tempPos]]][0] == 1){
                saveState = currentState = _table[currentState][_buffer[tempPos]];
                tempOut += temp + _buffer[tempPos];
                tempPos++;
                _pos = tempPos;
                temp.clear();
            } else{
                temp += _buffer[tempPos];
                currentState = _table[currentState][_buffer[tempPos]];
                tempPos++;
            }
        }
    }
    return -1;
}

string returnString(STokenizer& s, Token& t)
{
    int temp = s.get_token(0, t._token);
    t._type = temp;
    // This accounts for if the first value is a failure. ++ since there are
    //  no other machines to check.
    if(temp == 0)
        s._pos++;
    if(t._token.at(0) == '\"' && t._token.at(t.token_str().size() - 1) == '\"'){
        t._token.erase(0, 1);
        t._token.erase(t._token.size() - 1);
    }
    return t._token;
}

STokenizer& operator >> (STokenizer& s, Token& t){
    int temp = s.get_token(0, t._token);
    t._type = temp;
    // This accounts for if the first value is a failure. ++ since there are
    //  no other machines to check.
    if(temp == 0)
        s._pos++;
    if(t._token.at(0) == '\"' && t._token.at(t.token_str().size() - 1) == '\"'){
        t._token.erase(0, 1);
        t._token.erase(t._token.size() - 1);
    }
    return s;
}




