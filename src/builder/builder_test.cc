#include <gtest/gtest.h>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace primitive_builder {

struct HtmlElement {
  std::string name, text;
  std::vector<HtmlElement> elements;

  HtmlElement() = default;

  explicit HtmlElement(std::string name_, std::string text_)
    : name(std::move(name_)), text(std::move(text_))
  {}

  [[nodiscard]] std::string str(int indent = 0) const {
    std::stringstream oss;

    std::string res = " <" + name;
    for (const auto &elem : elements)
      oss << elem.str();


    return res;
  }
};

void test() {
  std::string words[] = {"hello", "world"};
  HtmlElement list{"ul", ""};

  for (const auto& w : words)
    list.elements.emplace_back("li", w);
  printf("%s", list.str().c_str());
}

struct HtmlBuilder {
  HtmlElement root;

  explicit HtmlBuilder(std::string root_name) { root.name = std::move(root_name); };

  HtmlBuilder &add_child(std::string child_name, std::string child_next) {
    root.elements.emplace_back(std::move(child_name), std::move(child_next));
    return *this;
  }

  [[nodiscard]] std::string str() const { return root.str(); }
};

void test2() {
  HtmlBuilder builder{"ul"};
  builder.add_child("li", "hello").add_child("li", "world");
  std::cout << builder.str() << std::endl;
  // You'll notice that, at the moment, the `add_child()` function is
  // void-returning. There are many things we could use the return value for,
  // but one
}

}

namespace html_element_with_builder {

struct HtmlBuilder;

struct HtmlElement {
  std::string name, text;
  std::vector<HtmlElement> elements;

  static std::unique_ptr<HtmlBuilder> create(const std::string &root_name) {
    return std::make_unique<HtmlBuilder>(root_name);
  }

  [[nodiscard]] std::string str(int indent = 0) const {
    // pretty-print the contents
    return "";
  }

protected:

  HtmlElement() = default;
  HtmlElement(std::string name_, std::string text_)
      : name(std::move(name_))
      , text(std::move(text_))
  {}

  friend struct HtmlBuilder;
};

struct HtmlBuilder {
  HtmlElement root;

  explicit HtmlBuilder(std::string root_name) { root.name = std::move(root_name); }

  HtmlBuilder &add_child(std::string child_name, std::string child_text) {
    root.elements.emplace_back(HtmlElement{std::move(child_name), std::move(child_text)});
    return *this;
  }

  operator HtmlElement() const { return root; }

  [[nodiscard]] std::string str() const { return root.str(); }

  HtmlElement &build() { return root; }
};

void test() {
  HtmlElement e = HtmlElement::create("ul")->add_child("li", "hello")
                      .add_child("li", "world");
  std::cout << e.str() << std::endl;
}

}

TEST(builder_pattern_test, test1) {
  primitive_builder::test();
}

TEST(builder_pattern_test, test2) {
  html_element_with_builder::test();
}

namespace groovy_feature_builder {

struct Tag {
  std::string name;
  std::string text;
  std::vector<Tag> children;
  std::vector<std::pair<std::string, std::string>> attributes;

  friend std::ostream &operator<<(std::ostream &os, const Tag &tag) {
    // implementation omitted.
    return os;
  }

protected:
  Tag(std::string name_, std::string text_)
    : name{std::move(name_)}
    , text{std::move(text_)}
  {}

  Tag(std::string name_, const std::vector<Tag> &children_)
    : name{std::move(name_)}
    , children{children_}
  {}
};

struct P : Tag {
  explicit P(std::string text_)
    : Tag{"p", std::move(text_)}
  {}

  P(std::initializer_list<Tag> children)
    : Tag("p", children)
  {}
};

struct IMG : Tag {
  explicit IMG(std::string url)
    : Tag{"img", ""}
  {
    attributes.emplace_back("src", std::move(url));
  }

};

class PersonAddressBuilder;
class PersonJobBuilder;
class PersonBuilder;

class Person {
  friend class PersonAddressBuilder;
  friend class PersonJobBuilder;
  friend class PersonBuilder;

  // address
  std::string street_address, post_code, city;

  // employment
  std::string company_name, position;
  int annual_income = 0;

public:
  Person() = default;

  static PersonBuilder create();

  friend std::ostream &operator<<(std::ostream &os, const Person &p) {
    os << "[\n\t{street_address: " << p.street_address << "}\n"
       << "\t{post_code: " << p.post_code << "}\n"
       << "\t{city: " << p.city << "}\n"
       << "\t{company_name: " << p.company_name << "}\n"
       << "\t{position: " << p.position << "}\n"
       << "\t{annual_income: " << p.annual_income << "}\n]\n";
    return os;
  }
};

class PersonBuilderBase {
protected:
  Person &person;

