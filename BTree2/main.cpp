

#include <iostream>
#include "btree.h"
#include "tempfunctions.h"
#include "mymap.h"
#include "mymultimap.h"
#include "mytable.h"
#include "record.h"
#include <vector>
#include <iomanip>

#include "bptree.h"
#include "stringtokenclass.h"
#include "parser.h"

#include "database.h"

using namespace std;

using namespace std;

MMap<string, long> get_word_indices(char* file_name);




void testTable();
void testBPTree();
void stringTest();
void testParser();

//int main()
//{
//    MMap<int, int> test;

//    for(int i = 0; i < 20; i++){
//        for(int j = 0; j < 5; j++)
//            test.insert(i, i);
//    }
////    for(int i = 0; i < 20; i++){
////        for(int j = 0; j < 5; j++)
////            test.insert(i, i);
////    }

//    MMap<int, int>::Iterator it(test.begin());
//    cout << test << endl;

//    while(!it.is_null()){
//        cout << "first value is " << *it<< endl;
//        it++;
//    }

//    MMap<int,int>::Iterator start, end;
//    start = test.lower_bound(7);
//    end = test.upper_bound(15);

//    cout << "testing lower bound\n\n";

//    while(start != end){
//        cout << *start << endl;
//        start++;
//    }
//    return 0;
//}


int main()
{
    cout << "start tests\n";

    string input, read;
    Database* test = new Database;
    char line[301];

    do{
    cout << "Hello!\nPlease input \"batch\" to run a batch file.\n"
         << "Please input \"command\" to input into the command line\n"
         << "Input \"new\" to create a new and fresh database program\n"
         << "Please input \"exit\" to exit the program\n";
        cin >> input;
        if(input == "batch")
        {
            cout << "please input batch name\n";
            cin >> read;
            test->batch(read);
            cout << "\n\nBatch reading is complete\n\n";
        }
        else if (input == "command")
        {
            cin.ignore();
            cout << "please input command: ";
           cin.getline(line, 300);
            cout << "\n\nProcessing Command:\n\n";
            test->command_line(line);
            cout << "\n\nCommand line processed\n";
        }
        else if (input == "new")
        {
            delete test;
            test = new Database;
        }
        else if (input == "exit"){
            cout << "exiting\n";
        }
    } while(input != "exit");

    cout << endl << endl << "end of program\n\n";
    return 0;
}

void stringTest()
{
    char s[] = ">=<=><";
    char j[] = "please god tell me this works.ad.wa.d..~~~";
    STokenizer stk(s);
    Token t;
    //The all too familiar golden while loop:
    stk>>t;
    while(stk.more()){
        //process token here...
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;


        t = Token();
        stk>>t;
    }
}

void testBPTree()
{
    BPTree<int> temp;
    for(int i = 1; i < 5; i++)
        temp.insert(i);
    cout << temp << endl;
    BPTree<int>::Iterator it, end;
    it = temp.begin();
    end = temp.end();
    cout << *end << endl;
    while(it != end){
        cout << *it << endl;
        it++;
    }
    cout << *it << endl;
    it = temp.upper_bound(1);
    while(it != end){
        cout << *it << endl;
        it++;
    }
    cout << *it << endl;


//    BPTree<int> temp2(temp);

//    cout << temp2 << endl;
//    BPTree<int>::Iterator it2, end2;
//    it2 = temp2.begin();
//    end2 = temp2.end();
//    cout << *end2 << endl;
//    while(it2 != end2){
//        cout << *it2 << endl;
//        it2++;
//    }
//    cout << *it2 << endl;

//    cout << "upperbound of 4\n";
//    it = temp.upper_bound(4);
//    cout << "it = " << *it << endl;

//    cout << "original table = \n\n";
//    cout << temp << endl << endl;
//    temp.remove(3);
//    cout << "changed table = \n";
//    cout << temp << endl;
//    cout << "testing copy constructor\n";
//    BPTree<int> temp2(temp);
//    cout << temp2 << endl;
}

void testParser()
{
    bool ret;
//    Parser temp("insert into student values Calvin,                 Woo,       Physics,22,        Uber");
    Parser temp("insert into employee values Blow,       Joe,             CS,         100000,    2018");
//    Parser temp("make table employee fields last, first, dep, salary, year");
//    Parser temp("make table employee fields  last,       first,                 dep,  salary,    year");
    cout << "constructor done\n";
    ret = temp.get_parse_tree();
    cout << "ret = " << ret << endl;
    cout << temp.parse_tree() << endl << endl;

    temp.setNewString("insert into employee values Blow,       Joe,             CS,         100000,    2018");

    temp.get_parse_tree();
    cout << "--------------SECOND PARSE TREE---------------\n\n";
    cout << temp.parse_tree() << endl;
    cout << "---------------end parse test------------------\n\n";
}

