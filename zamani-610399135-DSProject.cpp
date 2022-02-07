#include <iostream>
#include <vector>
#include <math.h>
#include<unordered_map>

using namespace std;

template <typename T> class Node;

template <typename T> class Btree;

template <typename T>  class BtreeNode {
public:
   int t; // upper bound for number of childes in a btree node.
   Node<T> **D; // pointer to data
   BtreeNode<T> **childes; // pointer to btree node.
   int n;  // number of elemntes that exist in our btree node.
   bool leaf; // a boelean for determine a node is leaf or not.
   // bool *status; // a boolean for determine node deleted or not!

   BtreeNode(int tt, bool is_leaf){
       t = tt;
       leaf = is_leaf;
       n = 0;
       D = new Node<T>*[2 * t -1];
       childes = new BtreeNode<T>*[2 * t];
       for(int i=0;i<2*t-1;i++){
           D[i] = nullptr;
       }
       for(int i=0;i<2*t;i++){
           childes[i] = nullptr;
       }
    
   }

    ~BtreeNode() {
        for(int i=0;i<2*t-1;i++){
            delete D[i];
        }
        for(int i=0;i<2*t;i++){
            delete childes[i];
        }
    }
   void insertNonFull(Node<T>* tmp);
   void splitChild(BtreeNode<T>* ptr , int tmp);
   void traverse();
   void GetAllDataOfBTree(vector<Node<T>*>& v_nodes);
   bool is_valid(T key);
   bool findAndDelete(T key);
   int FindBestId();
   void FindAllNodeWithAKey(vector<Node<T>*>& v , T key);
   void FindAllLessThan(vector<Node<T>*>& v, T key);
   void FindAllGreaterThan(vector<Node<T>*>& v, T key);
   void deleteNode(vector<vector<Btree<T> >>& bt, int type0fcondition,T key,int col,unordered_map<string,int> map,string tableName);
};

template <typename T> class Node{
public:
    T data;  
    Node<T>* nextField;
    // BtreeNode<T>* self;
    bool status;

    Node(T tmp_data){
        data = tmp_data;
        nextField = nullptr;
        // self = nullptr;
        status = false;
    }
};

template<typename T> class Btree{
public:    
    BtreeNode<T>* root;
    int t;
    int numberOfNodes;
    int numberOfDeletedNodes;

    Btree(int tmp_t){
        root = nullptr;
        t = tmp_t;
        numberOfNodes = 0;
        numberOfDeletedNodes = 0;
    }
    bool insert(Node<T>* data);
    
};


template<typename T>
void BtreeNode<T>::splitChild(BtreeNode<T>* ptr , int tmp){
    
    BtreeNode<T> *newptr = new BtreeNode<T>(ptr->t, ptr->leaf);
    newptr-> n = t - 1;

    for (int j = 0; j <= t - 2; j++){
      newptr->D[j] = ptr->D[j+t];
    }  

    if (!ptr->leaf) {
      for (int j = 0; j <= t-1; j++)
        newptr->childes[j] = ptr->childes[j + t];
    }

    ptr->n = t - 1;
    for (int j = n; j >= tmp + 1; j--)
      childes[j + 1] = childes[j];

    childes[tmp + 1] = newptr;

    for (int j = n - 1; j >= tmp; j--){
      D[j+1] = D[j];
    }  
   
    D[tmp] = ptr -> D[t-1];
    n++;
}


template<typename T>
void BtreeNode<T>::insertNonFull(Node<T>* tmp){
    if(leaf){
        int i = n-1;
        while(i>=0 && tmp->data < D[i] -> data){
            int j= i+1;
            D[j] = D[i];
            i--;
        }
        i++;
        n++;
        
        D[i] = tmp; 
        
    }else{
        int i= n-1;
        while (i >= 0 && D[i]->data > tmp->data)
            i--;

        if (childes[i + 1]-> n == 2 * t -1 ) {
            splitChild(childes[i + 1] , i + 1);

            if (D[i + 1]->data < tmp->data)
                i++;
        }
        childes[i + 1]->insertNonFull(tmp);
    }
};
 

