#include "../Exception.h"

TEST(ExceptionClass)
{
  Exception blah_exception("blah");
  CHECK_EQUAL("blah", blah_exception.what());
  CHECK_THROW(throw(blah_exception), Exception);
}
