MACHINE BlackJack

SETS 

PLAYER 

CONSTANTS 

max_balance, max_players, max_table_players

PROPERTIES

max_balance:NAT1 & max_players:NAT1 & max_table_players:NAT1

VARIABLES

balance, players, table_players, player_bet

INVARIANT

balance : players --> 0..max_balance 
                  & players <: PLAYER  
                  & card(table_players) <= max_table_players 
                  & table_players <: players
                  & player_bet: players +-> 1..max_balance

INITIALISATION 

balance :=  PLAYER * {} || players := {} || table_players := {} || player_bet := PLAYER * {}

OPERATIONS

add_player(p) =
  PRE p : PLAYER & p /: players & card(players) < max_players
  THEN players := players \/ {p} || balance := balance <+ {p |-> 0} 
  END;

remove_player(player) =
  PRE  player : players & player /: dom(player_bet)
  THEN balance := {player} <<|  balance || players := players - {player} || table_players := table_players - {player}
  END;

add_balance(player, amount) =
  PRE player : players & amount: NAT1 & player : dom(balance) & balance(player) + amount <= max_balance
  THEN balance(player) := balance(player) + amount
  END;

remove_balance(player, amount) =
  PRE player : players & amount : NAT1 & player : dom(balance) &  balance(player) - amount >= 0
  THEN balance(player) := balance(player) - amount 
   END;
    
enter_table(player) =
  PRE  player : players & card(table_players) < max_table_players 
  THEN table_players := table_players \/ {player}
  END;

exit_table(player) = 
   PRE player : players
   THEN table_players := table_players - {player}
   END;

bet(player, amount) =
  PRE  player : players &  balance(player)  - amount >= 0  & player :  table_players & amount > 0
  THEN player_bet := player_bet  <+ {player |-> amount}  || balance := balance  <+ {player |-> balance(player) - amount}
  END;
    
resp <-- count_players_query = 
  resp := card(dom(balance))


END
