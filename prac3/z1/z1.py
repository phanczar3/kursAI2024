import numpy as np
import copy
from collections import deque

def parse_input():
    f = open("zad_input.txt")
    n, m = f.readline().split()
    n, m = int(n), int(m)
    rows, cols = [], []
    for _ in range(n):
        row = [int(x) for x in f.readline().split()]
        rows.append(row)
    for _ in range(m):
        col = [int(x) for x in f.readline().split()]
        cols.append(col)
    f.close()
    return rows, cols

def print_answer(arr):
    f = open("zad_output.txt", "w")
    res = ""
    for line in arr:
        res += (''.join('#' if x == 2 else '.' for x in line)) + "\n"
    f.write(res)
    f.close()

def no_val(arr, val):
    for x in arr:
        if x == val:
            return False
    return True

def generate_possible(lis, blocks):
    # 0 - nic nie wiem, 1 - wiem ze nie ma, 2 - wiem ze jest
    def gen_possible_rec(lis_i, blocks_i):
        if lis_i == len(lis) and blocks_i == len(blocks):
            return [copy.deepcopy(lis)]
        if lis_i == len(lis):
            return []
        
        res = []

        if blocks_i < len(blocks) and no_val(lis[lis_i : (lis_i + blocks[blocks_i])], 1):
            cur = blocks[blocks_i]

            if lis_i + cur < len(lis) and lis[lis_i + cur] != 2:

                mem = copy.deepcopy(lis[lis_i : (lis_i + cur + 1)])
                lis[lis_i : (lis_i + cur)] = [2] * cur
                lis[lis_i + cur] = 1
                
                for l in gen_possible_rec(lis_i + cur + 1, blocks_i + 1):
                    res.append(l)
                
                lis[lis_i : (lis_i + cur + 1)] = mem
            elif lis_i + cur == len(lis):
                mem = copy.deepcopy(lis[lis_i : (lis_i + cur)])
                lis[lis_i : (lis_i + cur)] = [2] * cur

                for l in gen_possible_rec(lis_i + cur, blocks_i + 1):
                    res.append(l)

                lis[lis_i : (lis_i + cur)] = mem

        if sum(blocks[blocks_i:]) + len(blocks) - blocks_i <= len(lis) - lis_i and lis[lis_i] != 2:
            mem = lis[lis_i]
            lis[lis_i] = 1

            for l in gen_possible_rec(lis_i + 1, blocks_i):
                res.append(l)

            lis[lis_i] = mem
        
        return res
    
    return gen_possible_rec(0, 0)


def solve(arr, rows, cols):
    n, m = len(rows), len(cols)
    # 0 <= i < n -> ity wiersz , n <= i < m -> (i-n)ta kolumna 
    not_solved = deque([x for x in range(n + m)])
    while len(not_solved) > 0:
        x, lis, blocks = not_solved[0], [], []
        not_solved.popleft()
        if x < n:
            lis, blocks = arr[x, :], rows[x]
        else:
            lis, blocks = arr[:, (x-n)], cols[(x-n)]

        if no_val(lis, 0):
            continue
        

        new_lis = [-1 for _ in lis]

        for l in generate_possible(lis, blocks):
            for i in range(len(lis)):
                new_lis[i] &= l[i] # 1 wszedzie -> 1, 2 wszedzie -> 2, -> 0

        assert(new_lis[0] != -1)

        lis = new_lis
        if x < n:
            arr[x, :] = lis
        else:
            arr[:, (x-n)] = lis


        if no_val(lis, 0):
            continue
        else:
            not_solved.append(x)

    print_answer(arr)
 
rows, cols = parse_input()
n, m = len(rows), len(cols)
arr = np.array([[0 for _ in range(m)] for _ in range(n)])
solve(arr, rows, cols)
