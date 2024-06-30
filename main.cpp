#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>

using namespace std;

int Xpivot = 0;
int Ypivot = 0;

// Function to validate that an integer is odd
bool is_odd(int num) {
    return num % 2 != 0;
}

// Function to create a 2D vector with "#" as wall, arrow as door, "." as empty rooms.
vector<vector<string>> create_grid(int width, int height) { // Ok.
    vector<vector<string>> grid(height, vector<string>(width, "#"));
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if ((is_odd(i))&&(is_odd(j))) {
                grid[i][j]= ".";
            } else if ((j!=0)&&(j!=width-1)&&(is_odd(i))&&(!is_odd(j))) {
                grid[i][j]= ">";
            } else if ((i!=0)&&(i!=height-1)&&(j==width-2)&&(is_odd(j))) {
                grid[i][j]= "V";
            } else {
                grid[i][j]= "#";
            }
        }
    }
    return grid;
}

// Function to display the grid
void display_grid(const vector<vector<string>>& grid) { // Ok.
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            cout << cell << "";
        }
        cout << endl;
    }
}

// Function to save the grid to a file with explicit iteration
void save_grid_to_file(const vector<vector<string>>& grid, const string& filename) { // Ok.
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            outfile << grid[i][j] << "";
        }
        outfile << endl;
    }
    cout << "Grid saved to " << filename << endl;
}

void makeRoomAndPathEmpty(vector<vector<string>>& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if ((grid[i][j]==".")||(grid[i][j]=="<")||(grid[i][j]==">")||(grid[i][j]=="v")||(grid[i][j]=="V")||(grid[i][j]=="^")) {
                grid[i][j] = " ";
            }
        }
    }
}

void makeWallThin(vector<vector<string>>& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if ((grid[i][j]=="#")&&(!is_odd(i))&&(!is_odd(j))) {      // # for security.
                grid[i][j] = "+";
            }
            if ((grid[i][j]=="#")&&(is_odd(i))&&(!is_odd(j))) {      // # for security.
                grid[i][j] = "|";
            }
            if ((grid[i][j]=="#")&&(!is_odd(i))&&(is_odd(j))) {      // # for security.
                grid[i][j] = "-";
            }
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 4);
    int width = 41;
    int height = 19;
    //  cout << "Enter an odd and big width: ";
    //  cin >> width;
    while ((!is_odd(width))||(width<7)) {
        cout << "Width must be odd and big. Enter an odd and big width: ";
        cin >> width;
    }
    //  cout << "Enter an odd and big height: ";
    //  cin >> height;
    while (!is_odd(height)||(height<7)) {
        cout << "Height must be odd and big. Enter an odd and big height: ";
        cin >> height;
    }
    vector<vector<string>> grid = create_grid(width, height);
    // Special point, final destination, pivot, "origin":
    Xpivot = width-2;
    Ypivot = height-2;
    int    newXpivot = Xpivot;
    int newYpivot = Ypivot;
    int stepMax = width*height*20;
    for (long unsigned int step = 0; step<stepMax; step++) { // stepMax not included.
        cout << "Grid created at step " << step << ":" << endl;
        grid[Ypivot][Xpivot]= "O";
        //   display_grid(grid);
        //  cout << "" << endl;
// Move pivot:
        int movePivot = dis(gen);
        clog << "movePivot = " << movePivot << endl;
        switch (movePivot) { // char or int only.
        case 1:
            if (Xpivot == width-2) {
                break;
            }
            grid[Ypivot][Xpivot]= ".";
            Xpivot = Xpivot+2;
            grid[Ypivot][Xpivot-1]= ">";
            Ypivot = Ypivot;
            break;
        case 2:
            if (Ypivot == height-2) {
                break;
            }
            grid[Ypivot][Xpivot]= ".";
            Xpivot = Xpivot;
            Ypivot = Ypivot+2;
            grid[Ypivot-1][Xpivot]= "V";
            break;
        case 3:
            if (Xpivot == 1) {
                break;
            }
            grid[Ypivot][Xpivot]= ".";
            Xpivot = Xpivot-2;
            grid[Ypivot][Xpivot+1]= "<";
            Ypivot = Ypivot;
            break;
        case 4:
            if (Ypivot == 1) {
                break;
            }
            grid[Ypivot][Xpivot]= ".";
            Xpivot = Xpivot;
            Ypivot = Ypivot-2;
            grid[Ypivot+1][Xpivot]= "^";
            break;
        default:
            cerr << "Err movePivot!" << endl;
            break;
        }
// Add wall for bad arrow:
// No problem of exit!
        if (grid[Ypivot][Xpivot-1]== "<") {
            grid[Ypivot][Xpivot-1] = "#";
        }
        if (grid[Ypivot][Xpivot+1]== ">") {
            grid[Ypivot][Xpivot+1] = "#";
        }
        if (grid[Ypivot-1][Xpivot]== "^") {
            grid[Ypivot-1][Xpivot] = "#";
        }
        if (grid[Ypivot+1][Xpivot]== "V") {
            grid[Ypivot+1][Xpivot] = "#";
        }
    }
    cout << "Final grid:" << endl;
    display_grid(grid);
    string filename1 = "grid1.txt";
// save_grid_to_file(grid, filename1);
    makeRoomAndPathEmpty(grid);
    cout << "" << endl;
    display_grid(grid);
    makeWallThin(grid);
    cout << "" << endl;
    display_grid(grid);
    return 0;
}
