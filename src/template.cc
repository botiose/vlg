#include "template.hh"

#include <string>

Template::Template() : m_msg("This is a template project.") {
}

const std::string& 
Template::getMsg() {
  return m_msg;
}
