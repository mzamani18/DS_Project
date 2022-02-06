#include <iostream>
#include <string>
#include <vector>
#include <hash.h>
#include <BTree.h>

using namespace std;


// this function will delete all unexpected space in our input!
string ignore_space(string str)
{
    string tmp = "";
    bool flag = false;
    bool startspace = false;
    for (int i = 0; i < str.length(); i++)
    {
        if (!startspace)
        {
            if (str[i] != ' ')
            {
                startspace = true;
                tmp += str[i];
            }
        }
        else if (str[i] == '(')
        {
            flag = true;
            tmp += str[i];
        }
        else if (str[i] != ' ' && str[i] != ',')
        {
            tmp += str[i];
            flag = false;
        }
        else if (str[i] == ' ' && !flag)
        {
            tmp += str[i];
            flag = true;
        }
        else if (str[i] == ',')
        {
            tmp += str[i];
            flag = true;
        }
    }

    return tmp;
}

// this function check validation of timestamp.
bool valid_timestamp(string str)
{
    int i = 0;
    for (int j = 0; j < str.length(); j++)
    {
        if (str[j] == '/')
            i++;
    }
    if (i != 2)
        return false;

    i = 0;
    while (str[i] != '/')
        i++;

    if (i != 4)
    {
        return false;
    }
    i = str.length() - 1;
    string tmp = "";
    while (str[i] != '/')
    {
        tmp += str[i];
        i--;
    }

    tmp = reverse(tmp);
    int day = convert_to_int(tmp);

    // cout <<" day: " << tmp << endl;

    if (!((1 <= day) && (day <= 31)) || tmp[0] == '0')
        return false;
    i--;
    tmp = "";
    while (str[i] != '/')
    {
        tmp += str[i];
        i--;
    }

    tmp = reverse(tmp);
    int month = convert_to_int(tmp);

    if (month > 12 || month < 1 || tmp[0] == '0')
        return false;

    // cout << "month " << tmp<< endl;

    return true;
}

// this function check the input type is string or not.
bool valid_string(string str)
{
    if (str[0] == '"' && str[str.length() - 1] == '"')
        return true;
    return false;
}

// i defined int = 0 and string = 1 and timestamp = 2.
int determine_type_data(string type)
{
    try
    {
        if (type == "int")
            return 0;
        if (type == "string")
            return 1;
        else if (type == "timestamp")
            return 2;
        else
            throw type;
    }
    catch (...)
    {
        // cout << "Invalid Type ERROR!! status: 400 , msg:BadRequest " << endl;
    }
    return -1;
}

// in this function i will determine the condition is on what column.
string colNameOfCondition(string condition)
{
    int i = 0;
    string res = "";
    while (condition[i] != '=' && condition[i] != '>' && condition[i] != '<')
    {
        res += condition[i];
        i++;
    }
    return res;
}

// we define == = 0 and > = 1 and < = 2.
int TypeOfCondition(string condition)
{
    for (int i = 0; i < condition.length(); i++)
    {
        if (condition[i] == '=')
            return 0;
        else if (condition[i] == '>')
            return 1;
        else if (condition[i] == '<')
            return 2;
    }
    return 0;
}

// find the limit number or string or timestamp in conditions.
string FindLimitation(string condition)
{
    string res = "";
    int i = condition.length() - 1;
    while (condition[i] != '=' && condition[i] != '>' && condition[i] != '<')
    {
        res += condition[i];
        i--;
    }
    return res;
}

// this function is for printing some selected column of table and in this function i check the str in in the selected or not!
bool exist_col(string str, vector<string> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (str == v[i])
            return true;
    }
    return false;
}

