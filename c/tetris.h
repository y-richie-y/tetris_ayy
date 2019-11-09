#define COLS    10
#define ROWS    24
#define WIDTH   64

#define HEIGHT  96
#define SCALE   4
#define UPS     4

/********************************************/
/*          CORE TETRIS CONSTANTS           */
/********************************************/
// Point
typedef struct {
    int x, y;
} Point;
static const Point SPAWN = {COLS / 2, ROWS - 3};

// Pieces
typedef struct {
    Point ps[4];
} Piece;
static const Piece J = {{{ 1,  0}, { 0,  0}, {-1,  0}, {-1,  1}}};
static const Piece I = {{{ 2,  0}, { 1,  0}, { 0,  0}, {-1,  0}}};
static const Piece Z = {{{ 1,  0}, { 0,  0}, { 0,  1}, {-1,  1}}};
static const Piece L = {{{ 1,  1}, { 1,  0}, { 0,  0}, {-1,  0}}};
static const Piece O = {{{ 1, -1}, { 1,  0}, { 0,  0}, { 0, -1}}};
static const Piece T = {{{ 1,  0}, { 0,  0}, {-1,  0}, { 0,  1}}};
static const Piece S = {{{ 1,  1}, { 0,  1}, { 0,  0}, {-1,  0}}};
static const Piece PIECES[] = {J, I, Z, L, O, T, S};

typedef struct {
    int id;
    Point p;
    Piece piece;
} FloatingPiece;

// Board
typedef struct {
    int table[COLS][ROWS];
    FloatingPiece floatingPiece;
} Board;

/********************************************/
/*      16-BIT COLORS FOR SSD1331           */
/********************************************/
typedef struct {
    unsigned int r : 5;
    unsigned int g : 6;
    unsigned int b : 5;
} Color;
static const Color BLUE     = {0b00000, 0b000000, 0b11111};
static const Color CYAN     = {0b00000, 0b111111, 0b11111};
static const Color RED      = {0b11111, 0b000000, 0b00000};
static const Color ORANGE   = {0b11111, 0b011111, 0b00000};
static const Color YELLOW   = {0b11111, 0b111111, 0b00000};
static const Color PURPLE   = {0b11111, 0b000000, 0b11111};
static const Color GREEN    = {0b00000, 0b111111, 0b00000};
//static const Color COLORS[] = {BLUE, CYAN, RED, ORANGE, YELLOW, PURPLE, GREEN};

/********************************************/
/*          METHOD DECLARATIONS             */
/********************************************/
void printBoard(Board);
int setFloatingPiece(Board*, int);
int spawn(Board*, int);
void drop(Board*);
int checkSkirt(Board*);
void rotate(Board*, int);
void shift(Board*, int);
int update(Board*);
long long current_timestamp();
