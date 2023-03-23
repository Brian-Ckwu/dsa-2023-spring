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
void print_killed_players(Player* players, int player_pos, int last, int m, int round);
void print_player_stats(Player* players, int first, int last, int m);
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
      printf("Round %d: %d\n", i + 1, players[first].index);
      players[pos].index = index;
      players[pos].attack = attack;
      first = (first + 1) % m;
      last = (last + 1) % m;
    } else {
      print_killed_players(players, pos, last, m, index);
      players[pos].index = index;
      players[pos].attack = attack;
      last = pos;
    }
  }
  print_player_stats(players, first, last, m);
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

void print_killed_players(Player* players, int player_pos, int last, int m, int round) {
  int i;
  int num_players = get_num_players(players, player_pos, last, m);
  // TOOD: sort players between player_pos and last (or just print from the back??)
  printf("Round %d:", round);
  if (players[player_pos].attack != 0) {
    for (i = 0; i < num_players; i++) {
      int pos = (last - i) % m;
      pos = (pos < 0) ? pos + m : pos;
      printf(" %d", players[pos].index);
      players[pos].index = 0;
      players[pos].attack = 0;
    }
  }
  printf("\n");
}

void print_player_stats(Player* players, int first, int last, int m) {
  int i;
  int num_players = get_num_players(players, first, last, m);
  // TOOD: sort players between first and last
  printf("Final:");
  for (i = 0; i < num_players; i++) {
    int pos = (last - i) % m;
    pos = (pos < 0) ? pos + m : pos;
    printf(" %d", players[pos].index);
  }
}