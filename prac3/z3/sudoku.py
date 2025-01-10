import sys


def V(i,j):
    return 'V%d_%d' % (i,j)
    
def domains(Vs):
    return [ q + ' in 1..9' for q in Vs ]
    
def all_different(Qs):
    return 'all_distinct([' + ', '.join(Qs) + '])'
    
def get_column(j):
    return [V(i,j) for i in range(9)] 
            
def get_raw(i):
    return [V(i,j) for j in range(9)] 

def get_square(i):
    return [V( (i % 3) * 3 + j % 3, i // 3 * 3 + k % 3) for j in range(3) for k in range(3)]
                        
def horizontal():   
    return [all_different(get_raw(i)) for i in range(9)]

def vertical():
    return [all_different(get_column(j)) for j in range(9)]

def square():
    return [all_different(get_square(j)) for j in range(9)]



def print_constraints(Cs, indent, d):
    position = indent
    print(indent * ' ', end = '', file=output_file)
    for c in Cs:
        print (c + ',', end ='  ', file=output_file)
        position += len(c)
        if position > d:
            position = indent
            print ( file=output_file)
            print (indent * ' ', end='', file=output_file)

      
def sudoku(assigments):
    variables = [ V(i,j) for i in range(9) for j in range(9)]
    
    print (':- use_module(library(clpfd)).', file=output_file)
    print ('solve([' + ', '.join(variables) + ']) :- ', file=output_file)
    
    
    cs = domains(variables) + vertical() + horizontal() + square() 
    for i,j,val in assigments:
        cs.append( '%s #= %d' % (V(i,j), val) )
    

    print_constraints(cs, 4, 70),
    print (file=output_file)
    print ('    labeling([ff], [' +  ', '.join(variables) + ']).' , file=output_file)
    print (file=output_file)
    print (':- solve(X), write(X), nl.', file=output_file)       


if __name__ == "__main__":
    output_file = open("zad_output.txt", 'w')   
    input_file = open("zad_input.txt").readlines()

    raw = 0
    triples = []
    
    for x in input_file:
        x = x.strip()
        if len(x) == 9:
            for i in range(9):
                if x[i] != '.':
                    triples.append( (raw,i,int(x[i])) ) 
            raw += 1          
    sudoku(triples)

    output_file.close()


    
"""
89.356.1.
3...1.49.
....2985.
9.7.6432.
.........
.6389.1.4
.3298....
.78.4....
.5.637.48

53..7....
6..195...
.98....6.
8...6...3
4..8.3..1
7...2...6
.6....28.
...419..5
....8..79

3.......1
4..386...
.....1.4.
6.924..3.
..3......
......719
........6
2.7...3..
"""    
