#define COLS    10
#define ROWS    24
#define WIDTH   64

#define HEIGHT  96
#define SCALE   1
#define UPS     4
#define FPS     30

/********************************************/
/*          CORE TETRIS CONSTANTS           */
/********************************************/
typedef struct {
    int x, y;
} Point;
static const Point SPAWN = {COLS / 2, ROWS - 3};

/*
static const Point J[4] = {{ 1,  0}, { 0,  0}, {-1,  0}, {-1,  1}};
static const Point I[4] = {{ 2,  0}, { 1,  0}, { 0,  0}, {-1,  0}};
static const Point Z[4] = {{ 1,  0}, { 0,  0}, { 0,  1}, {-1,  1}};
static const Point L[4] = {{ 1,  1}, { 1,  0}, { 0,  0}, {-1,  0}};
static const Point O[4] = {{ 1, -1}, { 1,  0}, { 0,  0}, { 0, -1}};
static const Point T[4] = {{ 1,  0}, { 0,  0}, {-1,  0}, { 0,  1}};
static const Point S[4] = {{ 1,  1}, { 0,  1}, { 0,  0}, {-1,  0}};
*/
static const Point PIECES[7][4]= {
    {{ 1,  0}, { 0,  0}, {-1,  0}, {-1,  1}},   // J
    {{ 2,  0}, { 1,  0}, { 0,  0}, {-1,  0}},   // I
    {{ 1,  0}, { 0,  0}, { 0,  1}, {-1,  1}},   // Z
    {{ 1,  1}, { 1,  0}, { 0,  0}, {-1,  0}},   // L
    {{ 1, -1}, { 1,  0}, { 0,  0}, { 0, -1}},   // O
    {{ 1,  0}, { 0,  0}, {-1,  0}, { 0,  1}},   // T
    {{ 1,  1}, { 0,  1}, { 0,  0}, {-1,  0}},   // S
};

static const Point KICKS1[5] = {{ 0,  0}, {-1,  0}, {-1,  1}, { 0, -2}, {-1, -2}};
static const Point KICKS2[5] = {{ 0,  0}, {-2,  0}, { 1,  0}, {-2, -1}, { 1,  2}};

typedef struct {
    int id;
    Point p;
    int rotation;
    int visible;
    Point piece[4];
} FPiece;

typedef struct {
    int table[COLS][ROWS];
    FPiece fPiece;
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
int outOfBounds(Point);
int setFPieceVis(Board*, int);
void printBoard(Board);
int spawn(Board*, int);
int drop(Board*);
void rotate(Board*, int);
void shift(Board*, int);
long long currentTimestamp();
