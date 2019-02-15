#ifndef MYMAZEGENERATOR_HPP
#define MYMAZEGENERATOR_HPP
#include "MazeGenerator.hpp"
#include <vector>
#include <iostream>
#include <random>
#include "Direction.hpp"
#include "Maze.hpp"


class MyMazeGenerator: public MazeGenerator

{
public:

    MyMazeGenerator();
    virtual void generateMaze(Maze& maze);
    void print_visited_graph();
    void dig_tunnels(int x, int y, Maze& maze);
    bool stop_digging_or_not();
    bool target_cell_available( int row, int col);
    std::tuple<int, int> generate_next_cell (int current_row, int current_col, Direction d);
    bool is_there_way_out(int current_row, int current_col);
    std::vector<Direction> get_possible_direction(int current_x, int current_y);
    int decorate_direction(Direction d);
private:   
    std::vector<std::vector<bool>> visited;
    std::random_device device;
    std::default_random_engine engine{device()};
    std::uniform_int_distribution<int> distribution{0,3};                   

};

#endif
