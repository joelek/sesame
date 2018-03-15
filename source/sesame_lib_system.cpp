#include "sesame_lib_system.hpp"

namespace sesame {
  namespace lib {
    namespace system {
      auto uses_big_endian()
        -> bool {
        volatile auto integer = 1;
        return reinterpret_cast<volatile ui08*>(&integer)[0] == 0;
      }

      auto uses_little_endian()
        -> bool {
        volatile auto integer = 1;
        return reinterpret_cast<volatile ui08*>(&integer)[0] == 1;
      }

      auto uses_ones_complement()
        -> bool {
        volatile auto integer = 1;
        return ~integer == -1;
      }

      auto uses_twos_complement()
        -> bool {
        volatile auto integer = 0;
        return ~integer == -1;
      }
    }
  }
}
