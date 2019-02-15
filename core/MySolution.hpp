#ifndef MYSOLVER_HPP
#define MYSOLVER_HPP
#include "MazeSolver.hpp"
#include "Direction.hpp"
#include <vector>
#include "MazeSolution.hpp"
#include "Maze.hpp"
#include <iostream>

class MySolver:public MazeSolver

{
public:
    MySolver();
    virtual void solveMaze(const Maze& maze, MazeSolution& mazeSolution);
    std::vector<Direction> get_available_direction(int x, int y,const Maze& maze, MazeSolution& mazeSolution);
    void change_cell_bool_value(Direction d,int x, int y);
    void find_solution(int x, int y, const Maze& maze, MazeSolution& mazeSolution);
    void print_direction(Direction d);
private:
    
    std::vector<std::vector<bool>> visited;

};
#endif
