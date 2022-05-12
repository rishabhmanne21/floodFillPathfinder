/* 
Name: Rishabh Manne  
Date: 4/9/22
Description: Implementing dynamic array allocation by creating dynamic arrays and performing algorithms on them
Usage: manne_rishabh_hw4
*/  

#include <string.h> 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include<queue>
using namespace std; 


void PrintEnvironment(int**, int, int, bool, ofstream&);
bool isAvailable(int**, int, int, int, int, int, int);                      // forward-declaring all the functions that will be used for implementation
void PrintChar(char**, int, int, char, ofstream&);
bool isValid(int**, char**, int, int, int, int, int, int);


int main(int argc, char** argv)
{
    int w;                                                              // variables to store the user-inputted width and height
    int h;

    ofstream outputFile;                                                // creating an output file to print the environment and char arrays below
    outputFile.open("wave.txt");                                        // opening the file which is titled wave.txt                                  

    cout << "Welcome to CSE 240 Wavefront Pather\n\n";                  // printing a welcome message for user

    cout<< "Please tell me about the grid you want to generate\n";              // asking about grid generation details

    cout << "Enter the width (at least 10): ";
    cin >> w;

    cout << "Enter the height (at least 10): ";                                 // receiving the width and height in the form of cin(user input)
    cin >> h;
    cout << endl;



    int** mainArray = new int*[w];                                      // creating an array that will simulate a flood fill algorithm


    for(int i = 0; i < w; i++)
    {
        mainArray[i] = new int[h];                                  // initializing each column to have its own row in the array
         
    }

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            mainArray[i][j] = 0;                                    // initializing each element in the array to a default 0 value
        }
    }


    double p;
    char* a = new char[1];                                                  // creating more variables to include user input regarding percent of impassible elements


    cout << "Enter the percent of impassible terrain (0 - 100): ";
    cin >> p;                                                               // storing the percent of impassible in the variable
    cout << endl;



    while(p > 35)
    {
        cout << "Having a value greater than 35% might create poor results, do you wish to continue? (y/n): ";          // if p > 35, a warning method is printed
        cin >> a;

        if(strcmp(a, "n") == 0) 
        {
            
            cout << "Enter the percent of impassible terrain (0 - 100): ";                          // if they say no, then the re-prompting would keep occurring
            cin >> p;
            cout << endl;
        }
        
        else
        {
            
            break;                  // if they accept, then the loop is exited
        }
    }

  double numImpassibles = ((p) * (w*h))/(100);              // using algebra to determine the number of elements in array that should be impassible
  int counter = 0;                                          // a counter to keep track of numImpassibles


  time_t q;
  srand(time_t(&q));                        // will ensure a random assortment of impassible locations 

   
   while(counter < numImpassibles)
    {
        int index1 = rand() % w;
        int index2 = rand() % h;                            // a random x and y index that is within the bounds of the array

            
            if(mainArray[index1][index2] != -1)
            {
                mainArray[index1][index2] = -1;             // if the value isn't already -1, then it is assigned -1
                counter++;                                  // counter is incremented
            }

    }
    
    bool toFile = false;                                   // a bool to check if we need to output to outputFile 

    PrintEnvironment(mainArray, w, h, toFile, outputFile);                  // print the array via the helper method which is described at the end section of the code


    int x;
    int y;                                              // variables to hold the goal position of user


    cout << "Please enter the Goal Position X: ";
    cin >> x;
                                                                
    cout << "Please enter the Goal Position Y: ";                   // taking the user input here
    cin >> y;


    while(mainArray[x][y] == -1 || x > w || y > h)                      // if the selected position is an obstacle, we keep asking them to re-enter until valid
    {

    cout << "Sorry, that position is inside an obstacle" << endl << endl;
    cout << "Please enter the Goal Position X: ";
    cin >> x;

    cout << "Please enter the Goal Position Y: ";
    cin >> y;

    }

    int copyX = x;                      // a copy of x and y used for debugging purposes
    int copyY = y;

   
    

    int neighbor[8][2]= { {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}, {0,1}, {0,-1} };           // a neighbor array used to traverse the Moore neighborhood of a center value

    int initialX = x;                                       // initial x and y variables to ensure the first value is never re-examined
    int initialY = y;
    

    queue<int> holder;                                  // a queue used for BFS
    holder.push(x);                                     // pushing the first x and y values in the queue to examine its Moore neighborhood values
    holder.push(y);


    while(!holder.empty())                              // until there are no coordinates to examine, we continue examining Moore neighborhood values of center points
    {

        x = holder.front();                         // storing the first value of the queue to the x var
        holder.pop();                               // popping it to access the next y value
        y = holder.front();                         // storing the next top value of queue into the y value
        holder.pop();                               // popping it so that the next coordinates are available, should there be another iteration



        for(int i = 0; i < 8; i++)                  // looping through the neighbor array
        {
            if(isAvailable(mainArray, w, h, x + neighbor[i][0], y + neighbor[i][1],initialX,initialY))     // checking the validity(bounds, obstacle, checked?)
            {
                if(mainArray[x + neighbor[i][0]][y + neighbor[i][1]] <= mainArray[x][y])                // if the "neighbor" is less than or equal to center, we
                {                                                                                       // push those coordinates into stack and increment neighbor by center + 1
                    holder.push(x + neighbor[i][0]);
                    holder.push(y + neighbor[i][1]);

                    mainArray[x + neighbor[i][0]][y + neighbor[i][1]] += (mainArray[x][y] + 1);
                }
                

                else
                {
                    holder.push(x + neighbor[i][0]);                                                // otherwise we still push into queue because this coordinate may have unexamined 
                    holder.push(y + neighbor[i][1]);                                                // neighbors as well
                }
            }

           
        }

        
    }

    

    PrintEnvironment(mainArray, w, h, toFile, outputFile);                                                             // output the environment array along with two newlines 
    cout << endl << endl;
    

    char** charArray = new char*[w];                                                            // creating a character array that will simulate path from start to goal position
    char example = '#';                                                                         // the barrier will be signified by the '#' character


    for(int i = 0; i < w; i++)
    {
        charArray[i] = new char[h];                                                         // initializing a row for every column in char array
         
    }

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            if(mainArray[i][j] == -1)
            {
                charArray[i][j] = example;                                                  // going through and assigning the barrier character to values of -1 in array with the help of main array
            }
            
        }
    }

    int sPosX;                                                                              // creating variables to hold starting coordinates
    int sPosY;

    cout << "Please enter the Start Position X: ";
    cin >> sPosX;                                                                           // through user input, we extract the user-desired starting point

    cout << "Please enter the Start Position Y: ";
    cin >> sPosY;


    while(charArray[sPosX][sPosY] == example || sPosX > w || sPosY > h)                     // if the coordinates are in an obstacle or out of bounds then we continue to re-prompt user
    {

    cout << "Sorry, that position is inside an obstacle" << endl << endl;
    cout << "Please enter the Start Position X: ";
    cin >> sPosX;

    cout << "Please enter the Start Position Y: ";
    cin >> sPosY;

    }

    

    charArray[copyX][copyY] = '$';                                      // marking the goal position with a '$'
    charArray[sPosX][sPosY] = '@';                                      // marking starting position with a '@'
    bool accomplished = false;                                          // boolean to determine if target was reached
    int highestMin = charArray[sPosX][sPosY];
    int localMin = 0;                                                   // minimum variables to help in the minimum algorithm to determine which neighbor has the lowest value

    int firstX = sPosX;                                                 // initial coordinate to ensure that they aren't examined again
    int firstY = sPosY;

    int minX;
    int minY;                                                           // hold the index values for the minimum value found after iterating through all neighbors of a value


    queue<int>nextMin;
    nextMin.push(sPosX);                                                // creating a queue to start a BFS-style search and we push the first starting coordinates in it
    nextMin.push(sPosY);


    while(!nextMin.empty())                                             // loop throughout queue until it is empty
    {

        sPosX = nextMin.front();
        nextMin.pop();                                                  // same process as for environment array as the beginning of the queue is examined and then popped
        sPosY = nextMin.front();                                        // to access the next set of coordinates, should they be present
        nextMin.pop();

        for(int i = 0; i < 8; i++)
        {
            if(isValid(mainArray, charArray, w, h, sPosX + neighbor[i][0], sPosY + neighbor[i][1],firstX,firstY))       // checking the validity of neighbors
            {
                if(mainArray[sPosX + neighbor[i][0]][sPosY + neighbor[i][1]] < mainArray[sPosX][sPosY])                 // if neighbor is less than center it is the "temporary" minimum
                {
                    localMin = mainArray[sPosX + neighbor[i][0]][sPosY + neighbor[i][1]];                               // temporary minimum

                    if(localMin < highestMin)                                                                       // if temporary minimum is less than all-time minimum then all-time is assigned the temp minimum
                    {
                        highestMin = localMin;
                        minX = sPosX + neighbor[i][0];                                                      // then the smallest x and y values are assigned to this neighbor value
                        minY = sPosY + neighbor[i][1];
                    }
                }
                
               
            }
        }

        if(mainArray[minX][minY] == mainArray[copyX][copyY])
        {
            accomplished = true;                                        // if target value is found, bool is set to true and the loop is exited
            break;
        }

        else
        {
            charArray[minX][minY] = '*';
            sPosX = minX;                                             // otherwise the path is marked with a '*' and the new coordinates into the queue are updated
            sPosY = minY;
        }

        nextMin.push(sPosX);
        nextMin.push(sPosY);                                        // the new coordinates are pushed into the queue and process repeats

    }

    toFile = true;                                                  // now since we need to output environment array to file as well, the bool is updated accordingly

    if(accomplished == false)
    {
        PrintEnvironment(mainArray, w, h, toFile, outputFile);                                // after two newlines the original environment array is also fed into the outputFile
        outputFile << "\n\n";
        outputFile << "No Path Found" << endl;
        PrintChar(charArray, w, h, example, outputFile);             // at the end, if target isn't reached, then message is printed and outputted to the output file named wave.txt

        
    }

    else
    {
         PrintEnvironment(mainArray, w, h, toFile, outputFile);
         outputFile << "\n\n";
         PrintChar(charArray, w, h, example, outputFile);             // if it's successful then the charArray and environmentArray are both fed into the outputFile
         
        
    }

    cout << endl << "Environment array and character array successfully written into output file wave.txt." << endl;    // success message is printed

    outputFile.close();                     // the outputFile is closed as we have completed writing into it

    

    


    






    return 0;                               // main function has an int return type
}


