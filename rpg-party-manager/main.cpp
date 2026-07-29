#include <iostream>
#include <string>
#include <vector>

class Character {
private:
  std::string name;
  int health;
  int max_health;
  int mana;
  int level;

public:
  Character(std::string name, int max_health);
  ~Character();
  int get_level();
  int get_mana();
  int get_health();
  std::string get_name();
  void set_name(std::string new_name);
  void set_health(int health);
};

Character::Character(std::string name, int health)
    : name(name), health(health) {}

Character::~Character() { std::cout << "Name:" << name; };

int Character::get_level() { return level; };

std::string Character::get_name() { return name; }
int Character::get_mana() { return mana; };
int Character::get_health() { return health; };
void Character::set_name(std::string new_name) { name = new_name; }

void Character::set_health(int health) { health = health; }

void modify_character_name_by_reference(Character &c, std::string new_name) {
  c.set_name(new_name);
  return;
};

void modify_character_name_by_pointer(Character *c, std::string new_name) {
  c->set_name(new_name);
  return;
}

void heal(Character &c, const int heal_amount) {
  int curr_health = c.get_health();
  int new_health = curr_health + heal_amount;

  c.set_health(new_health);

  return;
}

void heal(std::vector<Character> s, const int heal_amount) {
  for (Character &c : s) {
    int curr_health = c.get_health();
    int new_health = curr_health + heal_amount;
    c.set_health(new_health);
  }
}

class Party {
public:
  std::string name;
  std::vector<Character> members;
  void add_character(Character c);
  const void print_members();
  Party(std::string name);
  ~Party();
};

void Party::add_character(Character c) { members.push_back(c); }

Party::~Party() { std::cout << name << "is being destroyed"; }
Party::Party(std::string name) {
  std::cout << "Creating party: " << name;
  name = name;
}

const void Party::print_members() {
  for (Character c : members) {
    std::string name = c.get_name();
    std::cout << name;
  };
}

int main() {
  Party my_party = Party("my_party");

  Character tom = Character("Tom", 100);
  Character dick = Character("dick", 200);
  Character harry = Character("harry", 300);

  my_party.add_character(tom);
  my_party.add_character(dick);
  my_party.add_character(harry);

  tom.set_health(60);
  dick.set_health(160);
  harry.set_health(260);

  heal(tom, 40);
  heal(my_party.members, 40);

  std::cout << tom.get_health();
  std::cout << dick.get_health();
  std::cout << harry.get_health();

  return 0;
}