// this function is for printing data of selected query.
void print_selectQuery(bool flag, vector<int> v_dataType, int col, vector<Node<long long> *> v, vector<string> v_columnName, vector<string> selectedCol, vector<string>& result)
{
    if (v.size() == 0)
    {
        // cout << "dose not exist any data by this limitation!" << endl;
        return;
    }
    if (flag)
    {
        vector<long long> Id;
        for(int i=0;i<v.size();i++){
            Node<long long>*tmp = v[i];
            int k = 0;
            while(k<v_columnName.size()-col){
                tmp = tmp -> nextField;
                k++;
            }
            Id.push_back(tmp->data);
        }
        int i, j; 
        for (i = 0; i < v.size(); i++) {    
      
            // Last i elements are already in place 
            for (j = 0; j < v.size()-i-1; j++) {
                if (Id[j] > Id[j+1]){ 
                    swap(Id[j], Id[j+1]); 
                    swap(v[j],v[j+1]);
                }   
            } 
        }
        for (int i = 0; i < v.size(); i++)
        {
            Node<long long> *p = v[i];
            vector<long long> v_tmp;
            while (p->nextField != v[i])
            {
                v_tmp.push_back(p->data);
                p = p->nextField;
            }
            v_tmp.push_back(p->data);
            // for(int i=0;i<v_tmp.size();i++){
            //     cout << v_tmp[i] <<" ";
            // }
            // cout << " col :" << col;
            int k = 0;
            for (int j = v_tmp.size() - col; j <= v_tmp.size() - 1; j++)
            {
                if (v_dataType[k] == 0)
                {
                    // cout << v_tmp[j] << " ";
                    result.push_back(to_string(v_tmp[j]));
                }
                else if (v_dataType[k] == 1)
                {
                    // cout << string_hash_inverse(v_tmp[j]) << " ";
                    result.push_back(string_hash_inverse(v_tmp[j]));
                }
                else if (v_dataType[k] == 2)
                {
                    // cout << timestamp_hash_inverse(v_tmp[j]) << " ";
                    result.push_back(timestamp_hash_inverse(v_tmp[j]));
                }
                k++;
            }
            for (int j = 0; j < v_tmp.size() - col; j++)
            {
                if (v_dataType[k] == 0)
                {
                    // cout << v_tmp[j] << " ";
                    result.push_back(to_string(v_tmp[j]));
                }
                else if (v_dataType[k] == 1)
                {
                    // cout << string_hash_inverse(v_tmp[j]) << " ";
                    result.push_back(string_hash_inverse(v_tmp[j]));
                }
                else if (v_dataType[k] == 2)
                {
                    // cout << timestamp_hash_inverse(v_tmp[j]) << " ";
                    result.push_back(timestamp_hash_inverse(v_tmp[j]));
                }
                k++;
            }
            // cout << endl;
            result.push_back("newline");
        }
    }
    else
    {
        vector<long long> Id;
        for(int i=0;i<v.size();i++){
            Node<long long>*tmp = v[i];
            int k = 0;
            while(k<v_columnName.size()-col){
                tmp = tmp -> nextField;
                k++;
            }
            Id.push_back(tmp->data);
        }
        int i, j; 
        for (i = 0; i < v.size(); i++) {    
      
            // Last i elements are already in place 
            for (j = 0; j < v.size()-i-1; j++) {
                if (Id[j] > Id[j+1]){ 
                    swap(Id[j], Id[j+1]); 
                    swap(v[j],v[j+1]);
                }   
            } 
        }
        for (int i = 0; i < v.size(); i++)
        {
            Node<long long> *p = v[i];
            vector<long long> v_tmp;
            while (p->nextField != v[i])
            {
                v_tmp.push_back(p->data);
                p = p->nextField;
            }
            v_tmp.push_back(p->data); 
            // for(int i=0;i<v_tmp.size();i++){
            //     cout << v_tmp[i] <<" ";
            // }
            // cout << " col :" << col;
            int k = 0;
            for (int j = v_tmp.size() - col; j <= v_tmp.size() - 1; j++)
            {
                if (exist_col(v_columnName[k], selectedCol))
                {
                    if (v_dataType[k] == 0)
                    {
                        // cout << v_tmp[j] << " ";
                        result.push_back(to_string(v_tmp[j]));
                    }
                    else if (v_dataType[k] == 1)
                    {
                        // cout << string_hash_inverse(v_tmp[j]) << " ";
                        result.push_back(string_hash_inverse(v_tmp[j]));
                    }
                    else if (v_dataType[k] == 2)
                    {
                        // cout << timestamp_hash_inverse(v_tmp[j]) << " ";
                        result.push_back(timestamp_hash_inverse(v_tmp[j]));
                    }
                }

                k++;
            }
            for (int j = 0; j < v_tmp.size() - col; j++)
            {
                if (exist_col(v_columnName[k], selectedCol))
                {
                    if (v_dataType[k] == 0)
                    {
                        // cout << v_tmp[j] << " ";
                        result.push_back(to_string(v_tmp[j]));
                    }
                    else if (v_dataType[k] == 1)
                    {
                        // cout << string_hash_inverse(v_tmp[j]) << " ";
                        result.push_back(string_hash_inverse(v_tmp[j]));
                    }
                    else if (v_dataType[k] == 2)
                    {
                        // cout << timestamp_hash_inverse(v_tmp[j]) << " ";
                        result.push_back(timestamp_hash_inverse(v_tmp[j]));
                    }
                }
                k++;
            }
            // cout << endl;
            result.push_back("newline");
        }
    }
}

