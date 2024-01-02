#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;


/**  DirectorSkipList.cpp:
 * 
 * DirectorSkipList.cpp implements the DirectorSkipList class and its functions.
 * Inserts, Searches, and prints the DirectorSkipList after all nodes have been inserted
 * **/

/** DirectorSkipList() - Constructor for DirectorSkipList with default capacity and levels
 * @param none
 * @return void
 **/

DirectorSkipList::DirectorSkipList() {
    // TODO
    levels= DEFAULT_LEVELS; //update levels of SL to default
    capacity=DEFAULT_CAPACITY;  //update capacity of SL to default
    size = 0; //update size of SL to 0
    head = new DirectorSLNode(" ",levels); //head should be a Node in the SL intialized to null
}


/** DirectorSkipList(int _cap, int _levels) - 
 * - Constructor for DirectorSkipList with given capacity and levels
 * @param _cap
 * @param _levels
 * @return void
 **/

DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    // TODO
    levels= _levels; //update levels to of SL to parameter which is _levels
    capacity= _cap; //update capacity of SL to parameter  which is _cap
    size = 0; //update size of SL to 0
    head = new DirectorSLNode(" ",levels); //head should be a Node in the SL intialized to null
 }



/** Destructor -  deletes all nodes in the skip list 
 * (but not the MovieNodes, which are shared with other data structures)
 * @param none
 * @return void
 **/
DirectorSkipList::~DirectorSkipList() 
{
    // TODO
    if (head->next[0] == nullptr)  // if there is nothing in the SL, return 
        return;

    // all the nodes are stored in the 0th level of the SL, so delete from the 0th level
    //iterate through the nodes on the 0th level until null is reached
    while(head->next[0]!=NULL) 
    {
        DirectorSLNode* cn_node= head;   //store the head of the SL on 0th level in temporary node
        head=head->next[0]; //update the head to its next value
        delete cn_node; // delete the original head, stored in the cn_node
        
    }
    delete head; //delete the head memory for no memory leaks
    head=nullptr; //update the head of SL to null, so the memory is deallocated 
}

/** Insert -  Inserts a movie node into the skip list with the specified director
 * @param director
 * @param _movie 
 * @return void
 **/

void DirectorSkipList::insert(string director, MovieNode* _movie) 
{
    // TODO
    //create new node with the parameter director information 
     DirectorSLNode* new_node= new DirectorSLNode(director,levels);
     DirectorSLNode* check_node= new DirectorSLNode(director,levels);
     delete check_node;

     // search for the director, and 
     //if the node with said director already exists in the SL,
     // just add the movie to the node's movie vector 

    if (search(director)!=nullptr) //node already exists in SL
    {
        search(director)->addMovie(_movie); //adding movie to node's movie vector
        //delete new node, so that we deallocate memory
         // because it is not needed if director node is already found
        delete new_node;
        return; //exist the insert function
    }
    
    //else the director does not exist, add the director to SL at the correct position 
    //and add movie to new_node's movie vector

    new_node->addMovie(_movie);

     //current node will iterate through the SL 
    //and get the correct position in which we would insert the new_node
    DirectorSLNode* current= head;
    
    //prev pointer of type DirectorSLNode to keep track of the previous node, whose next would be new_node
    DirectorSLNode* prev[levels];
   

    //iterating through SL to set the curr node to correct value
    for (int i = levels-1; i >= 0; i--)
    {
         prev[i]=nullptr; //so no memory leaks are created;
        if (current!=nullptr)
        { 
            //if current->next on ith level is not NULL and the director is < new_node->director,
            // we can update current to its next value
            while ((current->next[i] != nullptr) &&  (current->next[i]->director < new_node->director) )
            { /* Example: want to insert C
                0th:  A   B  D   E 
                
                current would be A, and current->next would be B
                current would move to B, because B < C */

                current = current->next[i]; //update current's value to its next value
            }
            prev[i]=current; //set the previous of level i to the current value 
        } 
    }
    
    //adding the node to the SL at the correct postion at n_levels based on coin flips
        int n_levels=0;
        bool coin_flip=true;

        //while there are levels left to add the new_node to and coin_flipped to true, we can add new_node
        while(n_levels<levels && coin_flip==true) 
        {
           //updating new_node next to the prev->next at the current n_levels
            new_node->next[n_levels]=prev[n_levels]->next[n_levels]; 
            prev[n_levels]->next[n_levels]=new_node; //updating prev->next to new_node

            /*EXAMPLE:
            new_node=C
            prev=B

            C->next=D (D is prev's ->next)
            B->next=C */


            n_levels++; //updating number of levels so the while loop stops and we add the new_node to n_levels only
            if(rand()%2==1) //coin flip simulator based on random probability
            {
                coin_flip=false;
            }
            
        }

    }

/** Search -  Searches for a node in the skip list with the specified director
 * @param director
 * @return DirectorSLNode
 **/ 
DirectorSLNode *DirectorSkipList::search(string director) {
    // TODO
    DirectorSLNode* curr_SL_node = head; //use a SL_node to iterate through SL, set to head of SL 
    
    //traversing SL in the for loop from top level to bottom level, and setting previous 
    for (int i = levels-1; i >= 0;--i) 
    {
        //if current->next on ith level is not NULL and the director is < new_node->director,
        // we can update current to its next value
        while(curr_SL_node->next[i]!=NULL && curr_SL_node->next[i]->director < director)
        {
            curr_SL_node=curr_SL_node->next[i]; //updating curr to next value
        }
    }
    curr_SL_node=curr_SL_node->next[0]; //Set curr to curr;s next node at level 0

     //if the director is found and curr_node is not NULL, return the movieNode where the director was found
    if (curr_SL_node!=NULL && curr_SL_node->director==director)
    {
        return curr_SL_node; 
    }

    //else the director was not found, return nullptr
    else 
    {
        return  nullptr;
    }
}


/** PrettyPrint -  Pretty-prints the skip list
 * @param none
 * @return void
 **/ 
void DirectorSkipList::prettyPrint() 
{
    // TODO

    //iterate through all the levels from 0 to levels
    for (int i=0;i<=levels;i++)
    {
        DirectorSLNode* node = head->next[i]; //have a curr node ptr to the head next (as head =null)
        cout << "Level " << i << ": ";
        while (node != NULL) //iterate through the current node's next values until not null and print them out
        {
            cout << node->director<<" ";
            node = node->next[i]; //updating current node to next, to stop while loop
        }
        cout <<endl;
    } 
}