void testTable()
{
    vectorstr insertTarget, fields;
    insertTarget.push_back("Edward");
    insertTarget.push_back("tatulian");
    insertTarget.push_back("God");
    fields.push_back("Record");
    fields.push_back("First");
    fields.push_back("Last");
    fields.push_back("Role");

    string firstNames[8] = {"John", "Ray", "Hunter", "Dave", "Ric",
                            "Chris", "Brock", "Charlotte"};
    string lastNames[8] = {"Cena", "Mysterio", "Helmsley",
                           "Batista", "Flair", "Jericho", "Lesnar", "Flair"};
    string orientation[8] = {"Face", "Face", "Heel", "Heel",
                             "Heel", "Heel", "Heel", "Heel"};
    // Fields for testing
    vectorstr list[8];




    for(int i = 0; i < 8; i++){
        list[i].push_back(to_string(i + 1));
    }
    for(int i = 0; i < 8; i++){
        list[i].push_back(firstNames[i]);
        list[i].push_back(lastNames[i]);
        list[i].push_back(orientation[i]);
    }
    cout << "setting tables\n";


    // Testing new table constructor
    Table t1("this_file", fields);
    cout << "displaying empty table\n\n";
    cout << t1 << endl;
    cout << "\n\nfields = " << t1.get_fields() << endl;

    cout << "\n================================\n";

    cout << "Testing new Table get_fields" << endl << endl;
    cout << t1.get_fields() << endl;

    cout << "\n================================\n";


    cout << "Inserting values into Table" << endl << endl;

    t1.insert(list[0]);

    cout << "Printing Table" << endl << endl;

    cout << "starting print\n";

    cout << t1 << endl;

    cout << "print ended\n";

    cout << "\n================================\n";

    cout << "inserting rest\n";

    for(int i = 1; i < 8; i++){
//        cout << list[i] << endl;
//        cout << "\n\nfinished insert " << i << endl << endl;
        t1.insert(list[i]);
    }



    cout << "Printing Table" << endl << endl;

    cout << t1 << endl;

    cout << "\n================================\n";

    cout << "testing copy constructor\n\n";
    Table testConst(t1);

    cout << "constructor worked, testing now\n";

    cout << testConst << endl << endl;

    cout << "\n================================\n";

    vectorstr col;
    col.push_back("Last");
    col.push_back("First");
    vectorstr row;
    row.push_back("Last");
    row.push_back("=");
    row.push_back("Flair");
    cout << "row1 = " << row[1] << endl;

    cout << t1.select(col, row) << endl;

    cout << "select tab\n";

    cout << "\n================================\n";

    cout << "testing select on constructor\n";

    cout << testConst.select(col, row) << endl;
    cout << "\n================================\n";


    cout << "end\n\n";

}


//int main(int argc, char *argv[])
//{

//    MMap<string, long> word_indices;
//    cout << "main started\n";
////    word_indices = get_word_indices("solitude.txt");
//    cout << "main started\n";
//    for(int i = 0; i < 20; i++){
//        word_indices.insert(to_string(i), i);
//    }

//    //list all nodes of the index mmap:
//    for (MMap<string, long>::Iterator it = word_indices.begin();
//         it != word_indices.end(); it++){
//        cout<<*it<<endl;
//    }


//    cout<<endl<<endl<<endl;
//    cout<<"---------------------------------------------------"<<endl;
//    string this_word = "5";
//    cout<<"---------------------------------------------------"<<endl;
//    cout<<"Indices of \""<<this_word<<"\""<<endl;
//    //list indices of this_word:
//    if (word_indices.contains(this_word)){
//        cout<<this_word<<": "<<word_indices[this_word]<<endl;
//    }
//    cout<<endl<<endl<<endl;

//    cout<<"---------------------------------------------------"<<endl;
//    string from = "10";
//    string to = "30";
//    //list from .. to:
//    cout<<"listing indices from \""<<from<<"\" to \""<<to<<"\""<<endl;
//    cout<<"---------------------------------------------------"<<endl;
//    for (MMap<string, long>::Iterator it =
//                word_indices.lower_bound(from);
//         it != word_indices.upper_bound(to) &&
//         it != word_indices.end(); it++){
//        cout<<*it<<endl;
//    }

////    cout << word_indices.greaterThanKey(from);

//    cout <<endl<<endl<<endl<< "========== E N D  ====================" << endl;
//    return 0;
//}


//MMap<string, long> get_word_indices(char* file_name){
//    const bool debug = false;
//    MMap<string, long> word_indices;
//    FTokenizer ftk("solitude.txt");
//    Token t;
//    long count = 0;

//    ftk >> t;
//    while (ftk.more()){
//        //only the "words"
//        if (t.type_string() == "ALPHA"){
//            string s;
//            s = t.token_str();
//            word_indices[s] += count;
//            count++;
//            if (debug)
//                cout<<"|"<<t.token_str()<<"|"<<endl;
//        }
//        ftk >> t;
//    }
//    return word_indices;
//}
