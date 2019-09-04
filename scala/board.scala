type Board = List[List[Boolean]]
type Point = (Int, Int)
type Piece = Vector[Point]
type OPiece = (Piece, Piece, Piece, Piece)

val pieceI:Piece = Vector((-1, 0), (0, 0), (1, 0), (2, 0))
val pieceO:Piece = Vector((0, 0), (0, 1), (1, 0), (1, 1))
val pieceJ:Piece = Vector((-1, 0), (-1, 1), (0, 0), (1, 0))
val pieceL:Piece = Vector((-1, 0), (0, 0), (1, 0), (1, 1))
val pieceS:Piece = Vector((-1, 0), (0, 0), (0, 1), (1, 1))
val pieceZ:Piece = Vector((-1, 1), (0, 1), (0, 0), (1, 0))
val pieceT:Piece = Vector((-1, 0), (0, 0), (0, 1), (1, 0))

val pieces = Vector(pieceI, pieceO, pieceJ, pieceL, pieceS, pieceZ, pieceT) 

def rotate(p:Point):Point = (-p._2, p._1)
def rotate(p:Piece):Piece = p.map(rotate)
def rotate(p:OPiece):OPiece = (rotate(p._1), rotate(p._2), rotate(p._3), rotate(p._4))

def gen_rotations(p:Piece):OPiece = (p, rotate(p), rotate(rotate(p)), rotate(rotate(rotate(p))))

val oPieces = pieces.map(gen_rotations)

val board:Board = List.fill(20)(List.fill(10)(false))

def display_board(b:Board) =
  b.map(_.map(if(_) "x" else ".").mkString("")).mkString("\n")

def set_cell(b:Board, xy:Point):Board =
  b.updated(xy._2, b(xy._2).updated(xy._1, true))

// TODO make Option Monad
def add_piece(b:Board, p:Piece, xy:Point):Board =
  p.foldLeft(b)((bb, xyxy) => set_cell(bb, (xyxy._1 + xy._1, xyxy._2 + xy._2)))
  


print(display_board(add_piece(board, pieceT, (5,5))))
