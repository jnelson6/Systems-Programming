open ReM
open Ast
open Dst


let rec chk_expr : expr -> texpr tea_result =
  fun e ->
  match e with
  | Int(_n) -> return IntType
  | Var(id) -> apply_tenv id
  | Add(e1,e2) | Sub(e1,e2) | Mul(e1,e2)  | Div(e1,e2) ->
    chk_expr e1 >>= fun t1 ->
    chk_expr e2 >>= fun t2 ->
    if t1=IntType && t2=IntType
    then return IntType
    else error "aop: arguments not ints"
    
  | IsZero(e) ->
    chk_expr e >>= fun te ->
    if te=IntType
    then return BoolType
    else error "zero?: argument not an int"
  | ITE(e1,e2,e3) ->
    chk_expr e1 >>= fun te1 ->
    if te1=BoolType
    then (chk_expr e2 >>= fun te2 ->
      chk_expr e3 >>= fun te3 ->
      if te2=te3
      then return te2
      else error "ITE: then and else branch have different types"
      )
    else error "ITE: condition is not a bool"
  

  | Let(id,e1,e2) ->
    chk_expr e1 >>= fun t1 ->
    extend_tenv id t1 >>+
    chk_expr e2
    
  | Proc(id,t1,e)  ->
    extend_tenv id t1 >>+
    chk_expr e >>= fun t2 ->
    return (FuncType(t1,t2))

  | App(e1,e2)  -> 
    chk_expr e1 >>= fun te1 ->
    (match te1 with
    | FuncType(t1,t2) ->
      (chk_expr e2 >>= fun te2 ->
         if t1=te2
         then return t2
           else error "app: incorrect argument type"
       )
    | _ -> error "app: operator is not a function")

  (* | Letrec(id,par,_tpar,_tres,e,target) ->
   *   extend_env_rec id par e >>+
   *   eval_expr target
   * | Pair(e1,e2) ->
   *   eval_expr e1 >>= fun ev1 ->
   *   eval_expr e2 >>= fun ev2 ->
   *   return @@ PairVal(ev1,ev2)
   * | Fst(e) ->
   *   eval_expr e >>=
   *   pair_of_pairVal >>= fun p ->
   *   return @@ fst p 
   * | Snd(e) ->
   *   eval_expr e >>=
   *   pair_of_pairVal >>= fun p ->
   *   return @@ snd p     
   * | NewRef(e) ->
   *   eval_expr e >>= fun ev ->
   *   return (RefVal (Store.new_ref g_store ev))
   * | DeRef(e) ->
   *   eval_expr e >>=
   *   int_of_refVal >>= 
   *   Store.deref g_store
   * | SetRef(e1,e2) ->
   *   eval_expr e1 >>=
   *   int_of_refVal >>= fun l ->
   *   eval_expr e2 >>= fun ev ->
   *   Store.set_ref g_store l ev >>= fun _ ->
   *   return UnitVal    
   * | BeginEnd([]) ->
   *   return UnitVal
   * | BeginEnd(es) ->
   *   sequence (List.map eval_expr es) >>= fun l ->
   *   return (List.hd (List.rev l))
   * | Record(fs) ->
   *   sequence (List.map (fun (_id,e) -> eval_expr e) fs) >>= fun evs ->
   *   return (RecordVal (addIds fs evs))
   * | Proj(e,id) ->
   *   eval_expr e >>=
   *   fields_of_recordVal >>= fun fs ->
   *   (match List.assoc_opt id fs with
   *   | None -> error "not found"
   *   | Some ev -> return ev)
   * | Unit -> return UnitVal
   * | Debug(_e) ->
   *   string_of_env >>= fun str_env ->
   *   let str_store = Store.string_of_store string_of_expval g_store 
   *   in (print_endline (str_env^"\n"^str_store);
   *   error "Reached breakpoint") *)
  | _ -> error ("Not implemented: "^string_of_expr e)


             
(* Parse a string into an ast *)

let parse s =
  let lexbuf = Lexing.from_string s in
  let ast = Parser.prog Lexer.read lexbuf in
  ast

let lexer s =
  let lexbuf = Lexing.from_string s
  in Lexer.read lexbuf 


(* Interpret an expression *)
let chk (s:string) : texpr result =
  let c = s |> parse |> chk_expr
  in runt c



