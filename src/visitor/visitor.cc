#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <unordered_map>

namespace visitor {
/**
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */

class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
public:
  virtual void VisitConcreteComponentA(const ConcreteComponentA *element) const = 0;
  virtual void VisitConcreteComponentB(const ConcreteComponentB *element) const = 0;

  virtual ~Visitor() = default;
};

/**
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */
class Component {
public:
  virtual void Accept(Visitor *visitor) const = 0;

  virtual ~Component() = default;
};

/**
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */
class ConcreteComponentA : public Component {
  /**
   * Not that we're calling `visitConcreteComponentA`, which matches the
   * current class name. This way we let the visitor know the class of the
   * component it works with.
   */
public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }

  /**
   * Concrete Components may have special methods that don't exist in their base
   * class or interface. The Visitor is still able to use these methods since
   * it's aware of the component's concrete class.
   */
  [[nodiscard]] std::string ExclusiveMethodOfConcreteComponentA() const {
    return "A";
  }
};

class ConcreteComponentB : public Component {
  /**
   * Same here: visitConcreteComponentB => ConcreteComponentB
   */
public:
  void Accept(Visitor *visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }
  [[nodiscard]] std::string SpecialMethodOfConcreteComponentB() const {
    return "B";
  }
};

/**
 * Concrete Visitors implement several versions of the same algorithm, which can
 * work with all concrete component classes.
 *
 * You can experience the biggest benefit of the Visitor pattern when using it
 * with a complex object structure, such as a Composite tree. In this case, it
 * might be helpful to store some intermediate state of the algorithm while
 * executing visitor's methods over various objects of the structure.
 */
class ConcreteVisitor1 : public Visitor {
public:
  void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
public:
  void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
  }

  void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
  }
};

/**
 * The Client code can run visitor operations over any set of elements without
 * figuring out their concrete classes. The accept operation directs a call to
 * the appropriate operation in the visitor object.
 */
void ClientCode(std::array<const Component *, 2> components, Visitor *visitor) {

  for (const Component *comp : components) {
    comp->Accept(visitor);
  }
}

} // end of namespace visitor

TEST(visitor, basic_demo) {
  using namespace visitor;

  std::array<const Component *, 2> components = {
                                  new ConcreteComponentA,
                                  new ConcreteComponentB};
  std::cout << "the client code works with all visitors via the base Visitor interface:\n";
  auto *visitor1 = new ConcreteVisitor1;
  ClientCode(components, visitor1);

  std::cout << "\nIt allows the same client code to work with different types of visitors:\n";

  auto *visitor2 = new ConcreteVisitor2;
  ClientCode(components, visitor2);

  for (const Component *comp : components) {
    delete comp;
  }

  delete visitor1;
  delete visitor2;
}

namespace visitor_factory {

class ResourceFile {
protected:
  std::string file_path_;

public:
  enum class ResourceFileType : uint8_t { PDF, PPT, WORD };

public:
  explicit ResourceFile(std::string file_path) : file_path_(std::move(file_path)) {}
  [[nodiscard]] virtual ResourceFileType getType() const = 0;
};

class PdfFile : public ResourceFile {
public:
  explicit PdfFile(std::string file_path) : ResourceFile(std::move(file_path)) {}
  [[nodiscard]] ResourceFileType getType() const override {
    return ResourceFileType::PDF;
  }
};

class PPTFile : public ResourceFile {
public:
  explicit PPTFile(std::string file_path) : ResourceFile(std::move(file_path)) {}
  [[nodiscard]] ResourceFileType getType() const override {
    return ResourceFileType::PPT;
  }
};

class WordFile : public ResourceFile {
public:
  explicit WordFile(std::string file_path) : ResourceFile(std::move(file_path)) {}
  [[nodiscard]] ResourceFileType getType() const override {
    return ResourceFileType::WORD;
  }
};

class Extractor {
public:
  virtual void extract2txt(ResourceFile &res_file) = 0;
};

class PdfExtractor : public Extractor {
public:
  void extract2txt(ResourceFile &res_file) override {
    std::cout << "pdf extractor.\n";
  }
};

class PPTExtractor : public Extractor {
public:
  void extract2txt(ResourceFile &res_file) override {
    std::cout << "PPT extractor.\n";
  }
};

class WordExtractor : public Extractor {
public:
  void extract2txt(ResourceFile &res_file) override {
    std::cout << "word extractor.\n";
  }
};

class ExtractorFactory {
private:
  using ExtractorMap = std::unordered_map<ResourceFile::ResourceFileType, std::shared_ptr<Extractor>>;
  inline static ExtractorMap maps_ = {
      {ResourceFile::ResourceFileType::PDF,std::make_shared<PdfExtractor>(PdfExtractor{})},
      {ResourceFile::ResourceFileType::PPT, std::make_shared<PPTExtractor>(PPTExtractor{})},
      {ResourceFile::ResourceFileType::WORD, std::make_shared<WordExtractor>(WordExtractor{})}
  };

public:
  static std::shared_ptr<Extractor> &getExtractor(ResourceFile::ResourceFileType type) {
    return maps_.at(type);
  }
};

}

TEST(visitor, visitor_factory_demo) {
  using namespace visitor_factory;

  std::vector<std::shared_ptr<ResourceFile>> res_files;
  res_files.emplace_back(std::make_shared<PdfFile>(PdfFile{"pdf"}));
  res_files.emplace_back(std::make_shared<PPTFile>(PPTFile{"ppt"}));
  res_files.emplace_back(std::make_shared<WordFile>(WordFile{"word"}));

  for (auto &res : res_files) {
    auto &extractor = ExtractorFactory::getExtractor(res->getType());
    extractor->extract2txt(*res);
  }
}