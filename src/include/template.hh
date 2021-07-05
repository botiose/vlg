#pragma once

#include <string>

class Template {
public:
  Template();

  const std::string& 
  getMsg();
private:
  const std::string m_msg;
};
