#include "solver.h"

#include <iostream>

int main()
{
    std::vector<std::vector<unsigned>> data(2, std::vector<unsigned> (2) );
    data[0][0] = 0;
    data[0][1] = 1;
    data[1][0] = 3;
    data[1][1] = 2;
    Board board(0);
    Solver solver(board);
    std::cout << board.size() << std::endl << board.is_solvable() << std::endl;
    std::cout << ((board == *solver.begin()) ? "true" : "false") << std::endl;
    /*
    for (const auto move : solver) {
        std::cout << move << std::endl;
    }
    */
}
