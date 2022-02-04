#include <iostream>
#include <string>
#include <vector>
#include <hash.h>
#include <BTree.h>

using namespace std;

// this function will delete all unexpected space in our input!
string ignore_space(string str){
    string tmp = "";
    bool flag = false;
    bool startspace = false;
    for (int i=0;i<str.length() ;i++){
        if(!startspace){
            if(str[i]!=' '){
                startspace=true;
                tmp+=str[i];
            }
        }
        else if(str[i]=='('){
            flag=true;
            tmp+=str[i];
        }    
        else if(str[i] != ' ' && str[i]!=','){
            tmp += str[i];
            flag= false;
        }
        else if(str[i]==' ' && !flag) {   
            tmp+=str[i];
            flag = true;
        }   
        else if(str[i] ==','){
            tmp+=str[i];
            flag=true;
        }    
    }
    
    return tmp;
}

// this function check validation of timestamp.
bool valid_timestamp(string str){
    int i=0;
    for(int j=0;j<str.length();j++){
        if(str[j]=='/')
            i++;
    }
    if(i!=2)
        return false;
    
    i=0;
    while (str[i]!='/')
        i++;
    
    if(i!=4){
        return false;
    }
    i=str.length()-1;
    string tmp = "";
    while(str[i]!='/'){
        tmp += str[i];
        i--;
    }    

    
    tmp = reverse(tmp);
    int day = convert_to_int(tmp);

    // cout <<" day: " << tmp << endl;

    if(!(1<= day <= 31)|| tmp[0]=='0')
        return false;  
    i--;  
    tmp = "";
    while(str[i]!='/'){
        tmp += str[i];
        i--;
    }
    
    tmp = reverse(tmp);
    int month = convert_to_int(tmp);

    if(month>12 || month<1 || tmp[0]=='0')
        return false;    

    // cout << "month " << tmp<< endl;  

    return true;         
}

// this function check the input type is string or not.
bool valid_string(string str){
    if(str[0] == '"' && str[str.length()-1]=='"')
        return true;
    return false;
}

// i defined int = 0 and string = 1 and timestamp = 2.
int determine_type_data(string type){
    try{
        if(type == "int")
            return 0;
        if(type=="string")
            return 1;
        else if (type=="timestamp")
            return 2;
        else
            throw type; 
    }catch(...){
        cout << "Invalid Type ERROR!! status: 400 , msg:BadRequest " << endl;
    }
    return -1;
}

// in this function i will determine the condition is on what column.
string colNameOfCondition(string condition){
    int i=0;
    string res ="";
    while(condition[i]!= '=' && condition[i]!='>' && condition[i]!='<'){
        res+=condition[i];
        i++;
    }
    return res;
}

// we define == = 0 and > = 1 and < = 2.
int TypeOfCondition(string condition){
    for(int i=0;i<condition.length();i++){
        if(condition[i]=='=')
            return 0;
        else if(condition[i]=='>')
            return 1;
        else if(condition[i]=='<')
            return 2;        
    }
    return 0;
}

string FindLimitation(string condition){
    string res = "";
    int i= condition.length()-1;
    while(condition[i]!= '=' && condition[i]!='>' && condition[i]!='<'){
        res+=condition[i];
        i--;
    }
    return res;
}

// CREATE TABLE {table name} (column1 type,column2 type,...)
// CREATE TABLE employee (name string,joinDate timestamp,income int)
void CreateTableQuery(string query, vector<int>& v_dataType, vector<string>& v_columnName , string& tableName,vector<Btree<long long>>& bt){
    try{
        string tmp = query.substr(0,13);

        if(tmp != "CREATE TABLE ")
            throw query;
        
        int i=13;
        while (query[i]!= ' ')
        {
            tableName+=query[i];
            i++;
        }
        // cout << tableName << endl;
        // i++;

        string str= "";
        for(int j=0;j<query.length();j++){
            if (query[j]=='('){
                str = query.substr(j);
                break;
            }    
        }
        // cout << str << endl;
        i=0;
        while(i<str.length()){
            i++;
            string t = "";
            while(str[i]!=' '){
                t+=str[i];
                i++;
            }
            // cout << "t: "<< t;
            v_columnName.push_back(t);
            t="";
            i++;
            while(str[i] !=','&& str[i]!= ')'){
                t+=str[i];
                i++;
            }  
            v_dataType.push_back(determine_type_data(t));
            if(str[i]==')')
                break;
        }


        for(int i=0;i<v_columnName.size();i++){
            bt.push_back(Btree<long long>(3));
        };

        // while (i<query.length())
        // {
        //     cout <<" " << query[i] << " ";
        //     if(query[i]=='(' || query[i]==','){
        //         i++;
        //         // while (query[i]!=')')
        //         // {
        //             string str ="";
        //             while (query[i]!=' ')
        //             {
        //                 str+=query[i];
        //                 i++;
        //             }
        //             i++;
        //             cout <<"str: " << str<< endl;
                    
        //             v_columnName.push_back(str);
        //             str="";
        //             while (query[i]!=','&& query[i]!=')')
        //             {
        //                 str+=query[i];
        //                 i++;
        //             }
        //             v_dataType.push_back(determine_type_data(str));
        //             cout <<"typr: " << str<< endl;
        //             str="";
        //         // } 
        //     }
        //     if(query[i]!='(' || query[i]!=',')
        //         i++;
        // }
        // for(int i=0;i<v_dataType.size();i++){
        //     cout << v_dataType[i] <<" ";
        // }
        // cout << endl;
        // for(int i=0;i<v_columnName.size();i++){
        //     cout << v_columnName[i] << " ";
        // }
        // cout << endl;
    }catch(...){
        cout << "Invalid Syntax for creating table! status:400 msg:BadRequest"<< endl;
    }
}

