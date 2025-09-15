#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "pathfinder.h"

using namespace std;

// constructor - optional, in case you need to initialize anything
pathfinder::pathfinder() { ; }

// public run() method - invokes private methods to do everything;
// returns the total elevation change cost of the best optimal path.
int pathfinder::run(string data_file_name, string save_file_name, bool use_recursion) {
    // suggested implementation - modify as desired!
    _use_recursion = use_recursion;
    if (!_read_data(data_file_name)) {
        cout << "Error reading map data from \"" << data_file_name << "\"!" << endl;
        return -1;
    }

    _draw_map();

    int best_cost = _draw_paths();

    _save_image(save_file_name);

    return best_cost;
}

/*******************/
/* PRIVATE METHODS */
/*******************/

// _read_data - read and store width, height, and elevation data from the
// provided data file; return false if something goes wrong
bool pathfinder::_read_data(string data_file_name) {
    string nextLine;
    string nextString; //initializing variables
    int nextNum;
    int colNum;
    int rowNum;
    int colCounter = 0;
    int rowCounter = 0;
    ifstream fin(data_file_name); //taking in file name

    if ( fin.fail() ) {
        cerr << "Error opening input file"; // error message for if it doesn't work
        exit(1);
    }

    for (int i = 0; i < 2; i++) { // for loop to read in num of cols, then num of rows
        getline(fin, nextLine);
        istringstream sin(nextLine); //declaring istream variable

        while(sin >> nextString) { // parsing through words in line
            if (stringstream(nextString) >> nextNum) { // if "word" is a number
                if (i == 0) { // reading in row number from column line
                    _width = nextNum;
                }
                else { // reading in row number from row line
                    _height = nextNum;
                }

            }
        }
    }

    _elevations = vector<vector<int>>(_height, vector<int>(_width)); // initializing 2-d vector

    while (!fin.eof()) { // while file still contains more words to be read in

        getline(fin, nextLine); // reading in next line
        istringstream sin(nextLine);

        while (sin >> nextNum) {
            _elevations[rowCounter][colCounter] = nextNum; //populating each spot in vector
            colCounter += 1; // increase col num for every num read
        }
        rowCounter += 1; // increase row num for every line read
        colCounter = 0; // reset col number

    }
    fin.close(); // close file

    return true;
}

// _draw_map - draw the elevation data as grayscale values on our Picture
// object.
void pathfinder::_draw_map() {
    int smallestVal = _elevations[0][0]; // assigning first value in vector to smallest and biggest values
    int biggestVal = _elevations[0][0];
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < (_width - 1); j++) {
            if (biggestVal <= (_elevations[i][j + 1])) { // if the next value is larger than the last, its new biggest
                biggestVal = (_elevations[i][j + 1]);
            }
        }
        for (int i = 0; i < _height; i++) { // same, but for smallest
            for (int j = 0; j < (_width - 1); j++) {
                if (smallestVal >= (_elevations[i][j + 1])) {
                    smallestVal = (_elevations[i][j + 1]);
                }
            }
        }
    }

    double differenceVal = biggestVal - smallestVal; //finding difference between two
    
    for (int row = 0; row < _height; row++) { //nested for loop to produce image
        for (int column = 0; column < _width; column++) {
            int r, g, b; //initializing rgb variables
            double gray = ((_elevations[row][column] - smallestVal)*(255/differenceVal)); //math to scale to 0-255
            r = g = b = gray; //setting all values to gray
            _image.set(row, column, r, g, b);
        }
    }
}

// _draw_paths() - find and draw optimal paths from west to east using either recursion or dynamic programming
// return the best optimal path cost
int pathfinder::_draw_paths() {

    if (_use_recursion == true) {
        _elevationCost = vector<int>(_height); // vector to keep track of vector cost
        smallestCostIndex = 0;
        rcFunction(0,0, 0); //call recursive function
        for (int i = 0; i < (_elevationCost.size() - 1); i++) { // finding smallest cost (or index of it)
            if (_elevationCost[smallestCostIndex] >= _elevationCost[i + 1]) {
                smallestCostIndex = i + 1;
            }
        }
        rcFunction(smallestCostIndex, 0, 1); // calling most optimal path to draw in yellow
    }
    else {
        dyFunction(0, _width - 1, 0, 0);
        smallestCostIndex = 0;
        for (int i = 0; i < (_elevationsCostDy.size() - 1); i++) { // finding smallest cost (or index of it)
            if (_elevationsCostDy[smallestCostIndex][0] > _elevationsCostDy[i + 1][0]) {
                smallestCostIndex = i + 1;
            }
        }
        dyFunction(0, _width - 1, 1, 0); //drawing blue paths
        dyFunction(smallestCostIndex, _width - 1, 1, 1); //drawing optimal path

    }

    return 0;
}

