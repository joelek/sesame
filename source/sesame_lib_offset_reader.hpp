#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"

namespace sesame {
  namespace lib {
    class offset_reader_t: public reader_t {
      public:
        offset_reader_t();
        offset_reader_t(owned_t<reader_t> reader, sinw offset);
        offset_reader_t(owned_t<reader_t> reader, sinw offset, sinw length);
        auto length() const override
          -> sinw;
        auto read(sinw offset, sinw length, ui08* target) override
          -> void;
      protected:
      private:
        owned_t<reader_t> $reader;
        sinw $offset;
        sinw $length;
    };
  }
}
