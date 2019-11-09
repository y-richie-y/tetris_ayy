#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <ncurses.h>
#include "tetris.h"

int outOfBounds(Point p) {
    return (p.x < 0 || p.x >= COLS || p.y < 0);
}

void fPiecePos(Board board, Point *ps) {
    for (int i = 0; i < 4; i++) {
        ps[i] = (Point){
            board.fPiece.p.x + board.fPiece.piece[i].x,
            board.fPiece.p.y + board.fPiece.piece[i].y,
        };
    }
}

int setFPieceVis(Board *board, int visible) {
    FPiece fPiece = board->fPiece;
    if (fPiece.visible == visible)
        return 0;

    Point ps[4];
    fPiecePos(*board, (Point*)&ps);
    for (int i = 0; i < 4; i++) {
        if (visible && (outOfBounds(ps[i]) || board->table[ps[i].x][ps[i].y])) {
            // clash: undo changes
            for (int j = i - 1; j >= 0; j--)
                board->table[ps[j].x][ps[j].y] = 0;
            return 1;
        }
        board->table[ps[i].x][ps[i].y] = visible ? fPiece.id : 0;
    }
    
    board->fPiece.visible = visible;
    return 0;
}

void printBoard(Board board) {
    move(0, 0);
    for (int y = ROWS - 1; y >= 0; y--) {
        printw("%-2d", y);
        for (int x = 0; x < COLS; x++)
            printw(board.table[x][y] ? "#" : " ");
        printw("\n");
    }
    printw("  1234567890\n");
    refresh();
}

void checkLines(Board *board) {
    for (int y = 0; y < ROWS; y++) {
        int clear = 1;
        for (int x = 0; x < COLS; x++) {
            if (!board->table[x][y]) {
                clear = 0;
                break;
            }
        }
        if (clear) {
            for (int yy = y--; yy < ROWS - 1; yy++)
                for (int x = 0; x < COLS; x++)
                    board->table[x][yy] = board->table[x][yy+1];
        }
    }
}

int spawn(Board *board, int pieceId) {
    board->fPiece = (FPiece){pieceId, SPAWN, 0, 0};
    for (int i = 0; i < 4; i++)
        board->fPiece.piece[i] = PIECES[pieceId][i];
    return (setFPieceVis(board, 1));
}

int drop(Board *board) {
    setFPieceVis(board, 0);
    int *y = &board->fPiece.p.y;
    (*y)--;
    if (setFPieceVis(board, 1)) {
        (*y)++;
        setFPieceVis(board, 1);
        checkLines(board);
        return spawn(board, rand()%7);
    }
    return 0;
}

void rotate(Board *board, int direction) {
    if (direction * direction != 1)
        return;
    setFPieceVis(board, 0);

    FPiece *fPiece = &board->fPiece;
    Point *piece = (Point*)&fPiece->piece;
    for (int i = 0; i < 4; i++)
        piece[i] = (Point){direction*piece[i].y, -direction*piece[i].x};
    fPiece->rotation = (fPiece->rotation + direction + 4) % 4;
    if (setFPieceVis(board, 1)) {
        Point kicks[5];
        int rotation = fPiece->rotation;
        Point flips = {
            rotation == 0 || rotation == 3 ? direction : -direction,
            rotation == 0 || rotation == 2 ? direction : -direction,
        };
        if (fPiece->id == 1)
            for (int i = 0; i < 5; i++)
                kicks[i] = (Point){KICKS1[i].x * flips.x, KICKS1[i].y * flips.y};
        else
            for (int i = 0; i < 5; i++)
                kicks[i] = (Point){KICKS2[i].x * flips.x, KICKS2[i].y * flips.y};
        
        // try 5 kick tests
        for (int i = 0; i < 5; i++) {
            fPiece->p.x += kicks[i].x;
            fPiece->p.y += kicks[i].y;
            if (setFPieceVis(board, 1) == 0) // success
                return;
            fPiece->p.x -= kicks[i].x;
            fPiece->p.y -= kicks[i].y;
        }
    }
    else // success
        return;
    // failure
    for (int i = 0; i < 4; i++)
        piece[i] = (Point){-direction*piece[i].y, direction*piece[i].x};
    fPiece->rotation = (fPiece->rotation - direction + 4) % 4;
}

void shift(Board *board, int direction) {
    if (direction * direction != 1)
        return;
    setFPieceVis(board, 0);

    int *x = &board->fPiece.p.x;
    *x += direction;
    if (setFPieceVis(board, 1)) {
        *x -= direction;
        setFPieceVis(board, 1);
    }
}

long long currentTimestamp() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return milliseconds;
}

int main() {
    initscr();
    noecho();
    timeout(0);
    keypad(stdscr, TRUE);
    
    Board board;
    for (int x = 0; x < COLS; x++)
        for (int y = 0; y < ROWS; y++)
            board.table[x][y] = 0;

    spawn(&board, rand()%7);
    printBoard(board);

    long long prev = currentTimestamp();
    int c;
    while (1) {
        c = getch();
        switch (c) {
            case KEY_LEFT:
                shift(&board, -1);
                printBoard(board);
                break;
            case KEY_RIGHT:
                shift(&board, 1);
                printBoard(board);
                break;
            case KEY_UP:
                rotate(&board, 1);
                printBoard(board);
                break;
            case KEY_DOWN:
                rotate(&board, -1);
                printBoard(board);
                break;
            case ' ':
                break;
        }

        if (currentTimestamp() - prev > 1000/UPS) {
            prev = currentTimestamp();
            if (drop(&board))
                break;
            printBoard(board);
        }
    }
    endwin();
}
