#include <iostream>

using namespace std;

template <typename T> class Node;

template<typename T> class Btree;

template <typename T>  class BtreeNode {
public:
   T *keys;
   int t; // upper bound for number of keys in a node.
   Node<T> **D; // pointer to data
   BtreeNode<T> **childes; // pointer to btree node.
   int n;  // number of elemntes that exist in our btree node.
   bool leaf; // a boelean for determine a node is leaf or not.

   BtreeNode(int tt, bool is_leaf){
      t = tt;
      leaf = is_leaf;
      n = 0;
      keys = new T[ 2 * t -1];
      D = new Node<T>*[2*t -1];
      childes = new BtreeNode<T>*[2*t];
   }

   void insertNonFull(int tmp);
   void splitChild(BtreeNode<T>* ptr , int tmp);
   void traverse();
   bool deleteNode(T d);
   int findInKeys(T d);
   void removeFromLeaf(int index);
   void removeFromNonLeaf(int index);
   int getPredecessor(int idx); //The largest key on the left child of a node is called its inorder predecessor.
   int getSuccessor(int idx); //The smallest key on the right child of a node is called its inorder successor.
   void fill(int idx);
   void borrowFromPrev(int idx);
   void borrowFromNext(int idx);
   void merge(int idx);
};

template <typename T> class Node{
public:
    T data;
    Node<T>* nextField;
    BtreeNode<T>* self;

    Node(T tmp_data){
        data = tmp_data;

    }
};

template<typename T> class Btree{
public:    
    BtreeNode<T>* root;
    int t;

    Btree(int tmp_t){
        root = nullptr;
        t = tmp_t;
    }
    bool insert(T data);
    bool deleteNode(T d);
    
};

template<typename T>
void BtreeNode<T>::insertNonFull(int tmp){
    if(leaf){
        int i = n-1;
        while(i>=0 && tmp<keys[i]){
            int j= i+1;
            keys[j] = keys[i];
            // cout << "j: " << keys[j] << "i: "<< keys[i] << endl;
            D[j] = D[i];
            i--;
        }
        i++;
        keys[i] = tmp;
        n++;
        // cout << "keys[i] "<< keys[i] << "    "<<" tmp "<< tmp << "i    "<<i << endl;
        // cout << keys[1] << endl;
        // for(int i=0;i<n;i++)
        //     cout << keys[i] << " k";
        // cout << endl;    
        D[i] = new Node<T>(tmp);
        // set next fieldes
        
    }else{
        int i= n-1;
        while (i >= 0 && keys[i] > tmp)
            i--;

        if (childes[i + 1]-> n == 2 * t -1 ) {
            splitChild(childes[i + 1] , i + 1);

            if (keys[i + 1] < tmp)
                i++;
        }
        childes[i + 1]->insertNonFull(tmp);
    }
}

template<typename T>
bool Btree<T>::insert(T data){
    if(root == nullptr){
        root = new BtreeNode<T>(t,true);
        root -> D[0] = new Node<T>(data);
        root -> keys[0] = data;
        // set next fieldes
        root->n +=1;
    }
    else if(root->n == 2 * t -1){
        
        BtreeNode<T>* pb = new BtreeNode<T>(t, false);

        pb -> childes[0] = root;

        pb->splitChild(root , 0);

        int i = 0;

        if (pb -> keys[0] < data)
          i++;
          
        pb -> childes[i] -> insertNonFull(data);

        root = pb;
    }else{
        root -> insertNonFull(data);
    }   
    return true;
}

template<typename T>
void BtreeNode<T>::splitChild(BtreeNode<T>* ptr , int tmp){
    
    BtreeNode<T> *newptr = new BtreeNode<T>(ptr->t, ptr->leaf);
    newptr-> n = t - 1;

    for (int j = 0; j < t - 1; j++){
      newptr->keys[j] = ptr->keys[j + t];
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
      keys[j + 1] = keys[j];
      D[j+1] = D[j];
    }  

    keys[tmp] = ptr->keys[t - 1];
    D[tmp] = ptr -> D[t-1];
    n++;
}


template <typename T>
void BtreeNode<T>::traverse(){
    int j = 0;
    for (int i = 0; i < n; i++) {
        j=i;  
        if (!leaf)
            childes[i]->traverse();

        cout << " " << D[i]->data;
  }
  j++; 
  if (!leaf)
    childes[j]->traverse();
}

template <typename T>
int BtreeNode<T>::findInKeys(T d){
    for(int i=0;i<n;i++){
        if(keys[i]>=d)
            return i;
    }
    return n;
}

template<typename T>
bool BtreeNode<T>::deleteNode(T d){
    int idx = findInKeys(d);

  if (idx < n && keys[idx] == d) {
    if (leaf)
      removeFromLeaf(idx);
    else
      removeFromNonLeaf(idx);
  } else {
    if (leaf) {
      cout << "The key " << d << " is does not exist in the tree\n";
      return false;
    }

    bool flag = ((idx == n) ? true : false);

    if (childes[idx]->n < t)
      fill(idx);

    if (flag && idx > n)
      childes[idx - 1]->deleteNode(d);
    else
      childes[idx]->deleteNode(d);
  }
  return true;
}

template<typename T>
void BtreeNode<T>::removeFromLeaf(int index){
    for (int i = index + 1; i < n; ++i){
        keys[i - 1] = keys[i];
        D[i-1] = D[i];
    }
    n--;

    return;
}

