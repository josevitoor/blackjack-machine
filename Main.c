#include <stdio.h>

#include "BlackJack.h"

const char *suit_names[] = {
    "Paus",
    "Diamantes",
    "Copas",
    "Espadas"};

const char *rank_names[] = {
    "As",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "Valete",
    "Rainha",
    "Rei"};

int main() {
  BlackJack__INITIALISATION();

  BlackJack__USER player1 = 1;
  BlackJack__USER banker = 0;
  BlackJack__SUIT player_suit[12];
  BlackJack__RANK player_rank[12];
  BlackJack__SUIT banker_suit[12];
  BlackJack__RANK banker_rank[12];

  printf("Inicializando Jogador 1 e Banqueiro\n");
  BlackJack__add_player(player1);
  BlackJack__initialize_table_with_banker(banker);

  printf("Adicionando 100 de saldo ao Jogador 1\n");
  BlackJack__add_balance(player1, 100);

  printf("Jogador 1 entrando na mesa\n");
  BlackJack__enter_table(player1);

  printf("\nInformações Iniciais do Jogador 1:\n");
  int32_t player1_bet, player1_balance;
  BlackJack__show_player_bet(&player1_bet, player1);
  BlackJack__show_player_balance(&player1_balance, player1);
  printf("Aposta: %d, Saldo: %d\n", player1_bet, player1_balance);

  printf("Jogador 1 realizando 10 em aposta\n");
  BlackJack__bet(player1, 10);

  printf("\nInformações do Jogador 1 após a aposta:\n");
  BlackJack__show_player_bet(&player1_bet, player1);
  BlackJack__show_player_balance(&player1_balance, player1);
  printf("Aposta: %d, Saldo: %d\n", player1_bet, player1_balance);

  printf("\nEmbaralhando o baralho\n");
  BlackJack__shuffle_deck();

  printf("Distribuindo cartas iniciais\n\n");
  BlackJack__deal_initial_banker_cards();
  // 2 é o indece do Jogador 1 na mesa
  BlackJack__deal_initial_player_cards(2);

  BlackJack__show_player_cards(player_rank, player_suit, player1);

  printf("Cartas do Jogador 1:\n");
  for (int i = 0; i < 2; ++i) {
    printf("%s de %s\n", rank_names[player_rank[i]], suit_names[player_suit[i]]);
  }

  BlackJack__show_banker_cards(banker_rank, banker_suit);

  printf("\nCartas do Banqueiro:\n");
  for (int i = 0; i < 1; ++i) {
    printf("%s de %s\n", rank_names[banker_rank[i]], suit_names[banker_suit[i]]);
  }

  printf("\nJogador 1 decide parar \n\n");
  BlackJack__stand_player(player1);

  printf("Banqueiro pega mais uma carta\n");
  BlackJack__hit_banker(banker);

  BlackJack__show_banker_cards(banker_rank, banker_suit);
  printf("Cartas do Banqueiro:\n");
  for (int i = 0; i < 2; ++i) {
    printf("%s de %s\n", rank_names[banker_rank[i]], suit_names[banker_suit[i]]);
  }

  printf("\nBanqueiro decide parar\n");
  BlackJack__stand_banker(banker);

  printf("\nVerificando pontuação:\n");
  int32_t banker_card_values, player_card_values;
  BlackJack__show_banker_cards_values(&banker_card_values, banker);
  BlackJack__show_player_cards_values(&player_card_values, player1);
  printf("Banqueiro: %d, Jogador 1: %d\n", banker_card_values, player_card_values);

  printf("Verificando resultados da partida\n\n");
  // 2 é o indece do Jogador 1 na mesa
  BlackJack__check_player_result(2, banker);

  printf("Informações do Jogador 1 após o fim da partida:\n");
  BlackJack__show_player_bet(&player1_bet, player1);
  BlackJack__show_player_balance(&player1_balance, player1);
  printf("Aposta: %d, Saldo: %d\n", player1_bet, player1_balance);

  BlackJack__USER winners[5];
  BlackJack__USER losers[5];

  printf("Mostrando vencedores...\n");
  BlackJack__show_winners(winners);

  printf("Vencedores:\n");
  for (int i = 0; i < 5; ++i) {
    if (winners[i] != 0) {
      printf("Jogador %d\n", winners[i]);
    }
  }

  printf("Mostrando perdedores...\n");
  BlackJack__show_losers(losers);

  printf("Perdedores:\n");
  for (int i = 0; i < 5; ++i) {
    if (losers[i] != 0) {
      printf("Jogador %d\n", losers[i]);
    }
  }

  printf("Finalizando jogo...\n");
  BlackJack__end_game(banker);

  return 0;
}