template<typename T>
bool Btree<T>::insert(Node<T>* data){
    numberOfNodes++;
    if(root == nullptr){
        root = new BtreeNode<T>(t,true);
        root -> D[0] = data;
        root->n +=1;
    }
    else if(root->n == 2 * t -1){
        
        BtreeNode<T>* pb = new BtreeNode<T>(t, false);

        pb -> childes[0] = root;

        pb->splitChild(root , 0);

        int i = 0;

        if (pb -> D[0]->data < data->data)
          i++;
          
        pb -> childes[i] -> insertNonFull(data);

        root = pb;
    }else{
        root -> insertNonFull(data);
    }   
    return true;
};


template <typename T>
bool BtreeNode<T>::is_valid(T key){
    for(int i=0;i<n;i++){
        if(D[i]->data == key && !D[i]->status)
            return true;
    }
    for(int i=0;i<n;i++){
        if(key<D[i]->data ){
            if(childes[i]!=nullptr)
                childes[i]->is_valid(key);
            else 
                return false;    
        }    
        else if(i==n-1){
            if(childes[i]!=nullptr)
                childes[n]->is_valid(key);
            else 
                return false;    
        }    
    }
    return false;
}


template <typename T>
bool BtreeNode<T>::findAndDelete(T key){
    for(int i=0;i<n;i++){
        if(D[i]->data == key && !D[i]->status){
            D[i]->status = true;
            return true;
        }    
    }
    for(int i=0;i<n;i++){
        if(key<D[i]->data ){
            if(childes[i]!=nullptr)
                childes[i]->findAndDelete(key);
            else 
                return false;    
        }    
        else if(i==n-1){
            if(childes[i]!=nullptr)
                childes[n]->findAndDelete(key);
            else 
                return false;    
        }
    }
    return false;
}


template<typename T>
void BtreeNode<T>::deleteNode(vector<vector<Btree<T> >>& bt, int type0fcondition,T key,int col,unordered_map<string,int> map,string tableName){
    vector<Node<long long>*> v;
    if(type0fcondition == 0){;
        bt[map[tableName]][col].root->FindAllNodeWithAKey(v,key);
    }else if (type0fcondition==2){
        bt[map[tableName]][col].root -> FindAllLessThan(v,key);
    }else if(type0fcondition== 1){
        bt[map[tableName]][col].root -> FindAllGreaterThan(v,key);
    }
    if(v.size()==0){
        // cout << "there are not any record with these limitations for delete!" << endl;
        return;
    }
    for(int i=0;i<v.size();i++){
        Node<long long>*p = v[i];
        while(p->nextField!=v[i]){
            p->status = true;
            p = p->nextField;
        }
        p->status = true;
        bt[map[tableName]][i].numberOfDeletedNodes++;
    }

    if( bt[map[tableName]][col].numberOfNodes>=50 &&  bt[map[tableName]][col].numberOfDeletedNodes > bt[map[tableName]][col].numberOfNodes/2){
        for(int i=0;i<bt.size();i++){
            vector<Node<T>*>v_nodes;
            bt[map[tableName]][i].root->GetAllDataOfBTree(v_nodes);
            delete bt[map[tableName]][i].root;
            int degree = bt[map[tableName]][i].t;
            bt[map[tableName]][i] = Btree<T>(degree);
            for(int j=0;j<v_nodes.size();j++){
                bt[map[tableName]][i].insert(v_nodes[j]);
            }
        }
    }
};


template <typename T>
void BtreeNode<T>::GetAllDataOfBTree(vector<Node<T>*>& v_nodes){
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf)
            childes[i]->GetAllDataOfBTree(v_nodes);
        if(! D[i]->status)
            // cout << " " << D[i]->data;
            v_nodes.push_back(D[i]);
  }
  j++; 
  if (!leaf)
    childes[j]->GetAllDataOfBTree(v_nodes);
}


template <typename T>
int BtreeNode<T>::FindBestId(){  
    vector<Node<T>*> v;
    GetAllDataOfBTree(v);
    for(int i=0;i<v.size();i++){
        if(v[i]->data!=i+1 || v[i]->status)
            return i+1;
    }
    return v.size()+1;
}

