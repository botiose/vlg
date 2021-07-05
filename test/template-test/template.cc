#include "gtest/gtest.h"

#include "template.hh"

TEST(Template, TemplateSucces) {
  Template tmplt{};
  const std::string& msg = tmplt.getMsg();
  EXPECT_STREQ(msg.c_str(), "This is a template project.");
}

TEST(Template, TemplateFailure) {
  Template tmplt{};
  const std::string& msg = tmplt.getMsg();
  EXPECT_STREQ(msg.c_str(), "This will not match");
}

