#ifndef _PATHFINDER_H
#define _PATHFINDER_H

/*
    pathfinder.h
        
    class definition for the pathfinder class.      

    author: <Chandler VonFeldt>
*/

#include <string>
#include <vector>
#include <cmath>

#include "picture.h"

using namespace std;

/******************************************************************************
   class pathfinder

   Given access to a source of topographical data (a file specifying width and
   height and listing elevation data for the resulting grid), a pathfinder
   object can:
     - read in the data
     - create an image showing the data as a grayscale image, with gray level 
       representing relative elevation
     - compute and overlay the optimal path from all western starting points
     - compute and overlay the single best optimal path from west to east
     - save the image data

******************************************************************************/

class pathfinder {
public:
    // constructor - optional, in case you need to initialize anything
    pathfinder();

    // our only public method: do everything and return the best path cost
    int run(string data_file_name, string save_file_name, bool use_recursion);

private:
    // TODO: put your internal data, methods, etc. here; the ones shown 
    //  below are suggested, but you can modify/add as desired (more methods
    //  are probably a good idea!)

    int _height, _width;                     // height and width of map
    int valOne, valTwo, valThree;           // values to compare costs at certain spots
    int northEast, east, southEast;         // values for calculations of next potential spots
    int rowSet = 0;                         // value to keep track of current row start (left most starting point)
    int smallestCostIndex;                  // smallest index of cost vector
    int varyingIndex;
    int checkVal;                           // value to check for yellow or blue path
    bool _use_recursion;                    // use recursion or not
    vector<vector<int>> _elevations;         // raw data from file
    Picture _image;                         // image to save at the end
    vector<int> _elevationCost;             // optimal cost of every starting point
    vector<vector<int>> _elevationsCostDy;   // dynamic cost tracker of elevations
    vector<vector<char>> _elevationsDirectionDy;   // dynamic cost tracker of elevations
    int dyFunction(int row, int column, int checkBlue, int checkYellow);  //dynamic function
    int rcFunction(int row, int column, int checkFinal);  // recursive function
    bool _read_data(string data_file_name);  // read in raw data
    void _draw_map();                        // draw the grayscale elevation map
    int _draw_paths();                       // find optimal paths, returning the most optimal cost
    void _save_image(string save_file_name); // save the image as a .png file
};

#endif
