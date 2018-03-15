#pragma once

#include "sesame_lib_header.hpp"

namespace sesame {
  namespace lib {
    namespace system {
      auto uses_big_endian()
        -> bool;
      auto uses_little_endian()
        -> bool;
      auto uses_ones_complement()
        -> bool;
      auto uses_twos_complement()
        -> bool;
    }
  }
}