template <typename T>
void BtreeNode<T>::FindAllNodeWithAKey(vector<Node<T>*>& v , T key){
    // if(this == nullptr)
    //     return;
    for(int i=0;i<n;i++){
        if(D[i]!=nullptr && D[i]->data == key && !D[i]->status)
            v.push_back(D[i]);
    }
    for(int i=0;i<n;i++){
        if(key<=D[i]->data ){
            if(childes[i]!=nullptr)
                childes[i]->FindAllNodeWithAKey(v,key);   
        }    
        else if(i==n-1){
            if(childes[i]!=nullptr && childes[n]!=nullptr)
                childes[n]->FindAllNodeWithAKey(v,key);   
        }    
    }
    return ;
}


template <typename T>
void BtreeNode<T>::FindAllLessThan(vector<Node<T>*>& v, T key){
    // if(this == nullptr)
    //     return;
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf && childes[i]!=nullptr)
            childes[i]->FindAllLessThan(v,key);
        if(! D[i]->status && D[i]->data < key)
            v.push_back(D[i]);
            // cout << " " << D[i]->data;
        if(D[i]->data > key)   
            return; 
  }
  j++; 
  if (!leaf && childes[j]!=nullptr)
    childes[j]->FindAllLessThan(v,key);
}


template <typename T>
void BtreeNode<T>::FindAllGreaterThan(vector<Node<T>*>& v, T key){
    // if(this==nullptr)
    //     return;
    int j = 0;
    for (int i = n-1; i >=0 ; i--) {
        j=i;  
        if (!leaf && childes[i+1]!=nullptr)
            childes[i+1]->FindAllGreaterThan(v,key);
        if(! D[i]->status && D[i]->data > key )
            v.push_back(D[i]);
        else if(D[i]->data<key)
            return;    
  }
  if (!leaf && childes[j]!=nullptr)
    childes[j]->FindAllGreaterThan(v,key);
   
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// we asuume that we have small letters.
string letter = "abcdefghijklmnopqrstuvwxyz0123456789A";

// get the position of the letter in letter.
int ASCII(char tmp)
{
    for(int i=0;i<letter.length();i++){
        if(letter[i]==tmp)
            return i+1;
    }
    return 0;
}

// this function converts a string to int.
int convert_to_int(string str)
{
    return stoi(str);
}

// this function give you reverse of a string that you give it as input.
string reverse(string str)
{
    string res = "";
    for (int i = str.length() - 1; i >= 0; i--)
        res += str[i];
    return res;
}

// hash function for converting a timestamp to a long long.
// idea : after every fieldes we will push a zero.
long long timestamp_hash(string str)
{
    long long res = 0;
    int i = 0;
    for (int c = 0; c < 3; c++)
    {
        while (str[i] != '/')
        {
            res *= 10;
            res += str[i] - '0';
            i++;
            if (i >= str.length())
            {
                break;
            }
        }
        if (c != 2)
            res *= 10;
        i++;
    }
    return res;
}

// inverse of timestamp hash function
// if you give me the hashed timestamp i will return the timestamp.
string timestamp_hash_inverse(long long Time)
{
    string time = to_string(Time);
    string year = time.substr(0, 4);
    // cout << "year : " << year << endl;
    string day = "";
    int i = 5;
    while (i < 7)
    {
        if (time[i] != '0')
        {
            day += time[i];
        }
        else if (time[i] == '0' && time[i + 1] == '0')
        {
            day += time[i];
            i++;
            break;
        }
        i++;
    }
    // cout <<" day :" << day << endl;

    i = time.length() - 1;
    string month = "";
    month += time[i];
    i--;
    if (time[i] != '0')
        month += time[i];

    return year + '/' + day + '/' + reverse(month);
}

// base convertor. convert decimal to base b.
void convert10tob(long long N, int b , string& ans)
{
    if (N == 0)
        return;

    long long x = N % b;
    N /= b;

    if (x < 0)
        N += 1;

    convert10tob(N, b, ans);
    
    x= x < 0 ? x + (b * -1) : x;
    ans= ans + to_string(x) + '-';
    return;
};

// this function hash the string that you give me it as input and will retun a long long as hash.
// idea : polynomial hash for strings. 
long long string_hash(string str)
{
    long long res = 0;
    for (int i = 1; i < str.length()-1; i++)
    {
        res += (pow(letter.length(), i) * ASCII(str[i]));
    }
    return res;
}

// if you give me the hashed string i will givr you the first string.
string string_hash_inverse(long long hashed)
{
    string res = "";
    string ans ="";
    convert10tob(hashed , letter.length(), ans);
    // cout << ans << endl;
    int i=0;
    while(i<ans.length()){
        string tmp="";
        while(ans[i]!='-'){
            tmp+=ans[i];
            i++;
        }
        res+=letter[stoi(tmp)-1];
        i++;    
    }
    
    return reverse(res);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            
            int k = 0;
            for (int j = v_tmp.size() - col; j <= v_tmp.size() - 1; j++)
            {
                if (v_dataType[k] == 0)
                {
                    result.push_back(to_string(v_tmp[j]));
                }
                else if (v_dataType[k] == 1)
                {
                    result.push_back(string_hash_inverse(v_tmp[j]));
                }
                else if (v_dataType[k] == 2)
                {
                    result.push_back(timestamp_hash_inverse(v_tmp[j]));
                }
                k++;
            }
            for (int j = 0; j < v_tmp.size() - col; j++)
            {
                if (v_dataType[k] == 0)
                {
                    result.push_back(to_string(v_tmp[j]));
                }
                else if (v_dataType[k] == 1)
                {
                    result.push_back(string_hash_inverse(v_tmp[j]));
                }
                else if (v_dataType[k] == 2)
                {
                    result.push_back(timestamp_hash_inverse(v_tmp[j]));
                }
                k++;
            }
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
                
                tmp->data = d;
                tmp = tmp->nextField;
                if (m == v_dataType.size() - 1)
                {
                    m = 0;
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
        map[tableName] = num_of_tree;
        num_of_tree++;
        

        string str = "";
        for (int j = 0; j < query.length(); j++)
        {
            if (query[j] == '(')
            {
                str = query.substr(j);
                break;
            }
        }
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
        
        if (tmp != "INSERT INTO ")
            throw query;
        string tableName="";
        int l = 12;
        while(query[l]!=' '){
            tableName+=query[l];
            l++;
        }
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

        for (int i = 1; i < v_dataType[map[tableName]].size(); i++)
        {
            long long data = 0;

            if (v_dataType[map[tableName]][i] == 0)
            {
                data = stoi(v_allDataToInsert[i - 1]);
            }
            else if (v_dataType[map[tableName]][i] == 1)
            {
                data = string_hash(v_allDataToInsert[i - 1]);
            }
            else if (v_dataType[map[tableName]][i] == 2)
            {
                data = timestamp_hash(v_allDataToInsert[i - 1]);
            }
            Node<long long> *p = new Node<long long>(data);
            prv->nextField = p;
            bt[map[tableName]][i-1].insert(prv);
            prv = p;
            // insert it to our btree.
            if (i == bt[map[tableName]].size() - 1)
            {
                p->nextField = ID; // the last one have a pointer to first one.
                bt[map[tableName]][i].insert(p);
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
       
        string colName = colNameOfCondition(condition);
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

        if (tmp != "DELETE FROM ")
            throw query;
        int l = 12;
        string tableName = "";
        while(query[l]!=' '){
            tableName += query[l];
            l++;
        }
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
        if (query.substr(i, 5) != "WHERE")
            throw query;

        i += 6;
        condition = query.substr(i);

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
        long long key = 0;
        if (v_dataType[map[tableName]][col] == 0)
        {
            key = stoi(limit);
        }
        else if (v_dataType[map[tableName]][col] == 1)
        {
            key = string_hash(limit);
        }
        else if (v_dataType[map[tableName]][col] == 2)
        {
            key = timestamp_hash(limit);
        }
        bt[map[tableName]][col].root->deleteNode(bt,typecondition,key,col,map,tableName);
        
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
        if (query.substr(i, 5) != "WHERE")
            throw query;

        i += 6;
        condition = query.substr(i);

        string colName = colNameOfCondition(condition);
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
}
