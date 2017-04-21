(* lab3/tree.mli *)

open Dict

(* |name| -- type for applied occurrences with annotations *)
type name = 
  { x_name: ident;              (* Name of the reference *)
    x_line: int;                (* Line number *)
    mutable x_def: def option}  (* Definition in scope *)

type expr = 
    Number of int
  | Bool of bool
  | Variable of name
  | Monop of Keiko.op * expr
  | Binop of Keiko.op * expr * expr
  | Call of expr * expr list

type stmt =
    Skip
  | Seq of stmt list
  | Assign of name * expr
  | Return of expr
  | IfStmt of expr * stmt * stmt
  | WhileStmt of expr * stmt
  | Print of expr
  | Newline

type block = Block of (ident * typ) list * proc list * stmt

and proc = Proc of name * (ident * typ) list * block * typ

type program = Program of block


(* seq -- neatly join a list of statements into a sequence *)
val seq : stmt list -> stmt

val makeName : ident -> int -> name

(* |get_def| -- rerieve definition from name *)
val get_def : name -> def

(* |print_tree| -- pretty-print a tree *)
val print_tree : out_channel -> program -> unit

val fType : typ -> Print.arg
