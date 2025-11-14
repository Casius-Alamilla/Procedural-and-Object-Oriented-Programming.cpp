
# Reinforcement Learning Maze Solver (Q-Learning in C++)

This project implements a **Reinforcement Learning (RL)** agent that learns to solve a randomly generated maze using the **Q-learning** algorithm.

It was originally built as a CS 2411 Machine Learning assignment with the following goals:

- Use **Reinforcement Learning** as the machine learning topic.
- Implement the solution in **C++**.
- Compile and run in **Visual Studio**.
- Use **file input** and **file output**:
  - `maze.txt` – generated maze grid (walls, start, goal).
  - `training_stats.txt` – training summary and best action sequence.
  - `final_path.txt` – ASCII visualization of the maze with the learned path.

---

## Features

- Random **maze generation** with guaranteed solvability.
- Q-learning RL agent that:
  - Explores the maze using an ε-greedy policy.
  - Receives rewards for reaching the goal and penalties for hitting walls or taking extra steps.
  - Learns a **shortest (or near-optimal) path** from Start (`A`) to Goal (`G`).
- Interactive terminal prompts:
  - Choose maze size (e.g. `6x10`, `10x50`).
  - Optionally display the generated maze.
  - Optionally show training summary.
  - Optionally show:
    - The exact **optimal path** overlayed on the maze (as `.` dots).
    - The **step-by-step action sequence** (`Up`, `Down`, `Left`, `Right`).
- Clean separation of:
  - Maze generation
  - Environment dynamics
  - Q-learning algorithm
  - Rendering and reporting

---

## How it works (high level)

1. **Maze generation**
   - User enters a size like `6x10`.
   - The program generates a random grid:
     - `#` = wall  
     - `.` = open cell  
     - `A` = start  
     - `G` = goal  
   - The generator runs a BFS check to make sure there is a path from `A` to `G`.  
     If no solvable maze is found after several attempts, it falls back to a simple open maze.
   - The final maze is written to **`maze.txt`**.

2. **Reinforcement Learning (Q-learning)**
   - Each maze cell `(row, col)` is treated as a **state**.
   - There are 4 possible **actions**:
     - `0 = Up`, `1 = Right`, `2 = Down`, `3 = Left`.
   - Reward function:
     - `+50` for reaching `G`.
     - `-1` for a normal move.
     - `-5` for hitting a wall (the agent stays in place).
   - Q-learning update:

     \[
     Q(s,a) \leftarrow Q(s,a) + \alpha \big[r + \gamma \max_{a'} Q(s',a') - Q(s,a)\big]
     \]

     where:
     - `α` (alpha) = learning rate  
     - `γ` (gamma) = discount factor  
     - `s` = current state, `a` = action  
     - `r` = reward, `s'` = next state  

   - The policy is **ε-greedy**:
     - With probability ε, choose a random action (exploration).
     - Otherwise, choose the action with max Q-value (exploitation).
     - ε decays gradually from 1.0 down to a minimum (e.g. 0.1).

3. **Best path reconstruction**
   - During training, every successful episode (that reaches the goal) logs its sequence of **real moves** (no wall hits).
   - The project tracks:
     - How many attempts were used.
     - How many runs were successful.
     - On which attempt the **fastest path** was discovered.
     - How many **steps** that fastest path took.
   - After training, the program **replays** the best action sequence on a copy of the maze:
     - Marks the path cells as `*` (internal).
     - Writes a neat ASCII version to **`final_path.txt`**.
     - In the terminal, it can show the same path with `.` dots.

---

## Files

- `main.cpp`  
  Main program source file. Contains:
  - Maze generation
  - BFS solvability check
  - Q-learning training loop
  - ASCII rendering and terminal UI

- `maze.txt`  
  Generated maze:
  - `#` = wall
  - `.` = open space
  - `A` = start
  - `G` = goal

- `training_stats.txt`  
  Training summary, including:
  - Total attempts used
  - Successful runs
  - Attempt where the **fastest path** was discovered
  - Steps in the fastest path
  - The action sequence (`Up`, `Right`, `Down`, `Left`) for that path

- `final_path.txt`  
  ASCII visualization of the maze with the learned path marked.

- `docs/algorithm_overview.md`  
  Extra documentation explaining the algorithm in more detail (suitable content for the course paper).

- `docs/usage_examples.md`  
  Example runs and sample outputs.

---

## Build & Run

### 1. Using g++ (Linux / macOS / Windows with MinGW)

```bash
g++ -std=c++11 -O2 -o maze_rl main.cpp
./maze_rl
