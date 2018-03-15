#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_interface.hpp"

namespace sesame {
  namespace lib {
    class reader_t: public interface_t {
      public:
        virtual auto length() const
          -> sinw = 0;
        virtual auto read(sinw offset, sinw length, ui08* target)
          -> void = 0;
      protected:
      private:
    };
  }
}
