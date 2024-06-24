#ifndef _BlackJack_h
#define _BlackJack_h

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BlackJack__USER__max 200
/* Clause SETS */
typedef int BlackJack__USER;
typedef enum {
  BlackJack__clubs,
  BlackJack__diamonds,
  BlackJack__hearts,
  BlackJack__spades

} BlackJack__SUIT;
#define BlackJack__SUIT__max 4
typedef enum {
  BlackJack__ace,
  BlackJack__two,
  BlackJack__three,
  BlackJack__four,
  BlackJack__five,
  BlackJack__six,
  BlackJack__seven,
  BlackJack__eight,
  BlackJack__nine,
  BlackJack__ten,
  BlackJack__jack,
  BlackJack__queen,
  BlackJack__king

} BlackJack__RANK;
#define BlackJack__RANK__max 13
typedef enum {
  BlackJack__yes,
  BlackJack__no

} BlackJack__ACE;
#define BlackJack__ACE__max 2

/* Clause CONCRETE_CONSTANTS */
/* Basic constants */
#define BlackJack__max_balance 100
#define BlackJack__max_players 50
/* Array and record constants */

/* Clause CONCRETE_VARIABLES */

extern void BlackJack__INITIALISATION(void);

/* Clause OPERATIONS */

extern void BlackJack__add_player(BlackJack__USER player);
extern void BlackJack__remove_player(BlackJack__USER player);
extern void BlackJack__add_balance(BlackJack__USER player, int32_t amount);
extern void BlackJack__remove_balance(BlackJack__USER player, int32_t amount);
extern void BlackJack__initialize_table_with_banker(BlackJack__USER banker);
extern void BlackJack__enter_table(BlackJack__USER player);
extern void BlackJack__exit_table(int32_t index);
extern void BlackJack__bet(BlackJack__USER player, int32_t amount);
extern void BlackJack__deal_initial_player_cards(int32_t index);
extern void BlackJack__deal_initial_banker_cards(void);
extern void BlackJack__hit_player(BlackJack__USER player);
extern void BlackJack__hit_banker(BlackJack__USER banker);
extern void BlackJack__stand_player(BlackJack__USER player);
extern void BlackJack__stand_banker(BlackJack__USER banker);
extern void BlackJack__check_player_result(int32_t index, BlackJack__USER banker);
extern void BlackJack__end_game(BlackJack__USER banker);
extern void BlackJack__shuffle_deck(void);
extern void BlackJack__show_winners(BlackJack__USER *ww);
extern void BlackJack__show_losers(BlackJack__USER *ll);
extern void BlackJack__show_player_bet(int32_t *bb, BlackJack__USER player);
extern void BlackJack__show_player_balance(int32_t *bb, BlackJack__USER player);
extern void BlackJack__show_player_cards(BlackJack__RANK *rr, BlackJack__SUIT *ss, BlackJack__USER player);
extern void BlackJack__show_banker_cards(BlackJack__RANK *rr, BlackJack__SUIT *ss);
extern void BlackJack__show_banker_cards_values(int32_t *rr, BlackJack__USER banker);
extern void BlackJack__show_player_cards_values(int32_t *rr, BlackJack__USER player);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BlackJack_h */
