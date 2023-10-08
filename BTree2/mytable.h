#include "record.h"
#include "mymultimap.h"
#include "myvector.h"
#include "mymap.h"

#ifndef MYTABLE_H
#define MYTABLE_H

static int serial = 1;

class Table{
public:
    Table(){
        Record r;
        fstream f;

        vectorstr vec;

        fileName = "tempToBeDeleted.tbl";

        open_fileRW(f, fileName.c_str());
        f.close();

    }
    Table(const string& name, const myVec<string> fields);
    //    ~Table();
    Table(const Table& other);
    void cleanAfter(){deleteAtEnd = true;}
    void save(){deleteAtEnd = false;}
    //    void reindex();
    void insert(myVec<string> values);
    //    Table select(const vectorstr &code);
    Table select(vectorstr shanti, vectorstr col);
    const vectorstr& get_fields() const{return fieldNames;}
    string get_table_name();
    Table select_all(){Table(*this);}
    void insert_into(const vectorstr fields);
    Table tableFromcol(vectorstr col);

private:

    long lastRecord;
    int mySerial;
    string tableName, fileName;
    myVec<string> fieldNames;
    Map<string, long> fieldIndecies;
    myVec<MMap<string,long>> table;
    bool empty, deleteAtEnd;

    Table returnTable(vectorstr fields, myVec<long> data);
    BPTree<long> returnPositions(vectorstr cond);
    Table select_helper(const vectorstr& col, const vectorstr& row);
    BPTree<long> handle_and(BPTree<long> &out, BPTree<long> &temp);
    BPTree<long> handle_or(BPTree<long> &out, BPTree<long> &temp);



    void copy(const Table& other);
    void setFieldNamesAndFieldIndecies(const vectorstr &fields);



public:
    Table& operator=(const Table &other){
        copy(other);
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Table& source)
    {
        fstream f;
        Record reader(source.fieldNames);

//        out << "---------------------------\n\n";
        out << "Table Name : " << source.fileName;
        out << ", records: " << source.lastRecord << "\n";
        open_fileRW(f, source.fileName.c_str());

        for(long i = 0; i <= source.lastRecord; i++){
            reader.read(f, i);
            out << reader << endl;
        }
        out << "\n\n\n";
        return out;
    }
};


BPTree<long> Table::handle_or(BPTree<long>& out, BPTree<long>& temp)
{
    BPTree<long> result = temp;

    BPTree<long>::Iterator it = out.begin();

    while(!it.is_null())
    {
        if(!result.contains(*it))
            result.insert(*it);
        it++;
    }
    return result;
}

BPTree<long> Table::handle_and(BPTree<long>& out, BPTree<long>& temp)
{
    BPTree<long> result;

    BPTree<long>::Iterator it = out.begin();

    while(!it.is_null())
    {
        if(temp.contains(*it))
            result.insert(*it);
        it++;
    }
    return result;
}

string Table::get_table_name()
{
    return tableName;
}

Table::Table(const Table& other)
{
    copy(other);
}



BPTree<long> Table::returnPositions(vectorstr cond)
{
    BPTree<long> out;
    if (cond[2] == "="){
        for(int i = 0; i < table[(fieldIndecies[cond[0]])][cond[1]].size(); i++)
            out.insert(table[(fieldIndecies[cond[0]])][cond[1]][i]);
        return out;
    }
    else if(cond[2] == ">")
    {
        out = table[(fieldIndecies[cond[0]])].greaterThanKey(cond[1]);
        return out;
    }
    else if (cond[2] == ">=")
    {
        out = table[(fieldIndecies[cond[0]])].greaterThanOrEqualKey(cond[1]);
        return out;
    }
    else if (cond[2] == "<")
    {
        out = table[(fieldIndecies[cond[0]])].lessThanKey(cond[1]);
        return out;
    }
    else if(cond[2] == "<=")
    {
        out = table[(fieldIndecies[cond[0]])].lessThanOrEqualKey(cond[1]);
        return out;
    }
    else if(cond[2] == "=")
    {
        for(int i = 0, j = table[(fieldIndecies[cond[0]])][cond[1]].size();
            i < j; i++)
            out.insert(table[(fieldIndecies[cond[0]])][cond[1]][i]);
        return out;
    }
}

Table Table::tableFromcol(vectorstr col)
{
    Record r;
    fstream f;

    vectorstr vec;

    open_fileRW(f, fileName.c_str());

    Table out("_select_table_", col);

    for (unsigned i = 1; i <= lastRecord; i++)
    {
        r.read(f, i);
        for (unsigned j = 0; j < col.size(); j++){
            vec.push_back(r[fieldIndecies[col[j]]]);
        }
        out.insert(vec);
        vec = vectorstr();
    }
    f.close();
    return out;
}