// _save_image - we'll do this one for you :)
void pathfinder::_save_image(string save_file_name) {
    _image.save(save_file_name);
}

int pathfinder::rcFunction(int row, int column, int checkFinal) {
    int r, g, b;
    int checkVal = 0;

    if (checkFinal == 0) { //if its not most optimal path
        r = 0;
        g = 0;
        b = 255;
    }
    else { // if it is most optimal
        r = 255;
        g = 255;
        b = 0;
        checkVal = 1;
    }
    _image.set(row, column, r, g, b); //highlight current spot

    if ((column == _width - 1) && (rowSet == (_height - 1))) { // if last spot for last row, return
        return 0;
    }
    if (column == (_width - 1)) { // if last spot for given row, go to next row
        rowSet += 1;
        rcFunction(rowSet, 0, 0);
    } else {
        if (row == 0) { // if on top row, only evaluate east and south east
            valOne = abs(_elevations[row][column] - _elevations[row][column + 1]); // east
            valTwo = abs(_elevations[row][column] - _elevations[row + 1][column + 1]); //southeast

            if (valOne <= valTwo) {  // choose east
                _elevationCost[rowSet] = _elevationCost[rowSet] + valOne; //updating cost
                _image.set(row, column + 1, r, g, b); // highlighting next spot
                rcFunction(row, column + 1, checkVal); // call function for next spot
            } else { // choose sotuh east
                _elevationCost[rowSet] = _elevationCost[rowSet] + valTwo;
                _image.set(row + 1, column + 1, r, g, b);
                rcFunction(row + 1, column + 1, checkVal);
            }
        } else if (row == _height - 1) { // same as above, but for bottom row (east and north east moves only)
            valOne = abs(_elevations[row][column] - _elevations[row][column + 1]);
            valTwo = abs(_elevations[row][column] - _elevations[row - 1][column + 1]);

            if (valOne <= valTwo) {
                _elevationCost[rowSet] = _elevationCost[rowSet] + valOne;
                _image.set(row, column + 1, r, g, b);
                rcFunction(row, column + 1, checkVal);
            } else {
                _elevationCost[rowSet] = _elevationCost[rowSet] + valTwo;
                _image.set(row - 1, column + 1, r, g, b);
                rcFunction(row - 1, column + 1, checkVal);
            }
        } else { // same as above, but middle, so now east, southeast, and northeast are all valid
            valOne = abs(_elevations[row][column] - _elevations[row][column + 1]);
            valTwo = abs(_elevations[row][column] - _elevations[row - 1][column + 1]);
            valThree = abs(_elevations[row][column] - _elevations[row + 1][column + 1]);

            if ((valOne < valTwo) && (valOne < valThree)) {
                _elevationCost[rowSet] = _elevationCost[rowSet] + valOne;
                _image.set(row, column + 1, r, g, b);
                rcFunction(row, column + 1, checkVal);
            } else if ((valTwo < valOne) && (valTwo < valThree)) {
                _elevationCost[rowSet] = _elevationCost[rowSet] + valTwo;
                _image.set(row - 1, column + 1, r, g, b);
                rcFunction(row - 1, column + 1, checkVal);
            } else if ((valThree < valOne) && (valThree < valTwo)) {
                _elevationCost[rowSet] = _elevationCost[rowSet] + valThree;
                _image.set(row + 1, column + 1, r, g, b);
                rcFunction(row + 1, column + 1, checkVal);
            }
        }
    }
}

