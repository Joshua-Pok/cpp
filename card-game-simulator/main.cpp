#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <vector>

enum class Suit { Hearts, Diamonds, Clubs, Spades };
enum class Rank {
  Ace,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  Jack,
  Queen,
  King
};

// 2. Arrays to safely loop through the enums at runtime
const Suit AllSuits[] = {Suit::Hearts, Suit::Diamonds, Suit::Clubs,
                         Suit::Spades};
const Rank AllRanks[] = {Rank::Ace,  Rank::Two, Rank::Three, Rank::Four,
                         Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
                         Rank::Nine, Rank::Ten, Rank::Jack,  Rank::Queen,
                         Rank::King};

template <typename T, typename J> class Card {

public:
  T suit;
  J rank;
  Card(T s, J r) : suit(s), rank(r) {}
};

template <typename T> class Deck {

public:
  std::vector<T> cards;
  void addCard(T item) { cards.push_back(item); };
  std::optional<T> drawCard() {
    if (cards.empty()) {
      return std::nullopt; // nullopt is to indicate std::optional does not have
                           // a value
    } else {
      return cards.pop_back();
    }
  };
  int checkSize() { return cards.size(); };
  void shuffle() {
    std::random_device
        rd; // uniformly-distributed integer random number generator that
            // produces non-deterministic random numbers.
    std::mt19937 rng(rd()); // random number generator that requires a seed
    std::shuffle(cards.begin, cards.end(), rng);
  };
};

class Scoreboard {
public:
  std::map<std::string, int> score;
  Scoreboard() { this->score = {}; }
  void add_player(std::string name) { score.insert({name, 0}); }
  void add_player_score(std::string name, std::optional<int> amount) {
    if (!(score.find(name) != score.end())) {
      std::cout << "Player record does not exist" << std::endl;
      return;
    }

    if (amount != std::nullopt) {
      score[name] +=
          amount.value(); // must use value to extact value from std::optional
    } else {
      score[name]++;
    }
    return;
  }
  void deduct_player_score(std::string name, std::optional<int> amount) {
    if (!(score.find(name) != score.end())) {
      std::cout << "Player record does not exist" << std::endl;
      return;
    }

    if (amount != std::nullopt) {
      score[name] -= amount.value();
    } else {
      score[name]--;
    }
  }

  void print_score() {
    for (const auto &[name, points] : score) {
      std::cout << name << ": " << points << std::endl;
    }
  }
};

int main() {
  Deck<Card<Suit, Rank>> myDeck;
  for (const auto s : AllSuits) {
    for (const auto r : AllRanks) {
      myDeck.cards.push_back(Card(s, r));
    }
  }
  return 0;
}
