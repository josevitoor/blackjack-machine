/* WARNING if type checker is not performed, translation could contain errors ! */

#include <stdlib.h>
#include <time.h>

#include "BlackJack.h"

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */

#define BlackJack__max_balance 100
#define BlackJack__max_players 50
/* Array and record constants */
/* Clause CONCRETE_VARIABLES */

static int32_t BlackJack__players_r[BlackJack__max_players + 1];
static int32_t BlackJack__players_counter;
static int32_t BlackJack__table_r[7];
static int32_t BlackJack__table_counter;
static BlackJack__RANK BlackJack__deck_r[53];
static BlackJack__SUIT BlackJack__deck_s[53];
static int32_t BlackJack__deck_counter;
static int32_t BlackJack__player_bet_r[201];
static int32_t BlackJack__player_balance_r[201];
static BlackJack__SUIT BlackJack__player_cards_s[BlackJack__max_players + 1][12];
static BlackJack__RANK BlackJack__player_cards_r[BlackJack__max_players + 1][12];
static int32_t BlackJack__player_cards_counter[201];
static bool BlackJack__playing_r[201];
static int32_t BlackJack__bankers_r[1];
static bool BlackJack__stand_r[201];
static int32_t BlackJack__winners_r[5];
static int32_t BlackJack__losers_r[5];
static int32_t BlackJack__winners_counter;
static int32_t BlackJack__losers_counter;
static BlackJack__ACE BlackJack__banker_ace_r;
static BlackJack__ACE BlackJack__player_ace_r[201];
static int32_t BlackJack__values_r[13];
static int32_t BlackJack__user_card_values_r[201];
static BlackJack__RANK BlackJack__banker_cards_r[12];
static BlackJack__SUIT BlackJack__banker_cards_s[12];
static int32_t BlackJack__banker_cards_counter;
/* Clause INITIALISATION */
void BlackJack__INITIALISATION(void) {
  unsigned int i = 0, j = 0;
  for (i = 0; i <= BlackJack__max_players; i++) {
    BlackJack__players_r[i] = 0;
  }
  BlackJack__players_counter = 0;
  for (i = 0; i <= 6; i++) {
    BlackJack__table_r[i] = 0;
  }
  BlackJack__table_counter = 0;
  for (i = 0; i <= 52; i++) {
    BlackJack__deck_r[i] = BlackJack__king;
  }
  for (i = 0; i <= 52; i++) {
    BlackJack__deck_s[i] = BlackJack__clubs;
  }
  BlackJack__deck_counter = 0;
  for (i = 0; i <= 200; i++) {
    BlackJack__player_bet_r[i] = 0;
  }
  for (i = 0; i <= 200; i++) {
    BlackJack__player_balance_r[i] = 0;
  }
  for (i = 0; i <= BlackJack__max_players; i++) {
    for (j = 0; j <= 11; j++) {
      BlackJack__player_cards_r[i][j] = BlackJack__king;
    }
  }
  for (i = 0; i <= BlackJack__max_players; i++) {
    for (j = 0; j <= 11; j++) {
      BlackJack__player_cards_s[i][j] = BlackJack__clubs;
    }
  }
  for (i = 0; i <= 200; i++) {
    BlackJack__player_cards_counter[i] = 0;
  }
  for (i = 0; i <= 11; i++) {
    BlackJack__banker_cards_s[i] = BlackJack__clubs;
  }
  for (i = 0; i <= 11; i++) {
    BlackJack__banker_cards_r[i] = BlackJack__king;
  }
  BlackJack__banker_cards_counter = 0;
  for (i = 0; i <= 0; i++) {
    BlackJack__bankers_r[i] = 0;
  }
  BlackJack__banker_ace_r = BlackJack__no;
  for (i = 0; i <= 200; i++) {
    BlackJack__stand_r[i] = false;
  }
  for (i = 0; i <= 4; i++) {
    BlackJack__winners_r[i] = 0;
  }
  BlackJack__winners_counter = 0;
  for (i = 0; i <= 4; i++) {
    BlackJack__losers_r[i] = 0;
  }
  BlackJack__losers_counter = 0;
  for (i = 0; i <= 200; i++) {
    BlackJack__player_ace_r[i] = BlackJack__no;
  }
  for (i = 0; i <= 200; i++) {
    BlackJack__playing_r[i] = false;
  }
  BlackJack__values_r[BlackJack__ace] = 1;
  BlackJack__values_r[BlackJack__two] = 2;
  BlackJack__values_r[BlackJack__three] = 3;
  BlackJack__values_r[BlackJack__four] = 4;
  BlackJack__values_r[BlackJack__five] = 5;
  BlackJack__values_r[BlackJack__six] = 6;
  BlackJack__values_r[BlackJack__seven] = 7;
  BlackJack__values_r[BlackJack__eight] = 8;
  BlackJack__values_r[BlackJack__nine] = 9;
  BlackJack__values_r[BlackJack__ten] = 10;
  BlackJack__values_r[BlackJack__jack] = 10;
  BlackJack__values_r[BlackJack__queen] = 10;
  BlackJack__values_r[BlackJack__king] = 10;
  for (i = 0; i <= 200; i++) {
    BlackJack__user_card_values_r[i] = 0;
  }
}

