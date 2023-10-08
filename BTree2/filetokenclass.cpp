#include "filetokenclass.h"

FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname);
    _pos = 0;
    _blockPos = 0;
    _more = true;
}

bool FTokenizer::more(){return _more;}

int FTokenizer::block_pos(){return _blockPos;}

int FTokenizer::pos(){return _pos;}

FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    t = f.next_token();
    return f;
}

Token FTokenizer::next_token()
{
    int firstPos;
    Token temp;
    if(_stk.done() && _more)
        _more = get_new_block();
    if(_stk.more()){
        firstPos = _stk.get_current_pos();
        _stk >> temp;
        _pos += _stk.get_current_pos() - firstPos;
    }
    return temp;
}

bool FTokenizer::get_new_block()
{
    char temp[301];
    if(!_f.eof()){
        _blockPos++;

        _f.read(temp, 300);
        _stk.set_string(temp);
    }
    return !(_f.eof() && _stk.done());
}
