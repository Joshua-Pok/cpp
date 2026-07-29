#include <iostream>
#include <string>
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
};

class Fighter : public Ship {
public:
  int missiles;

  Fighter(std::string int)
}

int Ship::totalShips = 0;

int main() { return 0; };
