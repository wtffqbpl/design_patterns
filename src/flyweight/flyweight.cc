#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace flyweight {
/**
 * Flyweight Design Pattern
 *
 * Intent: Lets you fit more objects into the available amount of RAM by sharing
 * common parts of state between multiple objects, instead of keeping all of the
 * data in each object.
 * 一般认为这部分内容不会发生变化，这样的话，就可以多个对象共享该部分内容。
 **/

struct SharedState {
  std::string brand_;
  std::string model_;
  std::string color_;

  SharedState(std::string brand, std::string model, std::string color)
      : brand_(std::move(brand)), model_(std::move(model)),
        color_(std::move(color)) {}

  friend std::ostream &operator<<(std::ostream &os, const SharedState &ss) {
    return os << "[" << ss.brand_ << ", " << ss.model_ << ", " << ss.color_
              << "]";
  }
};

struct UniqueState {
  std::string owner_;
  std::string plates_;

  UniqueState(std::string owner, std::string plates)
      : owner_(std::move(owner)), plates_(std::move(plates)) {}

  friend std::ostream &operator<<(std::ostream &os, const UniqueState &us) {
    return os << "[" << us.owner_ << " , " << us.plates_ << "]";
  }
};

/**
 * The Flyweight stores a common portion of the state (also called intrinsic
 * state) that belongs to multiple real business entities. The Flyweight accepts
 * the rest of the state (extrinsic state, unique for each entity) via its
 * method parameters.
 *
 */

class Flyweight {
private:
  SharedState *shared_state_;

public:
  Flyweight(const SharedState *shared_state)
      : shared_state_(new SharedState(*shared_state)) {}

  Flyweight(const Flyweight &other)
      : shared_state_(new SharedState(*other.shared_state_)) {}

  ~Flyweight() { delete shared_state_; }

  [[nodiscard]] SharedState *shared_state() const { return shared_state_; }

  void Operation(const UniqueState &unique_state) const {
    std::cout << "Flyweight: Displaying shared (" << *shared_state_
              << ") and unique (" << unique_state << ") state.\n";
  }
};

/**
 * Teh Flyweight Factory creates and manages the Flyweight objects. It ensures
 * that flyweights are shared correctly. When the client requests a flyweight,
 * the factory either returns an existing instance or creates a new one, if it
 * doesn't exist yet.
 */

class FlyweightFactory {
  /**
   * @var Flyweight[]
   */
private:
  std::unordered_map<std::string, Flyweight> flyweights_;

  /**
   * Returns a Flyweight's string hash for a given state.
   */
  std::string GetKey(const SharedState &ss) const {
    return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
  }

public:
  FlyweightFactory(std::initializer_list<SharedState> share_states) {
    for (const SharedState &ss : share_states)
      this->flyweights_.insert(std::make_pair<std::string, Flyweight>(
          this->GetKey(ss), Flyweight(&ss)));
  }

  Flyweight GetFlyweight(const SharedState &shared_state) {
    std::string key = this->GetKey(shared_state);
    if (this->flyweights_.find(key) == this->flyweights_.end()) {
      std::cout
          << "FlyweightFactory: Can't fidn a flyweight, creating new one.\n";
      this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
    } else {
      std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
    }

    return this->flyweights_.at(key);
  }

  void ListFlyweights() const {
    size_t count = this->flyweights_.size();
    std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
    for (std::pair<std::string, Flyweight> pair : this->flyweights_)
      std::cout << pair.first << std::endl;
  }
};

void AddCarToPoliceDatabase(FlyweightFactory &ff, const std::string &plates,
                            const std::string &owner, const std::string &brand,
                            const std::string &model,
                            const std::string &color) {
  std::cout << "\nClient: Adding a car to database." << std::endl;
  const Flyweight &flyweight = ff.GetFlyweight({brand, model, color});
  // The client code either stores or calculates extrinsic state and passes it
  // to the flyweight's methods.
  flyweight.Operation({owner, plates});
}

} // end of namespace flyweight

/**
 * The client code usually creates a bunch of pre-populated flyweights in the
 * initialization stage of teh application.
 */

TEST(flyweight, basic_demo){
  using namespace flyweight;

  auto *factory = new FlyweightFactory({
      {"Chevrolet", "Camaro2018", "pink"},
      {"Mercedes Benz", "C300", "black"},
      {"Mercedes Benz", "C500", "red"},
      {"BMW", "M5", "red"},
      {"BMW", "X6", "white"} });
  factory->ListFlyweights();

  AddCarToPoliceDatabase(*factory,
                         "CL234IR",
                         "james Doe",
                         "BMW",
                         "M5",
                         "red");

  AddCarToPoliceDatabase(*factory,
                         "CL234IR",
                         "James Doe",
                         "BMW",
                         "X1",
                         "red");

  factory->ListFlyweights();

  delete factory;
}

