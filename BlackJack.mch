MACHINE BlackJack

SETS 

PLAYER 

CONSTANTS 

max_balance, max_players

PROPERTIES

max_balance:NAT1 & max_players:NAT1

VARIABLES

balance

INVARIANT

balance : PLAYER <-> 0..max_players

INITIALISATION 

balance :=  {}

OPERATIONS

add_player(p) =
  PRE p : PLAYER & p /: dom(balance) 
  THEN balance := balance <+ {p |-> 0} 
  END;

remove_player(p) =
  PRE p : PLAYER & p : dom(balance) 
  THEN balance := balance - {p |-> 0} 
  END;

resp <-- count_players_query = 
  resp := card(dom(balance))


END
