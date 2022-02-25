#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{

   protected:
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache: public Cache {
public:

   LRUCache( int capacity ) {

    this->head = nullptr;
    this->tail = nullptr;
    this->cp = capacity;

    //this->size = 0;

    for ( int i=0; i < capacity; i++ ) {

        Node *node = new Node { nullptr, nullptr, 0, 0 };

        if ( head == nullptr ) {

            head = node;
            tail = node;

            head->prev = nullptr;
            head->next = tail;

            tail->prev = head;
            tail->next = nullptr;

        }
        else {

            node->prev = tail;
            node->next = nullptr;

            tail->next = node;

            if ( head == tail ) {

                //cout << "hola" << endl;
                head->next = node;

            }

            tail = node;

        }

    }

    /*
    Node *node = head;

    while ( node ) {

        cout << "Key: " << node->key << ", Value: " << node->value << endl;

        node = node->next;

    }
    */

   }

   ~LRUCache() {

      Node *node = head;

      while ( node ) {

          Node *tmp = node;

          node = node->next;

          delete tmp;

      }


   }

   inline void set(int key, int value) override {

    //cout << "set key: " << key << ", value: " << value << endl;

    if ( this->mp[ key ] ) {

        //cout << " exists" << endl;

        this->mp[ key ]->value = value;

    }
    else {

        Node *node = new Node( head, nullptr, key, value );

        if ( head != nullptr && head == tail ) {

            this->mp[ head->key ] = nullptr;

            delete head;

            head = node;
            tail = node;

            this->mp[ key ] = this->head;

        }
        else {

            this->mp[ node->key ] = node;

            this->mp[ tail->key ] = nullptr;

            //cout << "head->next: " << head->next << endl;

            //cout << "head->next->prev: " << head->next->prev << endl;

            head->prev = node;
            node->next = head;

            //head->next->prev = node;

            head = node;

            node = tail->prev;

            node->next = nullptr;

            delete tail;

            tail = node;

        }

        /*
        cout << " not exists" << endl;

        //cout << "head key: " << key << ", value: " << value << endl;

        if ( this->head ) {

           this->mp[ this->head->key ] = nullptr;

        }

        this->head->key = key;
        this->head->value = value;

        this->mp[ key ] = this->head;
        */

    }

    /*
    //   Node *node = head;

    //   while ( node ) {

    //       if ( node->key == key  ) {

    //           node->value = value;
    //           this->mp[ key ] = node;
    //           break;

    //       }

    //       node = node->next;

    //   }

    if ( head != nullptr && tail != nullptr ) {

       Node *node = head;

       this->size += 1;

    //    int count = 0;

    //    while ( node ) {

    //        count += 1;

    //        node = node->next;

    //    }

       if ( this->size >= this->cp ) {

           cout << "this->size: " << this->size << ", key: " << key << ", value: " << value << endl;

           if ( head != tail ) {

               //cout << "head key: " << head->key << ", value: " << head->value << endl;

               //head->key = key;
               //head->value = value;

               node = head;

               head->next->prev = nullptr;
               head = head->next;

               //this->mp.erase( node->key );
               this->mp[ node->key ] = nullptr;

               delete node;

           }
           else {

               this->mp[ node->key ] = nullptr;
               //this->mp.erase( head->key );

               delete head;

               head = nullptr;
               tail = nullptr;

           }

       }

    }

    if ( head == nullptr ) {

        Node* node = new Node( nullptr, nullptr, key, value );

        head = node;
        tail = node;

        this->mp[ key ] = node;

    }
    else {  // if ( head == tail ) {

        Node* node = new Node( tail, nullptr, key, value );

        //cout << "Tail "<< endl;

        tail->next = node;
        //node->prev = tail;

        if ( head == tail ) {

           head->next = node;

        }

        tail = node;

        this->mp[ key ] = node;

    }
    // else {

    //     Node* node = new Node( head, nullptr, key, value );

    //     tail->next = node;
    //     node->prev = tail;

    //     tail = node;

    // }
    */

   }

   inline int get(int key) override {

    int result = -1;

    Node *node = this->mp[ key ];

    if ( node ) {

        /*
        if ( node != head && node == tail ) {

           node->prev->next = nullptr;
           tail = node->prev;

           head->prev = node;
           node->next = head;

           head = node;

        }
        else if ( node != head ) {

           node->prev->next = node->next;
           node->next->prev = node->prev;

           head->prev = node;
           node->next = head;

           head = node;

        }
        */

        result = node->value; //this->mp[ key ]->value;

    }

    return result;

   }

//private:
//   u_int32_t size { 0 };

};

int main() {
