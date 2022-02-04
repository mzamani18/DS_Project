#include <iostream>
#include <vector>

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
   bool *status; // a boolean for determine node deleted or not!

   BtreeNode(int tt, bool is_leaf){
       t = tt;
       leaf = is_leaf;
       n = 0;
       D = new Node<T>*[2 * t -1];
       childes = new BtreeNode<T>*[2 * t];
       status = new bool[t-1];
       for(int i=0;i<t;i++){
           status[i] = false;
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
   
};

template <typename T> class Node{
public:
    T data;  // check it because it was int!
    Node<T>* nextField;
    BtreeNode<T>* self;

    Node(T tmp_data){
        data = tmp_data;
        nextField = nullptr;
        self = nullptr;
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
    void deleteNode(Node<T>* p);

    ~Btree(){
        delete [] root;
    }
};


template<typename T>
void BtreeNode<T>::splitChild(BtreeNode<T>* ptr , int tmp){
    
    BtreeNode<T> *newptr = new BtreeNode<T>(ptr->t, ptr->leaf);
    newptr-> n = t - 1;

    for (int j = 0; j < t - 1; j++){
    //   newptr->keys[j] = ptr->keys[j + t];
      newptr->D[j] = ptr->D[j+t];
    }  

    if (!ptr->leaf) {
      for (int j = 0; j < t; j++)
        newptr->childes[j] = ptr->childes[j + t];
    }

    ptr->n = t - 1;
    for (int j = n; j >= tmp + 1; j--)
      childes[j + 1] = childes[j];

    childes[tmp + 1] = newptr;

    for (int j = n - 1; j >= tmp; j--){
    //   keys[j + 1] = keys[j];
      D[j+1] = D[j];
    }  

    // keys[tmp] = ptr->keys[t - 1];
    D[tmp] = ptr -> D[t-1];
    n++;
}


template<typename T>
void BtreeNode<T>::insertNonFull(Node<T>* tmp){
    if(leaf){
        int i = n-1;
        while(i>=0 && tmp->data < D[i] -> data){
            int j= i+1;
            // keys[j] = keys[i];
            // cout << "j: " << keys[j] << "i: "<< keys[i] << endl;
            D[j] = D[i];
            i--;
        }
        i++;
        // keys[i] = tmp;
        n++;
        // cout << "keys[i] "<< keys[i] << "    "<<" tmp "<< tmp << "i    "<<i << endl;
        // cout << keys[1] << endl;
        // for(int i=0;i<n;i++)
        //     cout << keys[i] << " k";
        // cout << endl;    
        D[i] = new Node<T>(tmp->data);
        // set next fieldes
        
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
        // set next fieldes
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
        if(D[i]->data == key && !status[i])
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
        if(D[i]->data == key && !status[i]){
            status[i] = true;
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
void Btree<T>::deleteNode(Node<T>* p){
    if(numberOfDeletedNodes > numberOfNodes/2){
        vector<Node<T> *> v_nodes;
        root->GetAllDataOfBTree(v_nodes);
        int degree = root->t;
        // ~Btree();
        delete [] root;
        Btree<T> newbtree = Btree<T>(degree);
        for(int i=0;i<v_nodes.size();i++){
            if(v_nodes[i]->data!=p->data)
                newbtree.insert(v_nodes[i]);
        }
        // return newbtree;
        // return newbtree;
        // delete btree 
        // make a new btree
        // if(p->data!=v_nodes[i]->data)
        // insert to btree
        //rebuilt_btree();
        // return;
    }
    else{
        // cout << "numberofNodes" << numberOfNodes << endl;
        // cout << "numberofdeleted" << numberOfDeletedNodes << endl;
        root->findAndDelete(p->data);  // if we had same data we will get help from next fieldes!
        numberOfDeletedNodes++;
        // return;
        // vector<Node<T> *> v_nodes;
        // root->GetAllDataOfBTree(v_nodes);
        // cout << endl;
        // for(int i=0;i<v_nodes.size();i++){
        //     cout << v_nodes[i]->data << " ";
        // }
    }
};


template <typename T>
void BtreeNode<T>::GetAllDataOfBTree(vector<Node<T>*>& v_nodes){
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf)
            childes[i]->traverse();
        if(! status[i])
            // cout << " " << D[i]->data;
            v_nodes.push_back(D[i]);
  }
  j++; 
  if (!leaf)
    childes[j]->traverse();
}


template <typename T>
int BtreeNode<T>::FindBestId(){
    int index=1;
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf)
            childes[i]->traverse();
        if(! status[i])
            // cout << " " << D[i]->data;
            // v_id.push_back(D[i]->data);
            if(index!=D[i]->data)
                return index;
            else{
                index++;
            }    
  }
  j++; 
  if (!leaf)
    childes[j]->traverse();
  return index;  
}

template <typename T>
void BtreeNode<T>::FindAllNodeWithAKey(vector<Node<T>*>& v , T key){
    for(int i=0;i<n;i++){
        if(D[i]->data == key && !status[i])
            v.push_back(D[i]);
    }
    for(int i=0;i<n;i++){
        if(key<=D[i]->data ){
            if(childes[i]!=nullptr)
                childes[i]->FindAllNodeWithAKey(v,key);   
        }    
        else if(i==n-1){
            if(childes[i]!=nullptr)
                childes[n]->FindAllNodeWithAKey(v,key);   
        }    
    }
    return ;
}

template <typename T>
void BtreeNode<T>::FindAllLessThan(vector<Node<T>*>& v, T key){
    for(int i=0;i<n;i++){
        if(D[i]!=nullptr && D[i]->data > key){
            if(!status[i])
                v.push_back(D[i]);
            if(childes[i]!=nullptr)    
                childes[i]-> FindAllLessThan(v,key); 

            if(childes[i+1]!=nullptr)
                childes[i+1]->FindAllLessThan(v,key);      
        }

    }
}
template <typename T>
void BtreeNode<T>::traverse(){
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf)
            childes[i]->traverse();
        if(! status[i])
            cout << " " << D[i]->data;
  }
  j++; 
  if (!leaf)
    childes[j]->traverse();
};

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
    
//     cout << endl;
//     t.deleteNode(q);
//     t.root->traverse();
//     cout << endl << "test " << t.root->is_valid(100);
//     // t.insert(9);
//     // t.insert(10);
//     // t.insert(11);
//     // t.insert(15);
//     // t.insert(16);
//     // t.insert(17);
//     // t.insert(18);
//     // t.insert(20);
//     // t.insert(23);
//     // t.traverse();
// };