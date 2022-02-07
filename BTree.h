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


// template <typename T>
// void BtreeNode<T>::traverse(){
//     int j = 0;
//     for (int i = 0; i < n; i++) {
//         j=i;  
//         if (!leaf)
//             childes[i]->traverse();
//         if(! D[i]->status )
//             cout << " " << D[i]->data;
//     }        
//   j++; 
//   if (!leaf)
//     childes[j]->traverse();
// };

// test :
// int main(){
//     // 1. we get query of making a table
//     // 2. we shold requgnize types and every field name
//     // example : CREATE TABLE employee (name string,joinDate timestamp,income int)
//     // insert  
    // Btree<int> t = Btree<int>(3);
    // Node<int> *p = new Node<int>(8);
    // Node<int>* q=p;
    // t.insert(p);
    // p = new Node<int>(10);
    // t.insert(p);
    // p = new Node<int>(9);
    // t.insert(p);
    // p = new Node<int>(11);
    // t.insert(p);
    // t.root->traverse();
    // cout << endl;
    // t.deleteNode(q);
    // t.root->traverse();
    // cout << endl << "test " << t.root->is_valid(100);
    // t.insert(9);
    // t.insert(10);
    // t.insert(11);
    // t.insert(15);
    // t.insert(16);
    // t.insert(17);
    // t.insert(18);
    // t.insert(20);
    // t.insert(23);
    // t.traverse();
// }; 
