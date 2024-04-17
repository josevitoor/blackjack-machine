MACHINE BlackJack

SETS 
  PLAYER;
  SUIT = {clubs, diamonds, hearts, spades};
  RANK = {ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king}

CONSTANTS 
  max_balance, max_players, max_table_players

PROPERTIES
  max_balance:NAT1 & max_players:NAT1 & max_table_players:NAT1

VARIABLES
  balance, players, table_players, player_bet, deck, player_cards

INVARIANT
  balance : players --> 0..max_balance 
                  & players <: PLAYER  
	                & table_players : seq(players)
	                &  size(table_players) <= max_table_players
                  & player_bet: players +-> 1..max_balance
                  & player_cards : players --> (SUIT<-> RANK)
                  & deck : seq(SUIT * RANK)

INITIALISATION 
  balance :=  PLAYER * {} || players := {} || table_players := [] || player_bet := PLAYER * {} || deck := [] || player_cards := {}

OPERATIONS
add_player(player) =
  PRE player : PLAYER & player /: players & card(players) < max_players
  THEN players := players \/ {player} || balance := balance <+ {player |-> 0} || player_cards := player_cards \/ { player |-> {} }
  END;

remove_player(player) =
  PRE  player : players & player /: dom(player_bet) &  player /: ran(table_players)
  THEN balance := {player} <<|  balance || players := players - {player} || player_cards := {player} <<|  player_cards
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
  PRE  player : players & size(table_players) < max_table_players 
  THEN table_players := table_players \/ [player]
  END;

exit_table(player) = 
   PRE player : players
   THEN table_players := table_players - [player]
   END;

bet(player, amount) =
  PRE  player : players &  balance(player)  - amount >= 0   & amount > 0 & player : ran(table_players)
  THEN player_bet := player_bet  <+ {player |-> amount}  || balance := balance  <+ {player |-> balance(player) - amount}
  END;

hit(player) =
  PRE player : players  & size(deck) > 0 & player_bet(player) > 0 & player : ran(table_players)
  THEN player_cards(player) := player_cards(player) \/ {last(deck)} || deck := front(deck)
  END;

initialize_custom_deck =
  deck := 
    [hearts|->ace, hearts|->two, hearts|->three, hearts|->four, hearts|->five,
    hearts|->six, hearts|->seven, hearts|->eight, hearts|->nine, hearts|->ten,
    hearts|->jack, hearts|->queen, hearts|->king,
    clubs|->ace, clubs|->two, clubs|->three, clubs|->four, clubs|->five,
    clubs|->six, clubs|->seven, clubs|->eight, clubs|->nine, clubs|->ten,
    clubs|->jack, clubs|->queen, clubs|->king,
    diamonds|->ace, diamonds|->two, diamonds|->three, diamonds|->four, diamonds|->five,
    diamonds|->six, diamonds|->seven, diamonds|->eight, diamonds|->nine, diamonds|->ten,
    diamonds|->jack, diamonds|->queen, diamonds|->king,
    spades|->ace, spades|->two, spades|->three, spades|->four, spades|->five,
    spades|->six, spades|->seven, spades|->eight, spades|->nine, spades|->ten,
    spades|->jack, spades|->queen, spades|->king];
    
resp <-- count_players_query = 
  resp := card(dom(balance))

END
