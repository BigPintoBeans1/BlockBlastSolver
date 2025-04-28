#include "blockBlast.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;



template <typename streamtype>
void open_or_die(streamtype &stream, string file_path) {
    stream.open(file_path);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + file_path);
    }
}
int main(int argc, char *argv[]) {

    ifstream boardInput;
    ifstream shapeInput_one;
    ifstream shapeInput_two;
    ifstream shapeInput_three;
    open_or_die(boardInput, "demoBoard1.txt");
    open_or_die(shapeInput_one, "demoPiece1.txt");
    open_or_die(shapeInput_two, "demoPiece2.txt");
    open_or_die(shapeInput_three, "demoPiece3.txt");


    BlockBlast game;
    game.readBoard(boardInput);
    game.shapeOne = game.readShape(shapeInput_one, game.shapeOne);
    game.shapeTwo = game.readShape(shapeInput_two, game.shapeTwo);
    game.shapeThree = game.readShape(shapeInput_three, game.shapeThree);
    game.printShape(game.shapeOne);
    game.printShape(game.shapeTwo);
    game.printShape(game.shapeThree);

    game.solve();

    boardInput.close();
    shapeInput_one.close();
    shapeInput_two.close();
    shapeInput_two.close();

    return 0;
}