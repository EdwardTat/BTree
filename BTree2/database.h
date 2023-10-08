#ifndef DATABASE_H
#define DATABASE_H

#include "mytable.h"
#include "parser.h"
#include <iostream>
#include <fstream>
using namespace std;

class Database{
public:
    Database();
    void command_line(string command);
    void batch(string name);

private:
    Parser parser;
    Map<string, Table> data;
    int numOfTables;
    int commandNum;
};

void Database::batch(string name)
{
    int counter = 1;
    char line[300];
    int max = 300;
    fstream f;
    f.open(name);
    if(f.fail()){
        f.close();
        cout << "invalid fstream name\n";
        return;
    }
    f.getline(line, max);
    if(line[0] == '\0' || line[0] == '/'){
        cout <<  line << endl;
    }
    else
        command_line(line);
    counter++;
    while(!f.eof()){
        f.getline(line, max);
        if(line[0] == '\0' || line[0] == '/'){
            cout <<  line << endl;
        }
        else
            command_line(line);
    }
    f.close();
}

Database::Database()
{
    commandNum = 0;
    numOfTables = 0;
}

void Database::command_line(string command)
{
    //    cout << "command line called\n";
    //    cout << "command = " << command << endl;
    parser.setNewString(command.c_str());
    if(!parser.get_parse_tree())
        cout << "failed for command: " << command << endl;
    MMap<string, string> ptree = parser.parse_tree();

//    cout << "\n\n-------------PTREE------------\n\n";
//    cout << ptree << "\n\n---------------PTREE END---------------\n\n";

    if(ptree["command"][0] == "make")
    {
        //        cout << "make registered\n";
        data.insert(ptree["table name"][0], Table(ptree["table name"][0], ptree["fields"]));
        //        cout << "finished insert, now testing print\n";
        //        cout << data[ptree["table name"][0]] << endl;
        //        data[ptree["table name"][0]];
        //        cout << "make functional\n";
        parser.add_rows(ptree["fields"]);
        parser.add_table_name(data[ptree["table name"][0]].get_table_name());
        numOfTables++;
        //        cout << data[ptree["table name"][0]] << endl;;
    }
    else if (ptree["command"][0] == "insert" && numOfTables)
    {
        //        cout << "\n--------------insert called-------------\n\n";
        //        assert(ptree["table"].size() == data[ptree["table"][0]].get_fields().size());
        data[ptree["table name"][0]].insert(ptree["table"]);
        //        cout << "item being inserted: " << ptree["table"];
        //        data[ptree["table"][0]].insert(ptree["table"]);
        parser.add_columns(ptree["table"]);
        //        cout << data[ptree["table name"][0]] << endl;
        //        cout << data[ptree["table name"][0]] << endl;
        //        cout << "------------end of insert--------\n\n";
    }
    else if (ptree["command"][0] == "select" && numOfTables)
    {
        cout << "[" << commandNum << "] " << command << endl;
        commandNum++;
        //        cout << "ptable = " << endl << ptree << endl;
//        cout << "\n--------------select called-------------\n\n";
        vectorstr shanti, col;
        int fields_offset =  ptree["fields"].size() - ptree["operators"].size();
//        cout << "shanti length 1 = " << shanti.size() << endl;
        //        cout << "fields offset successful\n";
        for(int i = 0; i < fields_offset; i++)
            col.push_back(ptree["fields"][i]);
        if(col[0] == "*"){
            col = data[ptree["table name"][0]].get_fields();
        }
        if(ptree["operators"].size() > 0){
//            cout << "operators > 0\n";
            shanti.push_back(ptree["fields"][fields_offset]);
            shanti.push_back(ptree["table"][0]);
            shanti.push_back(ptree["operators"][0]);
            for(int i = 1, j = ptree["operators"].size(); i < j; i++)
            {
                shanti.push_back(ptree["fields"][i + fields_offset]);
                shanti.push_back(ptree["table"][i]);
                shanti.push_back(ptree["operators"][i]);
                shanti.push_back(ptree["mods"][i - 1]);
            }
        }




        //        cout << "shanti = " << shanti << endl << endl;
        //        cout << "col = " << col << "\n\n";
        cout << data[ptree["table name"][0]].select(shanti, col);
//        cout << "------------end of select--------\n\n";
    }
}

#endif // DATABASE_H

/*
    else if (ptree["command"][0] == "select")
    {
        //        cout << "ptable = " << endl << ptree << endl;
        cout << "\n--------------select called-------------\n\n";
        vectorstr shanti, col;
        int fields_offset =  ptree["fields"].size() - ptree["operators"].size();
        //        cout << "fields offset successful\n";
        for(int i = 0; i < fields_offset; i++)
            col.push_back(ptree["fields"][i]);
        //        cout << "cols pushed\n";
        //        cout << "cols = " << col << endl;
        if(col[0] == "*"){
            col = data[ptree["table name"][0]].get_fields();


        }
        if(ptree.contains("operators")){
            shanti.push_back(ptree["operators"][0]);
            cout << "operators present\n";
        }
        if(ptree.contains("table")){
        shanti.push_back(ptree["table"][0]);
            cout << "table present\n";
        }
        //            cout << "table pushed\n";
        //            cout << "ptree table pushed\n";
        if(ptree.contains("fields")){
        shanti.push_back(ptree["fields"][fields_offset]);
        //            cout << "fields offset pushed\n";




        //            cout << "operators pushed\n";
        //            cout << "operators size = " << ptree["operators"].size() << endl;

        for(int i = 1, j = ptree["operators"].size(); i < j; i++)
        {
            shanti.push_back(ptree["fields"][i + fields_offset]);
            shanti.push_back(ptree["table"][i]);
            shanti.push_back(ptree["operators"][i]);
            shanti.push_back(ptree["mods"][i - 1]);
        }
        //        cout << "shanti = " << shanti << endl << endl;
        //        cout << "col = " << col << "\n\n";
        cout << data[ptree["table name"][0]].select(shanti, col);
        cout << "------------end of select--------\n\n";
    }
     */
