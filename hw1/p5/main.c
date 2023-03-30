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

typedef struct {
  int* rev_rounds;
  int* rev_sums;
  int rev_first;
  int rev_last;
  unsigned char empty;
} RevStats;

int find_player_position(int attack, Player* players, int first, int last, int m);
void print_killed_players(Player* players, int player_pos, int last, int m, int round, int first, RevStats* rev_stats);
void print_player_stats(Player* players, int first, int last, int m, int n, RevStats* rev_stats);
int get_num_players(Player* players, int first, int last, int m);
void print_rev_stats(RevStats* rev_stats, int m);
void update_rev_stats(RevStats* rev_stats, int index, int m);
int calc_rev_score(RevStats* rev_stats, int player_index, int* cur, int m);

int main(void) {
  int n, m;
  scanf("%d %d", &n, &m);

  // Player stats
  Player* players = calloc(m, sizeof(Player)); // O(m) memory
  int first = 0; // also == king
  int last = 0;
  // Revolution stats
  RevStats rev_stats;
  rev_stats.rev_rounds = calloc(m, sizeof(int)); // O(m) memory
  rev_stats.rev_sums = calloc(m, sizeof(long long)); // O(m) memory
  rev_stats.rev_first = 0; // first valid rev
  rev_stats.rev_last = m - 1;
  rev_stats.empty = 1;

  // Add the first player
  players[0].index = 1;
  scanf("%d", &players[0].attack);
  printf("Round 1:\n");
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
      // update rev_stats and calculate rev_score
      int rev_score = 0;
      if (!rev_stats.empty) {
        while (!rev_stats.empty && players[pos].index>=rev_stats.rev_rounds[rev_stats.rev_first]) {
          if (rev_stats.rev_first == rev_stats.rev_last) {rev_stats.empty = 1;} // set to 0 when adding a new rev
          rev_stats.rev_first = (rev_stats.rev_first + 1) % m;
        }
        if (!rev_stats.empty) {
          rev_score = rev_stats.rev_sums[rev_stats.rev_last] - rev_stats.rev_sums[(rev_stats.rev_first - 1 + m) % m] - players[pos].index * ((rev_stats.rev_last + 1 - rev_stats.rev_first + m) % m);
        }
      }
      int score = m * (index - players[pos].index) - rev_score;
      printf("Round %d: %d,%d\n", index, players[pos].index, score);
      players[pos].index = index;
      players[pos].attack = attack;
      first = (first + 1) % m;
      last = (last + 1) % m;
      // update revolution status
      update_rev_stats(&rev_stats, index, m);
    } else {
      print_killed_players(players, pos, last, m, index, first, &rev_stats);
      players[pos].index = index;
      players[pos].attack = attack;
      last = pos;
    }
  }
  print_player_stats(players, first, last, m, n, &rev_stats);

  free(players);
  free(rev_stats.rev_rounds);
  free(rev_stats.rev_sums);
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

void print_killed_players(Player* players, int player_pos, int last, int m, int round, int first, RevStats* rev_stats) {
  int i;
  int num_players = get_num_players(players, player_pos, last, m);
  printf("Round %d:", round);
  if (players[player_pos].attack != 0) {
    int cur = rev_stats->rev_last;
    for (i = 0; i < num_players; i++) {
      int pos = (last + m - i) % m;
      int offset = (pos + m - first) % m;
      int revolution_score = calc_rev_score(rev_stats, players[pos].index, &cur, m);
      int score = (m - offset) * (round - players[pos].index) - revolution_score;
      printf(" %d,%d", players[pos].index, score);
      players[pos].index = 0;
      players[pos].attack = 0;
    }
  }
  printf("\n");
}

void print_player_stats(Player* players, int first, int last, int m, int n, RevStats* rev_stats) {
  int i;
  int num_players = get_num_players(players, first, last, m);
  printf("Final:");
  int cur = rev_stats->rev_last;
  for (i = 0; i < num_players; i++) {
    int pos = (last + m - i) % m;
    int offset = (pos + m - first) % m;
    int revolution_score = calc_rev_score(rev_stats, players[pos].index, &cur, m);
    int score = (m - offset) * (n + 1 - players[pos].index) - revolution_score;
    printf(" %d,%d", players[pos].index, score);
  }
}

int calc_rev_score(RevStats* rev_stats, int player_index, int* cur, int m) {
  int rev_score = 0;
  if (!rev_stats->empty) {
    while (player_index < rev_stats->rev_rounds[*cur]) {
      *cur = (*cur + m - 1) % m;
    }
    if (player_index < rev_stats->rev_rounds[rev_stats->rev_last]) {
      rev_score = rev_stats->rev_sums[rev_stats->rev_last] - rev_stats->rev_sums[*cur] - player_index * ((rev_stats->rev_last - *cur + m) % m);
    }
  }
  return rev_score;
}

void update_rev_stats(RevStats* rev_stats, int index, int m) {
  rev_stats->rev_last = (rev_stats->rev_last + 1) % m;
  if (!rev_stats->empty && rev_stats->rev_first==rev_stats->rev_last) {rev_stats->rev_first = (rev_stats->rev_first + 1) % m;} // move rev_first forward if rev_stats are full
  rev_stats->rev_rounds[rev_stats->rev_last] = index;
  rev_stats->rev_sums[rev_stats->rev_last] = rev_stats->rev_sums[(rev_stats->rev_last - 1 + m) % m] + index;
  rev_stats->empty = 0;
}

// for debugging
void print_rev_stats(RevStats* rev_stats, int m) {
  printf("Rev first: %d, rev last: %d, empty: %d ", rev_stats->rev_first, rev_stats->rev_last, rev_stats->empty);
  printf("Revolution rounds: ");
  int cur = rev_stats->rev_first;
  do {
    printf("%d ", rev_stats->rev_rounds[cur]);
    cur = (cur + 1) % m;
  } while ((cur - 1 + m) % m != rev_stats->rev_last);
  printf("Revolution sums: ");
  cur = rev_stats->rev_first;
  do {
    printf("%d ", rev_stats->rev_sums[cur]);
    cur = (cur + 1) % m;
  } while ((cur - 1 + m) % m != rev_stats->rev_last);
  printf("\n");
}