MACHINE BlackJack

SETS 
   USER;
   SUIT = {clubs, diamonds, hearts, spades};
   RANK = {ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king};
   ACE = {yes, no}

CONSTANTS 
   max_balance, max_players

PROPERTIES
   max_balance:NAT1 & max_players:NAT1

VARIABLES
   balance, 
   players, 
   table, 
   deck, 
   player_bet, 
   player_cards, 
   bankers, 
   banker_cards, 
   player_ace, 
   banker_ace, 
   stand, 
   values, 
   user_card_values,
   playing,
   winners,
   losers,
   winners_pos,
   losers_pos
   

INVARIANT
   players <: USER 
   & table : seq(USER)
   & size(table) <= 6
   & deck : seq(SUIT * RANK)
   & values : RANK --> 1..10
   & balance : players --> 0..2*max_balance
   & player_bet: players --> 0..2*max_balance
   & player_cards : players --> (SUIT <-> RANK)
   & bankers <: USER 
   & banker_cards : bankers --> (SUIT <-> RANK)
   & card(bankers) <= 1
   & player_ace: players +-> ACE
   & stand <: USER 
   & banker_ace: ACE
   & players /\ bankers = {}
   & !uu . (uu: stand => uu:ran(table))
   & user_card_values : USER <-> 0..30
   & !uu . (uu: dom(user_card_values)  => uu:ran(table))
   & playing <: players
   & winners: 0..4 +-> USER
   & losers: 0..4 +-> USER
   & !uu . (uu: playing  => uu:ran(table))
   & winners_pos: 0..4
   & losers_pos: 0..4

INITIALISATION 
   balance :=  USER * {} 
   || players := {} 
   || table := [] 
   || player_bet := USER * {} 
   || deck := [] 
   || player_cards := {} 
   || bankers := {}
   || banker_cards := {}
   || player_ace := {}
   || banker_ace := no
   || stand := {}
   || values := {ace |->1, two |-> 2, three |-> 3, four |-> 4, five |-> 5, six |-> 6, seven |-> 7, eight |-> 8, nine |-> 9, ten |-> 10, jack |-> 10, queen |-> 10, king |-> 10}
   || user_card_values := USER * {} 
   || playing := {}
   || winners := {0} * {}
   || losers := {0} * {}
   || winners_pos := 0
   || losers_pos := 0

OPERATIONS
add_player(player) =
   PRE player : USER & player /: bankers & player /: players & card(players) < max_players
   THEN players := players \/ {player} 
      || balance := balance <+ {player |-> 0} 
      || player_cards := player_cards \/ { player |-> {} } 
      || player_bet := player_bet <+ {player |-> 0}
   END;

remove_player(player) =
   PRE  player : USER & player : players & player_bet(player) = 0 & player /: ran(table) & balance(player) = 0
   THEN balance := {player} <<|  balance 
      || players := players - {player} 
      || player_cards := {player} <<|  player_cards
      || player_bet := {player} <<|  player_bet
   END;

add_balance(player, amount) =
   PRE player : USER & player : players & amount: NAT1 & player : dom(balance) & balance(player) + amount <= max_balance & player_bet(player) = 0
   THEN balance(player) := balance(player) + amount
   END;

remove_balance(player, amount) =
  PRE player : USER & player : players & amount : NAT1 & player : dom(balance) & balance(player) - amount >= 0
  THEN balance(player) := balance(player) - amount 
   END;

initialize_table_with_banker(banker) = 
   PRE banker : USER & banker /: players & size(table) = 0 
   THEN bankers := {banker} 
      || table := table ^ [banker]
      || banker_ace := no
      || banker_cards := banker_cards \/ { banker |-> {} } 
      || user_card_values := user_card_values <+ {banker |-> 0}
  END;

enter_table(player) =
   PRE  player : USER & player : players & size(table) < 6 & size(table) > 0 & player /: ran(table) & card(playing) = 0
   THEN table := table ^ [player] 
      || player_ace := player_ace <+ {player |-> no} 
      || user_card_values := user_card_values <+ {player |-> 0}
   END;

