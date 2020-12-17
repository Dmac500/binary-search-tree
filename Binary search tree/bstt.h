/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* beginNum; // the left most node

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
     beginNum = nullptr;
  }
// helper to copy and = 
void _copy(NODE* cur) 
{

if(cur == nullptr)
{
return;
}

insert(cur->Key , cur->Value);
_copy(cur->Left);
if (cur->isThreaded == false)
{
_copy(cur->Right);
}

}
  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
  this->Root = nullptr;
  this->Size = 0;
  _copy(other.Root);
 
  }



	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
 
  _bstt(Root);
 
 
  }
 
 
 // helper to the clear and destructor 
 
  void _bstt(NODE* cur)
  {
  if (cur == nullptr)
  {
  return;
  }
  else{
 
  _bstt(cur->Left);
  if(!cur->isThreaded)
  {
   _bstt(cur->Right);
  }
  
    delete cur;
    }

  }
  
  
  
  

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
  
 this->clear();
   
   _copy(other.Root);

    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear() // deletes the tree
  {
 if(Root == nullptr){
 return;
 }
  _bstt(Root);
  Root = nullptr;
   Size = 0;
   
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const // size of the tree
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
     NODE* cur = Root;
     
    while (cur != nullptr)
    {
      if (key == cur->Key )  // already in tree
      {
      value = cur->Value;
        return true ;
      }

      else if (key < cur->Key)
      {
        // search left:
      
        cur = cur->Left;
      }
      else
      {
        if (cur->isThreaded)
            cur = nullptr;
        else 
        cur = cur->Right;
      }
    }//while  
    
		return false;
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr;
    NODE* curr = Root; // pointer I need to do incert
    
   
      
      while(curr != nullptr)// search tree to see if the key exception
      {
         if (key == curr->Key  )  
      {
        return;
      }

      if (key < curr->Key)
      { // search left:
        prev = curr;
        curr = curr->Left;
      }
      else
      {
         if (curr->isThreaded)
         {
         prev = curr;
          curr = nullptr;
         }
         else 
         {
         prev = curr;
        curr = curr->Right;
        }
      }
      }
     NODE* newNode = new NODE(); // new node to add
     newNode->Left = nullptr;
     newNode->Right = nullptr;
     newNode->Key = key;
     newNode->Value = value;
   
      // adding on the new node
     if (prev == nullptr)
      Root = newNode;
 else if (key < prev->Key)
 {
      prev->Left = newNode;
      newNode->isThreaded = true;
      newNode->Right = prev;
      
      }
      
else
{
if (prev->isThreaded == true)
{
   
    newNode->isThreaded = true;
    newNode->Right = prev->Right;
    prev->Right = newNode;
    prev->isThreaded = false;
    

    
}
else {
      prev->Right = newNode;
      newNode->isThreaded = true;
      }
      }
      // increasing size of the tree
      
      Size++;
      
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const // this is to pull the value out
  {
  ValueT val;
    if(search(key,val) == true)
    {
      return val;
    }

   return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) // this is so I can get the key to the right of the key I am on
  {
  NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key ) 
      {
          if(cur ->Right != nullptr)
          {
        return cur->Right->Key;
        }
        else
        { 
        return KeyT{};
        }
      }

      if (key < cur->Key)
      {
        // search left:
      
        cur = cur->Left;
      }
      else
      {
        if (!cur->isThreaded)
          cur = cur->Right;
        else 
       return KeyT{ };
      }
    }//while
    

    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
  if(Root == nullptr)
  {
      return;
  }

    NODE* temp = Root;
   
        while(temp ->Left!= nullptr){ // loop down to the most left node.
        temp = temp->Left;
        }
        beginNum = temp;
  }
  int beginNumGet() // testing 
  {
     return beginNum->Key;
  
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key) // this is to get the next pointer.
  {
  if(beginNum == nullptr)
  {
  return false;
  }
   NODE* cur = beginNum;

  //key = cur -> Key;

  if(cur == nullptr)
  {
   return false;
  }
  else if (cur->isThreaded && cur->Right != nullptr){
      key = cur -> Key;
      cur = cur->Right;
      beginNum = cur;
      return true;
  }
  else if(cur->Right == nullptr){
      key = cur -> Key;
      cur = nullptr;
      beginNum = cur;
      return true;
  }

  else
  {
       key = cur -> Key;
       cur = cur->Right;
       beginNum = cur;

  while(cur->Left != nullptr)
  {
      cur = cur->Left;
      beginNum = cur;

  }
return true;

  }


  return false;
  }
void _dump(NODE* cur ,ostream& output) const // this is a helper to print the tree.
{

   if(cur == nullptr){
   return;
   }
   else {
   _dump(cur->Left,output );
     if(cur->isThreaded && cur->Right != nullptr)
     {
       output <<"("<< cur->Key << ","<< cur->Value << "," <<cur->Right->Key << ")"<<endl;
       }
      else {
      output <<"("<< cur->Key << ","<< cur->Value <<")"<<endl;
      }
      if (cur->isThreaded){
      return;
        }
      else{
    _dump(cur->Right,output );
    
   }
   
   
   
   }



}
  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
 
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
    if(Root == nullptr){
    output << "NO TREE" << endl;
    }
     _dump(Root, output);


    output << "**************************************************" << endl;
  }








};