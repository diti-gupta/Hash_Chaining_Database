#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;


/**  MovieHashTable.cpp:
 * 
 * 
 * MovieHashTable.cpp implements the MovieHashTable class and its functions.
 * Gets a hash value for each key, Inserts, Searches, and returns number of collisions
 * **/


/** MovieHashTable() - Constructor for MovieHashTable with default size
 * 
 * @param none
 * @return void
 **/ 
MovieHashTable::MovieHashTable() {
    // TODO
    table_size=DEFAULT_HTABLE_CAPACITY; //setting table_size to default values

    //creating the table with a new MovieNode for each position in table
    table=  new MovieNode*[table_size]; 

    n_collisions = 0; //default number of collisions = 0
   
    for (int i =0;i<table_size;i++)
    {
        //all the values in the table at position i are intitalized to null, later changed to hash value
        table[i]=nullptr; 
    }
}


 /** MovieHashTable(int s) - Constructor for MovieHashTable with given size
 * @param int
 * @return void 
 * **/
MovieHashTable::MovieHashTable(int s) {
    // TODO
    table_size=s; //setting table_size to the passed in parameter

    //creating the table with a new MovieNode for each position in table
    table= new MovieNode*[table_size];
    n_collisions = 0;

    //all the values in the table at position i are intitalized to null, later changed to hash value
    for (int i =0;i<table_size;i++)
    {
        
        table[i]= nullptr; 
    }


}


/** Destructor for MovieHashTable that deletes all nodes in the hash table
 * Destructor - deletes all the nodes in the hash table 
 * 
 * @param none
 * @return void
 **/ 
MovieHashTable::~MovieHashTable()
{
   
    // TODO
   MovieNode* curr_HT = nullptr;
    MovieNode* temp;
   
      // iterate through all buckets in the table
        for (int i = 0; i < table_size; i++) 
        {
            // get the head of the linked list at this bucket
            curr_HT = table[i];

            // iterate through the linked list and delete all nodes
            while (curr_HT != nullptr) {
                temp = curr_HT; //temp is the current (used later to delete)

                //changing curr to its next value, so we can delete that node next
                curr_HT = curr_HT->next; 

                delete temp; //deleting the orginal curr node

                //setting temp to nullptr to avoid memory leaks with deletion (no extra memory allocated)
                temp=nullptr; 
            }
            table[i]=nullptr; //setting each value in the table to null, to avoid extra memory allocation
        }
    
        delete[] table; //deleting table, and setting to null, avoiding extra memory allocation 
        table=nullptr; 
    }
           


// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// Students must use their identikey to come up with a creative hash function that minimizes collisions
// for the given IMDB-Movie database to ensure efficient insertion and retrieval of movie nodes.

/** Hash - calculates the hash of a key 
 * 
 * @param title
 * @return int
 **/ 

int MovieHashTable::hash(string title) {
    // TODO
    int hsh = 0;
    int title_total=0;
    int identikey_val=0;
    string identikey="digu6278";


    for (int i =0;i<identikey.length();i++) //iterate through my identikey
    {
        identikey_val+= (i * identikey[i]); //adding the (ascii value * i)  to identikey_total
    }


    for (int j =0;j<title.length();j++)
    {    
        title_total+= (j * title[j]); //adding the (ascii value * j) to total
    }
    //formula for calculating the hash value
    //(identikey_val + title_total)% 2000 decreases the collisions to 212
    hsh = (title_total + identikey_val) % 2000; 

    return hsh; //returning the hash value
    return -1;
}


// Inserts a movie node into the hash table with the specified title
/** Insert - insert a movieNode into the HashTable, after getting its hash value
 * 
 * @param title 
 * @param movie
 * @return void
 **/ 
void MovieHashTable::insert(string title, MovieNode* movie)
{
    // TODO
    MovieNode* new_movie= movie; //storing the movie we want to add into a movieNODE
    int hash_value=hash(title); //hash the movie for it's index in the HTable

    //if the hash_value position in the hash Table is empty, add the node in 
    if (table[hash_value] == nullptr) 
    {
        table[hash_value]= movie; //adding the movie node at the hash value position in table
    }

    //else if there is a node at the hash_value of the HTable, deal with collision w/ chaining
    else 
    {
        setCollisions(); //update the number of collisions in the table, by calling set collisions

        //get the last node at the Hash_Value position in the Table, 
        //this node's next would be the node we want to insert

        MovieNode* last_node = table[hash_value]; 
        
        //last node's next address should be NULL, 
        //if not iterate to the position where the last_node->next =NULL

        while(last_node->next != NULL)
        {
            last_node = last_node->next;
        }

        //add the newNode at the end of the linked list, as the next of the last_node
        last_node->next = new_movie;
    }
}






// Searches for a node in the hash table with the specified title
/** Search - Searches for a node in the hash table with the specified value
 * 
 * @param title 
 * @return MovieNode* (pointer to a MovieNode)
 **/ 
MovieNode* MovieHashTable::search(string title)
{
    // TODO

    //get the hash value(the position in table of where to insert) of the title
    int search_hash_val= hash(title); 

    //create a curr_node to keep track of the value at the hashed position of the table
    MovieNode* curr_node = table[search_hash_val];
    //using a boolean to keep track if the movie was found (originally not found)
    bool movie_found=false;

    if (curr_node!=nullptr) //checking if curr_node is not null, if curr_node is null, we have messed up hash_values
    {
        //iterating through the linked list at position in table until the movie we want is found
        while((curr_node!=NULL) && (!movie_found)) 
        {
            if (curr_node->title==title) //if movie found
            {
                movie_found=true; //set movie_found bool to true
            }
            else
            {
                curr_node=curr_node->next; //update curr to iterate through linked List
            }
        }
    }

    if (!(movie_found)) //if the movie is not found, return nullptr
    {
        return nullptr;
    }
    return curr_node; //else return the node in which the movie was found
}


//
/** getCollisions() -  Returns the number of collisions that have occurred during insertion
 *  into the hash table
 * 
 * @param none 
 * @return int (number of collisions)
 **/ 
int MovieHashTable::getCollisions() {
    // TODO
    return n_collisions; //returns the number of collisions 
}


// 
/** setCollisions() -  Increments the number of collisions that have occurred during insertion
 *  into the hash table
 * @param none 
 * @return void 
 **/ 
void MovieHashTable::setCollisions() {
    // TODO
    n_collisions = n_collisions+1; //updates the number of collisions
}
