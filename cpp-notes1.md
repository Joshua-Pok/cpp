# Notes: C++ Variables, Functions & Classes Refresher

## Variable declarations & fundamental types

**What it is:** C++ is statically typed — every variable has a type fixed at compile time. Core types: `int`, `double`, `float`, `char`, `bool`. `auto` lets the compiler deduce the type from the initializer (still static — it's resolved at compile time, not dynamic typing).

**Syntax:**
```cpp
int level = 1;
double health = 100.0;
bool isAlive = true;
auto mana = 50;        // deduced as int
int score{0};           // brace/uniform initialization (preferred - catches narrowing)
int uninitialized;      // DANGER: holds garbage, not 0
```

**Example:**
```cpp
double baseSpeed = 5.5;
auto speed = baseSpeed * 1.2;  // speed deduced as double
```

**Under the hood:** A local variable like `int level = 1;` lives on the **stack** — a contiguous region of memory that grows/shrinks as functions are entered/exited. The compiler assigns it a fixed offset from the stack frame's base at compile time; "declaring a variable" is really just reserving bytes and giving the compiler a name to map to that address. Unlike Java/Python, there is no hidden initialization — `int uninitialized;` really does contain whatever bits were previously in that memory, which is why C++ has a reputation for undefined-behavior bugs. Brace initialization (`int x{0}`) was added partly to make this class of bug harder to write accidentally, and it also refuses "narrowing" conversions (e.g. `int x{3.5}` is a compile error, `int x = 3.5` is not).

---

## References vs. pointers

**What it is:** Both let you refer to another variable's memory without copying it. A **reference** (`&`) is an alias — it must be bound at creation and can never be null or reseated. A **pointer** (`*`) holds a memory address as its own value — it can be null, reassigned, and used in arithmetic.

**Syntax:**
```cpp
int hp = 100;

int& hpRef = hp;      // reference: hpRef IS hp, just another name
hpRef = 90;             // this changes hp directly

int* hpPtr = &hp;     // pointer: holds the address of hp
*hpPtr = 90;            // dereference to change hp
hpPtr = nullptr;        // pointers can be reassigned or null; references cannot
```

**Example:**
```cpp
void healByRef(int& health) { health += 20; }
void healByPtr(int* health) { *health += 20; }

healByRef(hp);      // call site looks like a normal function call
healByPtr(&hp);     // call site must explicitly take the address
```

**Under the hood:** A reference is, in almost all real compiler implementations, just a pointer under the hood — but the compiler enforces at compile time that it's never null and never reseated, and it automatically dereferences on every use so the syntax reads like a plain variable. A pointer is a variable whose *value* is a memory address (on a 64-bit system, typically 8 bytes, whatever the pointed-to type). `*ptr` means "go to the address stored in ptr and read/write what's there" — this is why dereferencing a null or garbage pointer crashes (segfault) rather than throwing a catchable exception like in higher-level languages: the CPU is literally being told to access memory it doesn't own.

---

## const correctness

**What it is:** `const` is a promise, checked at compile time, that something won't be modified through that particular name/reference/pointer. It applies to variables, function parameters, and (for classes) member functions.

**Syntax:**
```cpp
const int maxHealth = 100;         // variable can't be reassigned

void printName(const std::string& name);   // won't modify name, and won't copy it

class Character {
public:
    int getHealth() const;          // promises not to modify *this
};
```

**Example:**
```cpp
int Character::getHealth() const {
    return health;   // fine — reading is allowed
    // health = 0;   // would fail to compile — const member function can't mutate
}
```

**Under the hood:** `const` is purely a compile-time construct — it costs nothing at runtime and there's no "const bit" stored anywhere. For a const member function, the compiler treats the implicit `this` pointer as `const Character*` instead of `Character*`, so any attempt to write to a member variable through it fails to compile. `const T&` parameters are a very common pattern specifically because they give you read access to a large object (like a `std::string` or `std::vector`) without the cost of copying it — you get pointer-speed passing with value-semantics safety.

---

## Function declarations, overloading & default arguments

**What it is:** A function's *signature* is its name plus parameter types (not the return type). Overloading means multiple functions can share a name if their parameter lists differ — the compiler picks the right one at compile time based on the arguments you pass. Default arguments let a parameter be omitted, falling back to a specified value.

**Syntax:**
```cpp
void heal(int amount);
void heal(int amount, bool isCritical);     // overload — different parameter list
void heal(int amount = 10);                  // default argument
```

**Example:**
```cpp
heal();          // uses default amount = 10
heal(25);        // matches void heal(int)
heal(25, true);  // matches the two-arg overload
```

**Under the hood:** Overload resolution happens entirely at **compile time** — the compiler looks at the argument types at each call site and picks the best-matching overload, then bakes in a direct call to that specific function. This is why it's sometimes called "static polymorphism," as opposed to the runtime polymorphism of virtual functions. Under the hood, each overload actually gets a distinct **mangled name** in the compiled object file (e.g. something like `_Z4heali` vs `_Z4healib`) so the linker can tell them apart — this is also why C++ code can't easily be called from C, which has no overloading and no name mangling.

---

## Pass by value vs. reference vs. pointer

**What it is:** Every function parameter is either a full **copy** (pass by value), an **alias** to the caller's variable (pass by reference), or an **address** of the caller's variable (pass by pointer). This determines whether the function can modify the caller's original data, and how expensive the call is.

**Syntax:**
```cpp
void byValue(Character c);     // copies the whole Character
void byRef(Character& c);      // no copy; can modify caller's object
void byConstRef(const Character& c);  // no copy; read-only
void byPointer(Character* c);  // no copy; can modify; can be nullptr
```

**Example:**
```cpp
Character hero("Hero", 100);
byValue(hero);      // hero unaffected no matter what byValue does
byRef(hero);         // byRef can mutate hero directly
```

**Under the hood:** Pass by value literally invokes the type's **copy constructor** to build a brand-new object on the callee's stack frame — for a small `int` this is nearly free, but for a large class it means duplicating every member, which can be surprisingly expensive if done accidentally (a classic C++ performance footgun). Pass by reference and pass by pointer both just pass an address under the hood (typically 8 bytes on a 64-bit machine) — the difference is purely at the language/compile-time level: references can't be null and don't need explicit `&`/`*` syntax at the call site, while pointers can be null, reassigned, and support pointer arithmetic.

---

## Classes: constructors & destructors

**What it is:** A class bundles data (member variables) and behavior (member functions). A **constructor** runs automatically when an object is created, to establish its initial valid state. A **destructor** runs automatically when an object goes out of scope or is deleted, to clean up (this is the basis of RAII — Resource Acquisition Is Initialization).

**Syntax:**
```cpp
class Character {
public:
    Character(std::string name, int health);   // constructor
    ~Character();                                 // destructor

private:
    std::string name;
    int health;
};

Character::Character(std::string name, int health)
    : name(name), health(health) {}   // member initializer list

Character::~Character() {
    // cleanup, if any
}
```

**Example:**
```cpp
{
    Character hero("Hero", 100);   // constructor runs here
    // ... use hero ...
}   // destructor runs here automatically, when hero goes out of scope
```

**Under the hood:** The **member initializer list** (`: name(name), health(health)`) isn't just style — it's the *only* way to initialize `const` members or reference members, and for class-type members it directly calls their constructor instead of default-constructing then reassigning, which is strictly more efficient. Destructors are what make C++ different from garbage-collected languages: object destruction is deterministic and tied to scope, not to an unpredictable GC pass, which is why C++ resource management (files, memory, locks) is built around "the destructor runs exactly when the object's lifetime ends." Stack-allocated objects (like `hero` above) are destroyed in the reverse order they were constructed, automatically, when the enclosing `{}` block exits.

---

## Access specifiers & encapsulation

**What it is:** `public` members are accessible from outside the class; `private` members are only accessible from within the class's own member functions. This lets a class control how its internal state can be read or changed, rather than letting any code reach in and mutate it directly.

**Syntax:**
```cpp
class Character {
public:
    Character(std::string name, int health);
    int getHealth() const;      // controlled read access
    void takeDamage(int amount); // controlled write access

private:
    std::string name;
    int health;                  // can't be touched directly from outside
};
```

**Under the hood:** Access specifiers are a **compile-time-only** concept — there's no runtime check, no memory difference between a public and private `int`. The compiler simply refuses to compile code outside the class that references a private member; once compiled, the resulting machine code has no notion of "private" at all. `struct` and `class` in C++ are actually the same construct with one difference: `struct` members default to `public`, `class` members default to `private`.

---

## Member functions & the `this` pointer

**What it is:** A member function is a function that operates on a specific object instance. Inside it, `this` is an implicit pointer to the object the function was called on.

**Syntax:**
```cpp
class Character {
public:
    void takeDamage(int amount) {
        health -= amount;          // implicitly this->health
        if (this->health < 0) {    // explicit this, same meaning
            this->health = 0;
        }
    }
private:
    int health;
};
```

**Example:**
```cpp
Character hero("Hero", 100);
hero.takeDamage(30);   // "this" inside takeDamage points at hero
```

**Under the hood:** `this` is not magic — under the hood, every non-static member function is compiled as if it took an extra hidden first parameter: `takeDamage(Character* this, int amount)`. `hero.takeDamage(30)` compiles down to essentially `Character::takeDamage(&hero, 30)`. That's also why const member functions matter at the type-system level: marking a function `const` changes that hidden parameter's type from `Character*` to `const Character*`, which the compiler then enforces.

---

## Supporting pieces you'll also need

These aren't the core concepts this refresher targets, but the project can't be built without them — quick reference so you don't have to look elsewhere.

### `std::string`
**What it is:** The standard library's growable text type (as opposed to a raw `char*` C-string).
**Syntax:**
```cpp
#include <string>
std::string name = "Hero";
name += " the Bold";        // concatenation
name.length();                // size
```
**Under the hood:** `std::string` manages its own heap-allocated buffer and grows it (typically doubling) as needed, similar to a dynamic array. Most implementations also use **small string optimization (SSO)** — short strings (commonly under ~15-22 bytes depending on the standard library) are stored directly inside the string object itself, on the stack, with no heap allocation at all. This is why passing `std::string` by `const&` (see the const-correctness note above) matters: without it, every pass could trigger a full buffer copy.

### `std::vector`
**What it is:** The standard library's growable, contiguous array type — your default container unless you have a specific reason for something else.
**Syntax:**
```cpp
#include <vector>
std::vector<Character> members;
members.push_back(hero);     // add to the end
members.size();                 // current count
members[0];                     // indexed access, no bounds check
members.at(0);                  // indexed access, throws if out of bounds
```
**Under the hood:** A `vector` stores its elements in one contiguous heap-allocated block, which is why indexing (`v[i]`) is a single pointer-arithmetic operation — as fast as a raw array. When it outgrows its current capacity, it allocates a new, larger block (commonly ~1.5x-2x the old size), copies or moves every existing element over, and frees the old block — an operation called reallocation. This is why `push_back` is usually fast but occasionally slow (whenever a reallocation is triggered), and why storing `Character` objects directly (rather than pointers to them) means those copy/move operations touch full character data, not just addresses.

### Range-based `for`
**What it is:** A loop that iterates directly over a container's elements without manual indexing.
**Syntax:**
```cpp
for (const Character& c : members) {   // read-only, no copy
    // use c
}
for (Character& c : members) {          // can modify elements in place
    // modify c
}
```
**Under the hood:** This is syntactic sugar — the compiler rewrites it into the equivalent iterator-based loop (`begin()`/`end()`, incrementing an iterator each pass). Whether you write `const Character&`, `Character&`, or plain `Character` in the loop variable determines pass-by-reference-vs-by-value for each element, exactly as in the function-parameter case above — writing `Character c` (no `&`) silently copies every element on every iteration.

### `<iostream>` (`std::cout`)
**What it is:** The standard library's console output stream.
**Syntax:**
```cpp
#include <iostream>
std::cout << "Health: " << health << "\n";
```
**Under the hood:** `<<` here isn't special syntax — it's the bitwise left-shift operator, **overloaded** for `std::ostream` to mean "insert this value into the stream" instead of "shift these bits." Each call returns a reference to the stream itself, which is exactly what lets you chain multiple `<<` calls in one statement.

