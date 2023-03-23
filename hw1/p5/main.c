/*
N players, N rounds, arena capacity M
The i-th player (with attack power a_i) enters the arena at the beginning of the i-th round -> kills all players with attack power less than a_i
King: the player with the highest a_i (tie -> the player with the lowest i)
If num_players > M -> the King is killed

Output: players killed in each round
Target time complexity: O(NlogM)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int index;
  int attack;
} Player;

int find_player_position(int attack, Player* players, int first, int last, int m);
void print_killed_players(Player* players, int player_pos, int last, int m, int round, int first);
void print_player_stats(Player* players, int first, int last, int m, int n);
int get_num_players(Player* players, int first, int last, int m);

int main(void) {
  int n, m;
  scanf("%d %d", &n, &m);

  Player* players = calloc(m, sizeof(Player)); // O(m) memory

  // Add the first player
  players[0].index = 1;
  scanf("%d", &players[0].attack);
  printf("Round 1:\n");
  int first = 0; // also == king
  int last = 0;
  // Add players one by one
  int i;
  for (i = 1; i < n; i++) {
    // Read current player
    int index = i + 1;
    int attack;
    scanf("%d", &attack);

    // Loop: find an appropriate position for the player in players (naive: linear search / better: binary search)
    int pos = find_player_position(attack, players, first, last, m);
    if (((last + 1) % m == first) && (attack <= players[last].attack)) { // revolution occurs
      int score = m * (index - players[pos].index);
      printf("Round %d: %d,%d\n", index, players[pos].index, score);
      players[pos].index = index;
      players[pos].attack = attack;
      first = (first + 1) % m;
      last = (last + 1) % m;
    } else {
      print_killed_players(players, pos, last, m, index, first);
      players[pos].index = index;
      players[pos].attack = attack;
      last = pos;
    }
  }
  print_player_stats(players, first, last, m, n);
}

int get_num_players(Player* players, int first, int last, int m) {
  int ans = (last + 1 - first) % m;
  return (ans <= 0) ? ans + m : ans;
}

int find_player_position(int attack, Player* players, int first, int last, int m) {
  int i;
  int left = 0;
  int right = (last + m - first) % m;
  while (left <= right) {
    int mid = (left + right) / 2;
    int pos = (first + mid) % m;
    if (attack > players[pos].attack) {
      right = mid -1;
    } else {
      left = mid + 1;
    }
  }
  return (first + left) % m;
}

void print_killed_players(Player* players, int player_pos, int last, int m, int round, int first) {
  int i;
  int num_players = get_num_players(players, player_pos, last, m);
  printf("Round %d:", round);
  if (players[player_pos].attack != 0) {
    for (i = 0; i < num_players; i++) {
      int pos = (last + m - i) % m;
      int offset = (pos + m - first) % m;
      int revolution_score = 0;
      int score = (m - offset) * (round - players[pos].index) - revolution_score;
      printf(" %d,%d", players[pos].index, score);
      players[pos].index = 0;
      players[pos].attack = 0;
    }
  }
  printf("\n");
}

void print_player_stats(Player* players, int first, int last, int m, int n) {
  int i;
  int num_players = get_num_players(players, first, last, m);
  printf("Final:");
  for (i = 0; i < num_players; i++) {
    int pos = (last + m - i) % m;
    int offset = (pos + m - first) % m;
    int revolution_score = 0;
    int score = (m - offset) * (n + 1 - players[pos].index) - revolution_score;
    printf(" %d,%d", players[pos].index, score);
  }
}
