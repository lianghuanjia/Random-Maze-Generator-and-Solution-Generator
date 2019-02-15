#include <ics46/factory/DynamicFactory.hpp>
#include "MyMaze.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, MyMazeGenerator, "Sam's Maze");

/*
This is the generator of the class
*/
MyMazeGenerator::MyMazeGenerator()
{

}

/*
This is the generateMaze function. It takes a Maze object and then
generate a maze. This is like the main function that uses all 
necessary functions to generate a random maze
*/
void MyMazeGenerator::generateMaze(Maze& maze)
{
    maze.addAllWalls();


    unsigned int col = maze.getWidth();
    unsigned int row = maze.getHeight();

    visited.resize(row);
    for (int i  = 0; i < row; ++i)
        {
            visited[i].resize(col);
            for (int n = 0; n < col; ++n)
                {
                visited[i][n] = false; 
                }
        }
    dig_tunnels(0,0,maze);
}   

/*
This is a helper function. Because the Direction object cannot be
cout, so I use integer to represent them, and according to the
direction it takes to return the corresponding integer, to do 
some test work.
*/
int MyMazeGenerator::decorate_direction(Direction d)
{
if (d == Direction::up)
    {
    return 1;
    }
else if (d == Direction::down)
    {
    return 2;
    }
else if (d == Direction::left)
    {
    return 3;
    }
else if (d == Direction::right)
    {
    return 4;
    }
}

/*
This is a helper function I use for testing too. It prints out the
vector<vector> visited grpah in the console so I can do some
test
*/
void MyMazeGenerator::print_visited_graph()
{

    std::cout << std::endl;
    for (int i = 0; i < visited.size(); ++i)
        {
            for (int n = 0; n < visited[i].size(); ++n)
                {
                    std::cout << visited[i][n];
                }
            std::cout << std::endl;
        }
    std::cout << std::endl;

}



/*
This is the most important function in the Maze generator.
It takes a x and y cordinator, and the Maze object. At first 
it mark this (x,y) cordinate as true in the visited graph, becuase 
this function means the (x,y) cordinate has visited. After that,
it checks if there are possible direction in this cordinate(x,y).
If it doesn't, then it means this is a dead end, we don't need to dig
anymore. 
If it has possible direction, then I use the member function
get_possible_direction(x,y) to get the possible direction I can
dig. 
This member function returns a vector that contains object Direction.if the vector.size() is greater than 0, then it means there are 
possible direction to dig. 
Then I use std::shuffle() to re-order the order of Directions in
my vector, so it makes the choice of direction random.
After shuffling the order, I use for loop to go over each direction.
In each direction, I remove the wall between the current cell and the
target direction cell.After this, I call dig_tunnels with the argument that is the next direction cell to form a recursion.

*/
void MyMazeGenerator::dig_tunnels(int x, int y, Maze& maze)
{

    visited[x][y] = true;
    bool has_way_out = is_there_way_out(x,y);
    if (has_way_out == true)
        {   
        std::vector<Direction> di_vec = get_possible_direction(x,y);
        if (di_vec.size() > 0)//it means there is direction to go
            {
            std::shuffle(di_vec.begin(),di_vec.end(),engine);
            for (int i = 0; i < di_vec.size(); ++i)
                {
                if (di_vec[i] == Direction::up && visited[x-1][y] == false)
                    {
                    maze.removeWall(y,x, di_vec[i]);
                    std::tuple<int, int> next_cell = generate_next_cell(x,y,di_vec[i]);
                    dig_tunnels(std::get<0>(next_cell),std::get<1>(next_cell),maze);
                    }
                else if (di_vec[i] == Direction::down && visited[x+1][y] == false)
                    {
                    maze.removeWall(y,x, di_vec[i]);
                    std::tuple<int, int> next_cell = generate_next_cell(x,y,di_vec[i]);
                    dig_tunnels(std::get<0>(next_cell),std::get<1>(next_cell),maze);
                    }
                else if (di_vec[i] == Direction::left && visited[x][y-1] == false)
                    {
                    maze.removeWall(y,x, di_vec[i]);
                    std::tuple<int, int> next_cell = generate_next_cell(x,y,di_vec[i]);
                    dig_tunnels(std::get<0>(next_cell),std::get<1>(next_cell),maze);
                    }
                else if (di_vec[i] == Direction::right && visited[x][y+1] == false)
                    {
                    maze.removeWall(y,x, di_vec[i]);
                    std::tuple<int, int> next_cell = generate_next_cell(x,y,di_vec[i]);
                    dig_tunnels(std::get<0>(next_cell),std::get<1>(next_cell),maze);
                    }
                }
            }
        }
}