// in this function i will updete record base on some data that i collected them in UpdeteQuery.
void Update_data(int col, vector<string> v_columnName, vector<int> v_dataType, vector<string> v_allDataForUpdate, vector<Node<long long> *> v)
{
    if (v.size() == 0)
    {
        // cout << "there are not any record with these limitation!" << endl;
        return;
    }
    for (int i = 0; i < v.size(); i++)
    {
        Node<long long> *tmp = v[i];
        int m = col;
        while (tmp->nextField != v[i])
        {
            if (v_columnName[m] != "ID")
            {
                long long d = 0;
                if (v_dataType[m] == 0)
                {
                    d = stoi(v_allDataForUpdate[m - 1]);
                }
                else if (v_dataType[m] == 1)
                {
                    d = string_hash(v_allDataForUpdate[m - 1]);
                }
                else if (v_dataType[m] == 2)
                {
                    d = timestamp_hash(v_allDataForUpdate[m - 1]);
                }
                // cout << "d: " << d << endl;
                // cout << " before: " << tmp->data << endl;
                tmp->data = d;
                // cout << " after : " << tmp->data << endl;
                tmp = tmp->nextField;
                if (m == v_dataType.size() - 1)
                {
                    m = 0;
                    // tmp = tmp->nextField;
                }
                else
                {
                    m++;
                }
            }
            else
            {
                tmp = tmp->nextField;
                m++;
            }
        }
        if (v_columnName[m] != "ID")
        {
            long long d = 0;
            if (v_dataType[m] == 0)
            {
                d = stoi(v_allDataForUpdate[m - 1]);
            }
            else if (v_dataType[m] == 1)
            {
                d = string_hash(v_allDataForUpdate[m - 1]);
            }
            else if (v_dataType[m] == 2)
            {
                d = timestamp_hash(v_allDataForUpdate[m - 1]);
            }
            // cout << " before: " << tmp->data << endl;
            tmp->data = d;
            // cout << " after : " << tmp->data << endl;
        }
    }
}

