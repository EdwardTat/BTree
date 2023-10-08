#ifndef RECORD_H
#define RECORD_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <assert.h>
#include "myvector.h"

using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) ;
void open_fileW(fstream& f, const char filename[]);

typedef myVec<string> vectorstr;

class Record{
public:
    Record(){
        for (int i = 0; i<ROW_MAX; i++)
            record[i][0] = NULL;
        recno = fieldsNum = -1;
    }

    Record(vectorstr v){
        fieldsNum = v.size();
        for (int i = 0; i<ROW_MAX; i++)
            record[i][0] = NULL;
        for (int i=0; i<v.size(); i++)
            strcpy(record[i], v[i].c_str());
        recno = -1;
    }


    vectorstr get_fields();
    long write(fstream& outs);
    long read(fstream& ins, long recno);


    friend ostream& operator<<(ostream& outs,
                               const Record& r){
        for (int i = 0; i < r.fieldsNum; i++)
            outs << setw(COL_MAX + 1) << r.record[i];
        return outs;
    }
    const string operator[](const int& i) const{
//        cout << " i = " << i << endl << "fieldnum = " << fieldsNum << endl;
//        assert(i < fieldsNum);
        return string(record[i]);
    }
private:
    static const int ROW_MAX = 20;
    static const int COL_MAX = 20;
    int recno;
    int fieldsNum;
    char record[ROW_MAX][COL_MAX];
};


long Record::write(fstream &outs){
    //write to the end of the file.
    outs.seekg(0, outs.end);
    long pos = outs.tellp();

    //outs.write(&record[0], sizeof(record));
    outs.write(&record[0][0], sizeof(record));

    return pos;
}
long Record::read(fstream &ins, long recno){
    long pos= recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);


    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();

}
template <class T>
ostream& operator<<(ostream& outs, const myVec<T>& v){
    for (int i= 0; i<v.size(); i++)
        outs<<v[i]<<" ";
    return outs;
}
//ostream& operator<<(ostream& outs,
//                    const Record& r){
//    for (int i=0; i<r.ROW_MAX; i++)
//            outs<<r.record[i]<<endl;
//    return outs;
//}

vectorstr Record::get_fields()
{
    vectorstr out;
    for(int i = 0; i < fieldsNum; i++)
        out.push_back(string(record[i]));
    return out;
}

//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}



#endif // RECORD_H
