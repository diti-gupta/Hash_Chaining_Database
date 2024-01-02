#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;


     
// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();

/** Main -  runs the menu, until user input!=5, and gives information on the movies based on movie name or director name
 * 
 * @param none
 * @return runs program
 **/
int main(int argc, char* argv[]) {

    //if the user does NOT put in at least 4 arguments in the cmd line, give error message
    if (argc < 4)
    {
      cout << "Invalid Number of arguments" << endl;
      cout<<"Usage ./<program name> <csv file> <hashTable size> <skipList size>"<<endl;
      return 0;
    }
    
    //user cmd line had correct number of arguments with filename and capacity for HT and DSL
    else
    {
        int HT_arg= stoi(argv[3]);//convert 3rd command line arg to string

        //create a new HashTable with cmd line arg
        MovieHashTable* HTable= new MovieHashTable( stoi(argv[3])); 
        DirectorSkipList* DSL= new DirectorSkipList(2000,10);


    // TODO
        string fileName = argv[1]; //get the filename from the commandline
    
        readMovieCSV(fileName,  *HTable, *DSL); //read the file, and update HTable and DSL accordingly
        

        //print number of collisons
        
        cout<<"Number of collisions: "<<HTable->getCollisions()<<endl;
        int user_input=0;

        //print the menu, until user_input !=5
        while(user_input!=5)
        {
            display_menu();
            // if the user's input is not an integer, clear the input and ask for a new input
            while (cin.fail()) 
            {   
                cin.clear();
                cin.ignore();
                cin>>user_input;
            }
            
            cin>>user_input; //get user input

            //Find the director of the movie,
            // call the HashTable functions to search for movie and print its director
            if (user_input==1)
            {
                string movie_name;
                cout<<"Enter movie name: ";
                cin.ignore(); //ignore all whitespace in the user input for movie_name
                getline(cin,movie_name); //get the user input for movie name and store in movie_name variable
    

                //search for movie in Hashtable and if not found (null), print movie not found,
                // else print the director name
                if (HTable->search(movie_name)==nullptr)
                {
                    cout<<"Movie not found"<<endl;
                }
                else //movie found, print its director
                {
                    cout<<"The director of "<< movie_name<< " is " << HTable->search(movie_name)->director<<endl;
                }
                
            }


            //Find the number of movies by a director,
            // call the SL functions to search for director 
            //and print the number of elements in director's vector of movies
            else if( user_input==2)
            {
                string director_name;
                cout<<"Enter director name: ";
                cin.ignore(); //ignore all the white spaces in user's input for director name
                getline(cin,director_name); //get director name from user
                
                //search for director,
                // if not found (null), print No movies found for director,
                //else director is found and print the number of movies for said director
                if (DSL->search(director_name)==nullptr)
                {
                    cout<<"No Movies for director "<< director_name<<endl;
                }

                else  //director found, print number of movies
                {
                    cout<<director_name <<" directed " << DSL->search(director_name)->movies.size()<< " movies"<<endl;
                }
            }

            //Find the description of the movie
            //call the HashTable search functions to find movie
            //and print the description of said movie
            else if (user_input==3)
            {
                string movie_name;
                cout<<"Enter movie name: ";
                cin.ignore(); //ignore all whitespace in the user input for movie_name
                getline(cin,movie_name); //get user input for movie name

                  //search for movie in Hashtable and if not found (null), print movie not found,
                // else print the movie description with actors, year, genre etc.
                if (HTable->search(movie_name)==nullptr)
                {
                    cout<<"Movie not found"<<endl;
                }


                else //movie found, print description of said movie
                {
                    cout<<"Summary: " << movie_name << " is " << "a " 
                    << HTable->search(movie_name)->year << 
                    " ("<< HTable->search(movie_name)->genre << ") film featuring "
                    << "\""<< HTable->search(movie_name)->actors <<"\" "<<endl; 

                    cout<<"Plot: \"" <<HTable->search(movie_name)->description<<"\" "<<endl;
                }
            }

            //List the movies of a director,
            // call the SL functions to search for director 
            //and print the movies names in director's vector of movies
            else if (user_input==4) 
            {
                string director_name;
                cout<<"Enter director name: ";
                cin.ignore(); //ignore all the whitespace for director name 
                getline(cin,director_name); //get user input for director name

                 //search for director,
                // if not found (null), print No movies found for director,
                //else director is found and print the movies for said director

                //if director not found, print no movies found for director
                if (DSL->search(director_name)==nullptr) 
                {
                    cout<<"No Movies for director "<< director_name<<endl;
                }
                
                else //if director found, print the movies he/she has directed
                {
                    cout<<director_name<<" directed the following movie(s):  "<<endl;
                    for(int i =0; i<DSL->search(director_name)->movies.size();i++)
                    {
                        cout<< "   " << i << ". " << DSL->search(director_name)->movies[i]->title<<endl;
                    }
                }
            }

            //quit the program 
            //delete the HTable and DSL
            else if (user_input ==5)
            {
                 //delete would call destructors, 
                //and delete would free up memory(for no mem leaks)
                delete HTable;
                delete DSL;
            }
        }
    }
}

// parseMovieLine function
/** parseMovieLine -  Function to parse a CSV line into a MovieNode object
 * @param line
 * @return MovieNode
 **/
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}


// readMovieCSV function
/** readMovieCSV -  Function to read a CSV file into a vector of MovieNode objects
 * @param fileName
 * @param movieTable (address)
 * @param directorList (address)
 * @return void
 **/
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    // TODO
     string line=""; //will store the information in this line
   

      // open the file
      ifstream myfile;
      myfile.open(filename); //open file
      getline(myfile,line,'\n'); //ignoring the first line in the file (Rank,Title,Genre...)
      while (getline(myfile, line,'\n')) //get the line from the file
        { 
            //parse through each line with the parseMovieLine function 
            //and store the returned node in node movie
           MovieNode* movie = parseMovieLine(line); 
        
        //if the movie is not found in the DSL and the Hash Table, insert the movie into the DSL and HashTable
           if (movie!=nullptr)
           {
                movieTable.insert(movie->title,movie);
                directorList.insert(movie->director, movie);
           }
           
        }
        myfile.close(); //close the file
}


// display_menu() function
/** display_menu() -  Function to display the menu options
 * @param none
 * @return void
 **/

void display_menu() {
    // TODO

        //print out the menu (will call this function in main until user_input!=5)
        cout<<"Please select an option:"<<endl;
        cout<<"1. Find the director of a movie"<<endl;
        cout <<"2. Find the number of movies by a director"<<endl;
        cout <<"3. Find the description of a movie"<<endl;
        cout<<"4. List the movies by a director"<<endl;
        cout<<"5. Quit"<<endl;

        cout<<endl;
        cout<<endl;
        cout<<"Enter an Option: "<<endl;
       
}