void PrintEnvironment(int** array, int width, int height, bool toFile, ofstream& oFile)
{
    for(int r = 0; r < width; r++)                                  // helper method that prints the environment array and formats it using the <iomanip> library
    {
        for(int c = 0; c < height; c++)
        {
            if(toFile == true)                                      // if we have to write into file then we follow this syntax otherwise we use cout
            {
                oFile << setw(6) << array[r][c];
            }

            else
            {
                cout << setw(6) << array[r][c];
            }

        }

        if(toFile == true)
        {
            oFile << "\n";
        }

        else
        {  
            cout << endl;
        }
    }

}

void PrintChar(char** array, int width, int height, char barrier, ofstream& file)
{
    for(int r = 0; r < width; r++)
    {
        for(int c = 0; c < height; c++)
        {
            if(array[r][c] == barrier)                                                  // helper method that prints the charArray, to the outputFile which is fed in as a parameter here
            {
                file << setw(4) << array[r][c];
            }
            else if(array[r][c] == '*' || array[r][c] == '@' || array[r][c] == '$')      // only prints the starting, ending, and barrier symbols
            {
                file << setw(4) << array[r][c];
            }

            else
            {
                file << setw(4) << "";                                                      // the remaining values are left blank so that the path can be shown
            }
        }

        file << endl;
    }

}

bool isAvailable(int** array, int width, int height, int newX, int newY, int firstX, int firstY)
{

    if(newX >= width || newX == -1 || newY >= height || newY == -1)
    {
        return false;
    }

    else if(array[newX][newY] == -1)                                            // checking to see if there is a -1, bounds error, or if the neighbor is already examined
    {                                                                           // then false is returned
        return false;
    }

    else if(array[newX][newY] > 0 || (newX == firstX && newY == firstY))
    {
        return false;
    }
    

    return true;                                                                    // if all checks are made then the neighbor is good to go for examination
}


bool isValid(int** firstArray, char** array, int width, int height, int newX, int newY, int firstX, int firstY)
{

    if(newX >= width || newX == -1 || newY >= height || newY == -1)
    {
        return false;
    }

    else if(firstArray[newX][newY] == -1)                                                     // checking to see if there is a -1, bounds error, or if the neighbor is already examined
    {                                                                                         // then false is returned(mainArray examined because it has -1 and not the barrier)
        return false;
    }

    else if((newX == firstX && newY == firstY))
    {
        return false;
    }
    

    return true;                                                                                // otherwise true is returned
}









