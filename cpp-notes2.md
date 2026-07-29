# Notes: C++ Level 2 — Inheritance, Polymorphism, Operators, Smart Pointers, Exceptions

## Inheritance

**Wh though s's static type is Ship*
```

**Under the hood:** Each class with virtual functions gets a compiler-generated **vtable** (virtual function table) — essentially an array of function pointers, one per virtual function. Every object of that class carries a hidden pointer (the **vptr**) to its class's vtable, set automatically by the constructor. A virtual call like `s->fireWeapon()` compiles down to "look up `fireWeapon`'s slot in the vtable *this object's vptr points to*, and call whatever's there" — an extra pointer indirection compared to a normal function call, which is the (usually negligible) performance cost of polymorphism. **Always give a base class with virtual functions a virtual destructor** — without it, `delete`-ing a derived object through a base pointerat it is:** A class can derive from another, inheriting its members and extending or specializing them. The base class defines shared structure; derived classes add or override behavior. `protected` members are like `private` (invisible outside the class) except derived classes *can* access them directly.

**Syntax:**
```cpp
class Ship {
protected:  only runs the base class's destructor, silently skipping the derived class's cleanup.

---

## Abstract classes & pure virtual functions

**What it is:** A pure virtual function (`= 0`) has no implementation in the base class and *forces* every derived class to provide one. A class with at least one pure virtual function is abstract — it can't be instantiated directly, only through a derived class.

**Syntax:**       eapon()
```

**Under the hood:** In vtable terms, a pure virtual function's slot is typically filled with a null pointer (or a pointer to a trap function) in the base class's vtable. The compiler refuses to let you construct an object of a class whose vtable would have any unfilled pure-virtual slots — which is a compile-time check, not a runtime one. This is how C++ expresses an "interface": the abstract base class defines *what* every ship must be able to do; each derived class defines *how*.

---

## Static members
                         ember function can be called without any instance at all, and can only access other static members.

**Syntax:**
```cpp
class Ship {
public:
    static int totalShipsBuilt;   // declaration inside the class

    Ship() { totalShipsBuilt++; }
};

int Ship::totalShipsBuilt = 0;    // definition, once, outside the class (required!)
```

**Example:**
```cpp
Fighter a("Alpha"), b("Beta");
std::cout << Ship::totalShipsBuilt;   // 2 — called on the class, not an instance
```

**Under the hood:** Unlike ordinary member variables, a static member doesn't live inside any individupublic:
    int firepower;                                                                               egin(), fleet.end());  // works automatically once operator< exists
```

**Under the hood:** `a == b` for a class type is really just syntactic sugar for a funrs to add member functions to). The compiler picks whichever overload matches the operand types, using the same overload-resolution rules as any other function overload. There's no special "operator magic" beyond this — you're just naming a function `operator==` instead of `equals`, and the compiler translates the infix syntax into a call to it.

---
                                                                           `) had to be manually `delete`d — miss one and you leak memory; `delete` twice and you get undefined behavior; forget who "owns" a pointer and you get dangling references. `std::unique_ptr` wraps a raw pointer and automatically deletes what it owns when the `unique_ptr` itself goes out of scope — ownership is exclusive (it can't be copied, only *moved*).

**Syntax:**
```cpp                                                           s out of scope