// CREATE TABLE {table name} (column1 type,column2 type,...)
// CREATE TABLE employee (name string,joinDate timestamp,income int)
void CreateTableQuery(string query, vector<vector<int>> &v_dataType, vector<vector<string>> &v_columnName, vector<vector<Btree<long long>> > &bt , unordered_map<string,int>& map,int & num_of_tree)
{
    try
    {
        string tmp = query.substr(0, 13);
        string tableName = "";
        if (tmp != "CREATE TABLE ")
            throw query;

        int i = 13;
        while (query[i] != ' ')
        {
            tableName += query[i];
            i++;
        }
        // cout << " " << tableName << " " << endl;
        map[tableName] = num_of_tree;
        // cout << map[tableName] << endl;
        num_of_tree++;
        // cout << tableName << endl;
        // i++;

        string str = "";
        for (int j = 0; j < query.length(); j++)
        {
            if (query[j] == '(')
            {
                str = query.substr(j);
                break;
            }
        }
        // cout << str << endl;
        vector<string> tmp_name;
        vector<int> tmp_type;
        tmp_name.push_back("ID");
        tmp_type.push_back(0);
        i = 0;
        while (i < str.length())
        {
            i++;
            string t = "";
            while (str[i] != ' ')
            {
                t += str[i];
                i++;
            }
            // cout << "t: "<< t;
            tmp_name.push_back(t);
            t = "";
            i++;
            while (str[i] != ',' && str[i] != ')')
            {
                t += str[i];
                i++;
            }
            tmp_type.push_back(determine_type_data(t));
            if (str[i] == ')')
                break;
        }
        v_columnName.push_back(tmp_name);
        v_dataType.push_back(tmp_type);
        vector<Btree<long long>> v_b;
        for (int i = 0; i < v_columnName[map[tableName]].size(); i++)
        {
            // bt[map[tableName]].push_back(Btree<long long>(3));
            v_b.push_back(Btree<long long>(3));
        };
        bt.push_back(v_b);
        
    }
    catch (...)
    {
        // cout << "Invalid Syntax for creating table! status:400 msg:BadRequest" << endl;
    }
}

// INSERT INTO {table name} VALUES (field1,field2,...)
// INSERT INTO employee VALUES ("hamid",2022/8/6,45000)
void InsertIntoQuery(string query, vector<vector<int>> v_dataType, vector<vector<string>> v_columnName, vector<vector<Btree<long long>>> &bt,unordered_map<string,int> &map)
{
    try
    {
        // if (bt.size() == 0)
        // {
        //     throw bt;
        // }

        vector<string> v_allDataToInsert;
        string tmp = query.substr(0, 12);
        // cout << tmp;
        if (tmp != "INSERT INTO ")
            throw query;
        string tableName="";
        int l = 12;
        while(query[l]!=' '){
            tableName+=query[l];
            l++;
        }
        // cout << " " << tableName << " " << endl;
        string str;
        for (int i = 0; i < query.length(); i++)
        {
            if (query[i] == '(')
            {
                str = query.substr(i);
            }
        }

        int i = 0;
        while (i < str.length())
        {
            i++;
            string t = "";
            while (str[i] != ',' && str[i] != ')')
            {
                t += str[i];
                i++;
            }
            v_allDataToInsert.push_back(t);
            if (str[i] == ')')
                break;
        }
        // for(int i=0;i<v_allDataToInsert.size();i++){
        //     cout << v_allDataToInsert[i] << " ";
        // }
        // cout << endl;

        // in this section i should check validation type
        for (int i = 1; i < v_dataType[map[tableName]].size(); i++)
        {
            if (v_dataType[map[tableName]][i] == 2)
            {
                if (!valid_timestamp(v_allDataToInsert[i - 1]))
                {
                    throw v_allDataToInsert;
                }
            }
            else if (v_dataType[map[tableName]][i] == 1)
            {
                if (!valid_string(v_allDataToInsert[i - 1]))
                {
                    throw v_allDataToInsert;
                }
            }
        }

        // find best id
        int id=0;
        if (bt[map[tableName]][0].root == nullptr)
        {
            id = 1;
        }
        else
        {
            id = bt[map[tableName]][0].root->FindBestId();
        }

        // make all nodes
        Node<long long> *prv = new Node<long long>(id);
        Node<long long> *ID = prv;
        // bt[0].insert(prv);

        for (int i = 1; i < v_dataType[map[tableName]].size(); i++)
        {
            long long data = 0;

            if (v_dataType[map[tableName]][i] == 0)
            {
                data = stoi(v_allDataToInsert[i - 1]);
                // cout << "0      "<< data << endl;
            }
            else if (v_dataType[map[tableName]][i] == 1)
            {
                data = string_hash(v_allDataToInsert[i - 1]);
                // cout <<"1    " << data << endl;
            }
            else if (v_dataType[map[tableName]][i] == 2)
            {
                data = timestamp_hash(v_allDataToInsert[i - 1]);
                // cout <<"2    "<< data << endl;
            }
            Node<long long> *p = new Node<long long>(data);
            prv->nextField = p;
            bt[map[tableName]][i-1].insert(prv);
            // cout << prv->nextField->data<<endl;
            prv = p;
            // insert it to our btree.
            if (i == bt[map[tableName]].size() - 1)
            {
                p->nextField = ID; // the last one have a pointer to first one.
                bt[map[tableName]][i].insert(p);
                // cout << p->nextField->data << endl;
                // cout << p->data << endl;
            }
            // cout << p->nextField->data << endl;
        };
    }
    catch (vector<Btree<int> > bt)
    {
        // cout << "At first you should create table!  status : 400 , msg: BadRequest." << endl;
    }
    catch (vector<string> v_allDataForUpdate)
    {
        // cout << "invalid Timestamp || string! status: 400 , msg: BadRequest" << endl;
    }
    catch (...)
    {
        // cout << "Invalid Syntax ERROR! InsetIntoTable  status:400 , msg : BadRequest. " << endl;
    }
}

