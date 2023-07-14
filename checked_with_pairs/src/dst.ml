open ReM



type tenv =
  | EmptyTEnv
  | ExtendTEnv of string*Ast.texpr*tenv

(* Type-environment abstracted result *)
type 'a tea_result =  (tenv,'a) a_result


let rec apply_tenv : string -> Ast.texpr tea_result =
  fun id env ->
  match env with
  | EmptyTEnv -> Error (id^" not found!")
  | ExtendTEnv(v,tex,tail) ->
    if id=v
    then Ok tex
    else apply_tenv id tail
             
let extend_tenv : string -> Ast.texpr -> tenv tea_result =
  fun id ty ->
  fun tenv ->
  Ok (ExtendTEnv(id,ty,tenv))
    
let runt (c:'a tea_result) : 'a result =
  c EmptyTEnv
