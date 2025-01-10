import sys
from typing import Tuple, List
track = []
rows, cols = -1, -1
GAMMA = 0.99

def parse_input():
    global rows, cols
    file = sys.argv[1]
    lines = open(file, 'r')
    for line in lines:
        track.append(line)
        rows += 1
        cols = max(cols, len(line))

class State:
    def __init__(self, x, y, vx, vy) -> None:
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
    def is_end(self) -> bool:
        return self.x == -1 or track[self.y][self.x] in ['e', '.']
    def __str__(self) -> str:
        return f"(x: {self.x}, y: {self.y}, vx: {self.vx}, vy {self.vy})"
    def __eq__(self, other) -> bool:
        return (isinstance(other, State) and 
                self.x == other.x and 
                self.y == other.y and 
                self.vx == other.vx and 
                self.vy == other.vy)
    def __hash__(self) -> int:
        return hash((self.x, self.y, self.vx, self.vy))

class Action:
    def __init__(self, dvx, dvy) -> None:
        self.dvx = dvx
        self.dvy = dvy
    def __str__(self) -> str:
        return f"(dvx: {self.dvx}, dvy: {self.dvy})"

directions = [Action(i, j) for i in range(-1, 2) for j in range(-1, 2)]

def actions(s : State) -> List[Action]:
    if track[s.y][s.x] == 'o':
        return [ Action(0,0) ]
    return directions

def transition(s : State, action : Action) -> State:
    dvx, dvy = action.dvx, action.dvy
    vx, vy = max(min(s.vx + dvx, 3), -3), max(min(s.vy + dvy, 3), -3)
    x, y = s.x + vx, s.y + vy
    if 0 <= x < cols and 0 <= y < rows:
        return State(x, y, vx, vy)
    else:
        return -1 # out of board
    
# yield all possible states that i can visit in s using action
def possible_states(s : State, action : Action):
    if track[s.y][s.x] != 'o':
        yield transition(s, action)
    else:
        for action in directions:
            yield transition(s, action)

def next_states(s : State) -> List[State]:
    res = []
    for action in actions(s):
        t = transition(s, action)
        res.append(t)
    return res

def reward(s : State, action : Action, t : State) -> int:
    if track[t.y][t.x] == 'e':
        return 99
    if track[t.y][t.x] == '.':
        return -101
    return -1
# probability that s->t with a
# guaranteed that t is a next state of s
def prob(s : State, action : Action, t : State) -> float:
    if track[s.y][s.x] != 'o':
        return 1
    return 1 / 9

def starting_state() -> State:
    for y in range(0, rows):
        for x in range(0, cols):
            if track[y][x] == 's':
                return State(x, y, 0, 0)

def get_all_states() -> List[State]:
    res = []
    for y in range(0, rows):
        for x in range(0, cols):
            for vx in range(-3, 4):
                for vy in range(-3, 4):
                    s = State(x, y, vx, vy)
                    if not s.is_end():
                        res.append(s)
    return res

def debug(s : State, x, y, vx, vy, v1, v2) -> None:
    if s.x == x and s.y == y and s.vx == vx and s.vy == vy:
        print(s, v1, v2)

def solve_MDP() -> dict:
    values, new_values, eps = {}, {}, 1e-3
    
    all_states = get_all_states()
    for s in all_states:
        values[s] = 0

    changed = True
    while changed:
        changed = False
        
        for s in all_states:
            max_value = -1e18

            for action in actions(s):
                
                cur_value = 0
                for t in possible_states(s, action):   
                    if not isinstance(t, State):
                        cur_value += \
                            prob(s, action, t) * -101
                    elif t.is_end():
                        cur_value += \
                            prob(s, action, t) * (reward(s, action, t) + GAMMA * 0)
                    else:
                        cur_value += \
                            prob(s, action, t) * (reward(s, action, t) + GAMMA * values[t])

                if cur_value > max_value:
                    max_value = cur_value
                
            if abs(values[s] - max_value) > eps:
                changed = True
            new_values[s] = max_value
        
        values = new_values
    
    return values

def optimal_policy(values) -> List[str]:
    res = []
    all_states = get_all_states()
    for s in all_states:
        max_value, arg_max = -1e18, Action(0, 0)
        for action in actions(s):
            cur_value = 0
            for t in possible_states(s, action):
                if not isinstance(t, State):
                    cur_value += \
                        prob(s, action, t) * -101
                else:
                    cur_value += \
                        prob(s, action, t) * (reward(s, action, t) + GAMMA * values[t])
            
            if cur_value > max_value:
                max_value = cur_value
                arg_max = action
        res.append(f"{s.x} {s.y} {s.vx} {s.vy} {arg_max.dvx} {arg_max.dvy}")

    return res

parse_input()
values = solve_MDP()
print(values[starting_state()])

# output = open(sys.argv[2], 'w')
# for line in optimal_policy(values):
#     output.write(line + '\n')