// UPDATE {table name} SET ("Hamid",2022/8/7,50000)
// UPDATE employee SET ("Hamid",2022/8/7,50000) WHERE name=="Hamid"
void UpdateQuery(string query, vector<vector<int>> v_dataType, vector<vector<string>> v_columnName, vector<vector<Btree<long long> >> &bt,unordered_map<string,int>& map)
{
    try
    {
        vector<string> v_allDataForUpdate;
        string condition = "";
        string tmp = query.substr(0, 7);
        // cout << tmp;
        if (tmp != "UPDATE ")
            throw query;
        string tableName = "";    
        int l = 7;
        while(query[l]!=' '){
            tableName+=query[l];
            l++;
        }
        // cout << " " << tableName << " " << endl;
        string str;
        for (int i = 0; i < query.length(); i++)
        {
            if (query[i] == '(')
            {
                str = query.substr(i);
            }
        }

        int i = 0;
        while (i < str.length())
        {
            i++;
            string t = "";
            while (str[i] != ',' && str[i] != ')')
            {
                t += str[i];
                i++;
            }
            v_allDataForUpdate.push_back(t);
            if (str[i] == ')')
                break;
        }
        i++;
        if (str.substr(i + 1, 5) != "WHERE")
            throw query;

        i += 7;
        condition = str.substr(i);
        // for(int i=0;i<v_allDataForUpdate.size();i++){
        //     cout << v_allDataForUpdate[i] << " ";
        // }
        // cout << endl;
        // cout << "condition : " << condition;
        string colName = colNameOfCondition(condition);
        // cout << colName << endl;
        // we define == = 0 and > = 1 and < = 2.
        int typecondition = TypeOfCondition(condition);
        // cout << typecondition << endl;
        int col;
        for (int i = 0; i < v_columnName[map[tableName]].size(); i++)
        {
            if (v_columnName[map[tableName]][i] == colName)
                col = i;
        }

        string limit = FindLimitation(condition);
        limit = reverse(limit);
        // cout << colName << " "<< typecondition << " " << col <<" "<< limit<< endl;
        if (typecondition == 0)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, stoi(limit));
                // cout << col << endl;
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, string_hash(limit));
                // cout << v.size() << endl;
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, timestamp_hash(limit));
                // print_selectQuery(flag,v_dataType,col,v,v_columnName,selectedCol);
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
        }
        else if (typecondition == 2)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, stoi(limit));
                // cout << v[0]->nextField ->nextField-> data;
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, string_hash(limit));
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, timestamp_hash(limit));
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
        }
        else if (typecondition == 1)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, stoi(limit));
                // cout << v[0]->nextField ->nextField-> data;
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, string_hash(limit));
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, timestamp_hash(limit));
                Update_data(col, v_columnName[map[tableName]], v_dataType[map[tableName]], v_allDataForUpdate, v);
            }
        }
    }
    catch (string query)
    {
        // cout << "Invalid Syntax ERROR! UpdateTable  status:400 , msg : BadRequest. " << endl;
    }
    catch (...)
    {
        // cout << "Unhandeled ERROR! in UpdateTable  status:500 , msg : BadRequest. " << endl;
    }
}

