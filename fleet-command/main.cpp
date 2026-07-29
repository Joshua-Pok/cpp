#include <iostream>
#include <memory>
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
    name = name;
    energy = energy;
    firepower = firepower;
  }

  virtual void fireWeapon();
  virtual void specialManuever();
  virtual ~Ship();

  bool operator==(const Ship &other) const { return name == other.name; }
  bool operator<(const Ship &other) const {
    return firepower < other.firepower;
  }
  void operator<<() const {
    std::cout << "name: " << name << std::endl;
    std::cout << "energy: " << energy << std::endl;
    std::cout << "firepower: " << firepower << std::endl;
  }
};

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
    something = something;
  }

  void fireWeapon() override { std::cout << "Cruiser is fring"; }

  void specialManuever() override { std::cout << "sidespin"; }
};

class Freighter : public Ship {
public:
  int crew;

  Freighter(std::string name, int energy, int firepower, int crew)
      : Ship(name, energy, firepower) {
    crew = crew;
  };

  void fireWeapon() override { std::cout << "Freighter is firing"; }
};
int Ship::totalShips = 0;

int main() {
  std::vector<std::unique_ptr<Ship>> fleet;

  fleet.push_back() return 0;
};
