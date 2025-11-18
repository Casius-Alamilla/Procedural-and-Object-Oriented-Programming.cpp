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

#include <iostream>   // For input/output: std::cout, std::cin, std::endl
#include <vector>     // For std::vector dynamic arrays (auto-resizing lists)
#include <fstream>    // For file input/output: std::ifstream, std::ofstream
#include <string>     // For using std::string and text manipulation functions
#include <random>     // For modern random number generation: mt19937, distributions
#include <ctime>      // For time functions, often used to seed random generators
#include <algorithm>  // For common algorithms: std::sort, std::find, std::max, etc.
#include <queue>      // For FIFO std::queue and std::priority_queue containers
#include <climits>    // For integer limit constants: INT_MAX, INT_MIN, etc.

using namespace std;

// ---------------------------------------------------------------------
// Simple helper function for yes/no questions.
// Asks the user something like "Do X? (y/n): " and returns true on y/Y.
// ---------------------------------------------------------------------
bool askYesNo(const string& question) {
    cout << question << " (y/n): ";
    char ch;
    cin >> ch;
    return (ch == 'y' || ch == 'Y');
}

// ---------------------------------------------------------------------
// Maze representation.
//  - grid: vector of strings; each char is '#', '.', 'A', or 'G'
//  - rowCount, colCount: dimensions
//  - startRow/startCol: coordinates of 'A'
//  - goalRow/goalCol  : coordinates of 'G'
// ---------------------------------------------------------------------
struct Maze {
    vector<string> grid;
    int rowCount = 0;
    int colCount = 0;
    int startRow = 0;
    int startCol = 0;
    int goalRow  = 0;
    int goalCol  = 0;
};

// ---------------------------------------------------------------------
// Convert an action index (0–3) to a human-readable word.
// This is just for printing paths like "Up, Right, Down,..."
// Action mapping (consistent with Q-learning and applyAction):
//   0 = Up, 1 = Right, 2 = Down, 3 = Left
// ---------------------------------------------------------------------
string actionToWord(int action) {
    switch (action) {
        case 0: return "Up";
        case 1: return "Right";
        case 2: return "Down";
        case 3: return "Left";
        default: return "?";
    }
}