// INSERT INTO {table name} VALUES (field1,field2,...)
// INSERT INTO employee VALUES ("hamid",2022/8/6,45000)
void InsertIntoQuery(string query , vector<int> v_dataType , vector<string> v_columnName, string tableName,vector<Btree<long long>>& bt){
    try{
        if(bt.size()==0){
            throw bt;
        }

        vector<string> v_allDataToInsert;
        string tmp = query.substr(0,12);
        // cout << tmp;
        if(tmp != "INSERT INTO ")
            throw query;
        
        string str;
        for(int i=0;i<query.length();i++){
            if(query[i]=='('){
                str = query.substr(i);
            }
        }

        int i=0;
        while(i<str.length()){
            i++;
            string t = "";
            while(str[i] !=','&& str[i]!= ')'){
                t+=str[i];
                i++;
            }
            v_allDataToInsert.push_back(t);
            if(str[i]==')')
                break;
        }
        // for(int i=0;i<v_allDataToInsert.size();i++){
        //     cout << v_allDataToInsert[i] << " ";
        // }
        // cout << endl;

        // in this section i should check validation type
        for(int i=1;i<v_dataType.size();i++){
            if(v_dataType[i]==2){
                if(! valid_timestamp(v_allDataToInsert[i-1])){
                    throw v_allDataToInsert;
                }
            }  
            else if(v_dataType[i] == 1){
                if(!valid_string(v_allDataToInsert[i-1])){
                    throw v_allDataToInsert;
                }
            }      
        }
         
        // find best id
        int id;
        if(bt[0].root == nullptr){
           id = 1;    
        }else{
            id = bt[0].root->FindBestId();
        }

        // cout << "size :"<< v_dataType.size() << endl;

        // for(int i=0;i<v_dataType.size();i++){
        //     cout << v_dataType[i] << " ";
        // }
        // cout << endl;

        // make all nodes
        Node<long long>*prv = new Node<long long>(id); 
        Node<long long>*ID = prv;
        bt[0].insert(prv);

        for(int i=1;i<v_dataType.size();i++){
            long long data = 0;
            
            if(v_dataType[i] == 0){ 
                data = stoi(v_allDataToInsert[i-1]);
                // cout << "0      "<< data << endl;
            }
            else if(v_dataType[i] == 1){
                data = string_hash(v_allDataToInsert[i-1]);
                // cout <<"1    " << data << endl;
            }
            else if(v_dataType[i]== 2){
                data = timestamp_hash(v_allDataToInsert[i-1]);
                // cout <<"2    "<< data << endl;
            }
            
            Node<long long> *p = new Node<long long>(data);
            prv->nextField = p;
            prv = p;
            // insert it to our btree.
            bt[i].insert(p);
            if(i==bt.size()-1)
                p->nextField = ID;   // the last one have a pointer to first one.

        };

    }catch(vector<Btree<int>>bt){
        cout << "At first you should create table!  status : 400 , msg: BadRequest." << endl;
    }catch(vector<string> v_allDataForUpdate){
        cout << "invalid Timestamp || string! status: 400 , msg: BadRequest" << endl;
    }
    catch(...){
        cout << "Invalid Syntax ERROR! InsetIntoTable  status:400 , msg : BadRequest. " << endl;
    }

}


