MACHINE BlackJack

SETS 

PLAYER 

CONSTANTS 

max_balance, max_players

PROPERTIES

max_balance:NAT1 & max_players:NAT1

VARIABLES

balance, players

INVARIANT

balance : players --> 0..max_players & players <: PLAYER

INITIALISATION 

balance :=  PLAYER * {} || players := {}

OPERATIONS

add_player(p) =
  PRE p : PLAYER & p /: dom(balance) & card(players) < max_players
  THEN players := players \/ {p} || balance := balance <+ {p |-> 0} 
  END;

remove_player(p) =
  PRE p : PLAYER & p : dom(balance) & card(players) > 0 & p : players
  THEN balance := balance - {p |-> 0} || players := players - {p}
  END;

resp <-- count_players_query = 
  resp := card(dom(balance))


END
