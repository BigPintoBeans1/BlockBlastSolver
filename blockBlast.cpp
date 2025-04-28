#include "blockBlast.h"

BlockBlast::BlockBlast() {
    board = vector<vector<int>> (BOARD_SIZE);
    // initialize board to all 0s
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i].push_back(0);
        }
    }
    // initialize shapes valid Counts to 0
    shapeOne.validCount = 0;
    shapeTwo.validCount = 0;
    shapeThree.validCount = 0;

    shapeOne.id = 1;
    shapeTwo.id = 2;
    shapeThree.id = 3;

    whereShapeOneFit = new queue<pair<int, int>>();
    whereShapeTwoFit = new queue<pair<int, int>>();
    whereShapeThreeFit = new queue<pair<int, int>>();
}

BlockBlast::~BlockBlast() {
    delete whereShapeOneFit;
    delete whereShapeTwoFit;
    delete whereShapeThreeFit;
}

void BlockBlast::readBoard(istream &input) {
    // read in board left to right and top to bottom must be a 1 or a 0
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int tempSquare;
            //cout << "Enter 0 or 1 for row " << i+1 << ", columnn " << j+1 << endl;
            input >> tempSquare;
            if (tempSquare != 0 && tempSquare != 1) {
                cout << "Invalid input. Please enter 0 or 1." << endl;
                j--;
            } else {
                board[i][j] = tempSquare; 
            }
        }
    }
}

void BlockBlast::printBoard(vector<vector<int>> &boardCopy) {
    // prints 1 for taken square and space for empty square
    cout << endl;
    cout << "Board" << endl;
    cout << "_________________________________" << endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << "| ";
            if (boardCopy[i][j] == 1) {
                cout << boardCopy[i][j] << " ";
            } else {
                cout << "  ";
            }
        }
        cout << "|" << endl;
    }
    cout << "_________________________________" << endl;
}

BlockBlast::shape BlockBlast::readShape(istream &input, shape curr) {
    curr.shapeGrid = vector<vector<int>> (SHAPE_SIZE);
    // initialize shape to all 0s
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            curr.shapeGrid[i].push_back(0);
        }
    }
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            int tempSquare;
            //cout << "Enter 0 or 1 for row " << i+1 << ", columnn " << j+1 << endl;
            input >> tempSquare;
            if (tempSquare != 0 && tempSquare != 1) {
                cout << "Invalid input. Please enter 0 or 1." << endl;
                j--;
            } else {
                curr.shapeGrid[i][j] = tempSquare;
                if (tempSquare == 1) {
                    curr.validCount ++;
                }
            }
        }
    }
    return curr;
}

void BlockBlast::printShape(shape curr) {
    cout << endl;
    cout << "Shape " << curr.id << endl;
    cout << "____________________" << endl;
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            cout << "| ";
            if (curr.shapeGrid[i][j] == 1) {
                cout << curr.shapeGrid[i][j] << " ";
            } else {
                cout << "  ";
            }
        }
        cout << "|" << endl;
    }
    cout << "____________________" << endl;
}

void BlockBlast::solve() {
    vector<BlockBlast::shape> shapeArray = {shapeOne, shapeTwo, shapeThree};
    vector<pair<int, int>> placements; // To store the sequence of placements
    bool foundSolution = false;

    // Generate all permutations of the shapes
    do {
        placements.clear(); // Clear placements for the new permutation
        vector<vector<int>> boardCopy = board; // Copy of the board for manipulation

        if (solveHelper(shapeArray, 0, boardCopy, placements)) {
            foundSolution = true;

            // Output the first solution for this permutation
            printBoard(board);
            cout << endl << "Solution found, order: ";
            for (size_t i = 0; i < placements.size(); ++i) {
                cout << "Shape " << shapeArray[i].id << " ";
            }
            cout << endl;
            vector<vector<int>> boardTemp = boardCopy;
            for (size_t i = 0; i < placements.size(); ++i) {
                cout << endl << "Shape " << shapeArray[i].id << ": (" << placements[i].first << ", " << placements[i].second << ")\n";
                insertShape(shapeArray[i], placements[i].first, placements[i].second, boardTemp);
                printBoard(boardTemp);
                fullRowOrColumn(boardTemp);
            }
            printBoard(boardTemp);
            cout << "Done!" << endl;
                
            
            // Continue searching for the next permutation
        }
    } while (next_permutation(shapeArray.begin(), shapeArray.end()));

    if (not foundSolution) {
        cout << "No solution found.\n";
    } else {
        
    }
}
        


