# Algorithm Overview: Q-Learning Maze Solver

## Topic: Reinforcement Learning

This project uses **Reinforcement Learning (RL)**, specifically **Q-learning**, to teach an agent how to navigate a maze. RL is a branch of machine learning where an agent interacts with an environment, takes actions, and receives rewards. Over time, the agent learns a policy that maximizes its expected long-term reward.

In this maze environment:

- Each maze cell is a **state**.
- The agent chooses from 4 possible **actions** (moves): up, right, down, and left.
- The agent starts at cell `A` and must reach the goal cell `G`.

### Reward Structure

The reward function is designed to encourage shorter, efficient paths and penalize collisions with walls:

- `+50` when the agent reaches the goal (`G`).
- `-1` for each normal move (small step penalty).
- `-5` when the agent attempts to move into a wall (`#`) and stays in place.

This creates a clear learning signal: the agent is rewarded for reaching the goal quickly and punished for wasting moves or hitting walls.

### Q-Learning

Q-learning maintains a table of values `Q(s, a)` representing the expected future reward of taking action `a` in state `s` and then following the current policy.

After each step, the Q-value is updated using:

\[
Q(s,a) \leftarrow Q(s,a) + \alpha \big[r + \gamma \max_{a'} Q(s',a') - Q(s,a)\big]
\]

- `α` (alpha) is the **learning rate** (how much new information overrides old).
- `γ` (gamma) is the **discount factor** (how much we value future rewards).
- `r` is the immediate reward.
- `s'` is the next state after taking action `a`.

### Exploration vs. Exploitation

The agent uses an **ε-greedy policy**:

- With probability `ε`, it chooses a random action (exploration).
- With probability `1 - ε`, it chooses the action with the highest `Q(s, a)` (exploitation).

The parameter `ε` starts at 1.0 (completely random behavior) and gradually decays to a minimum value (e.g. 0.1). This means the agent explores a lot at the beginning and then slowly becomes more greedy as it learns a good policy.

### Episode Structure

One **episode** (or training run) works like this:

1. Place the agent at the start cell `A`.
2. Repeat up to a maximum number of steps:
   - Choose an action using ε-greedy policy.
   - Apply the action to the maze, observe the new position and reward.
   - Update the Q-table using the Q-learning formula.
   - If the agent reaches `G`, the episode ends successfully.
3. If the agent does not reach `G` within the step limit, the episode ends unsuccessfully.

The program runs many episodes (for example, 500). During training it records:

- How many episodes ended successfully.
- The **fastest** successful episode (fewest steps).
- On which episode that fastest path was discovered.

### Reconstructing the Optimal Path

After training, the program reconstructs the best path discovered by the agent:

1. It replays the stored action sequence for the fastest successful episode from `A` to `G`.
2. It marks the cells along this route in a copy of the maze.
3. It outputs:
   - `final_path.txt` – an ASCII maze with the learned path.
   - An optional view in the terminal where the path is drawn with `.` dots.

This produces a clear, visual demonstration that the agent learned a path through the maze rather than being hard-coded with a solution.
