from pieces import *
import subprocess as sp
import time

def mapl(x,y):
    return list(map(x,y))

width, height = (10, 20)
board = [[False] * width for _ in range(height)]

def show_board(b):
    for i in range(height):
        raw_row = b[-i]
        row = mapl(lambda x: 'o' if x else '.', raw_row)
        print("".join(row))

def copy_board(b):
    return [bb[:] for bb in b]

ox, oy = (5, 16)

for piece_id in "JIZLOTS":
    cur_piece = rotation_table[piece_id]

    for i in range(8):
        orient_piece = cur_piece[i % 4]
        temp_board = copy_board(board)
        for cell in orient_piece:
            print(cell)
            temp_board[oy + cell[0]][ox + cell[1]] = True

        show_board(temp_board)
        time.sleep(0.5)
        sp.call('clear',shell=True)
