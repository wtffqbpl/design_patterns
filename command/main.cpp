#include <iostream>
#include <vector>

struct walk {
  void operator()() {
    std::cout << "walk" << std::endl;
  }
};

struct jog {
  void operator()() {
    std::cout << "jog" << std::endl;
  }
};

struct run {
  void operator()() {
    std::cout << "run" << std::endl;
  }
};

struct macroCommand : std::vector<std::function<void()>> {
  void operator()() {
    for (auto &&f : *this)
      f();
  }
};

void doIt(std::function<void()> f) {
  f();
}

int main() {
  doIt(walk{});
  doIt(jog{});
  doIt(run{});

  macroCommand cardio_workout;
  cardio_workout.push_back(walk{});
  cardio_workout.push_back(jog{});
  cardio_workout.push_back(run{});
  cardio_workout();

  return 0;
}

