#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
class Ship {
protected:
  std::string name;
  int hull_points;
  int energy;
  int firepower;

public:
  static int totalShips;
  Ship(std::string name, int energy, int firepower) {
    this->name = name;
    this->energy = energy;
    this->firepower = firepower;
  }

  virtual void fireWeapon() = 0;
  virtual void specialManuever() = 0;
  virtual ~Ship() = default;

  bool operator==(const Ship &other) const { return name == other.name; }
  bool operator<(const Ship &other) const {
    return firepower < other.firepower;
  }

  friend std::ostream &operator<<(std::ostream &os, const Ship &ship) {
    os << "name" << ship.name << std::endl;
    os << "energy" << ship.energy << std::endl;
    os << "firepower" << ship.firepower << std::endl;
    return os;
  }
};
// friend function is a function that is not a member of the class but is
// granted access to its
//  private and protected members

class Fighter : public Ship {
public:
  int missiles;

  Fighter(std::string name, int energy, int firepower, int missiles)
      : Ship(name, energy, firepower) {
    this->missiles = missiles;
  };

  void fireWeapon() override { std::cout << "Fighter is firing"; };

  void specialManuever() override { std::cout << "backflip"; }
};

class Cruiser : public Ship {
public:
  int something;

  Cruiser(std::string name, int energy, int firepower, int something)
      : Ship(name, energy, firepower) {
    this->something = something;
  }

  void fireWeapon() override { std::cout << "Cruiser is fring"; }

  void specialManuever() override { std::cout << "sidespin"; }
};

class Freighter : public Ship {
public:
  int crew;

  Freighter(std::string name, int energy, int firepower, int crew)
      : Ship(name, energy, firepower) {
    this->crew = crew;
  };

  void fireWeapon() override { std::cout << "Freighter is firing"; }
  void specialManuever() override {
    std::cout << "no special manuever" << std::endl;
  };
};
int Ship::totalShips = 0;

int main() {
  std::vector<std::unique_ptr<Ship>> fleet;

  auto my_fighter = std::make_unique<Fighter>(
      "fightera", 10, 10,
      10); // make_unique creates the object on the heap unique ptr on the stack
  auto my_cruiser = std::make_unique<Cruiser>("cruisera", 20, 20, 20);
  auto my_freighter = std::make_unique<Freighter>("freightera", 30, 30, 30);

  // unique ptrs cannot be copied so we use std::move() to move it around
  //
  fleet.push_back(std::move(my_fighter));
  fleet.push_back(std::move(my_cruiser));
  fleet.push_back(std::move(my_freighter));

  try {
    for (const auto &f : fleet) {
      std::cout << *f << std::endl;
    }

    for (const auto &f : fleet) {
      f->fireWeapon();
    }

  } catch (std::runtime_error) {
    std::cout << "Error firing weapon rip" << std::endl;
  }
  return 0;
};