// UPDATE {table name} SET ("Hamid",2022/8/7,50000)
// UPDATE employee SET ("Hamid",2022/8/7,50000) WHERE name=="Hamid"
void UpdateQuery(string query , vector<int> v_dataType , vector<string> v_columnName, string tableName,vector<Btree<long long>>& bt){
    try{
        vector<string> v_allDataForUpdate;
        string condition ="";
        string tmp = query.substr(0,7);
        // cout << tmp;
        if(tmp != "UPDATE ")
            throw query;
        
        string str;
        for(int i=0;i<query.length();i++){
            if(query[i]=='('){
                str = query.substr(i);
            }
        }

        int i=0;
        while(i<str.length()){
            i++;
            string t = "";
            while(str[i] !=','&& str[i]!= ')'){
                t+=str[i];
                i++;
            }
            v_allDataForUpdate.push_back(t);
            if(str[i]==')')
                break;
        }
        i++;
        if(str.substr(i+1,5)!="WHERE")
            throw query;

        i+=7;

        condition = str.substr(i);
        // for(int i=0;i<v_allDataForUpdate.size();i++){
        //     cout << v_allDataForUpdate[i] << " ";
        // }
        // cout << endl;
        // cout << "condition : " << condition;

        // TODO: make condition to a readable condition.
        // TODO : find the data by that condition and updete it.


    }catch(...){
        cout << "Invalid Syntax ERROR! UpdateTable  status:400 , msg : BadRequest. " << endl;
    }
}

// DELETE FROM {table name} WHERE {condition}
// DELETE FROM employee WHERE income>40000
void DeleteFromQuery(string query , vector<int> v_dataType , vector<string> v_columnName, string tableName,vector<Btree<long long>>& bt){
    try{
        string condition ="";
        string tmp = query.substr(0,12);
        // cout << tmp;
        if(tmp != "DELETE FROM ")
            throw query;

        int i=0,c=0;
        while(c!=3) {
            if(query[i]==' ')
                c++;
            i++;
            if(i>query.length()) {
                throw query;
                return;
            }   
        }  
        // cout << "where : " << query.substr(i,5) ; 
        if(query.substr(i,5)!="WHERE")
            throw query;

        i+=6;
        condition = query.substr(i);

        // TODO: make condition to a readable condition.
        // TODO : find the data by that condition and delete it from our btree.

        // cout << "condition : " << condition;
    }catch(...){
        cout << "Invalid Syntax ERROR! DeleteFromTable.  status:400 , msg : BadRequest. " << endl;
    }
}


// SELECT {(column1,column2,...) or *} FROM {table name} WHERE condition
// SELECT * FROM employee WHERE income>45000
// SELECT (name,income) FROM employee WHERE income>45000
void SelectQuery(string query , vector<int> v_dataType , vector<string> v_columnName, string tableName,vector<Btree<long long>>& bt){
    try{
        bool flag = false;
        string condition ="";
        string tmp = query.substr(0,7);
        // cout << tmp;
        if(tmp != "SELECT ")
            throw query;

        for(int i=0;i<query.length();i++){
            if(query[i]=='*'){
                flag = true;
                break;
            }
        }    
        int i=0,c=0;
        while(c!=4) {
            if(query[i]==' ')
                c++;
            i++;
            if(i>query.length()) {
                throw query;
                return;
            }   
        } 

        if(query.substr(i,5)!="WHERE")
            throw query;

        i+=6;
        condition = query.substr(i);

        // cout << condition << endl;

        // TODO:
        // if flag == true thn : show all columns
        // else you should show selected columns.

        string colName = colNameOfCondition(condition);
        // cout << colName << endl;
        // we define == = 0 and > = 1 and < = 2.
        int typecondition = TypeOfCondition(condition);
        // cout << typecondition << endl;
        int col;
        for(int i=0;i<v_columnName.size();i++){
            if(v_columnName[i] == colName)
                col = i;
        }

        string limit = FindLimitation(condition);
        limit = reverse(limit);

        // cout << colName << " "<< typecondition << " " << col <<" "<< limit<< endl;
        if(typecondition == 0){
            vector<Node<long long>*> v;
            if(v_dataType[col] == 0){
                bt[col].root->FindAllNodeWithAKey(v,stoi(limit));
            }else if(v_dataType[col] == 1){
                bt[col].root->FindAllNodeWithAKey(v,string_hash(limit));
            }else if(v_dataType[col] == 2){
                bt[col].root->FindAllNodeWithAKey(v,timestamp_hash(limit));
            }
            // for(int i=0;i<v.size();i++){
            //     Node<long long>* p =v[i];
            //     while (p->nextField != v[i])
            //     {
            //         cout << p->data << " ";
            //         p = p-> nextField;
            //     }
            //     cout << endl;    
            // }
        }else if(typecondition == 1){

        }else if(typecondition == 2){

        }

        // search in the bt[i].
        
        // then print.

    }catch(...){
        cout << "Invalid Syntax ERROR! SelectQuery.  status:400 , msg : BadRequest. " << endl;
    }
}