// DELETE FROM {table name} WHERE {condition}
// DELETE FROM employee WHERE income>40000
void DeleteFromQuery(string query, vector<vector<int>> v_dataType, vector<vector<string>> v_columnName, vector<vector<Btree<long long> >> &bt,unordered_map<string,int>& map)
{
    try
    {
        string condition = "";
        string tmp = query.substr(0, 12);
        // cout << tmp;
        if (tmp != "DELETE FROM ")
            throw query;
        int l = 12;
        string tableName = "";
        while(query[l]!=' '){
            tableName += query[l];
            l++;
        }
        // cout << " " << tableName << " " << tableName.length()<<endl;
        int i = 0, c = 0;
        while (c != 3)
        {
            if (query[i] == ' ')
                c++;
            i++;
            if (i > query.length())
            {
                throw query;
                return;
            }
        }
        // cout << "where : " << query.substr(i,5) ;
        if (query.substr(i, 5) != "WHERE")
            throw query;

        i += 6;
        condition = query.substr(i);

        // cout << "condition : " << condition << endl;

        string colName = colNameOfCondition(condition);
        // cout << colName << endl;
        // we define == = 0 and > = 1 and < = 2.
        int typecondition = TypeOfCondition(condition);
        // cout << typecondition << endl;
        int col;
        for (int i = 0; i < v_columnName[map[tableName]].size(); i++)
        {
            if (v_columnName[map[tableName]][i] == colName)
                col = i;
        }

        string limit = FindLimitation(condition);
        limit = reverse(limit);
        // cout << "col : " << col << "limit: " << limit << endl;
        // cout << v_dataType[col] << endl;
        long long key = 0;
        if (v_dataType[map[tableName]][col] == 0)
        {
            // bt[col].root->deleteNode(bt,typecondition,stoi(limit),col);
            key = stoi(limit);
        }
        else if (v_dataType[map[tableName]][col] == 1)
        {
            // bt[col].root->deleteNode(bt,typecondition,string_hash(limit),col);
            key = string_hash(limit);
        }
        else if (v_dataType[map[tableName]][col] == 2)
        {
            // bt[col].root->deleteNode(bt,typecondition,timestamp_hash(limit),col);
            key = timestamp_hash(limit);
        }
        bt[map[tableName]][col].root->deleteNode(bt,typecondition,key,col,map,tableName);
    //     vector<Node<long long>*> v;
    // cout << map[tableName] << " ";
    // if(typecondition == 0){;
    //     bt[map[tableName]][col].root->FindAllNodeWithAKey(v,key);
    // }else if (typecondition==2){
    //     bt[map[tableName]][col].root -> FindAllLessThan(v,key);
    // }else if(typecondition== 1){
    //     bt[map[tableName]][col].root -> FindAllGreaterThan(v,key);
    // }
    // if(v.size()==0){
    //     // cout << "there are not any record with these limitations for delete!" << endl;
    //     return;
    // }
    // for(int i=0;i<v.size();i++){
    //     Node<long long>*p = v[i];
    //     while(p->nextField!=v[i]){
    //         p->status = true;
    //         p = p->nextField;
    //     }
    //     p->status = true;
    //     bt[map[tableName]][i].numberOfDeletedNodes++;
    // }
        
    }
    catch (...)
    {
        // cout << "Invalid Syntax ERROR! DeleteFromTable.  status:400 , msg : BadRequest. " << endl;
    }
}

