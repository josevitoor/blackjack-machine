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
  user_card_values

INVARIANT
  players <: USER 
  & table : seq(USER)
  & size(table) <= 6
  & deck : seq(SUIT * RANK)
  & values : RANK <-> 1..11
  & balance : players --> 0..2*max_balance
  & player_bet: players --> 0..max_balance
  & player_cards : players --> (SUIT<-> RANK)
  & bankers <: USER 
  & banker_cards : bankers --> (SUIT<-> RANK)
  & card(bankers) <= 1
  & player_ace: players +-> ACE
  & stand <: USER 
  & banker_ace: ACE
  & players /\ bankers = {}
  & !u . (u: stand => u:ran(table))
  & user_card_values : USER <-> 0..32
  & !u . (u: dom(user_card_values)  => u:ran(table))

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

OPERATIONS
add_player(player) =
  PRE player : USER & player /: bankers & player /: players &card(players) < max_players
  THEN players := players \/ {player} 
       || balance := balance <+ {player |-> 0} 
       || player_cards := player_cards \/ { player |-> {} } 
       || player_bet := player_bet <+ {player |-> 0}
  END;

remove_player(player) =
  PRE  player : players & player_bet(player) = 0 &  player /: ran(table)
  THEN balance := {player} <<|  balance 
       || players := players - {player} 
       || player_cards := {player} <<|  player_cards
       || player_bet := {player} <<|  player_bet
  END;

add_balance(player, amount) =
  PRE player : players & amount: NAT1 & player : dom(balance) & balance(player) + amount <= max_balance & player_bet(player) = 0
  THEN balance(player) := balance(player) + amount
  END;

remove_balance(player, amount) =
  PRE player : players & amount : NAT1 & player : dom(balance) &  balance(player) - amount >= 0
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
  PRE  player : players & size(table) < 6 & size(table) > 0 & player /: ran(table)
  THEN table := table ^ [player] 
        || player_ace := player_ace <+ {player |-> no} 
        || user_card_values := user_card_values <+ {player |-> 0}
  END;

exit_table(player) = 
  PRE player : players & player /: dom(player_bet) & player : ran(table)
  THEN table := table - [player]  
        || player_ace  := {player} <<|  player_ace 
        || stand := stand - {player}
        || user_card_values := {player} <<|  user_card_values
  END;

bet(player, amount) =
  PRE  player : players &  balance(player)  - amount >= 0 & amount > 0 & player : ran(table) & player_cards(player) = {} & player /: stand
  THEN player_bet := player_bet  <+ {player |-> amount}  
       || balance := balance  <+ {player |-> balance(player) - amount}
  END;