/* Clause OPERATIONS */

void BlackJack__add_player(BlackJack__USER player) {
  BlackJack__players_r[BlackJack__players_counter] = player;
  BlackJack__players_counter = BlackJack__players_counter + 1;
}

void BlackJack__remove_player(BlackJack__USER player) {
  BlackJack__players_r[BlackJack__players_r[player]] = BlackJack__players_r[BlackJack__players_counter];
  BlackJack__players_counter = BlackJack__players_counter - 1;
  BlackJack__player_balance_r[player] = 0;
  BlackJack__player_bet_r[player] = 0;
  BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[player]]] = 0;
}

void BlackJack__add_balance(BlackJack__USER player, int32_t amount) {
  BlackJack__player_balance_r[player] = BlackJack__player_balance_r[player] + amount;
}

void BlackJack__remove_balance(BlackJack__USER player, int32_t amount) {
  BlackJack__player_balance_r[player] = BlackJack__player_balance_r[player] - amount;
}

void BlackJack__initialize_table_with_banker(BlackJack__USER banker) {
  BlackJack__table_counter = BlackJack__table_counter + 1;
  BlackJack__table_r[BlackJack__table_counter] = banker;
}

void BlackJack__enter_table(BlackJack__USER player) {
  BlackJack__table_counter = BlackJack__table_counter + 1;
  BlackJack__table_r[BlackJack__table_counter] = player;
}

void BlackJack__exit_table(int32_t index) {
  BlackJack__table_r[index] = BlackJack__table_r[BlackJack__table_counter];
  BlackJack__table_counter = BlackJack__table_counter - 1;
}

void BlackJack__bet(BlackJack__USER player, int32_t amount) {
  BlackJack__player_balance_r[player] = BlackJack__player_balance_r[player] - amount;
  BlackJack__player_bet_r[player] = BlackJack__player_bet_r[player] + amount;
}

void BlackJack__deal_initial_player_cards(int32_t index) {
  BlackJack__playing_r[BlackJack__table_r[index]] = true;
  {
    int32_t pos;

    pos = BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]];
    BlackJack__player_cards_s[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]][pos] = BlackJack__deck_s[BlackJack__deck_counter];
    BlackJack__player_cards_s[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]][pos + 1] = BlackJack__deck_s[BlackJack__deck_counter + 1];
    BlackJack__player_cards_r[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]][pos] = BlackJack__deck_r[BlackJack__deck_counter];
    BlackJack__player_cards_r[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]][pos + 1] = BlackJack__deck_r[BlackJack__deck_counter + 1];
    BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[BlackJack__table_r[index]]]] = pos + 2;
  }
  {
    int32_t value;

    value = 0;
    value = BlackJack__values_r[BlackJack__deck_r[BlackJack__deck_counter]];
    value = value + BlackJack__values_r[BlackJack__deck_r[BlackJack__deck_counter + 1]];
    BlackJack__user_card_values_r[BlackJack__table_r[index]] = value;
  }
  {
    BlackJack__RANK uu;
    BlackJack__RANK ii;

    ii = BlackJack__deck_r[BlackJack__deck_counter];
    uu = BlackJack__deck_r[BlackJack__deck_counter + 1];
    if ((ii == BlackJack__ace) ||
        (uu == BlackJack__ace)) {
      BlackJack__player_ace_r[BlackJack__table_r[index]] = BlackJack__yes;
      BlackJack__user_card_values_r[BlackJack__table_r[index]] = BlackJack__user_card_values_r[BlackJack__table_r[index]] + 10;
    }
  }
  BlackJack__deck_counter = BlackJack__deck_counter + 2;
}