// SELECT {(column1,column2,...) or *} FROM {table name} WHERE condition
// SELECT * FROM employee WHERE income>45000
// SELECT (name,income) FROM employee WHERE income>45000
void SelectQuery(string query, vector<vector<int>> v_dataType, vector<vector<string>> v_columnName, vector<vector<Btree<long long>> > &bt,vector<string>& result,unordered_map<string,int>&map)
{
    try
    {
        bool flag = false;
        string condition = "";
        string tmp = query.substr(0, 7);
        // cout << tmp;
        if (tmp != "SELECT ")
            throw query;

        for (int i = 0; i < query.length(); i++)
        {
            if (query[i] == '*')
            {
                flag = true;
                break;
            }
        }
        vector<string> selectedCol;
        if (!flag)
        {
            int i = 0;
            for (int j = 0; j < query.length(); j++)
            {
                if (query[j] == '(')
                {
                    i = j;
                    break;
                }
            }
            while (i < query.length())
            {
                i++;
                string t = "";
                while (query[i] != ',' && query[i] != ')')
                {
                    t += query[i];
                    i++;
                }
                selectedCol.push_back(t);
                if (query[i] == ')')
                    break;
            }
        }
        // for(int i=0;i<selectedCol.size();i++){
        //     cout << selectedCol[i].length() << " ";
        // }
        // cout << endl;
        // return;
        int i = 0, c = 0;
        while (c != 4)
        {
            if (query[i] == ' ')
                c++;
            i++;
            if (i > query.length())
            {
                throw query;
                return;
            }
        }
        string tableName = "";
        int l = i-2;
        while(query[l]!=' '){
            tableName += query[l];
            l--;
        }
        tableName = reverse(tableName);
        // cout << " " << tableName << " " << endl;
        if (query.substr(i, 5) != "WHERE")
            throw query;

        i += 6;
        condition = query.substr(i);
        // cout << condition << endl;

        string colName = colNameOfCondition(condition);
        // cout << colName << endl;
        // we define == = 0 and > = 1 and < = 2.
        int typecondition = TypeOfCondition(condition);
        // cout << typecondition << endl;
        int col;
        for (int i = 0; i < v_columnName[map[tableName]].size(); i++)
        {
            if (v_columnName[map[tableName]][i] == colName)
                col = i;
        }

        string limit = FindLimitation(condition);
        limit = reverse(limit);

        // cout << colName << " "<< typecondition << " " << col <<" "<< limit<< endl;
        if (typecondition == 0)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, stoi(limit));
                // cout << v[0]->nextField ->nextField-> data;
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, string_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllNodeWithAKey(v, timestamp_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
        }
        else if (typecondition == 2)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, stoi(limit));
                // cout << v[0]->nextField ->nextField-> data;
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, string_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllLessThan(v, timestamp_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
        }
        else if (typecondition == 1)
        {
            vector<Node<long long> *> v;
            if (v_dataType[map[tableName]][col] == 0)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, stoi(limit));
                // cout << v.size() << endl;
                // bt[col].root->traverse();
                // cout << endl;
                print_selectQuery(flag,v_dataType[map[tableName]],col,v,v_columnName[map[tableName]],selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 1)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, string_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
            else if (v_dataType[map[tableName]][col] == 2)
            {
                bt[map[tableName]][col].root->FindAllGreaterThan(v, timestamp_hash(limit));
                print_selectQuery(flag, v_dataType[map[tableName]], col, v, v_columnName[map[tableName]], selectedCol,result);
            }
        }
    }
    catch (...)
    {
        // cout << "Invalid Syntax ERROR! SelectQuery.  status:400 , msg : BadRequest. " << endl;
    }
}

