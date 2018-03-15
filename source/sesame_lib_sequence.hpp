#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_interface.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class sequence_t: public interface_t {
      public:
        virtual auto at(sinw index)
          -> T& = 0;
        virtual auto at(sinw index) const
          -> const T& = 0;
        virtual auto length() const
          -> sinw = 0;
      protected:
      private:
    };
  }
}

#include "sesame_lib_sequence_iterator.hpp"
