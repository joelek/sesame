#pragma once

#include "sesame_lib_buffer.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_reader.hpp"

namespace sesame {
  namespace lib {
    class buffer_reader_t: public reader_t {
      public:
        buffer_reader_t();
        buffer_reader_t(buffer_t buffer);
        auto length() const override
          -> sinw;
        auto read(sinw offset, sinw length, ui08* target) override
          -> void;
      protected:
      private:
        buffer_t $buffer;
    };
  }
}