  explicit PersonBuilderBase(Person &person_)
    : person{person_} {}

public:
  operator Person() { return std::move(person); }

  // builder facets
  [[nodiscard]] PersonAddressBuilder lives() const;
  [[nodiscard]] PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase {
  Person p; // object being built

public:
  PersonBuilder() : PersonBuilderBase{p} {}
};

PersonBuilder Person::create() { return PersonBuilder{}; }

class PersonAddressBuilder : public PersonBuilderBase {
  using self = PersonAddressBuilder;

public:
  explicit PersonAddressBuilder(Person &person) : PersonBuilderBase{person} {}

  self &at(std::string street_address) {
    person.street_address = std::move(street_address);
    return *this;
  }

  self &with_postcode(std::string post_code) {
    person.post_code = std::move(post_code);
    return *this;
  }

  self &in(std::string city) {
    person.city = std::move(city);
    return *this;
  }
};

class PersonJobBuilder : public PersonBuilderBase {
  using self = PersonJobBuilder;

public:
  explicit PersonJobBuilder(Person &person) : PersonBuilderBase{person} {}

  self &at(std::string company_name) {
    person.company_name = std::move(company_name);
    return *this;
  }

  self &as_a(std::string position_str) {
    person.position = std::move(position_str);
    return *this;
  }

  self &earning(int income) {
    person.annual_income = income;
    return *this;
  }
};

PersonAddressBuilder PersonBuilderBase::lives() const {
  return PersonAddressBuilder{person};
}

PersonJobBuilder PersonBuilderBase::works() const {
  return PersonJobBuilder{person};
}

class MailService {

  class Email {
  public:
    std::string from, to, subject, body;

    // possibly other members here
  };

  class EmailBuilder {
    Email &email;

  public:
    explicit EmailBuilder(Email &email) : email{email} {}

    EmailBuilder &from(std::string from) {
      email.from = std::move(from);
      return *this;
    }
  };

};

}

TEST(builder_pattern, test1) {
  using namespace groovy_feature_builder;

  Person p = Person::create()
                 .lives().at("123 London Road")
                         .with_postcode("SW1 1GB")
                         .in("London")
                 .works().at("PragmaSoft")
                         .as_a("Consultant")
                         .earning(10e6);

  std::stringstream oss;
  testing::internal::CaptureStdout();

  std::cout << p;

  auto act_output = testing::internal::GetCapturedStdout();

  oss << "[\n"
         "\t{street_address: 123 London Road}\n"
         "\t{post_code: SW1 1GB}\n"
         "\t{city: London}\n"
         "\t{company_name: PragmaSoft}\n"
         "\t{position: Consultant}\n"
         "\t{annual_income: 10000000}\n"
         "]\n";

  EXPECT_EQ(oss.str(), act_output);
}

namespace builder_inheritance {

class Person {
public:
  std::string name, position, date_of_birth;

  friend std::ostream &operator<<(std::ostream &os, const Person &obj) {
    return os << "name: " << obj.name
              << " position: " << obj.position
              << " date_of_birth: " << obj.date_of_birth;
  }
};

class PersonBuilder {
protected:
  Person person;

public:
  [[nodiscard]] Person build() const { return person; }
};

template <typename TSelf>
class PersonInfoBuilder : public PersonBuilder {
public:
  TSelf &called(const std::string &name) {
    person.name = name;
    return static_cast<TSelf&>(*this);
    // alternatively, *static_cast<TSelf*>(this)
  }
};

template <typename TSelf>
class PersonJobBuilder : public PersonInfoBuilder<PersonJobBuilder<TSelf>> {
public:
  TSelf &works_as(const std::string &position) {
    this->person.position = position;
    return static_cast<TSelf&>(*this);
  }
};

template <typename TSelf>
class PersonBirthDateBuilder : public PersonJobBuilder<PersonBirthDateBuilder<TSelf>> {
public:
  TSelf &born_on(const std::string &date_of_birth) {
    this->person.date_of_birth = date_of_birth;
    return static_cast<TSelf&>(*this);
  }
};

class MyBuilder : public PersonBirthDateBuilder<MyBuilder> {};

} // namespace builder_inheritance

TEST(builder_inheritance_test, test1) {
  namespace bi = builder_inheritance;

  std::stringstream oss;
  testing::internal::CaptureStdout();

  bi::MyBuilder mb;

  auto me = mb.called("Dmitri")
              .works_as("Programmer")
              .born_on("01/01/1980")
              .build();

  std::cout << me << std::endl;

  auto act_output = testing::internal::GetCapturedStdout();

  oss << "name: Dmitri position: Programmer date_of_birth: 01/01/1980\n";

  EXPECT_EQ(oss.str(), act_output);
}