void BlackJack__deal_initial_banker_cards(void) {
  BlackJack__banker_cards_r[BlackJack__banker_cards_counter] = BlackJack__deck_r[BlackJack__deck_counter];
  BlackJack__banker_cards_s[BlackJack__banker_cards_counter] = BlackJack__deck_s[BlackJack__deck_counter];
  BlackJack__user_card_values_r[BlackJack__table_r[1]] = BlackJack__values_r[BlackJack__deck_r[BlackJack__deck_counter]];
  {
    BlackJack__RANK ii;

    ii = BlackJack__deck_r[BlackJack__deck_counter];
    if (ii == BlackJack__ace) {
      BlackJack__banker_ace_r = BlackJack__yes;
      BlackJack__user_card_values_r[BlackJack__table_r[1]] = BlackJack__user_card_values_r[BlackJack__table_r[1]] + 10;
    }
  }
  BlackJack__deck_counter = BlackJack__deck_counter + 1;
  BlackJack__banker_cards_counter = BlackJack__banker_cards_counter + 1;
}

void BlackJack__hit_player(BlackJack__USER player) {
  {
    int32_t pos;

    pos = BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[player]]];
    BlackJack__player_cards_s[BlackJack__players_r[BlackJack__table_r[player]]][pos] = BlackJack__deck_s[BlackJack__deck_counter];
    BlackJack__player_cards_r[BlackJack__players_r[BlackJack__table_r[player]]][pos] = BlackJack__deck_r[BlackJack__deck_counter];
    BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[player]]] = pos + 1;
  }
  {
    BlackJack__RANK ii;
    BlackJack__ACE cc;

    ii = BlackJack__deck_r[BlackJack__deck_counter];
    cc = BlackJack__player_ace_r[player];
    if (ii == BlackJack__ace) {
      {
        int32_t aa;
        bool bb;

        aa = BlackJack__user_card_values_r[player] + 11;
        bb = (((aa) > (21)) ? true : false);
        if (bb == true) {
          BlackJack__user_card_values_r[player] = BlackJack__user_card_values_r[player] + 1;
        } else {
          BlackJack__player_ace_r[player] = BlackJack__yes;
          BlackJack__user_card_values_r[player] = BlackJack__user_card_values_r[player] + 11;
        }
      }
    } else if (cc == BlackJack__yes) {
      BlackJack__user_card_values_r[player] = BlackJack__user_card_values_r[player] + 1;
    } else {
      BlackJack__user_card_values_r[player] = BlackJack__user_card_values_r[player] + BlackJack__values_r[BlackJack__deck_r[BlackJack__deck_counter]];
    }
  }
  BlackJack__deck_counter = BlackJack__deck_counter + 1;
}

void BlackJack__hit_banker(BlackJack__USER banker) {
  BlackJack__banker_cards_r[BlackJack__banker_cards_counter] = BlackJack__deck_r[BlackJack__deck_counter];
  BlackJack__banker_cards_s[BlackJack__banker_cards_counter] = BlackJack__deck_s[BlackJack__deck_counter];
  {
    BlackJack__RANK ii;

    ii = BlackJack__deck_r[BlackJack__deck_counter];
    if (ii == BlackJack__ace) {
      {
        int32_t aa;
        bool bb;

        aa = BlackJack__user_card_values_r[banker] + 11;
        bb = (((aa) > (21)) ? true : false);
        if (bb == true) {
          BlackJack__user_card_values_r[banker] = BlackJack__user_card_values_r[banker] + 1;
        } else {
          BlackJack__player_ace_r[banker] = BlackJack__yes;
          BlackJack__user_card_values_r[banker] = BlackJack__user_card_values_r[banker] + 11;
        }
      }
    } else if (BlackJack__banker_ace_r == BlackJack__yes) {
      BlackJack__user_card_values_r[banker] = BlackJack__user_card_values_r[banker] + 1;
    } else {
      BlackJack__user_card_values_r[banker] = BlackJack__user_card_values_r[banker] + BlackJack__values_r[BlackJack__deck_r[BlackJack__deck_counter]];
    }
  }
  BlackJack__deck_counter = BlackJack__deck_counter + 1;
}