Table Table::select(vectorstr shanti, vectorstr col)
{
    BPTree<long> out, temp;
    vectorstr info;
    bool inUse = false;
    int i = 0;

    for(int i = 0; i < col.size(); i++){
        Pair<string, long> colTester(col[i], long());
        if(!fieldIndecies.contains(colTester))
        return Table();
    }

    while(i < static_cast<int>(shanti.size())){
        if(shanti[i] == "and")
        {
            out = handle_and(out, temp);
            i++;
        }
        else if (shanti[i] == "or")
        {
            out = handle_or(out, temp);
            i++;
        }
        else
        {
            info.push_back(shanti[i]);
            info.push_back(shanti[i + 1]);
            info.push_back(shanti[i + 2]);
            i = i + 3;
            if(!inUse)
            {
                out = returnPositions(info);
                inUse = true;
                info = vectorstr();
            }
            else
            {
                temp = returnPositions(info);
                info = vectorstr();
            }
        }

    }

    Record r;
    fstream f;

    vectorstr vec;

    open_fileRW(f, fileName.c_str());

    Table result("_select_table_", col);

    if(shanti.size() == 0)
    {
        for (int i = 1; i <= lastRecord; i++)
        {
            r.read(f, i);
            for (unsigned j = 0; j < col.size(); j++){
                vec.push_back(r[fieldIndecies[col[j]]]);
            }
            result.insert(vec);
            vec = vectorstr();
        }

        f.close();
        return result;
    }

    BPTree<long>::Iterator it = out.begin();

    while(it != out.end() && (!it.is_null()))
    {
        r.read(f, *it);
        for (unsigned j = 0; j < col.size(); j++){
            vec.push_back(r[fieldIndecies[col[j]]]);
        }
        if(vec.size() && (!vec[0].empty())){
            result.insert(vec);
        }
        vec = vectorstr();
        it++;
    }
    if(!it.is_null())
    {
        r.read(f, *it);
        for (unsigned j = 0; j < col.size(); j++){
            vec.push_back(r[fieldIndecies[col[j]]]);
        }
        result.insert(vec);
        vec = vectorstr();
    }

    f.close();
    return result;
}


Table Table::select_helper(const vectorstr& col, const vectorstr& row)
{
    Record r;
    fstream f;

    myVec<long> positions;
    vectorstr vec;

    open_fileRW(f, fileName.c_str());

    Table out("temp_print", col);


    if (row.size() > 0)
    {
        if (row[1] == "="){
            positions = table[(fieldIndecies[row[0]])][row[2]];
        }

        for (unsigned i = 0; i < positions.size(); i++)
        {
            r.read(f, positions[i]);
            for (unsigned j = 0; j < col.size(); j++){
                vec.push_back(r[fieldIndecies[col[j]]]);
            }
            out.insert(vec);
            vec = vectorstr();
        }
    }

    f.close();

    return out;
}

Table Table::returnTable(vectorstr fields, myVec<long> positions)
{
    Record r;
    fstream f;

    vectorstr vec;

    open_fileRW(f, fileName.c_str());

    Table out("temp_print", fields);

    for (unsigned i = 0; i < positions.size(); i++)
    {
        r.read(f, positions[i]);
        for (unsigned j = 0; j < fields.size(); j++){
            vec.push_back(r[fieldIndecies[fields[j]]]);
        }
        out.insert(vec);
        vec = vectorstr();
    }
    f.close();
    return out;
}



void Table::insert(myVec<string> values)
{
    assert(fieldNames.size() == values.size());

    fstream file;
    Record r(values);
    open_fileRW(file, fileName.c_str());
    r.write(file);
    file.close();

    lastRecord++;

    for(int i = 0; i < fieldNames.size(); i++){
        table[i][values[i]] += lastRecord;
    }

    if(empty)
        empty = false;
}



void Table::copy(const Table& other)
{
    assert(file_exists(other.fileName.c_str()));
    mySerial = serial;
    serial++;

    tableName = other.tableName;
    fileName = tableName + to_string(mySerial) + ".tbl";
    fieldNames = other.fieldNames;
    lastRecord = other.lastRecord;
    table = other.table;
    empty = other.empty;
    deleteAtEnd = other.deleteAtEnd;
    fieldIndecies = other.fieldIndecies;
    table = other.table;

    Record r;
    fstream in, out;
    open_fileRW(in, other.fileName.c_str());
    open_fileW(out, fileName.c_str());

    //    cout << "worked1\n";
    for(long i = 0, val = r.read(in, i);val; i++, val = r.read(in, i))
    {
        r.write(out);
    }

    in.close();
    out.close();

}

void Table::setFieldNamesAndFieldIndecies(const vectorstr &fields)
{
    fieldNames = fields;
    for(int i = 0; i < fields.size(); i++){
        table.push_back(MMap<string, long>());
        fieldIndecies.insert(fields[i], i);
    }
}


Table::Table(const string& name, const myVec<string> fields)
{
    tableName = name;
    fileName = name + to_string(serial) + ".tbl";
    mySerial = serial;
    serial++;
    lastRecord = 0;

    setFieldNamesAndFieldIndecies(fields);

    Record temp(fields);
    fstream f;

    open_fileW(f, fileName.c_str());
    temp.write(f);

    deleteAtEnd = false;

    f.close();
}





#endif // MYTABLE_H