/*
Get_Possible_direction:
this function takes x and y cordinator to represent a cell. According to the cell, the function returns a vector that contains all the possible direction inside.
*/

std::vector<Direction> MyMazeGenerator::get_possible_direction(int current_x, int current_y)
{
std::vector<Direction> v;

//UP
    {
    std::tuple<int, int> next_cell = generate_next_cell(current_x, current_y,Direction::up);
    if (target_cell_available(std::get<0>(next_cell),std::get<1>(next_cell)))
        v.push_back(Direction::up);
    }
//DOWN
    {
    std::tuple<int, int> next_cell = generate_next_cell(current_x,current_y,Direction::down);
    if (target_cell_available(std::get<0>(next_cell),std::get<1>(next_cell)))
        v.push_back(Direction::down);
    }

//LEFT
    {
    std::tuple<int, int> next_cell = generate_next_cell(current_x,current_y,Direction::left);
    if (target_cell_available(std::get<0>(next_cell),std::get<1>(next_cell)))
        v.push_back(Direction::left);
    }

//RIGHT
    {
    std::tuple<int, int> next_cell = generate_next_cell(current_x,current_y,Direction::right);
    if (target_cell_available(std::get<0>(next_cell),std::get<1>(next_cell)))
        v.push_back(Direction::right);
    }

return v;

}

/*
This function is a helper function that detects should the program
keep digging or not. i.e. There are still cells that have not visitedIf there are cells that are not visited, then return false.
If all cells have been visited, then it return true, meaning 
stop digging.
*/
bool MyMazeGenerator::stop_digging_or_not()
{

    for (int i = 0; i < visited.size(); ++i)
        {
            for (int n = 0; n < visited[i].size(); ++n)
                {  
                if (visited[i][n] == false)
                    return false;
                }
        }
    return true;

}

/*
This function is a helper function. It takes x & y cordinate,
and return true if this cell is able to dig,
return false if this cell is not able to dig(e.g. out of range
or has visited)
*/
bool MyMazeGenerator::target_cell_available( int row, int col)
{

    if ( 0 <= row && row < visited.size())
        {
        if (0 <= col && col < visited[0].size())
            {
                if (visited[row][col] == false)
                    {
                    return true;
                    }
                else
                    {
                    std::cout << "target cell has visited before." << std::endl;
                    return false;
                    }
            }
        else
            {
            std::cout << "column out of boundary" << std::endl;
            return false;
            }
        }
    else
        {
        std::cout << "row out of boundary" << std::endl;
        return false;
        }

}


/*
This funciton is a helper funciton
It takes x & y cordinates, and a direction. 
It generate the next cordinate according to the current x and y
cordinates and the direction.
Then it store the cordinate in a tuple, and returns it.
*/
std::tuple<int, int> MyMazeGenerator::generate_next_cell(int current_row,  int current_col, Direction d)
{
  
    if (d == Direction::up)
        {
            return std::make_tuple(current_row-1,current_col);
        }   
    
    else if (d == Direction::down)
        {
            return std::make_tuple(current_row+1,current_col);
        }

    else if (d == Direction::right)
        {
            return std::make_tuple(current_row, current_col+1);
        }

    else if (d == Direction::left)
        {
            return std::make_tuple(current_row, current_col-1);
        }


}

/*
This is a helper function. It takes a x & y cordinates, to check
if the current cordinate is at the dead end. 
It returns true if there is STILL way to dig,
returns false if there is no way to dig, which is a dead end.
*/
bool MyMazeGenerator::is_there_way_out(int current_row, int current_col)
{

    std::vector<bool> check_direction = {true,true,true,true};//up,down,left,right
    if (target_cell_available(current_row-1,current_col) == false)//if up is available
        {
        std::cout << "Up not" << std::endl;
        check_direction[0] = false;
        }
    if (target_cell_available(current_row+1,current_col) == false)
        {
        std::cout << "Down not" << std::endl;
        check_direction[1] = false;
        }
    if (target_cell_available(current_row, current_col-1)==false)
        {
        std::cout << "Left not" << std::endl;
        check_direction[2] = false;
        }
    if (target_cell_available(current_row, current_col+1) == false)
        {
        std::cout << "Right not" << std::endl;
        check_direction[3] = false;
        }

    for (int i = 0; i < check_direction.size(); ++i)
        {
        if (check_direction[i] == true)
            return true;
        }
    return false;

}




