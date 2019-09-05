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

val pieces = Vector(
  "I" -> pieceI,
  "O" -> pieceO,
  "J" -> pieceJ,
  "L" -> pieceL,
  "S" -> pieceS,
  "Z" -> pieceZ,
  "T" -> pieceT) 

def rotate(p:Point):Point = (-p._2, p._1)
def rotate(p:Piece):Piece = p.map(rotate)
def rotate(p:OPiece):OPiece = (rotate(p._1), rotate(p._2), rotate(p._3), rotate(p._4))

def gen_rotations(p:Piece):OPiece = (p, rotate(p), rotate(rotate(p)), rotate(rotate(rotate(p))))

val oPieces = pieces.map({case (s,p) => (s,gen_rotations(p))})

val board:Board = List.fill(20)(List.fill(10)(false))

def display_board(ob:Option[Board]) = {
  val x = ob match {
    case None => List.fill(20)(List.fill(10)(true))
    case Some(b) => b
  }
  x.reverse.map(_.map(if(_) "x" else ".").mkString("")).mkString("\n")
}

def clear_lines(b:Board):Board = 
  b.filter(!_.forall(_ == true)).padTo(20, List.fill(10)(false))

def set_cell(ob:Option[Board], xy:Point):Option[Board] = {
  ob match {
    case None => None
    case Some(b) =>
      b.lift(xy._2) match {
        case None => None
        case Some(r) => r.lift(xy._1) match {
          case None => None
          case Some(c) =>
            if (b(xy._2)(xy._1)) None
            else Option(b.updated(xy._2, b(xy._2).updated(xy._1, true)))
        }
      }
  }
}

def add_piece(b:Board, p:Piece, xy:Point):Option[Board] = 
  p.foldLeft(Option(b))((bb, xyxy) =>
      set_cell(bb, (xyxy._1 + xy._1, xyxy._2 + xy._2))) match {
    case None => None
    case Some(b) => Some(clear_lines(b))
  }
  
def harddrop(b: Board, p:Piece, xy:Point) = {
  val (x, y) = xy
  val bs:List[Option[Board]] = (1 to 20).toList.map(yy => add_piece(b,p,(x,y-yy)))
  (bs zip bs.drop(1) :+ None).filter(_._2 == None).head._1
}

def read_board():Board = {
  val b = List.fill(20)(scala.io.StdIn.readLine()).filter(_.forall(c => c == 'x' || c == '.'))
  if (b.length == 20) b.map(_.map(_ == 'x').toList).reverse
  else { print("Format error, try again"); read_board() }
}

def gen_states(b:Board, p:Piece):List[Board] = 
  (0 to 9).toList.map(x => harddrop(b, p, (x, 18))).filter(_ != None).map{
    case Some(x) => x
    case None => List.fill(20)(List.fill(10)(true))
  }

//print(display_board(Some(read_board())))
gen_states(read_board(), pieceT).map(b => {print(display_board(Some(b))); print("\n\n")})
//print(display_board(add_piece(board, pieceT, (5,18))))
//print(display_board(harddrop(board, pieceT, (5,18))))
  //print(display_board(add_piece(board, pieceT, (5,5))))