deal_initial_cards =
  PRE size(deck) >= 2 * size(table) & size(table) > 1 & (#p . (p: players & player_bet(p) > 0)) & banker_cards(table(1)) = {}
  THEN
    IF player_bet(table(2)) > 0 & player_cards(table(2)) = {}
      THEN 
        player_cards(table(2)) := player_cards(table(2)) \/  {last(deck)} \/ {deck(size(deck)-1)};
        IF ace: ran(player_cards(table(2)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN player_ace(table(2)) := yes END;
        ANY first_card WHERE first_card : ran({last(deck)} \/ {deck(size(deck)-1)})
        THEN
          ANY second_card WHERE second_card : ran({last(deck)} \/ {deck(size(deck)-1)}) & first_card /= second_card
          THEN 
            user_card_values(table(2)) := values(first_card) +  values(second_card)
          END
        END;
        deck := front(front(deck))
    END;
    IF size(table) > 2
      THEN
        IF player_bet(table(3)) > 0 & player_cards(table(3)) = {}
          THEN 
            player_cards(table(3)) := player_cards(table(3)) \/  {last(deck)} \/ {deck(size(deck)-1)};
            IF ace: ran(player_cards(table(3)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN player_ace(table(3)) := yes END;
            ANY first_card WHERE first_card : ran({last(deck)} \/ {deck(size(deck)-1)})
            THEN
              ANY second_card WHERE second_card : ran({last(deck)} \/ {deck(size(deck)-1)}) & first_card /= second_card
              THEN 
                user_card_values(table(3)) := values(first_card) +  values(second_card)
              END
            END;
            deck := front(front(deck))
        END
    END;
    IF size(table) > 3
      THEN
        IF player_bet(table(4)) > 0 & player_cards(table(4)) = {}
          THEN 
            player_cards(table(4)) := player_cards(table(4)) \/  {last(deck)} \/ {deck(size(deck)-1)};
            IF ace: ran(player_cards(table(4)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN player_ace(table(4)) := yes END;
            ANY first_card WHERE first_card : ran({last(deck)} \/ {deck(size(deck)-1)})
            THEN
              ANY second_card WHERE second_card : ran({last(deck)} \/ {deck(size(deck)-1)}) & first_card /= second_card
              THEN 
                user_card_values(table(4)) := values(first_card) +  values(second_card)
              END
            END;
            deck := front(front(deck))
        END
    END;
    IF size(table) > 4
      THEN
        IF player_bet(table(5)) > 0 & player_cards(table(5)) = {}
          THEN 
            player_cards(table(5)) := player_cards(table(5)) \/  {last(deck)} \/ {deck(size(deck)-1)};
            IF ace: ran(player_cards(table(5)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN player_ace(table(5)) := yes END;
            ANY first_card WHERE first_card : ran({last(deck)} \/ {deck(size(deck)-1)})
            THEN
              ANY second_card WHERE second_card : ran({last(deck)} \/ {deck(size(deck)-1)}) & first_card /= second_card
              THEN 
                user_card_values(table(5)) :=  values(first_card) +  values(second_card)
              END
            END;
            deck := front(front(deck))
        END
    END;
    IF size(table) > 5
      THEN
        IF player_bet(table(6)) > 0 & player_cards(table(6)) = {}
          THEN 
            player_cards(table(6)) := player_cards(table(6)) \/  {last(deck)} \/ {deck(size(deck)-1)};
            IF ace: ran(player_cards(table(6)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN player_ace(table(6)) := yes END;
            ANY first_card WHERE first_card : ran({last(deck)} \/ {deck(size(deck)-1)})
            THEN
              ANY second_card WHERE second_card : ran({last(deck)} \/ {deck(size(deck)-1)}) & first_card /= second_card
              THEN 
                user_card_values(table(6)) :=  values(first_card) +  values(second_card)
              END
            END;
            deck := front(front(deck))
        END
    END;
    banker_cards(table(1)) := banker_cards(table(1)) \/  {last(deck)};
    IF ace: ran(banker_cards(table(1)) \/ {last(deck)} \/ {deck(size(deck)-1)}) THEN banker_ace := yes END;
     ANY first_card WHERE first_card : ran({last(deck)})
       THEN  user_card_values(table(1)) :=  values(first_card)
     END;
    deck := front(deck)
  END; 

hit_player(player) =
  PRE player : players  & size(deck) > 0 & player : ran(table) & player_bet(player) > 0  & card(player_cards(player)) > 0 & player /: stand & user_card_values(player) <= 21
  THEN player_cards(player) := player_cards(player) \/ {last(deck)} ||
       IF ace: ran({last(deck)})
        THEN player_ace(player) := yes ||
          IF user_card_values(player) + 11 <= 21
          THEN user_card_values(player) := user_card_values(player) + 11
           ELSE
             ANY value WHERE value : ran({last(deck)})
             THEN  user_card_values(player) :=  user_card_values(player) + values(value)
              END
            END
         ELSE
            ANY value WHERE value : ran({last(deck)})
            THEN  user_card_values(player) :=  user_card_values(player) + values(value)
             END
       END||
        deck := front(deck)
  END;

hit_banker(banker) =
  PRE banker : bankers & size(deck) > 0 & banker : ran(table) & card(banker_cards(banker)) > 0 & banker /: stand & (!p . (p : players & card(player_cards(p)) >= 2 => p : stand)) & user_card_values(banker) < 17
  THEN banker_cards(banker) := banker_cards(banker) \/ {last(deck)};
       IF ace: ran({last(deck)})
         THEN banker_ace := yes ||
          IF user_card_values(banker) + 11 <= 21
          THEN  user_card_values(banker) := user_card_values(banker) + 11
           ELSE            
             ANY value WHERE value : ran({last(deck)})
             THEN  user_card_values(banker) :=  user_card_values(banker) + values(value)
              END
           END 
        ELSE
          ANY value WHERE value : ran({last(deck)})
          THEN  user_card_values(banker) :=  user_card_values(banker) + values(value)
           END
        END ||
        deck := front(deck)
  END;

stand_player(player) =
  PRE player : players & player: ran(table) & card(player_cards(player)) >= 2 & player /: stand
  THEN stand := stand \/ {player}
  END;

stand_banker(banker) =
  PRE banker: bankers & card(banker_cards(banker)) >= 2 & banker /: stand & user_card_values(banker) >= 17
  THEN stand := stand \/ {banker}
  END;

result_game(banker) = 
  PRE banker : bankers & banker : stand
  THEN
    IF user_card_values(table(2)) >= user_card_values(banker) & user_card_values(table(2)) <= 21
    THEN
      IF user_card_values(table(2)) = user_card_values(banker)  
      THEN 
        balance(table(2)) := balance(table(2)) + player_bet(table(2));
        player_bet(table(2)) := 0
      ELSE 
        balance(table(2)) := balance(table(2)) + (player_bet(table(2)) * 2);
        player_bet(table(2)) := 0
      END
    ELSE 
      IF user_card_values(table(2)) <= 21 & user_card_values(banker) > 21
      THEN 
        balance(table(2)) := balance(table(2)) + (player_bet(table(2)) * 2);
        player_bet(table(2)) := 0
      ELSE
        player_bet(table(2)) := 0
      END
    END;
    player_cards(table(2)) := {}
    || user_card_values(table(2)) := 0;
    IF size(table) > 2
    THEN
      IF card(player_cards(table(3))) >= 2
      THEN
        IF user_card_values(table(3)) >= user_card_values(banker) & user_card_values(table(3)) <= 21
        THEN
          IF user_card_values(table(3)) = user_card_values(banker)  
          THEN 
            balance(table(3)) := balance(table(3)) + player_bet(table(3));
            player_bet(table(3)) := 0
          ELSE 
            balance(table(3)) := balance(table(3)) + (player_bet(table(3)) * 2);
            player_bet(table(3)) := 0
          END
        ELSE 
          player_bet(table(3)) := 0
        END;
        player_cards(table(3)) := {}
        || user_card_values(table(3)) := 0
      END
    END;
    IF size(table) > 3
    THEN
      IF card(player_cards(table(4))) >= 2
      THEN
        IF user_card_values(table(4)) >= user_card_values(banker) & user_card_values(table(4)) <= 21
        THEN
          IF user_card_values(table(4)) = user_card_values(banker)  
          THEN 
            balance(table(4)) := balance(table(4)) + player_bet(table(4));
            player_bet(table(4)) := 0
          ELSE 
            balance(table(4)) := balance(table(4)) + (player_bet(table(4)) * 2);
            player_bet(table(4)) := 0
          END
        ELSE 
          player_bet(table(4)) := 0
        END;
        player_cards(table(4)) := {}
        || user_card_values(table(4)) := 0
      END
    END;
    IF size(table) > 4
    THEN
      IF card(player_cards(table(5))) >= 2
      THEN
        IF user_card_values(table(5)) >= user_card_values(banker) & user_card_values(table(5)) <= 21
        THEN
          IF user_card_values(table(5)) = user_card_values(banker)  
          THEN 
            balance(table(5)) := balance(table(5)) + player_bet(table(5));
            player_bet(table(5)) := 0
          ELSE 
            balance(table(5)) := balance(table(5)) + (player_bet(table(5)) * 2);
            player_bet(table(5)) := 0
          END
        ELSE 
          player_bet(table(5)) := 0
        END;
        player_cards(table(5)) := {}
        || user_card_values(table(5)) := 0
      END
    END;
    IF size(table) > 5
    THEN
      IF card(player_cards(table(6))) >= 2
      THEN
        IF user_card_values(table(6)) >= user_card_values(banker) & user_card_values(table(6)) <= 21
        THEN
          IF user_card_values(table(6)) = user_card_values(banker)  
          THEN 
            balance(table(6)) := balance(table(6)) + player_bet(table(6));
            player_bet(table(6)) := 0
          ELSE 
            balance(table(6)) := balance(table(6)) + (player_bet(table(6)) * 2);
            player_bet(table(6)) := 0
          END
        ELSE 
          player_bet(table(6)) := 0
        END;
        player_cards(table(6)) := {}
        || user_card_values(table(6)) := 0
      END
    END;
    banker_cards(banker) := {}
    || user_card_values(banker) := 0
  END;

initialize_custom_deck =
  PRE size(table) > 1 & size(deck) = 0 & (#p . (p: players & player_bet(p) > 0))
  THEN deck := 
        [hearts|->eight, spades|->ace, diamonds|->seven, spades|->jack, clubs|->queen,
        hearts|->three, spades|->five, diamonds|->king, clubs|->ace, diamonds|->jack,
        hearts|->king, spades|->nine, diamonds|->three, clubs|->six, hearts|->two,
        diamonds|->two, clubs|->nine, hearts|->queen, clubs|->eight, spades|->four,
        clubs|->jack, diamonds|->ten, hearts|->seven, spades|->three, clubs|->king,
        hearts|->ten, diamonds|->queen, spades|->six, clubs|->seven, hearts|->four,
        diamonds|->ace, spades|->two, hearts|->six, clubs|->four, diamonds|->eight,
        spades|->seven, clubs|->five, hearts|->five, diamonds|->four, spades|->ten,
        hearts|->nine, spades|->eight, clubs|->two, diamonds|->five, spades|->queen,
        hearts|->ace, clubs|->ten, diamonds|->nine, spades|->king, clubs|->three]
  END

END