void BlackJack__stand_player(BlackJack__USER player) {
  BlackJack__stand_r[player] = true;
}

void BlackJack__stand_banker(BlackJack__USER banker) {
  BlackJack__stand_r[banker] = true;
}

void BlackJack__check_player_result(int32_t index, BlackJack__USER banker) {
  {
    int32_t uu_v;
    int32_t bk_v;
    bool ug;
    bool bg;
    bool eq;
    bool uo;
    bool bo;

    uu_v = BlackJack__user_card_values_r[BlackJack__table_r[index]];
    bk_v = BlackJack__user_card_values_r[banker];
    uo = (((uu_v) > (21)) ? true : false);
    bo = (((bk_v) > (21)) ? true : false);
    ug = (((uu_v) > (bk_v)) ? true : false);
    bg = (((bk_v) > (uu_v)) ? true : false);
    eq = ((uu_v == bk_v) ? true : false);
    if (uo == true) {
      BlackJack__losers_r[BlackJack__losers_counter] = BlackJack__table_r[index];
      BlackJack__losers_counter = BlackJack__losers_counter + 1;
    } else if (bo == true) {
      BlackJack__player_balance_r[BlackJack__table_r[index]] = BlackJack__player_balance_r[BlackJack__table_r[index]] + (BlackJack__player_bet_r[BlackJack__table_r[index]] * 2);
      BlackJack__winners_r[BlackJack__winners_counter] = BlackJack__table_r[index];
      BlackJack__winners_counter = BlackJack__winners_counter + 1;
    } else if (ug == true) {
      BlackJack__player_balance_r[BlackJack__table_r[index]] = BlackJack__player_balance_r[BlackJack__table_r[index]] + (BlackJack__player_bet_r[BlackJack__table_r[index]] * 2);
      BlackJack__winners_r[BlackJack__winners_counter] = BlackJack__table_r[index];
      BlackJack__winners_counter = BlackJack__winners_counter + 1;
    } else if (eq == true) {
      BlackJack__player_balance_r[BlackJack__table_r[index]] = BlackJack__player_balance_r[BlackJack__table_r[index]] + BlackJack__player_bet_r[BlackJack__table_r[index]];
    } else if (bg == true) {
      BlackJack__losers_r[BlackJack__losers_counter] = BlackJack__table_r[index];
      BlackJack__losers_counter = BlackJack__losers_counter + 1;
    }
  }
  BlackJack__player_bet_r[BlackJack__table_r[index]] = 0;
  BlackJack__user_card_values_r[BlackJack__table_r[index]] = 0;
  BlackJack__player_ace_r[BlackJack__table_r[index]] = BlackJack__no;
  BlackJack__stand_r[BlackJack__table_r[index]] = false;
  BlackJack__player_cards_counter[BlackJack__players_r[BlackJack__table_r[index]]] = 0;
}

void BlackJack__end_game(BlackJack__USER banker) {
  unsigned int i = 0;
  {
    BlackJack__banker_ace_r = BlackJack__no;
    for (i = 0; i <= 200; i++) {
      BlackJack__stand_r[i] = false;
    }
    for (i = 0; i <= 200; i++) {
      BlackJack__playing_r[i] = false;
    }
    BlackJack__banker_cards_counter = 0;
  }
}