// CREATE TABLE employee (name string,joinDate timestamp,income int)
// INSERT INTO employee VALUES ("hamid",2022/8/6,45000)
// DELETE FROM employee WHERE name=="Hamid"
// UPDATE employee SET ("Hamid",2022/8/7,50000) WHERE name=="Hamid"
// SELECT {(column1,column2,...) or *} FROM {table name} WHERE condition
void type_of_query(string str , vector<int>& v_dataType , vector<string>& v_columnName, string& tableName, vector<Btree<long long>>& bt){
    try{
        int i=0;
        string tmp="";
        while (str[i]!=' ')
        {
            tmp+=str[i];
            i++;
        }
        if(tmp =="CREATE")
            CreateTableQuery(str,v_dataType,v_columnName,tableName,bt);
        else if(tmp =="INSERT")
            InsertIntoQuery(str,v_dataType,v_columnName,tableName,bt);
        else if(tmp == "DELETE")
            DeleteFromQuery(str,v_dataType,v_columnName,tableName,bt);
        else if(tmp == "UPDATE")
            UpdateQuery(str,v_dataType,v_columnName,tableName,bt);
        else if(tmp == "SELECT")
            SelectQuery(str,v_dataType,v_columnName,tableName,bt);
        else 
            throw i;
    }catch(...){
        cout << "Syntax ERROR! check your input. typeOfQuery. status : 400 , msg : BadRequest." << endl;
    }
    return;
}


int main(){
    // int number_of_req;
    // cin >> number_of_req;  
        
    // string query;
    // getline(cin , query);
    
    
    // vector<int> v_dataType;          // in this vector we save type od every column.
    // v_dataType.push_back(0);         // by defult every table has a ID column that is a integer.
    // vector<string> v_columnName;     // in this vector we save title of every column of table.
    // v_columnName.push_back("ID");    // by defult every table hase a ID column.
    // string tableName="";             // in this string we will save name of the table.
    // vector<Btree<long long>> bt;           // create all btrees that we need to them.

    // // query = ignore_space(query);

    // // type_of_query(query, v_dataType,v_columnName,tableName,bt);

    // for(int i=0;i<number_of_req;i++){
    //     query ="";
    //     getline(cin, query);
    //     query = ignore_space(query);
    //     type_of_query(query, v_dataType,v_columnName,tableName,bt);
    // }



    // for(int j=0;j<v_columnName.size();j++){
    //     cout << v_columnName[j]<<" ";
    // }
    // cout << endl;
    // for(int i=0;i<v_dataType.size();i++){
    //     cout << v_dataType[i] << " ";
    // }
    // }

    

    
    
        
    // Btree<int> t = Btree<int>(3);
    // Node<int> *p = new Node<int>(2);
    // Node<int>* q=p;
    // t.insert(p);
    // p = new Node<int>(8);
    // t.insert(p);
    // p = new Node<int>(8);
    // t.insert(p);
    // p = new Node<int>(11);
    // t.insert(p);
    // p = new Node<int>(123);
    // t.insert(p);
    // p = new Node<int>(8);
    // t.insert(p);
    // p = new Node<int>(1);
    // t.insert(p);
    // t.root->traverse(); 

    // vector<Node<int>*> v;
    // t.root->FindAllLessThan(v,8);
    // // t.root ->FindAllNodeWithAKey(v,8);
    // cout << endl;
    // for(int i=0;i<v.size();i++){
    //     cout << v[i]->data << " ";
    // }
    // cout << endl;

    // string name;
    // cin >> name;
    // cout << string_hash(name) << endl;
    // cout << string_hash_inverse(string_hash(name)) << endl;

    // string tmp;
    // getline(cin , tmp);
    // cout << tmp << endl;
    // cout << tmp.substr(0,13);
    // type_of_query(tmp);
    // CreateTableQuery(tmp, v_dataType,v_columnName,tableName);
    
    // getline(cin , tmp);
    // InsertIntoQuery(tmp, v_dataType,v_columnName,tableName);

    // UpdateQuery(tmp, v_dataType,v_columnName,tableName);

    // DeleteFromQuery(tmp, v_dataType,v_columnName,tableName);

    // SelectQuery(tmp, v_dataType,v_columnName,tableName);

// ISERT INTO employee   VALUES ("Atena",   2019/7/6,55000)
}