def B(i,j):
    return 'B_%d_%d' % (i,j)
    
def domains(bs):
    return [b + ' in 0..1' for b in bs]

def horizontal_sum(rows, cols):
    return [ ' + '.join([ B(i, j) for j in range(len(cols))]) + ' #= ' + str(rows[i]) for i in range(len(rows)) ]

def vertical_sum(rows, cols):
    return [ ' + '.join([ B(i, j) for i in range(len(rows))]) + ' #= ' + str(cols[j]) for j in range(len(cols)) ]

def horizontal_illegal(R, C):
    return ['%s + 2*%s + 3*%s #\= 2' % (B(i, j - 1), B(i, j), B(i, j + 1)) for i in range(R) for j in range(1, C - 1)]

def vertical_illegal(R, C):
    return ['%s + 2*%s + 3*%s #\= 2' % (B(i - 1, j), B(i, j), B(i + 1, j)) for i in range(1, R - 1) for j in range(C)]

def squares1(R, C):
    return ['%s + %s + %s + %s #\= 3' % (B(i, j), B(i, j + 1), B(i + 1, j), B(i + 1, j + 1))  for i in range(R - 1) for j in range(C - 1)]

def squares2(R, C):
    return ['%s + 2*%s + 4*%s + 8*%s #\= 6' % (B(i, j), B(i, j + 1), B(i + 1, j), B(i + 1, j + 1))  for i in range(R - 1) for j in range(C - 1)]

def squares3(R, C):
    return ['%s + 2*%s + 4*%s + 8*%s #\= 9' % (B(i, j), B(i, j + 1), B(i + 1, j), B(i + 1, j + 1))  for i in range(R - 1) for j in range(C - 1)]


def print_constraints(cs, indent, d):
    position = indent
    writeln(indent * ' ')
    for c in cs:
        writeln(c + ',')
        position += len(c)
        if position > d:
            position = indent
            writeln()
            writeln(indent * ' ')

def storms(rows, cols, triples):
    writeln(':- use_module(library(clpfd)).')
    
    R = len(rows)
    C = len(cols)
    
    bs = [ B(i,j) for i in range(R) for j in range(C)]
    
    writeln('solve([' + ', '.join(bs) + ']) :- ')
    
    cs = domains(bs) + horizontal_sum(rows, cols) + vertical_sum(rows, cols) + horizontal_illegal(R, C) + vertical_illegal(R, C) + squares1(R, C) + squares2(R, C) + squares3(R, C)

    for i, j, val in triples:
        cs.append( '%s #= %d' % (B(i, j), val))

    
    print_constraints(cs, 4, 70)

    # writeln('    [%s] = [1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0],' % (', '.join(bs),)) #only for test 1

    writeln('    labeling([ff], [' +  ', '.join(bs) + ']).' )
    writeln('')
    writeln(":- tell('prolog_result.txt'), solve(X), write(X), nl, told.")

def writeln(s = ""):
    output.write(s + '\n')

txt = open('zad_input.txt').readlines()
output = open('zad_output.txt', 'w')

rows = list(map(int, txt[0].split()))
cols = list(map(int, txt[1].split()))
triples = []

for i in range(2, len(txt)):
    if txt[i].strip():
        triples.append(list(map(int, txt[i].split())))

storms(rows, cols, triples)            
        