// SELECT (name,income) FROM employee WHERE income<62000
// CREATE TABLE employee (name string,joinDate timestamp,income int)
// INSERT INTO employee VALUES ("hamid",2022/8/6,45000)
// DELETE FROM employee WHERE name=="Hamid"
// UPDATE employee SET ("Hamid",2022/8/7,50000) WHERE name=="Hamid"
// SELECT {(column1,column2,...) or *} FROM {table name} WHERE condition
void type_of_query(string str, vector<vector<int>> &v_dataType, vector<vector<string>> &v_columnName, vector<vector<Btree<long long>> > &bt,vector<string>& result,unordered_map<string,int>& map,int &num_of_tree)
{
    try
    {
        int i = 0;
        string tmp = "";
        while (str[i] != ' ')
        {
            tmp += str[i];
            i++;
        }
        if (tmp == "CREATE")
            CreateTableQuery(str, v_dataType, v_columnName, bt,map,num_of_tree);
        else if (tmp == "INSERT")
            InsertIntoQuery(str, v_dataType, v_columnName, bt,map);
        else if (tmp == "DELETE")
            DeleteFromQuery(str, v_dataType, v_columnName, bt,map);
        else if (tmp == "UPDATE")
            UpdateQuery(str, v_dataType, v_columnName, bt,map);
        else if (tmp == "SELECT")
            SelectQuery(str, v_dataType, v_columnName, bt, result,map);
        else
            throw i;
    }
    catch (...)
    {
        // cout << "Syntax ERROR! check your input. typeOfQuery. status : 400 , msg : BadRequest." << endl;
    }
    return;
}

int main()
{
    int number_of_req;
    cin >> number_of_req;

    string query;
    
    vector<string> result;
    vector<vector<int>> v_dataType;       // in this vector we save type od every column.
    
    vector<vector<string>> v_columnName;  // in this vector we save title of every column of table.
    
    vector<vector<Btree<long long>>> bt; // create all btrees that we need to them.
    unordered_map<string,int> map;  // we will shoe every tree by a number.

    cin.ignore();
    int num_of_tree = 0;
    
    // query = ignore_space(query);

    // type_of_query(query, v_dataType,v_columnName,tableName,bt);

    for (int i = 0; i < number_of_req; i++)
    {
        query = "";
        getline(cin, query);
        query = ignore_space(query);
        type_of_query(query, v_dataType, v_columnName, bt,result,map,num_of_tree);
    }
    for(int i=0;i<result.size();i++){
        if(result[i]!="newline"){
            cout << result[i] <<" ";
        }
        else cout << endl;
    }

    // Btree<int> t = Btree<int>(3);
    // Node<int> *p = new Node<int>(2);
    // // Node<int>* q=p;
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
    // // t.root->traverse();
    // cout << endl;
    // cout << t.root->FindBestId();

    // vector<Node<int>*> v;
    // t.root->FindAllGreaterThan(v,34);
    // // // // t.root ->FindAllNodeWithAKey(v,45);
    // // // // cout << endl;
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

    // ISERT INTO employee   VALUES ("Atena",   2019/7/6,55000);
}

// test case :
/*
13
CREATE TABLE employee (name string,joinDate timestamp,income int)
INSERT INTO employee   VALUES ("atena",   2019/7/6,55000)
INSERT INTO employee VALUES ("sajede",2020/5/24,20000)
INSERT INTO employee VALUES ("zahra",2019/4/5,70000)
INSERT INTO employee VALUES ("narges",2018/4/5,40000)
INSERT INTO employee VALUES ("mohammad",2020/1/1,67000)
DELETE FROM employee WHERE income<50000
INSERT INTO emp0 loyee VALUES ("asha",2018/12/5,60000)
SELECT * FROM employee WHERE name=="asha"
SELECT (name,income) FROM employee WHERE income>60000
CREATE TABLE empl (name string,joinDate timestamp,income int)
INSERT INTO empl VALUES ("narges",2018/4/5,40000)
SELECT (name,income) FROM empl WHERE income<60000
*/