exit_table(index) = 
   PRE index : NAT1 & index > 1 & index <= size(table) & table(index) : players & player_bet(table(index)) = 0 & table(index) /: playing
   THEN table := (table /|\ index-1) ^ (table \|/ index)
      || player_ace  := {table(index)} <<|  player_ace 
      || stand := stand - {table(index)}
      || user_card_values := {table(index)} <<|  user_card_values
   END;

bet(player, amount) =
   PRE  player : USER & player : players & amount : NAT1 & balance(player) - amount >= 0 & player : ran(table) & player_cards(player) = {} & player /: stand & balance(player) <= max_balance & player /: playing
   THEN player_bet := player_bet  <+ {player |-> amount}  
      || balance := balance  <+ {player |-> balance(player) - amount}
   END;

deal_initial_player_cards(index) = 
   PRE index : NAT1 & index > 1 & index <= size(table) & table(index) : players & (size(deck) >= 2 * size(table)) & (#pp . (pp: players & player_bet(pp) > 0)) & banker_cards(table(1)) = {} & player_cards(table(index)) = {}
   THEN  player_cards(table(index)) := player_cards(table(index)) \/ {last(deck)} \/ {last(deck /|\ size(deck)-1)}
      || playing := playing \/ {table(index)} ||
      IF ace: ran({last(deck)} \/ {last(deck /|\ size(deck))})
      THEN player_ace(table(index)) := yes
      END ||
      ANY first_card WHERE first_card : ran({last(deck)} \/ {last(deck /|\ size(deck)-1)})
      THEN
         ANY second_card WHERE second_card : ran({last(deck)} \/ {last(deck /|\ size(deck)-1)}) & first_card /= second_card
         THEN
            IF player_ace(table(index)) = yes
            THEN user_card_values(table(index)) := values(first_card) +  values(second_card) + 10
            ELSE user_card_values(table(index)) := values(first_card) +  values(second_card)
            END
         END
      END
      || deck := deck /|\ (size(deck) -2)
   END;

deal_initial_banker_cards = 
   PRE size(deck) > 0 & (#pp . (pp: players & player_bet(pp) > 0)) & (!pp . (pp: players & player_bet(pp) > 0 => card(player_cards(pp)) = 2)) & banker_cards(table(1)) = {}
   THEN banker_cards(table(1)) := banker_cards(table(1)) \/  {last(deck)} ||
      IF ace: ran({last(deck)}) THEN banker_ace := yes END ||
      ANY first_card WHERE first_card : ran({last(deck)})
      THEN
         IF banker_ace = yes
         THEN user_card_values(table(1)) := values(first_card) + 10
         ELSE user_card_values(table(1)) :=  values(first_card)
         END
      END 
      || deck := front(deck)
      || winners := {0} * {}
      || losers := {0} * {}
      || winners_pos := 0
      || losers_pos := 0
   END; 

hit_player(player) =
   PRE player : USER & player : players  & size(deck) > 0 & player : ran(table) & player_bet(player) > 0  & card(player_cards(player)) > 0 & player /: stand & user_card_values(player) < 21 & banker_cards(table(1)) /= {}
   THEN player_cards(player) := player_cards(player) \/ {last(deck)} ||
      IF ace: ran({last(deck)})
      THEN
         IF player_ace(player) = yes or user_card_values(player) + 11 > 21
         THEN user_card_values(player) := user_card_values(player) + 1
         ELSE player_ace(player) := yes 
            || user_card_values(player) :=  user_card_values(player) + 11
         END
      ELSE
         ANY value WHERE value : ran({last(deck)})
         THEN  user_card_values(player) :=  user_card_values(player) + values(value)
         END
      END
      || deck := front(deck)
  END;

hit_banker(banker) =
   PRE banker : USER & banker : bankers & size(deck) > 0 & banker : ran(table) & card(banker_cards(banker)) > 0 & banker /: stand & (!pp . (pp : players & card(player_cards(pp)) >= 2 => pp : stand)) & user_card_values(banker) < 17
   THEN banker_cards(banker) := banker_cards(banker) \/ {last(deck)}||
      IF ace: ran({last(deck)})
      THEN
         IF banker_ace = yes or user_card_values(banker) + 11 > 21
         THEN  user_card_values(banker) := user_card_values(banker) + 1
         ELSE banker_ace := yes
            || user_card_values(banker) :=  user_card_values(banker) + 11
         END 
      ELSE
         ANY value WHERE value : ran({last(deck)})
         THEN  user_card_values(banker) :=  user_card_values(banker) + values(value)
         END
      END 
      || deck := front(deck) 
   END;

stand_player(player) =
   PRE player : USER & player : players & player: ran(table) & card(player_cards(player)) >= 2 & player /: stand & banker_cards(table(1)) /= {}
   THEN stand := stand \/ {player}
   END;

stand_banker(banker) =
   PRE banker : USER & banker: bankers & card(banker_cards(banker)) >= 2 & banker /: stand & user_card_values(banker) >= 17
   THEN stand := stand \/ {banker}
   END;

check_player_result(index, banker) = 
   PRE index : NAT1 & index > 1 & index <= 6 & banker : USER & banker : bankers & banker : stand
   THEN
      IF user_card_values(table(index)) > 21
      THEN  player_bet(table(index)) := 0  || IF table(index) /: ran(losers) THEN  losers(losers_pos) := table(index)  || losers_pos := losers_pos + 1 END
      ELSIF user_card_values(banker) > 21
      THEN balance(table(index)) := balance(table(index)) + (player_bet(table(index)) * 2) 
         || player_bet(table(index)) := 0
         || IF table(index) /: ran(winners) THEN winners(winners_pos) := table(index)  || winners_pos := winners_pos + 1 END
      ELSIF  user_card_values(table(index)) >  user_card_values(banker)
      THEN balance(table(index)) := balance(table(index)) + (player_bet(table(index)) * 2) 
         || player_bet(table(index)) := 0
         || IF table(index) /: ran(winners) THEN winners(winners_pos) := table(index)  || winners_pos := winners_pos + 1 END
      ELSIF user_card_values(table(index)) =  user_card_values(banker)
      THEN balance(table(index)) := balance(table(index)) + player_bet(table(index))
         || player_bet(table(index)) := 0
      ELSIF user_card_values(table(index)) <  user_card_values(banker)
      THEN player_bet(table(index)) := 0 || IF table(index) /: ran(losers) THEN  losers(losers_pos) := table(index)  || losers_pos := losers_pos + 1 END
      END
      || player_cards(table(index)) := {}
      || user_card_values(table(index)) := 0
      || player_ace(table(index)) := no
      || stand := stand - {table(index)}
   END;

end_game(banker) = 
   PRE banker : USER & banker : bankers & banker : stand & (!pp . (pp : players => pp /: stand))
   THEN banker_cards(banker) := {}
      || banker_ace := no
      || user_card_values(banker) := 0
      || stand := {}
      || playing := {}
   END;

shuffle_deck =
   PRE size(table) > 1 & size(deck) <=  26 & (#pp . (pp: players & player_bet(pp) > 0))
   THEN deck := [hearts|->eight, spades|->ace, diamonds|->seven, spades|->jack, clubs|->queen,
         hearts|->three, spades|->five, diamonds|->king, clubs|->ace, diamonds|->jack,
         hearts|->king, spades|->nine, diamonds|->three, clubs|->six, hearts|->two,
         diamonds|->two, clubs|->nine, hearts|->queen, clubs|->eight, spades|->four,
         clubs|->jack, diamonds|->ten, hearts|->seven, spades|->three, clubs|->king,
         hearts|->ten, diamonds|->queen, spades|->six, clubs|->seven, hearts|->four,
         diamonds|->ace, spades|->two, hearts|->six, clubs|->four, diamonds|->eight,
         spades|->seven, clubs|->five, hearts|->five, diamonds|->four, spades|->ten,
         hearts|->nine, spades|->eight, clubs|->two, diamonds|->five, spades|->queen,
         hearts|->ace, clubs|->ten, diamonds|->nine, spades|->king, clubs|->three]
   END;

ww <-- show_winners = 
   PRE card ( playing ) = 0 & (!ii . ((ii : 0..4 & ww : 0..4 --> USER)  => ww(ii) : USER))
   THEN ww := %ii.(ii : 0..4 | winners(0)) <+ winners
   END;

ll <-- show_losers = 
   PRE card ( playing ) = 0 & (!ii . ((ii : 0..4 & ll : 0..4 --> USER)  => ll(ii) : USER))
   THEN ll := %ii.(ii : 0..4 | losers(0)) <+ losers
   END

END
