#include <iostream>
#include <string>

#include "template.hh"

int main() {
  Template tmplt{};
  const std::string& msg = tmplt.getMsg();
  std::cout << msg << std::endl; 
}