std::vector<std::unique_ptr<Ship>> fleet;
fleet.push_back(std::make_unique<Fighter>("Interceptoreanup is tied to the wrapper object's scope, not to you remembering to write `delete`. Because copying a `unique_ptr` is disabled (the compiler deletes that constructor), the only way to transfer ownership is `std::move`, which hands over the raw pointer and leaves the source `unique_ptr` null — there is never a moment where two `unique_ptr`s think they own the same object, which is precisely the bug class it exists to prevent.
                                                 echanism for signaling and responding to error conditions that a function can't
---
                                            reasonably handle on the spot. `throw` raises an exception; execution immediately unwinds up the call stack until a matching `catch` block is found.

**Syntax:**
```cpp                           {
    std::cout << "Action fai`cpp
try {
    fleet[0]->fireWeapon();   // might throw if energy is too low
} catch (const sts any local objects that were fully constructed (this is why RAII and exceptions are designed to work together: a `unique_ptr` on the stack still gets cleaned up correctly even if an exception blows through the function that owns it). This continues until it finds a `catch` block whose type matches the thrown object, at which point execution resumes there. If no matching `catch` exists anywhere up the call stack, the program calls `std::terminate` and aborts — which is why production code generally wants at least one catch-all near the top of the call stack (e.g. in `main`).
                t auto& shipPtr : fleet) {
    std::cout <t.
o get the Ship, then use operator<<
}
```
**Under the hood:** `auto` here deduces to `const std::unique_ptr<Ship>&` — you're iterating references to the smart pointers themselves (never copying one, since `unique_ptr` can't be copied), then dereferencing (`*shipPtr`) to reach the underlying `Ship` objec< *shipPtr << "\n";   // dereference t::runtime_error` and `std::exception`
**What it is:** `std::runtime_error` is a standard library class representing a runtime error, with a message accessible via `.what()`. It's one of several classes derived from the common base `std::exception`, which is why `catch (const std::exception& e)` can catch it (and most other standard exceptions) polymorphically — same dynamic-dispatch mechanism as your `Ship` hierarchy.

### `std::vector<std::unique_ptr<T>>` iteration
**What it is:** Iterating a vector of smart pointers with range-based `for`.
**Syntax:**
```cpp
for (cons it is:** An optional but strongly recommended keyword on a derived class's function, asserting "this is meant to override a virtual function from the base class."
**Syntax:** `void fireWeapon() override { ... }`
**Under the hood:** `override` has zero runtime effect — it's a compile-time-only safety check. Without it, a typo like `fireWeapon` vs `FireWeapon`, or a mismatched parameter list, silently creates a brand-new, unrelated function instead of overriding — and you'd get no warning that polymorphism isn't working. With `override`, that same typo becomes a compile error.

### `std
---

## Supporting pieces you'll also need

### `override` keyword
**Whatd::exception& e) {
    std::cout << e.what() << "\n";   // program keeps running instead of crashing
}
```

**Under the hood:** When `throw` executes, the runtime immediately begins **stack unwinding** — it walks back up through every function call currently in progress, and for each one, properly destroyled: " << e.what() << "\n";
}
```

**Example:**
``
#include <stdexcept>

void Fighter::fireWeapon() {
    if (energy < requiredEnergy) {
        throw std::runtime_error(name + " doesn't have enough energy to fire!");
    }
    energy -= requiredEnergy;
    // ... firing logic ...
}

// at the call site:
try {
    someShip->fireWeapon();
} catch (const std::runtime_error& e)
## Exception handling

**What it is:** A m"));
fleet.push_back(std::make_unique<Cruiser>("Dreadnought"));

for (const auto& ship : fleet) {
    ship->fireWeapon();     // polymorphic call through the smart pointer
}
```

**Under the hood:** `unique_ptr` is a thin wrapper class around a raw pointer, with essentially zero runtime overhead — its destructor simply calls `delete` on the pointer it holds, which is exactly the RAII pattern from the constructors/destructors note in your last refresher: the cl
#include <memory>

std::unique_ptr<Ship> s = std::make_unique<Fighter>("Interceptor");
s->fireWeapon();          // use it like a normal pointer
// no delete needed — happens automatically when s goe
## Smart pointers (`std::unique_ptr`)

**What it is:** Before smart pointers, dynamically allocated objects (`new Fighter(...)ction call — either `a.operator==(b)` (member form) or `operator==(a, b)` (free-function form, which is why `<<` is written outside the class: `std::cout << ship` needs the stream on the *left*, but the stream class isn't you
    std::string name;

    bool operator==(const Ship& other) const {
        return name == other.name;
    }
    bool operator<(const Ship& other) const {
        return firepower < other.firepower;
    }
};

std::ostream& operator<<(std::ostream& out, const Ship& s) {
    out << s.name << " (firepower: " << s.firepower << ")";
    return out;    // enables chaining
}
```

**Example:**
```cpp
if (fighterA == fighterB) { ... }
if (fighterA < cruiser) { ... }        // uses operator<
std::cout << fighterA << "\n";          // uses operator<<
std::sort(fleet.bal object's memory — it lives in one fixed location, similar to a global variable, and every instance's access to it resolves to that same address. This is exactly why it needs a separate out-of-class definition (`int Ship::totalShipsBuilt = 0;`): the in-class line is just a declaration saying "this static variable exists"; something, somewhere, has to actually allocate storage for it once.

---

## Operator overloading

**What it is:** C++ lets you define what built-in operators (`==`, `<`, `<<`, `+`, etc.) mean for your own types, so objects can be compared or printed with natural syntax instead of named methods.

**Syntax:**
```cpp
class Ship {

**What it is:** A `static` member variable is shared across *all* instances of a class — there's exactly one copy, not one per object. A `static` m
```cpp
class Ship {
public:
    virtual void fireWeapon() = 0;   // pure virtual — no body, no default
    virtual ~Ship() {}
};

// Ship s(...);        // ERROR: cannot instantiate abstract class
Fighter f("Interceptor");  // fine — Fighter provides fireW
    std::string name;
    int hull;
    int energy;

public:
    Ship(std::string name, int hull, int energy)
        : name(name), hull(hull), energy(energy) {}
};

class Fighter : public Ship {
public:
    Fighter(std::string name)
        : Ship(name, /*hull*/ 50, /*energy*/ 100) {}   // calls base constructor
};
```

**Example:**
```cpp
Fighter f("Interceptor");   // Fighter's constructor runs, which runs Ship's constructor first
```

**Under the hood:** A derived object physically *contains* its base subobject as a prefix of its memory layout — a `Fighter` in memory starts with all of `Ship`'s members, then adds `Fighter`'s own. Construction always runs base-first, derived-second (you can see this by adding print statements to each constructor); destruction runs in the reverse order, derived-first, base-second. `public` inheritance (`class Fighter : public Ship`) means "a Fighter *is-a* Ship" for the type system — a `Fighter*` can be used anywhere a `Ship*` is expected, which is the foundation polymorphism (next section) is built on.

---

## Virtual functions & dynamic dispatch

**What it is:** By default, which function gets called is decided at compile time based on the *static* type of the pointer/reference you're using. `virtual` changes that: the call is resolved at runtime based on the object's *actual* type. This is what lets a `Ship*` pointing at a `Fighter` correctly call `Fighter`'s version of a function.

**Syntax:**
```cpp
class Ship {
public:

kkkk
    virtual void fireWeapon() { /* default behavior, if any */ }
    virtual ~Ship() {}   // virtual destructor — see note below
};

class Fighter : public Ship {
public:
    void fireWeapon() override {   // 'override' catches typos/signature mismatches at compile time
        // Fighter-specific firing behavior
    }
};
```

**Example:**
```cpp
Ship* s = new Fighter("Interceptor");
s->fireWeapon();   // calls Fighter::fireWeapon, even
