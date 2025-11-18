/*
    CS 2411 - Machine Learning Assignment
    Topic: Reinforcement Learning (Q-Learning)
    Project: Maze Solver AI in C++
    Student: Casius Alamilla

    Description:
    This program generates a random, solvable maze and trains a Q-learning
    reinforcement learning agent to navigate from a start cell (A) to a goal
    cell (G). It uses file input/output and produces:
      - maze.txt           : internal maze grid
      - training_stats.txt : training summary + best action sequence
      - final_path.txt     : ASCII maze with learned optimal path
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

// Ask a yes/no question. Returns true for y/Y, false otherwise.
bool askYesNo(const string& question) {
    cout << question << " (y/n): ";
    char ch;
    cin >> ch;
    return (ch == 'y' || ch == 'Y');
}

// Maze representation.
struct Maze {
    vector<string> grid;
    int rowCount = 0;
    int colCount = 0;
    int startRow = 0;
    int startCol = 0;
    int goalRow  = 0;
    int goalCol  = 0;
};

// Convert an action index to a word.
string actionToWord(int action) {
    switch (action) {
        case 0: return "Up";
        case 1: return "Right";
        case 2: return "Down";
        case 3: return "Left";
        default: return "?";
    }
}

// Load maze from a file: characters '#', '.', 'A', 'G'.
Maze loadMaze(const string& filename) {
    Maze maze;
    ifstream in(filename);
    string line;

    if (!in) {
        cerr << "Error: could not open maze file: " << filename << endl;
        exit(1);
    }

    while (getline(in, line)) {
        if (!line.empty()) {
            maze.grid.push_back(line);
        }
    }

    maze.rowCount = (int)maze.grid.size();
    maze.colCount = (maze.rowCount > 0) ? (int)maze.grid[0].size() : 0;

    bool foundStart = false, foundGoal = false;

    for (int r = 0; r < maze.rowCount; ++r) {
        for (int c = 0; c < maze.colCount; ++c) {
            char ch = maze.grid[r][c];
            if (ch == 'A') {
                maze.startRow = r;
                maze.startCol = c;
                foundStart = true;
            } else if (ch == 'G') {
                maze.goalRow = r;
                maze.goalCol = c;
                foundGoal = true;
            }
        }
    }

    if (!foundStart || !foundGoal) {
        cerr << "Error: maze must contain A (start) and G (goal)." << endl;
        exit(1);
    }

    return maze;
}

// Check if a given cell is inside bounds and not a wall.
bool isOpenCell(int row, int col, const Maze& maze) {
    if (row < 0 || row >= maze.rowCount || col < 0 || col >= maze.colCount) return false;
    if (maze.grid[row][col] == '#') return false;
    return true;
}

// Map (row, col) → state index for Q-table.
int getStateIndex(int row, int col, const Maze& maze) {
    return row * maze.colCount + col;
}

// Result of taking an action.
struct MoveResult {
    int newRow;
    int newCol;
    double reward;
    bool reachedGoal;
};

// Apply one move in the maze.
MoveResult applyAction(int row, int col, int action, const Maze& maze) {
    int nextRow = row;
    int nextCol = col;

    if (action == 0)      nextRow--;
    else if (action == 1) nextCol++;
    else if (action == 2) nextRow++;
    else if (action == 3) nextCol--;

    if (!isOpenCell(nextRow, nextCol, maze)) {
        // Hit a wall: stay put, negative reward.
        return { row, col, -5.0, false };
    }

    if (nextRow == maze.goalRow && nextCol == maze.goalCol) {
        // Goal reached.
        return { nextRow, nextCol, 50.0, true };
    }

    // Normal move.
    return { nextRow, nextCol, -1.0, false };
}

// ε-greedy policy: with probability ε choose random, else choose max Q.
int chooseAction(int stateIndex,
                 const vector<vector<double>>& qTable,
                 double exploreRate,
                 mt19937& rng) {
    uniform_real_distribution<double> dist01(0.0, 1.0);
    double roll = dist01(rng);

    if (roll < exploreRate) {
        uniform_int_distribution<int> actionDist(0, 3);
        return actionDist(rng);
    }

    const auto& qValues = qTable[stateIndex];
    return (int)(max_element(qValues.begin(), qValues.end()) - qValues.begin());
}

// BFS: check if maze has any path from A to G (used during generation).
bool mazeHasPath(const Maze& maze) {
    vector<vector<bool>> visited(maze.rowCount, vector<bool>(maze.colCount, false));
    queue<pair<int,int>> q;

    q.push({maze.startRow, maze.startCol});
    visited[maze.startRow][maze.startCol] = true;

    const int dRow[4] = {-1, 0, 1, 0};
    const int dCol[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        pair<int,int> cell = q.front();
        q.pop();

        int r = cell.first;
        int c = cell.second;

        if (r == maze.goalRow && c == maze.goalCol) return true;

        for (int k = 0; k < 4; ++k) {
            int nr = r + dRow[k];
            int nc = c + dCol[k];
            if (isOpenCell(nr, nc, maze) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
    return false;
}

// Randomly generate a solvable maze and write to maze.txt.
void generateRandomMazeToFile(int rows, int cols, double wallChance, const string& filename) {
    if (rows < 3 || cols < 3) {
        cerr << "Maze size must be at least 3x3.\n";
        exit(1);
    }

    mt19937 rng((unsigned)time(nullptr));
    uniform_int_distribution<int> interiorRow(1, rows - 2);
    uniform_int_distribution<int> interiorCol(1, cols - 2);
    uniform_real_distribution<double> dist01(0.0, 1.0);

    Maze maze;
    maze.rowCount = rows;
    maze.colCount = cols;

    bool createdSolvableMaze = false;

    for (int attempt = 0; attempt < 1000 && !createdSolvableMaze; ++attempt) {
        maze.grid.assign(rows, string(cols, '.'));

        // Border walls.
        for (int r = 0; r < rows; ++r) {
            maze.grid[r][0] = '#';
            maze.grid[r][cols - 1] = '#';
        }
        for (int c = 0; c < cols; ++c) {
            maze.grid[0][c] = '#';
            maze.grid[rows - 1][c] = '#';
        }

        // Random start & goal.
        maze.startRow = interiorRow(rng);
        maze.startCol = interiorCol(rng);
        do {
            maze.goalRow = interiorRow(rng);
            maze.goalCol = interiorCol(rng);
        } while (maze.goalRow == maze.startRow && maze.goalCol == maze.startCol);

        // Interior walls.
        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                if ((r == maze.startRow && c == maze.startCol) ||
                    (r == maze.goalRow && c == maze.goalCol)) {
                    continue;
                }
                if (dist01(rng) < wallChance) {
                    maze.grid[r][c] = '#';
                }
            }
        }

        maze.grid[maze.startRow][maze.startCol] = 'A';
        maze.grid[maze.goalRow][maze.goalCol]   = 'G';

        if (mazeHasPath(maze)) {
            createdSolvableMaze = true;
        }
    }

    // Fallback if somehow never solvable.
    if (!createdSolvableMaze) {
        cerr << "Warning: could not generate a solvable maze; using open maze.\n";
        maze.grid.assign(rows, string(cols, '.'));
        for (int r = 0; r < rows; ++r) {
            maze.grid[r][0] = '#';
            maze.grid[r][cols - 1] = '#';
        }
        for (int c = 0; c < cols; ++c) {
            maze.grid[0][c] = '#';
            maze.grid[rows - 1][c] = '#';
        }
        maze.startRow = 1; maze.startCol = 1;
        maze.goalRow  = rows - 2; maze.goalCol = cols - 2;
        maze.grid[maze.startRow][maze.startCol] = 'A';
        maze.grid[maze.goalRow][maze.goalCol]   = 'G';
    }

    ofstream out(filename);
    if (!out) {
        cerr << "Error: could not open maze output file.\n";
        exit(1);
    }
    for (const auto& line : maze.grid) {
        out << line << "\n";
    }
}

// Render maze as ASCII (with borders)
vector<string> renderAsciiMaze(const Maze& maze) {
    vector<string> lines;
    int R = maze.rowCount;
    int C = maze.colCount;

    string top = " ";
    for (int c = 0; c < C; ++c) {
        top += (maze.grid[0][c] == '#') ? '#' : '_';
    }
    lines.push_back(top);

    for (int r = 0; r < R; ++r) {
        string line = "|";
        for (int c = 0; c < C; ++c) {
            char cell = maze.grid[r][c];

            bool bottomWall = (r == R - 1) || (maze.grid[r + 1][c] == '#');
            bool rightWall  = (c == C - 1) || (maze.grid[r][c + 1] == '#');

            char inside;
            if (cell == '#') {
                inside = '#';
            } else if (cell == 'A' || cell == 'G' || cell == '*') {
                inside = cell;
            } else {
                inside = bottomWall ? '_' : ' ';
            }

            char rightChar = rightWall ? '|' : ' ';
            line += inside;
            line += rightChar;
        }
        lines.push_back(line);
    }

    return lines;
}

// Stores RL training stats.
struct TrainingSummary {
    int attemptsUsed       = 0;
    int successfulRuns     = 0;
    int solvedAttempt      = -1;      // tracked but not printed
    int bestSteps          = 0;       // steps on fastest run
    int bestStepsAttempt   = -1;      // which attempt discovered fastest path
    vector<int> bestRunActions;       // sequence of moves (no wall hits)
};

// Q-learning training loop.
TrainingSummary trainAgentWithQLearning(
    const Maze& maze,
    vector<vector<double>>& qTable,
    int trainingRuns,
    double learningRate,
    double discountFactor,
    double startExploreRate,
    double minExploreRate,
    const string& statsOutputFile
) {
    mt19937 rng((unsigned)time(nullptr));
    TrainingSummary summary;

    double exploreRate = startExploreRate;
    double exploreDecayFactor = 0.995;

    summary.attemptsUsed      = trainingRuns;
    summary.successfulRuns    = 0;
    summary.solvedAttempt     = -1;
    summary.bestSteps         = INT_MAX;
    summary.bestStepsAttempt  = -1;

    for (int run = 0; run < trainingRuns; ++run) {

        int row = maze.startRow;
        int col = maze.startCol;
        bool reachedGoal = false;
        int steps = 0;

        vector<int> actionSequence;  // only real moves

        while (steps < 1000) {
            int state = getStateIndex(row, col, maze);
            int action = chooseAction(state, qTable, exploreRate, rng);

            MoveResult mv = applyAction(row, col, action, maze);
            int nextState = getStateIndex(mv.newRow, mv.newCol, maze);

            double maxNextQ = *max_element(qTable[nextState].begin(), qTable[nextState].end());
            double oldQ = qTable[state][action];
            double updatedQ = oldQ + learningRate * (mv.reward + discountFactor * maxNextQ - oldQ);
            qTable[state][action] = updatedQ;

            // Only count steps when we actually move.
            if (mv.newRow != row || mv.newCol != col) {
                actionSequence.push_back(action);
                steps++;
            }

            row = mv.newRow;
            col = mv.newCol;

            if (mv.reachedGoal) {
                reachedGoal = true;
                break;
            }
        }

        // Decay epsilon.
        if (exploreRate > minExploreRate) {
            exploreRate *= exploreDecayFactor;
            if (exploreRate < minExploreRate) exploreRate = minExploreRate;
        }

        if (reachedGoal) {
            summary.successfulRuns++;

            if (summary.solvedAttempt == -1) {
                summary.solvedAttempt = run + 1;  // first time ever solved (tracked)
            }

            if (steps < summary.bestSteps) {
                summary.bestSteps = steps;
                summary.bestStepsAttempt = run + 1;
                summary.bestRunActions = actionSequence;
            }
        }
    }

    if (summary.successfulRuns == 0) {
        summary.bestSteps = 0;
        summary.bestStepsAttempt = -1;
        summary.bestRunActions.clear();
    }

    // Write stats to file
    ofstream out(statsOutputFile);
    if (out) {
        out << "Training Summary\n";
        out << "----------------\n";
        out << "Actual attempts used: " << summary.attemptsUsed << "\n";
        out << "Successful runs: " << summary.successfulRuns << "\n";
        if (summary.successfulRuns > 0) {
            out << "Fastest path discovered on attempt: " << summary.bestStepsAttempt << "\n";
            out << "Steps to finish (fastest path): " << summary.bestSteps << "\n";
            out << "\nFastest path step sequence:\n";
            for (int act : summary.bestRunActions) {
                out << actionToWord(act) << "\n";
            }
        } else {
            out << "Agent could not solve the maze.\n";
        }
    }

    return summary;
}

// Replay bestRunActions into a fresh copy of the maze and mark '*' path.
int buildPathFromBestActions(const Maze& maze,
                             const vector<int>& actions,
                             Maze& solvedMaze) {
    solvedMaze = maze;
    int row = maze.startRow;
    int col = maze.startCol;
    int steps = 0;

    for (int act : actions) {
        MoveResult mv = applyAction(row, col, act, maze);

        if (mv.newRow == row && mv.newCol == col) {
            continue; // shouldn't happen, but guard
        }

        row = mv.newRow;
        col = mv.newCol;
        steps++;

        if (solvedMaze.grid[row][col] == '.') {
            solvedMaze.grid[row][col] = '*';
        }

        if (mv.reachedGoal) break;
    }

    return steps;
}

// Write ASCII maze with path to file.
void writeAsciiMazeToFile(const Maze& maze, const string& filename) {
    vector<string> lines = renderAsciiMaze(maze);
    ofstream out(filename);
    if (!out) {
        cerr << "Error: could not open file for ASCII maze: " << filename << endl;
        return;
    }
    for (const auto& line : lines) {
        out << line << "\n";
    }
}

// Print file contents to console.
void printFileToConsole(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open file: " << filename << endl;
        return;
    }
    cout << "\n--- Contents of " << filename << " ---\n";
    string line;
    while (getline(in, line)) {
        cout << line << "\n";
    }
    cout << "--- End of " << filename << " ---\n\n";
}

// --------------------------------------------------
// Print just the exact fastest path in the terminal.
//
// Uses:
//   '#' = walls
//   'A' = start
//   'G' = goal
//   '.' = cells along the learned fastest path
//   ' ' = all other empty cells
// --------------------------------------------------
void printPathAsDots(const Maze& solvedMaze) {
    cout << "\nFastest Path View ('.' = path, # = wall, A = start, G = goal):\n\n";
    for (int r = 0; r < solvedMaze.rowCount; ++r) {
        string line;
        for (int c = 0; c < solvedMaze.colCount; ++c) {
            char ch = solvedMaze.grid[r][c];
            char disp;
            if (ch == '#') {
                disp = '#';
            } else if (ch == 'A' || ch == 'G') {
                disp = ch;
            } else if (ch == '*') {
                disp = '.';  // path as dots
            } else {
                disp = ' ';  // other open cells not on path
            }
            line += disp;
        }
        cout << line << "\n";
    }
    cout << "\n";
}

int main() {
    cout << "Enter maze size (rows x cols), e.g. 6x10: ";
    string sizeInput;
    cin >> sizeInput;

    int rows = 0, cols = 0;
    size_t xPos = sizeInput.find('x');
    if (xPos == string::npos) xPos = sizeInput.find('X');

    if (xPos != string::npos) {
        try {
            rows = stoi(sizeInput.substr(0, xPos));
            cols = stoi(sizeInput.substr(xPos + 1));
        } catch (...) {
            rows = cols = 6;
            cout << "Invalid size. Defaulting to 6x6.\n";
        }
    } else {
        rows = cols = 6;
        cout << "Invalid size. Defaulting to 6x6.\n";
    }

    if (rows < 3 || cols < 3) {
        cout << "Maze too small. Using 6x6 instead.\n";
        rows = cols = 6;
    }

    string mazeFile = "maze.txt";
    double wallChance = 0.3;
    generateRandomMazeToFile(rows, cols, wallChance, mazeFile);

    Maze maze = loadMaze(mazeFile);

    // Optional: show the generated maze.
    if (askYesNo("Do you want to see the generated maze?")) {
        vector<string> asciiInitial = renderAsciiMaze(maze);
        cout << "\nGenerated Maze (ASCII view):\n";
        for (const auto& line : asciiInitial) cout << line << "\n";
    }

    int stateCount  = maze.rowCount * maze.colCount;
    int actionCount = 4;
    vector<vector<double>> qTable(stateCount, vector<double>(actionCount, 0.0));

    int    trainingRuns      = 500;
    double learningRate      = 0.1;
    double discountFactor    = 0.95;
    double startExploreRate  = 1.0;
    double minExploreRate    = 0.1;

    string statsFile = "training_stats.txt";
    TrainingSummary summary = trainAgentWithQLearning(
        maze, qTable,
        trainingRuns, learningRate, discountFactor,
        startExploreRate, minExploreRate,
        statsFile
    );

    Maze solvedMaze = maze;
    int finalPathLength = 0;
    string pathFile = "final_path.txt";

    if (summary.successfulRuns > 0 && !summary.bestRunActions.empty()) {
        finalPathLength = buildPathFromBestActions(maze, summary.bestRunActions, solvedMaze);
    }

    writeAsciiMazeToFile(solvedMaze, pathFile);

    cout << "\nTraining finished. Files saved to:\n";
    cout << "  " << mazeFile  << " (internal maze grid)\n";
    cout << "  " << pathFile  << " (ASCII maze with learned path)\n";

    if (askYesNo("Do you want to see a training summary here in the terminal?")) {
        cout << "\n-----------------------------------------\n";
        cout << "Training Summary\n";
        cout << "-----------------------------------------\n";
        cout << "Actual attempts used: " << summary.attemptsUsed << "\n";
        cout << "Successful runs: " << summary.successfulRuns << "\n";

        if (summary.successfulRuns > 0) {
            cout << "Fastest path discovered on attempt: " << summary.bestStepsAttempt << "\n";
            cout << "Steps to finish (fastest path): " << finalPathLength << "\n";
        } else {
            cout << "The agent did not reach the goal in any training run.\n";
        }
    }

    // Flow:
    // 1) Ask to show exact path (dots in grid).
    if (summary.successfulRuns > 0 && finalPathLength > 0 &&
        askYesNo("Do you want to see the exact path the agent took for the fastest solution?")) {

        printPathAsDots(solvedMaze);
    }

    // 2) Then ask to show the step-by-step action sequence.
    if (summary.successfulRuns > 0 &&
        askYesNo("Do you want to see the step-by-step moves for the fastest path?")) {

        cout << "\nFastest Path Step Sequence:\n";
        for (int act : summary.bestRunActions) {
            cout << actionToWord(act) << "\n";
        }
    }

    return 0;
}