bool BlockBlast::solveHelper(vector<shape> shapeArray, int index, vector<vector<int>> &boardCopy_one, vector<pair<int, int>> &placements) {
    // Base case: all shapes have been placed
    if (index == 3) {
        return true; // Successfully placed all shapes
    }

    // Temporary queue to store potential positions for the current shape
    queue<pair<int, int>> *tempQueue = new queue<pair<int, int>>();
    whereShapeFit(shapeArray[index], tempQueue, boardCopy_one);

    // Process each potential position for the current shape
    while (!tempQueue->empty()) {
        pair<int, int> tempPair = tempQueue->front();
        tempQueue->pop();

        // Create a copy of the board to test this placement
        vector<vector<int>> boardCopy = boardCopy_one;

        // Place the current shape on the board
        insertShape(shapeArray[index], tempPair.first, tempPair.second, boardCopy);

        // Clear full rows and columns
        fullRowOrColumn(boardCopy);

        // Add the current placement to the sequence
        placements.push_back(tempPair);

        // Recursively solve for the next shape
        if (solveHelper(shapeArray, index + 1, boardCopy, placements)) {
            return true; // Found a valid solution
        }

        // Backtrack: remove this placement from the sequence
        placements.pop_back();
    }
    delete tempQueue;
    return false; // No valid configuration found for this shape order
}

void BlockBlast::whereShapeFit(shape curr, queue<pair<int, int>> *queue, vector<vector<int>> &boardCopy) {
    for (int y = 0; y < (BOARD_SIZE + SHAPE_SIZE - 1); y++) {
        for (int x = 0; x < (BOARD_SIZE + SHAPE_SIZE - 1); x++) {
            int count = 0;
            // at each square on this upscaled board, check if every 1 on shape
            // matches to the boards 0
            for (int i = 0; i < SHAPE_SIZE; i++) {
                for (int j = 0; j < SHAPE_SIZE; j++) {
                    // if we are out of bounds of the board, don't access
                    if (y - i < 0 || x - j < 0 || y - i >= BOARD_SIZE || x - j >= BOARD_SIZE) {
                        continue;
                    }
                    // if the shape is 1 and board is 0, increment count
                    if (curr.shapeGrid[SHAPE_SIZE - 1 - i][SHAPE_SIZE - 1 - j] == 1 && (boardCopy[y - i][x - j] == 0)) {
                        count ++;
                    }
                }
            }
            
            // if the shape fit on the board add the current (x, y) to the queue
            if (count == curr.validCount) {
                pair<int, int> temp;
                temp.first = x;
                temp.second = y;
                queue->push(temp);
            }
        }
    }

}

void BlockBlast::insertShape(shape curr, int x, int y, vector<vector<int>> &boardCopy) {
    // inserts the shape's bottom right square onto the board at (x, y)
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            // if we are out of bounds of the board, don't insert
            if (y - i < 0 || x - j < 0) {
                continue;
            }
            if (curr.shapeGrid[SHAPE_SIZE - 1 - i][SHAPE_SIZE - 1 - j] == 1) {
                boardCopy[y - i][x - j] = 1;
            }
        }
    }
}

void BlockBlast::removeShape(shape curr, int x, int y, vector<vector<int>> &boardCopy) {
    // inserts the shape's bottom right square onto the board at (x, y)
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            // if we are out of bounds of the board, don't insert
            if (y - i < 0 || x - j < 0) {
                continue;
            }
            // on the valid square of the shape, make that board square a 0
            if (curr.shapeGrid[SHAPE_SIZE - 1 - i][SHAPE_SIZE - 1 - j] == 1) {
                boardCopy[y - i][x - j] = 0;
            }
        }
    }
}

void BlockBlast::fullRowOrColumn(vector<vector<int>> &boardCopy) {
    vector<bool> fullRows(BOARD_SIZE, true);
    vector<bool> fullCols(BOARD_SIZE, true);

    // Step 1: Identify full rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (boardCopy[i][j] == 0) {
                fullRows[i] = false;
                fullCols[j] = false;
            }
        }
    }

    // Step 2: Clear full rows
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (fullRows[i]) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                boardCopy[i][j] = 0;
            }
        }
    }

    // Step 3: Clear full columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (fullCols[i]) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                boardCopy[j][i] = 0;
            }
        }
    }
}