int pathfinder::dyFunction(int row, int column, int checkBlue, int checkYellow) {
    if (checkBlue == 0) {
        _elevationsDirectionDy = vector<vector<char>>(_height, vector<char>(_width)); // initializing tracker vectors

        _elevationsCostDy = vector<vector<int>>(_height, vector<int>(_width));
        for (int i = 0; i < _height; i++) {
            _elevationsCostDy[i][_width - 1] = 0;
        }

        for (int k = (_width - 2); k >= 0; k--) {
            for (int j = 0; j < _height; j++) {
                if (j == 0) { // if on top row, only evaluate east and south east
                    east = abs(_elevations[j][k] - _elevations[j][k + 1]) + _elevationsCostDy[j][k + 1]; // east
                    southEast = abs(_elevations[j][k] - _elevations[j + 1][k + 1]) +
                                _elevationsCostDy[j + 1][k + 1]; //southeast

                    if (east <= southEast) {  // choose east
                        _elevationsCostDy[j][k] = east;  //updating cost
                        _elevationsDirectionDy[j][k] = 'e'; // updating direction
                    } else { // choose south east
                        _elevationsCostDy[j][k] = southEast;
                        _elevationsDirectionDy[j][k] = 's';
                    }
                } else if (j == _height - 1) { // same as above, but for bottom row (east and north east moves only)
                    east = abs(_elevations[j][k] - _elevations[j][k + 1]) + _elevationsCostDy[j][k + 1];
                    northEast = abs(_elevations[j][k] - _elevations[j - 1][k + 1]) + _elevationsCostDy[j - 1][k + 1];

                    if (east <= northEast) {  // choose east
                        _elevationsCostDy[j][k] = east; //updating cost
                        _elevationsDirectionDy[j][k] = 'e'; // updating direction
                    } else { // choose north east
                        _elevationsCostDy[j][k] = northEast;
                        _elevationsDirectionDy[j][k] = 'n';
                    }
                } else { // same as above, but middle, so now east, southeast, and northeast are all valid
                    east = abs(_elevations[j][k] - _elevations[j][k + 1]) + _elevationsCostDy[j][k + 1];
                    northEast = abs(_elevations[j][k] - _elevations[j - 1][k + 1]) + _elevationsCostDy[j - 1][k + 1];
                    southEast = abs(_elevations[j][k] - _elevations[j + 1][k + 1]) + _elevationsCostDy[j + 1][k + 1];

                    if ((southEast < east) && (southEast < northEast)) {
                        _elevationsCostDy[j][k] = southEast;
                        _elevationsDirectionDy[j][k] = 's';
                    } else if ((northEast <= east) && (northEast <= southEast)) {
                        _elevationsCostDy[j][k] = northEast;
                        _elevationsDirectionDy[j][k] = 'n';
                    } else if ((east <= northEast) && (east <= southEast)) {
                        _elevationsCostDy[j][k] = east; //updating cost
                        _elevationsDirectionDy[j][k] = 'e'; // updating direction
                    }
                }
            }
        }
    } else {
        if (checkYellow == 0) {

            for (int i = 0; i < _height; i++) {
                _image.set(i, 0, 0, 0, 255);
                varyingIndex = i;
                for (int j = 0; j < (_width - 1); j++) {
                    if (_elevationsDirectionDy[varyingIndex][j] == 'n') { // if next move is north east, change varying index to go up
                        varyingIndex -= 1;
                        _image.set(varyingIndex, j + 1, 0, 0, 255); // color pixel with blue in next spot
                    } else if (_elevationsDirectionDy[varyingIndex][j] == 'e') { //if next move is east, don't change varying index
                        _image.set(varyingIndex, j + 1, 0, 0, 255);
                    } else if (_elevationsDirectionDy[varyingIndex][j] == 's') { //if next move is south east, change varying index to go down
                        varyingIndex += 1;
                        _image.set(varyingIndex, j + 1, 0, 0, 255);
                    }
                }
            }
        } else {
            _image.set(smallestCostIndex, 0, 255, 255, 0);
            varyingIndex = smallestCostIndex; // varying index now just equals the index of smallest cost (optimal path start)
            for (int j = 0; j < (_width - 1); j++) { //same for loop as blue path drawer, except now it draws yellow with optimal path
                if (_elevationsDirectionDy[varyingIndex][j] == 'n') {
                    varyingIndex -= 1;
                    _image.set(varyingIndex, j + 1, 255, 255, 0);
                } else if (_elevationsDirectionDy[varyingIndex][j] == 'e') {
                    _image.set(varyingIndex, j + 1, 255, 255, 0);
                } else if (_elevationsDirectionDy[varyingIndex][j] == 's') {
                    varyingIndex += 1;
                    _image.set(varyingIndex, j + 1, 255, 255, 0);
                }

            }
        }
    }
    return 0;
}