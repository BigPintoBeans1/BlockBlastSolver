#ifndef _BLOCK_BLAST
#define _BLOCK_BLAST

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

const int BOARD_SIZE = 8;
const int SHAPE_SIZE = 5;

class BlockBlast {
public:

    struct shape {
        // for the shape, the largest shape is a 1x5 or 5x1 so need to make a
        // 5x5 grid and fill in appropriate coordinates for the shape
        vector<vector<int>> shapeGrid;
        int validCount;
        int id;

        // Define the < operator for shape
        bool operator<(const shape& other) const {
            return id < other.id;
        }
    };
    
    shape shapeOne;
    shape shapeTwo;
    shape shapeThree;

    BlockBlast();

    void readBoard(istream &input);

    void printBoard(vector<vector<int>> &boardCopy);

    void solve();

    bool solveHelper(vector<shape> shapeArray, int index, vector<vector<int>> &boardCopy_one, vector<pair<int, int>> &placements);

    void printShape(shape curr);

    shape readShape(istream &input, shape curr);

    void insertShape(shape curr, int x, int y, vector<vector<int>> &board);

    void fullRowOrColumn(vector<vector<int>> &board);

    void removeShape(shape curr, int x, int y, vector<vector<int>> &board);

    void whereShapeFit(shape curr, queue<pair<int, int>> *queue, vector<vector<int>> &boardCopy);

    ~BlockBlast();

    // i is x coordinate, j is y coordinate
    vector<vector<int>> board;

    queue<pair<int, int>> *whereShapeOneFit;
    queue<pair<int, int>> *whereShapeTwoFit;
    queue<pair<int, int>> *whereShapeThreeFit;
    
private:
};

#endif