// ---------------------------------------------------------------------
// Load maze from a text file. Expected characters in the file:
//   '#' : wall
//   '.' : open cell
//   'A' : start
//   'G' : goal
// Also detects the start and goal positions.
// ---------------------------------------------------------------------
Maze loadMaze(const string& filename) {
    Maze maze;
    ifstream in(filename);
    string line;

    if (!in) {
        cerr << "Error: could not open maze file: " << filename << endl;
        exit(1);
    }

    // Read all non-empty lines into the grid.
    while (getline(in, line)) {
        if (!line.empty()) {
            maze.grid.push_back(line);
        }
    }

    maze.rowCount = (int)maze.grid.size();
    maze.colCount = (maze.rowCount > 0) ? (int)maze.grid[0].size() : 0;

    bool foundStart = false, foundGoal = false;

    // Scan for 'A' (start) and 'G' (goal)
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

// ---------------------------------------------------------------------
// Check if a given cell is inside the bounds of the maze AND not a wall.
// Returns true if you can stand on (row, col); false if wall or out-of-range.
// ---------------------------------------------------------------------
bool isOpenCell(int row, int col, const Maze& maze) {
    if (row < 0 || row >= maze.rowCount || col < 0 || col >= maze.colCount) return false;
    if (maze.grid[row][col] == '#') return false;
    return true;
}

// ---------------------------------------------------------------------
// Flatten (row, col) into a single integer state index.
// This is used to index the Q-table, which is 1D over states.
// stateIndex = row * numCols + col
// ---------------------------------------------------------------------
int getStateIndex(int row, int col, const Maze& maze) {
    return row * maze.colCount + col;
}

// ---------------------------------------------------------------------
// Struct to represent the result of applying an action in the maze:
//  - newRow, newCol: where the agent ends up
//  - reward       : numeric reward from this move
//  - reachedGoal  : true if this moved onto the goal cell (G)
// ---------------------------------------------------------------------
struct MoveResult {
    int newRow;
    int newCol;
    double reward;
    bool reachedGoal;
};

// ---------------------------------------------------------------------
// Apply a single action in the maze.
// Action convention: 0 = Up, 1 = Right, 2 = Down, 3 = Left.
// If the move hits a wall, agent stays in place and gets a penalty.
// If the move reaches 'G', agent gets a large positive reward.
// Otherwise, movement succeeds with a small negative step cost.
// ---------------------------------------------------------------------
MoveResult applyAction(int row, int col, int action, const Maze& maze) {
    int nextRow = row;
    int nextCol = col;

    // Move the agent based on the action
    if (action == 0)      nextRow--;   // Up
    else if (action == 1) nextCol++;   // Right
    else if (action == 2) nextRow++;   // Down
    else if (action == 3) nextCol--;   // Left

    // If it’s not a legal open cell, we treat as hitting a wall.
    if (!isOpenCell(nextRow, nextCol, maze)) {
        // Hit a wall: stay put, give a negative reward.
        return { row, col, -5.0, false };
    }

    // Check if the agent reached the goal cell.
    if (nextRow == maze.goalRow && nextCol == maze.goalCol) {
        return { nextRow, nextCol, 50.0, true }; // large positive reward
    }

    // Normal, valid move, but not goal: small negative cost to encourage shorter paths.
    return { nextRow, nextCol, -1.0, false };
}

// ---------------------------------------------------------------------
// ε-greedy policy: choose an action given the current state.
//  - With probability ε (exploreRate), pick a random action.
//  - With probability (1 - ε), pick the action with the highest Q-value.
// ---------------------------------------------------------------------
int chooseAction(int stateIndex,
                 const vector<vector<double>>& qTable,
                 double exploreRate,
                 mt19937& rng) {
    uniform_real_distribution<double> dist01(0.0, 1.0);
    double roll = dist01(rng);

    // Explore: random action
    if (roll < exploreRate) {
        uniform_int_distribution<int> actionDist(0, 3);
        return actionDist(rng);
    }

    // Exploit: pick best action based on Q-values
    const auto& qValues = qTable[stateIndex];
    return (int)(max_element(qValues.begin(), qValues.end()) - qValues.begin());
}

// ---------------------------------------------------------------------
// BFS check: is there any path from A to G in the current maze layout?
// Used during random maze generation to ensure we can actually solve it.
// ---------------------------------------------------------------------
bool mazeHasPath(const Maze& maze) {
    vector<vector<bool>> visited(maze.rowCount, vector<bool>(maze.colCount, false));
    queue<pair<int,int>> q;

    q.push({maze.startRow, maze.startCol});
    visited[maze.startRow][maze.startCol] = true;

    // Directions: Up, Right, Down, Left
    const int dRow[4] = {-1, 0, 1, 0};
    const int dCol[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        pair<int,int> cell = q.front();
        q.pop();

        int r = cell.first;
        int c = cell.second;

        // If we reach the goal, path exists.
        if (r == maze.goalRow && c == maze.goalCol) return true;

        // Try 4 neighbors.
        for (int k = 0; k < 4; ++k) {
            int nr = r + dRow[k];
            int nc = c + dCol[k];
            if (isOpenCell(nr, nc, maze) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
    // No path found.
    return false;
}

// ---------------------------------------------------------------------
// Randomly generate a solvable maze and write it to maze.txt.
//  - rows, cols: maze size
//  - wallChance: probability that an interior cell becomes a wall '#'
// This function keeps trying random mazes until one has a path A → G.
// ---------------------------------------------------------------------
void generateRandomMazeToFile(int rows, int cols, double wallChance, const string& filename) {
    if (rows < 3 || cols < 3) {
        cerr << "Maze size must be at least 3x3.\n";
        exit(1);
    }

    mt19937 rng((unsigned)time(nullptr));
    uniform_int_distribution<int> interiorRow(1, rows - 2);  // valid interior rows
    uniform_int_distribution<int> interiorCol(1, cols - 2);  // valid interior cols
    uniform_real_distribution<double> dist01(0.0, 1.0);

    Maze maze;
    maze.rowCount = rows;
    maze.colCount = cols;

    bool createdSolvableMaze = false;

    // Try up to 1000 random mazes to find one with a path
    for (int attempt = 0; attempt < 1000 && !createdSolvableMaze; ++attempt) {
        // Start with all '.' (open) cells
        maze.grid.assign(rows, string(cols, '.'));

        // Put walls around the outer border
        for (int r = 0; r < rows; ++r) {
            maze.grid[r][0] = '#';
            maze.grid[r][cols - 1] = '#';
        }
        for (int c = 0; c < cols; ++c) {
            maze.grid[0][c] = '#';
            maze.grid[rows - 1][c] = '#';
        }

        // Choose random interior cells for start (A) and goal (G)
        maze.startRow = interiorRow(rng);
        maze.startCol = interiorCol(rng);
        do {
            maze.goalRow = interiorRow(rng);
            maze.goalCol = interiorCol(rng);
        } while (maze.goalRow == maze.startRow && maze.goalCol == maze.startCol);

        // Place interior random walls with probability wallChance
        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                // Keep 'A' and 'G' open
                if ((r == maze.startRow && c == maze.startCol) ||
                    (r == maze.goalRow && c == maze.goalCol)) {
                    continue;
                }
                if (dist01(rng) < wallChance) {
                    maze.grid[r][c] = '#';
                }
            }
        }

        // Mark start and goal on the grid
        maze.grid[maze.startRow][maze.startCol] = 'A';
        maze.grid[maze.goalRow][maze.goalCol]   = 'G';

        // Check if this maze is solvable from A to G
        if (mazeHasPath(maze)) {
            createdSolvableMaze = true;
        }
    }

    // Fallback: if we somehow failed to make a solvable maze, create a simple one.
    if (!createdSolvableMaze) {
        cerr << "Warning: could not generate a solvable maze; using open maze.\n";
        maze.grid.assign(rows, string(cols, '.'));
        // Border walls again
        for (int r = 0; r < rows; ++r) {
            maze.grid[r][0] = '#';
            maze.grid[r][cols - 1] = '#';
        }
        for (int c = 0; c < cols; ++c) {
            maze.grid[0][c] = '#';
            maze.grid[rows - 1][c] = '#';
        }
        // Simple start and goal
        maze.startRow = 1; maze.startCol = 1;
        maze.goalRow  = rows - 2; maze.goalCol = cols - 2;
        maze.grid[maze.startRow][maze.startCol] = 'A';
        maze.grid[maze.goalRow][maze.goalCol]   = 'G';
    }

    // Write the final maze to file.
    ofstream out(filename);
    if (!out) {
        cerr << "Error: could not open maze output file.\n";
        exit(1);
    }
    for (const auto& line : maze.grid) {
        out << line << "\n";
    }
}

// ---------------------------------------------------------------------
// Render maze as ASCII lines with some border styling.
// This is for nicer viewing in the file / terminal, not for RL itself.
// ---------------------------------------------------------------------
vector<string> renderAsciiMaze(const Maze& maze) {
    vector<string> lines;
    int R = maze.rowCount;
    int C = maze.colCount;

    // Top border row
    string top = " ";
    for (int c = 0; c < C; ++c) {
        top += (maze.grid[0][c] == '#') ? '#' : '_';
    }
    lines.push_back(top);

    // Each maze row
    for (int r = 0; r < R; ++r) {
        string line = "|";
        for (int c = 0; c < C; ++c) {
            char cell = maze.grid[r][c];

            bool bottomWall = (r == R - 1) || (maze.grid[r + 1][c] == '#');
            bool rightWall  = (c == C - 1) || (maze.grid[r][c + 1] == '#');

            char inside;
            if (cell == '#') {
                inside = '#';               // walls as '#'
            } else if (cell == 'A' || cell == 'G' || cell == '*') {
                inside = cell;              // show special characters as-is
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

// ---------------------------------------------------------------------
// Struct to store RL training statistics:
//  - attemptsUsed      : total training episodes performed
//  - successfulRuns    : number of episodes that reached the goal
//  - solvedAttempt     : first episode index where the agent succeeded
//  - bestSteps         : length (in steps) of the fastest successful path
//  - bestStepsAttempt  : episode index where fastest path was discovered
//  - bestRunActions    : sequence of actions for that best path
// ---------------------------------------------------------------------
struct TrainingSummary {
    int attemptsUsed       = 0;
    int successfulRuns     = 0;
    int solvedAttempt      = -1;      // tracked but not printed
    int bestSteps          = 0;       // steps on fastest run
    int bestStepsAttempt   = -1;      // which attempt discovered fastest path
    vector<int> bestRunActions;       // sequence of moves (no wall hits)
};

// ---------------------------------------------------------------------
// Core Q-learning training loop.
//  - qTable is updated in-place.
//  - We run `trainingRuns` episodes.
//  - On each episode, we start at A and move until we hit G or exceed max steps.
//  - We use ε-greedy for exploration vs. exploitation.
//  - All training stats are recorded in TrainingSummary and statsOutputFile.
// ---------------------------------------------------------------------
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
    mt19937 rng((unsigned)time(nullptr));   // RNG for ε-greedy and random actions
    TrainingSummary summary;

    double exploreRate = startExploreRate;  // initial ε
    double exploreDecayFactor = 0.995;      // multiplicative decay each episode

    summary.attemptsUsed      = trainingRuns;
    summary.successfulRuns    = 0;
    summary.solvedAttempt     = -1;
    summary.bestSteps         = INT_MAX;
    summary.bestStepsAttempt  = -1;

    // Run multiple training episodes
    for (int run = 0; run < trainingRuns; ++run) {

        int row = maze.startRow;
        int col = maze.startCol;
        bool reachedGoal = false;
        int steps = 0;

        // We only store actual moves (ignore "hit wall" and stay put).
        vector<int> actionSequence;

        while (steps < 1000) { // hard cap on steps per episode
            int state = getStateIndex(row, col, maze);

            // Choose action using ε-greedy strategy
            int action = chooseAction(state, qTable, exploreRate, rng);

            // Apply action in maze environment
            MoveResult mv = applyAction(row, col, action, maze);
            int nextState = getStateIndex(mv.newRow, mv.newCol, maze);

            // Q-learning update:
            // Q(s,a) ← Q(s,a) + α [ r + γ max_a' Q(s',a') - Q(s,a) ]
            double maxNextQ = *max_element(qTable[nextState].begin(), qTable[nextState].end());
            double oldQ = qTable[state][action];
            double updatedQ = oldQ + learningRate * (mv.reward + discountFactor * maxNextQ - oldQ);
            qTable[state][action] = updatedQ;

            // Only count steps when the agent actually changes cells
            if (mv.newRow != row || mv.newCol != col) {
                actionSequence.push_back(action);
                steps++;
            }

            // Move agent to the new state
            row = mv.newRow;
            col = mv.newCol;

            if (mv.reachedGoal) {
                reachedGoal = true;
                break; // end this episode
            }
        }

        // Decay ε (exploration rate), but keep it above a minimum
        if (exploreRate > minExploreRate) {
            exploreRate *= exploreDecayFactor;
            if (exploreRate < minExploreRate) exploreRate = minExploreRate;
        }

        // If this episode solved the maze, update summary stats.
        if (reachedGoal) {
            summary.successfulRuns++;

            // Record first success
            if (summary.solvedAttempt == -1) {
                summary.solvedAttempt = run + 1;  // 1-based attempt index
            }

            // Record fastest path if this episode is better (fewer steps)
            if (steps < summary.bestSteps) {
                summary.bestSteps = steps;
                summary.bestStepsAttempt = run + 1;
                summary.bestRunActions = actionSequence;
            }
        }
    }

    // If never solved, clear path info.
    if (summary.successfulRuns == 0) {
        summary.bestSteps = 0;
        summary.bestStepsAttempt = -1;
        summary.bestRunActions.clear();
    }

    // Write training stats to text file
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

// ---------------------------------------------------------------------
// Take the bestRunActions (sequence of action indices) and replay them
// in a fresh copy of the maze. Mark the path cells with '*' so we can
// visualize it later in ASCII.
// Returns the number of steps in this replayed path.
// ---------------------------------------------------------------------
int buildPathFromBestActions(const Maze& maze,
                             const vector<int>& actions,
                             Maze& solvedMaze) {
    solvedMaze = maze;  // start from original maze
    int row = maze.startRow;
    int col = maze.startCol;
    int steps = 0;

    for (int act : actions) {
        MoveResult mv = applyAction(row, col, act, maze);

        // Guard: if we somehow didn't move, just skip
        if (mv.newRow == row && mv.newCol == col) {
            continue;
        }

        row = mv.newRow;
        col = mv.newCol;
        steps++;

        // Mark path cells with '*', but keep 'A' and 'G' as they are.
        if (solvedMaze.grid[row][col] == '.') {
            solvedMaze.grid[row][col] = '*';
        }

        if (mv.reachedGoal) break;
    }

    return steps;
}

// ---------------------------------------------------------------------
// Write an ASCII representation (with borders and '|', '_', etc.) of
// the maze to a file. This is used for final_path.txt.
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Utility function: print the contents of a text file to the console.
// (Not heavily used right now, but handy for debugging.)
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Print just the learned fastest path in the terminal as a dot-grid.
// Legend:
//   '#' = walls
//   'A' = start
//   'G' = goal
//   '.' = cells along the learned fastest path
//   ' ' = open cells not used in the path
// ---------------------------------------------------------------------
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
                disp = '.';  // show path cells as '.'
            } else {
                disp = ' ';  // other empty cells
            }
            line += disp;
        }
        cout << line << "\n";
    }
    cout << "\n";
}

int main() {
    // -------------------------------------------
    // 1. Ask the user for maze size: e.g. "6x10"
    // -------------------------------------------
    cout << "Enter maze size (rows x cols), example: 6x10 ";
    string sizeInput;
    cin >> sizeInput;

    int rows = 0, cols = 0;
    size_t xPos = sizeInput.find('x');
    if (xPos == string::npos) xPos = sizeInput.find('X'); // also allow "6X10"

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

    // Enforce a minimum size to avoid trivial/invalid mazes
    if (rows < 3 || cols < 3) {
        cout << "Maze too small. Using 6x6 instead.\n";
        rows = cols = 6;
    }

    // -------------------------------------------
    // 2. Generate a random solvable maze and save
    // -------------------------------------------
    string mazeFile = "maze.txt";
    double wallChance = 0.3; // ~30% of interior cells become walls
    generateRandomMazeToFile(rows, cols, wallChance, mazeFile);

    // Load it back into a Maze struct
    Maze maze = loadMaze(mazeFile);

    // Optional: ask if the user wants to see the generated maze (ASCII view)
    if (askYesNo("Do you want to see the generated maze?")) {
        vector<string> asciiInitial = renderAsciiMaze(maze);
        cout << "\nGenerated Maze (ASCII view):\n";
        for (const auto& line : asciiInitial) cout << line << "\n";
    }

    // -------------------------------------------
    // 3. Set up Q-table for all states and actions
    // -------------------------------------------
    int stateCount  = maze.rowCount * maze.colCount; // one state per cell
    int actionCount = 4;                             // up, right, down, left
    vector<vector<double>> qTable(stateCount, vector<double>(actionCount, 0.0));
    // Q-table initially all zeros.

    // -------------------------------------------
    // 4. Q-learning hyperparameters
    // -------------------------------------------
    int    trainingRuns      = 500;   // number of episodes
    double learningRate      = 0.1;   // α
    double discountFactor    = 0.95;  // γ
    double startExploreRate  = 1.0;   // initial ε (fully random)
    double minExploreRate    = 0.1;   // minimum ε after decay

    // -------------------------------------------
    // 5. Train the agent and write training stats
    // -------------------------------------------
    string statsFile = "training_stats.txt";
    TrainingSummary summary = trainAgentWithQLearning(
        maze, qTable,
        trainingRuns, learningRate, discountFactor,
        startExploreRate, minExploreRate,
        statsFile
    );

    // -------------------------------------------
    // 6. Use best run to build the visual path
    // -------------------------------------------
    Maze solvedMaze = maze;   // copy maze to mark path
    int finalPathLength = 0;
    string pathFile = "final_path.txt";

    if (summary.successfulRuns > 0 && !summary.bestRunActions.empty()) {
        // Replay bestRunActions and mark '*' for path cells.
        finalPathLength = buildPathFromBestActions(maze, summary.bestRunActions, solvedMaze);
    }

    // Write ASCII representation (with path) to file.
    writeAsciiMazeToFile(solvedMaze, pathFile);

    cout << "\nTraining finished. Files saved to:\n";
    cout << "  " << mazeFile  << " (internal maze grid)\n";
    cout << "  " << pathFile  << " (ASCII maze with learned path)\n";

    // -------------------------------------------
    // 7. Optional: show summary in the terminal
    // -------------------------------------------
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

    // -------------------------------------------
    // 8. Optional: visualize the exact path as dots
    // -------------------------------------------
    if (summary.successfulRuns > 0 && finalPathLength > 0 &&
        askYesNo("Do you want to see the exact path the agent took for the fastest solution?")) {

        printPathAsDots(solvedMaze);
    }

    // -------------------------------------------
    // 9. Optional: print the actual action sequence in words
    // -------------------------------------------
    if (summary.successfulRuns > 0 &&
        askYesNo("Do you want to see the step-by-step moves for the fastest path?")) {

        cout << "\nFastest Path Step Sequence:\n";
        for (int act : summary.bestRunActions) {
            cout << actionToWord(act) << "\n";
        }
    }

    return 0;
}