/* Operação gerada automaticamentee para inciar o baralho de forma fixa
void BlackJack__shuffle_deck(void) {
  {
    int32_t ii;
    BlackJack__SUIT su[4];
    BlackJack__RANK ra[13];

    su[0] = BlackJack__clubs;
    su[1] = BlackJack__diamonds;
    su[2] = BlackJack__hearts;
    su[3] = BlackJack__spades;
    ra[0] = BlackJack__ace;
    ra[1] = BlackJack__two;
    ra[2] = BlackJack__three;
    ra[3] = BlackJack__four;
    ra[4] = BlackJack__five;
    ra[5] = BlackJack__six;
    ra[6] = BlackJack__seven;
    ra[7] = BlackJack__eight;
    ra[8] = BlackJack__nine;
    ra[9] = BlackJack__ten;
    ra[10] = BlackJack__jack;
    ra[11] = BlackJack__queen;
    ra[12] = BlackJack__king;
    ii = 0;
    while ((ii) < (52)) {
      BlackJack__deck_s[ii] = su[ii % 4];
      BlackJack__deck_r[ii] = ra[ii / 4];
      ii = ii + 1;
    }
  }
}*/

// Operação criada para embaralhar aletaroriamente
void BlackJack__shuffle_deck(void) {
  int index = 0;
  for (int suit = BlackJack__clubs; suit < BlackJack__SUIT__max; ++suit) {
    for (int rank = BlackJack__ace; rank < BlackJack__RANK__max; ++rank) {
      BlackJack__deck_s[index] = suit;
      BlackJack__deck_r[index] = rank;
      ++index;
    }
  }

  srand((unsigned int)time(NULL));
  for (int i = 51; i > 0; --i) {
    int j = rand() % (i + 1);
    BlackJack__SUIT temp_suit = BlackJack__deck_s[i];
    BlackJack__RANK temp_rank = BlackJack__deck_r[i];
    BlackJack__deck_s[i] = BlackJack__deck_s[j];
    BlackJack__deck_r[i] = BlackJack__deck_r[j];
    BlackJack__deck_s[j] = temp_suit;
    BlackJack__deck_r[j] = temp_rank;
  }
}

void BlackJack__show_winners(BlackJack__USER* ww) {
  memmove(ww, BlackJack__winners_r, (5) * sizeof(int32_t));
}

void BlackJack__show_losers(BlackJack__USER* ll) {
  memmove(ll, BlackJack__losers_r, (5) * sizeof(int32_t));
}

// Operação criada para consultar aposta do jogador
void BlackJack__show_player_bet(int32_t* bb, BlackJack__USER player) {
  *bb = BlackJack__player_bet_r[player];
}

// Operação criada para consultar saldo do jogador
void BlackJack__show_player_balance(int32_t* bb, BlackJack__USER player) {
  *bb = BlackJack__player_balance_r[player];
}

// Operação criada para consultar quantidade de vencedores
void BlackJack__show_winners_counter(int32_t* cc) {
  *cc = BlackJack__winners_counter;
}

// Operação criada para consultar quantidade de perdedores
void BlackJack__show_losers_counter(int32_t* cc) {
  *cc = BlackJack__losers_counter;
}

// Operação criada para consultar cartas do jogador
void BlackJack__show_player_cards(BlackJack__RANK* rr, BlackJack__SUIT* ss, BlackJack__USER player) {
  memmove(rr, BlackJack__player_cards_r[BlackJack__players_r[BlackJack__table_r[player]]], (12) * sizeof(BlackJack__RANK));
  memmove(ss, BlackJack__player_cards_s[BlackJack__players_r[BlackJack__table_r[player]]], (12) * sizeof(BlackJack__SUIT));
}

// Operação criada para consultar cartas do banqueiro
void BlackJack__show_banker_cards(BlackJack__RANK* rr, BlackJack__SUIT* ss) {
  memmove(rr, BlackJack__banker_cards_r, (12) * sizeof(BlackJack__RANK));
  memmove(ss, BlackJack__banker_cards_s, (12) * sizeof(BlackJack__SUIT));
}

// Operação criada para consultar valor das cartas do banqueiro
void BlackJack__show_banker_cards_values(int32_t* rr, BlackJack__USER banker) {
  *rr = BlackJack__user_card_values_r[banker];
}

// Operação criada para consultar valor das cartas do jogador
void BlackJack__show_player_cards_values(int32_t* rr, BlackJack__USER player) {
  *rr = BlackJack__user_card_values_r[player];
}
