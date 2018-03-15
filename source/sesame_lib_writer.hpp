#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_interface.hpp"

namespace sesame {
  namespace lib {
    class writer_t: public interface_t {
      public:
        virtual auto length() const
          -> sinw = 0;
        virtual auto write(sinw offset, sinw length, const ui08* source)
          -> void = 0;
      protected:
      private:
    };
  }
}
