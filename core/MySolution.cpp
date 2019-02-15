#include <ics46/factory/DynamicFactory.hpp>
#include "MySolution.hpp"
//#include "MyMaze.hpp"
ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, MySolver  , "Sam's solution")

MySolver::MySolver()
{
}
/*
This is the solveMaze funciton that impletement the whole solution 
*/
void MySolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
    mazeSolution.restart();
    std::pair<int, int> start_point = mazeSolution.getStartingCell();

    int width = mazeSolution.getWidth();
    int height = mazeSolution.getHeight();

    visited.resize(width);
        for (int i= 0; i < width; i++)
            {
            visited[i].resize(height);
            }
        for (int n= 0; n < width; n++)
            for (int x = 0; x < height; x++ )
                {   
                visited[n][x] = false;
                }

    
    find_solution(start_point.first,start_point.second,maze, mazeSolution);

}


/*
The get_available_direction() function takes a x and y cordinate, maze object, and mazeSolution object
and then it will return all possible directions to go based on the x and y cordinates.
*/
std::vector<Direction> MySolver::get_available_direction(int x, int y,const Maze& maze, MazeSolution& mazeSolution)
{
    std::vector<Direction> avai;

    if (maze.wallExists(x,y,Direction::up)== false)
        {
        if (visited[x][y-1] == false)
            {
            avai.push_back(Direction::up);
            }
        }
    if (maze.wallExists(x,y,Direction::down) == false) 
        {
        if (visited[x][y+1] == false)
            {
            avai.push_back(Direction::down);
            }
        }
    if (maze.wallExists(x,y, Direction::left) == false )
        {
        if(visited[x-1][y]==false)
            {
            avai.push_back(Direction::left);
            }
        }

    if (maze.wallExists(x,y, Direction::right) == false)
        {
        if (visited[x+1][y] == false)
            {
            avai.push_back(Direction::right);
            }
        }
    return avai;
}

/*
This find_solution() function takes x and y cordinates, Maze object and MazeSolution,
then keep going to different direction recursively to reach the end of the maze. It will keep going
if the mazeSolution is not complete or it doesn't reach the starting point.
*/
void MySolver::find_solution(int x, int y,const Maze& maze, MazeSolution& mazeSolution)
{
    std::pair<int,int> cell;
    visited[x][y] = true;
    std::vector<Direction> path = get_available_direction(x,y,maze,mazeSolution);
    if (!mazeSolution.isComplete())
        {
        if (path.size() > 0)
        {
            for (int i = 0; i < path.size(); i++)
            {
                Direction d = path[i];
                if (!mazeSolution.isComplete())
                    {
                    try
                        {
                        mazeSolution.extend(d);
                        }
                    catch(...)
                        {
                        return;
                        }
                    }
                cell = mazeSolution.getCurrentCell();
                change_cell_bool_value(d, x, y);
                find_solution(cell.first,cell.second,maze,mazeSolution);
            }
        }

        else if (path.size() == 0)
        {
            mazeSolution.backUp();
            cell = mazeSolution.getCurrentCell();
            if(cell!=mazeSolution.getStartingCell())
                {
                find_solution(cell.first,cell.second,maze,mazeSolution);
                }
            else if (cell == mazeSolution.getStartingCell())
                {
                return;
                }
        }
        }
}

/*
This function change the cell's bool value according to the given direction.
E.G. If the direction is up, then the up cell's bool value will be changed to true,
to tell that it's visited.
*/
void MySolver::change_cell_bool_value(Direction d, int x, int y)
{
    if (d == Direction::left && visited[x-1][y] == false)
        visited[x-1][y] = true;
    else if (d == Direction::right && visited[x+1][y] == false)
        visited[x+1][y] = true;
    else if (d == Direction::up && visited[x][y-1] == false)
        visited[x][y-1] = true;
    else if (d == Direction::down && visited[x][y+1] == false)
        visited[x][y+1] = true;
}




/*
This is a helper function that takes the direciton and print out the direction in a char type.
It's good for me to test to see the direction.
*/
void MySolver::print_direction(Direction d)
    {
    if (d == Direction::up)
        std::cout << "UP" << std::endl;
    else if (d == Direction::down)
        std::cout << "DOWN" << std::endl;
    else if (d == Direction::left)
        std::cout << "LEFT" << std::endl;
    else if (d == Direction::right)
        std::cout << "RIGHT" << std::endl;
    }
    




