template <typename T>
void BtreeNode<T>::removeFromNonLeaf(int index) {
  T k = keys[index];

  if (childes[index]->n >= t) {
    T pred = getPredecessor(index);
    keys[index] = pred;  // in this section i should set D
    BtreeNode<T> *cur = childes[index];
    while (!cur->leaf)
      cur = cur->childes[cur->n];
    D[index] = cur->D[cur->n-1];  
    childes[index]->deleteNode(pred);
  }

  else if (childes[index + 1]->n >= t) {
    T succ = getSuccessor(index);
    keys[index] = succ; // in this section i should set D
    BtreeNode<T> *cur = childes[index+1];
    while (!cur->leaf)
      cur = cur->childes[0];
    D[index] = cur->D[0];

    childes[index + 1]->deleteNode(succ);
  }

  else {
    merge(index);
    childes[index]->deleteNode(k);
  }
  return;
}

template <typename T>
int BtreeNode<T>::getPredecessor(int idx) {
  BtreeNode<T> *cur = childes[idx];
  while (!cur->leaf)
    cur = cur->childes[cur->n];

  return cur->keys[cur->n - 1];
}

template<typename T>
int BtreeNode<T>::getSuccessor(int idx) {
  BtreeNode<T> *cur = childes[idx + 1];
  while (!cur->leaf)
    cur = cur->childes[0];

  return cur->keys[0];
}

template <typename T>
void BtreeNode<T>::fill(int idx) {
  if (idx != 0 && childes[idx - 1]->n >= t)
    borrowFromPrev(idx);

  else if (idx != n && childes[idx + 1]->n >= t)
    borrowFromNext(idx);

  else {
    if (idx != n)
      merge(idx);
    else
      merge(idx - 1);
  }
  return;
}

template <typename T>
void BtreeNode<T>::borrowFromPrev(int idx) {
  BtreeNode<T> *child = childes[idx];
  BtreeNode<T> *sibling = childes[idx - 1];

  for (int i = child->n - 1; i >= 0; --i){
    child->keys[i + 1] = child->keys[i];
    child->D[i+1] = child->D[i];
  }
  if (!child->leaf) {
    for (int i = child->n; i >= 0; --i)
      child->childes[i + 1] = child->childes[i];
  }

  child->keys[0] = keys[idx - 1];
  child->D[0] = D[idx-1];

  if (!child->leaf)
    child->childes[0] = sibling->childes[sibling->n];

  keys[idx - 1] = sibling->keys[sibling->n - 1];
  D[idx-1] = sibling->D[sibling->n-1];

  child->n += 1;
  sibling->n -= 1;

  return;
}

// Borrow from the next
template <typename T>
void BtreeNode<T>::borrowFromNext(int idx) {
  BtreeNode<T> *child = childes[idx];
  BtreeNode<T> *sibling = childes[idx + 1];

  child->keys[(child->n)] = keys[idx];
  child->D[(child->n)] = D[idx];

  if (!(child->leaf))
    child->childes[(child->n) + 1] = sibling->childes[0];

  keys[idx] = sibling->keys[0];
  D[idx] = sibling->D[0];

  for (int i = 1; i < sibling->n; ++i){
    sibling->keys[i - 1] = sibling->keys[i];
    sibling->D[i - 1] = sibling->D[i];
  }  

  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->n; ++i)
      sibling->childes[i - 1] = sibling->childes[i];
  }

  child->n += 1;
  sibling->n -= 1;

  return;
}

// Merge
template <typename T>
void BtreeNode<T>::merge(int idx) {
  BtreeNode<T> *child = childes[idx];
  BtreeNode<T> *sibling = childes[idx + 1];

  child->keys[t - 1] = keys[idx];
  child->D[t - 1] = D[idx];

  for (int i = 0; i < sibling->n; ++i){
    child->keys[i + t] = sibling->keys[i];
    child->D[i + t] = sibling->D[i];
  }  

  if (!child->leaf) {
    for (int i = 0; i <= sibling->n; ++i)
      child->childes[i + t] = sibling->childes[i];
  }

  for (int i = idx + 1; i < n; ++i){
    keys[i - 1] = keys[i];
    D[i - 1] = D[i];
  }  

  for (int i = idx + 2; i <= n; ++i)
    childes[i - 1] = childes[i];

  child->n += sibling->n + 1;
  n--;

  delete (sibling);
  return;
}

template<typename T>
bool Btree<T>::deleteNode(T d){
    // in oor btree max degree of child is t
    // so every btree node have at most t-1 keys
    // our node should have [m/2] keys in the minimum case
    if(root == nullptr){
        cout << "The Btree is empty!!";
        return false;

    }
    root->deleteNode(d);

    if (root->n == 0) {
        BtreeNode<T> *tmp = root;
        if (root->leaf == true)
            root = nullptr;

        else
            root = root->childes[0];

        delete tmp;
  }
  return true;
}


int main(){
    // 1. we get query of making a table
    // 2. we shold requgnize types and every field name
    // example : CREATE TABLE employee (name string,joinDate timestamp,income int)
    // insert  
    Btree<int> t = Btree<int>(3);
    

    t.insert(8);
    t.insert(9);
    t.insert(10);
    t.insert(11);
    t.insert(15);
    // t.insert(16);
    t.insert(20);
    // t.insert(18);
    t.insert(17);
    // t.insert(23);
    t.root -> traverse();
    cout << endl;
    t.deleteNode(11);
    if(t.root!=nullptr){
        t.root -> traverse();
    }
    // cout << endl;
    // for(int i=0;i<2;i++){
    //     cout << t.root->childes[1]->keys[i] <<" ";
    // }
    
}
