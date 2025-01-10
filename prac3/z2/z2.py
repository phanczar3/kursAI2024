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


def deduce(arr, rows, cols):
    n, m = len(rows), len(cols)
    # 0 <= i < n -> ity wiersz , n <= i < m -> (i-n)ta kolumna 
    bounded = deque([x for x in range(n + m)])
    while len(bounded) > 0:
        x, lis, blocks = bounded[0], [], []
        bounded.popleft()
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

        if new_lis[0] == -1:
            return False

        added = False
        for i in range(len(lis)):
            if lis[i] != new_lis[i]:
                added = True
                if x < n:
                    bounded.append(i + n)
                else:
                    bounded.append(i)
        
        if added:
            bounded.append(x)        

        lis = new_lis
        if x < n:
            arr[x, :] = lis
        else:
            arr[:, (x-n)] = lis
    
    return True
 
def first_zero(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            if arr[i][j] == 0:
                return (i, j)
    return (-1, -1)

# def best_zero(arr):
#     n, m = len(arr), len(arr[0])
#     sums_row, sums_col = [0] * n, [0] * m
#     for i in range(n):
#         sums_row[i] = sum([1 if x != 0 else 0 for x in arr[i, :]])
#     for j in range(m):
#         sums_col[j] = sum([1 if x != 0 else 0 for x in arr[:, j]])
#     x, y, zero_sum = -1, -1, -1
#     for i in range(n):
#         for j in range(m):
#             if arr[i][j] == 0 and zero_sum < sums_row[i] + sums_col[j]:
#                 x, y, zero_sum = i, j, sums_row[i] + sums_col[j]
#     return x, y


def solve(arr, rows, cols):
    row_sum, col_sum = [sum(row) for row in rows], [sum(col) for col in cols]
    def backtracking(arr, rows, cols):
        # print(arr)
        x, y = first_zero(arr)
        if x == -1 and y == -1:
            return arr
        p = row_sum[x] / len(rows) * col_sum[y] / len(cols)
        vals = [1, 2]
        if p > 0.5:
            vals = [2, 1]
        for val in vals:
            mem = copy.deepcopy(arr)
            arr[x][y] = val
            infer = deduce(arr, rows, cols)
            if infer == True:
                res = backtracking(arr, rows, cols)
                if type(res) != type(False):
                    return res
            arr = mem
        return False
        
    print_answer(backtracking(arr, rows, cols))


rows, cols = parse_input()
n, m = len(rows), len(cols)
arr = np.array([[0 for _ in range(m)] for _ in range(n)])

solve(arr, rows, cols)
