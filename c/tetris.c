#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <ncurses.h>
#include "tetris.h"

void printBoard(Board board) {
    move(0, 0);
    for (int y = ROWS - 1; y >= 0; y--) {
        printw("%-2d ", y);
        for (int x = 0; x < COLS; x++)
            printw(board.table[x][y] ? "#" : " ");
        printw("\n");
    }
    printw(" 1234567890\n");
    refresh();
}

int setFloatingPiece(Board *board, int visible) {
    FloatingPiece floatingPiece = board->floatingPiece;
    Point *floatingPoints = (Point*)floatingPiece.piece.ps;
    for (int i = 0; i < 4; i++) {
        Point p = {
            floatingPiece.p.x + floatingPiece.piece.ps[i].x,
            floatingPiece.p.y + floatingPiece.piece.ps[i].y
        };
        if (p.x < 0 || p.x >= COLS || (visible && board->table[p.x][p.y])) {
            // clash: undo changes
            for (int j = i-1; j >= 0; j--)
                board->table[p.x][p.y] = 0;
            return 1;
        }
        board->table[p.x][p.y] = visible ? floatingPiece.id + 1 : 0;
    }
    return 0;
}

int spawn(Board *board, int pieceId) {
    board->floatingPiece = (FloatingPiece){pieceId, SPAWN, PIECES[pieceId]};
    return setFloatingPiece(board, 1);;
}

void drop(Board *board) {
    setFloatingPiece(board, 0);
    board->floatingPiece.p.y--;
    setFloatingPiece(board, 1);
}

int checkSkirt(Board *board) {
    FloatingPiece floatingPiece = board->floatingPiece;

    int skirt[4] = {3};
    for (int i = 0; i < 4; i++) {
        Point p = floatingPiece.piece.ps[i];
        if (p.y < skirt[p.x + 2])
            skirt[p.x + 2] = p.y;
    }

    for (int i = 0; i < 4; i++) {
        if (skirt[i] == 3)
            continue;
        Point pSkirt = {floatingPiece.p.x + i - 2, floatingPiece.p.y + skirt[i]};
        if (board->table[pSkirt.x][pSkirt.y - 1] || pSkirt.y <= 0) {
            return 1;
        }
    }

    return 0;
}

void rotate(Board *board, int direction) {
    if (direction * direction != 1)
        return;
    setFloatingPiece(board, 0);
    Piece *piece = &board->floatingPiece.piece;
    for (int i = 0; i < 4; i++)
        piece->ps[i] = (Point){direction*piece->ps[i].y, -direction*piece->ps[i].x};
    while (checkSkirt(board))
        board->floatingPiece.p.y++;
    setFloatingPiece(board, 1);
}

void shift(Board *board, int direction) {
    if (direction * direction != 1)
        return;
    setFloatingPiece(board, 0);
    board->floatingPiece.p.x += direction;
    if (setFloatingPiece(board, 1)) {
        board->floatingPiece.p.x -= direction;
        setFloatingPiece(board, 1);
    }
}

int update(Board *board) {
    FloatingPiece *floatingPiece = &board->floatingPiece;
    Point *floatingPoints = floatingPiece->piece.ps;

    // check
    if (checkSkirt(board))
        return spawn(board, rand() % 7);

    // check passed
    drop(board);
    return 0;
}

long long current_timestamp() {
    struct timeval tv; 
    gettimeofday(&tv, NULL); // get current time
    long long milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    timeout(0);
    
    Board board;
    for (int x = 0; x < COLS; x++)
        for (int y = 0; y < ROWS; y++)
            board.table[x][y] = 0;

    spawn(&board, 0);
    printBoard(board);
    int c;
    long long prev = current_timestamp();
    while (1) {
        c = getch();
        FloatingPiece *floatingPiece = &board.floatingPiece;
        int dx = 0, drot = 0;
        switch (c) {
            case KEY_LEFT:
                shift(&board, -1);
                break;
            case KEY_RIGHT:
                shift(&board, 1);
                break;
            case KEY_UP:
                rotate(&board, 1);
                break;
            case KEY_DOWN:
                rotate(&board, -1);
                break;
            case ' ':
                while (!checkSkirt(&board))
                    update(&board);
        }

        if (current_timestamp() - prev > 1000 / UPS) {
            prev = current_timestamp();
            if (update(&board))
                break;
            printBoard(board);
        }
    }
    endwin();
}
