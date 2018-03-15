#pragma once

#include "sesame_lib_buffer.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_primitive_le.hpp"
#include "sesame_lib_reader.hpp"

namespace sesame {
  namespace lib {
    class lzss_reader_t: public reader_t {
      public:
        lzss_reader_t();
        lzss_reader_t(owned_t<reader_t> reader, sinw length);
        auto length() const override
          -> sinw;
        auto read(sinw offset, sinw length, ui08* target) override
          -> void;
      protected:
        auto append(ui08 byte)
          -> void;
      private:
        owned_t<reader_t> $reader;
        sinw $length;
        sinw $length_unpacked;
        buffer_t $buffer;
        ui08 $history[1 << 12];
        sinw $history_position;
        sinw $cursor;
    };
  }
}
