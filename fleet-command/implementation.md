# Project: Fleet Command

## What you're building

A small space fleet simulator. An abstract `Ship` base class defines what every ship must be able to do; `Fighter`, `Cruiser`, and `Freighter` each implement it differently. You'll run a combat round where ships fire through base-class pointers (polymorphism doing the work), sort/compare the fleet using operators you overload yourself, and deliberately trigger — and gracefully handle — an exception when a ship tries to fire without enough energy.

## Concepts you'll practice

- Inheritance — Steps 1, 2
- Virtual functions & dynamic dispatch — Steps 2, 5
- Abstract classes & pure virtual functions — Step 1
- Static members — Step 1
- Operator overloading — Step 3
- Smart pointers (`std::unique_ptr`) — Step 4
- Exception handling — Steps 2, 5

## Steps

### Step 1: Design the abstract `Ship` base class
- Declare `Ship` with `protected` members for at least: name, hull points, energy, and firepower.
- Add a constructor that initializes all of them.
- Declare `fireWeapon()` as pure virtual — `Ship` should not be instantiable on its own.
- Declare `specialManeuver()` as virtual as well (not necessarily pure — decide whether it makes sense to give it a default no-op behavior in the base class, or force every subclass to define one).
- Add a virtual destructor. Think back to the notes on *why* this matters before moving on — you'll be deleting derived objects through base-class pointers later.
- Add a `static` member tracking the total number of ships ever constructed, incremented in the constructor. Remember it needs a definition outside the class, not just inside.

### Step 2: Implement three derived ship types
- Create `Fighter`, `Cruiser`, and `Freighter`, each inheriting from `Ship` with a constructor that sets reasonable, *different* starting stats per te a `std::vector<std::unique_ptr<Ship>>` to represent the fleet.
- Populate it with a mix of Fighters, Cruisers, and Freighters using `std::make_unique`, so ownership is automatic — no manual `new`/`delete` anywhere.
- Iterate the fleet with a range-based `for` loop and print every ship using your `operator<<`, confirming the right derived-class data shows up for each one even though you're only holding `Ship` pointers.
- Sort the fleet by firepower using your `operator<` (check what the standard library gives you for sorting a container, and whether it needs anything extra to work with `unique_ptr` elements specifically).
 range-based `for` loop and print every ship using your `operator<<`, confirming the right derived-class data shows up for each one even though you're only holding `Ship` pointers.
- Sort the fleet by firepower using your `operator<` (check what the standard library gives you for sorting a container, and whether it needs anything extra to work with `unique_ptr` elements specifically).
### Step 3: Overload comparison and printing operators on `Ship`
- Overload `operator==` to compare two ships (decide what makes two ships "equal" — name is a reasonable choice).
- Overload `operator<` to compare ships by firepower, so a collection of ships becomes sortable.
- Overload `operator<<` (as a free function, not a member — the notes explain why) to print a ship's name and key stats in one line.
- These should work correctly no matter which derived type the `Ship` actually is — verify this once Step 4 gives you a mixed fleet to test against.

### Step 4: Build the fleet with smart pointers
- Declare a `std::vector<std::unique_ptr<Ship>>` to represent the fleet.
- Populate it with a mix of Fighters, Cruisers, and Freighters using `std::make_unique`, so ownership is automatic — no manual `new`/`delete` anywhere.
- Iterate the fleet with a range-based `for` loop and print every ship using your `operator<<`, confirming the right derived-class data shows up for each one even though you're only holding `Ship` pointers.
- Sort the fleet by firepower using your `operator<` (check what the standard library gives you for sorting a container, and whether it needs anything extra to work with `unique_ptr` elements specifically).


### Step 5: Run a combat round with exception handling
- Loop over the fleet and call `fireWeapon()` on every ship polymorphically.
- Wrap each call (or the whole loop — decide which granularity makes more sense and think about the tradeoff) in a `try`/`catch` so that a ship without enough energy doesn't crash rately construct at least one ship with energy too low to fire, so you actually exercise the throw/catch path raurally at the end of `main()` and add a destructor print statement (in `Ship` or each derived class) so you can watch every ship — including the ones that failed to fire — get properly cleaned up with no manual `delete` anywhere in your codcompiles cleanly with `-Wall` and no warnings about missing overrides or virtual destructors.
- Running it prints a mixed fleet roster (via your `operator<<`), shows the fleet correctly sorted by firepower, runs a combat round where at least one ship succeeds and at least one throws-and-is-caught without crashing the program, and ends with destructor messages for every ship.
- You never wrote a manual `new`/`delete` pair — ownership is handled entirely by `unique_ptr`.
e.

## Done when

- The program across every instance rather than resetting.
- Let the fleet go out of scope natcouple of ships too, printing the result.

### Step 6: Wrap up and report
- Print the total ships built using the static counter, both partway through and at the end, to confirm it's tracking ther than just writing code that could theoretically handle it.
- Call `specialManeuver()` on a the whole program — catch the exception, print a message using `.what()`, and continue.
- Delibemparison and printing operators on `Ship`
- Overload `operator==` to compare two ships (decide what makes two ships "equal" — name is a reasonable choice).
- Overload `operator<` to compare ships by firepower, so a collection of ships becomes sortable.
- Overload `operator<<` (as a free function, not a member — the notes explain why) to print a ship's name and key stats in one line.
- These should work correctly no matter which derived type the `Ship` actually is — verify this once Step 4 gives you a mixed fleet to test against.

### Step 4: Build the fleet with smart pointers
- Declarype (e.g. a Fighter might have low hull/high firepower, a Freighter the reverse).
- Override `fireWeapon()` in each with genuinely different behavior — not just different numbers, different *logic*. At least one type's `fireWeapon()` should check whether there's enough energy and `throw` a `std::runtime_error` if not.
- Override `specialManeuver()` in at least two of the three types with something distinct (e.g. a Fighter might have an evasive maneuver, a Cruiser a shield boost).
- Mark every override with the `override` keyword.

### Step 3: Overload co
