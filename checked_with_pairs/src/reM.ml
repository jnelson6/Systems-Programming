type 'a result = Ok of 'a | Error of string

type ('b,'a) a_result = 'b -> 'a result


let return (v:'a) : ('e,'a) a_result =
  fun _env ->
  Ok v

let error (s:string) : ('e,'a) a_result =
  fun _env ->
  Error s

let (>>=) (c:('e,'a) a_result) (f: 'a -> ('e,'b) a_result) : ('e,'b) a_result =
  fun env ->
  match c env with
  | Error err -> Error err
  | Ok v -> f v env

let (>>+) (c:('e,'e) a_result) (d:('e,'a) a_result): ('e,'a) a_result =
  fun env ->
  match c env with
  | Error err -> Error err
  | Ok newenv -> d newenv
