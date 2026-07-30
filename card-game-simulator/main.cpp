#include <algorithm>
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
}

int main() {
  Deck<Card<Suit, Rank>> myDeck;
  return 0;
}
