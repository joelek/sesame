#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    class buffer_t {
      public:
        buffer_t();
        buffer_t(sinw length);
        buffer_t(sinw length, ui08 value);
        buffer_t(owned_t<reader_t> reader);
        buffer_t(const buffer_t& that);
        buffer_t(buffer_t&& that);
        ~buffer_t();
        auto operator =(buffer_t that)
          -> buffer_t&;
        auto length()
          -> sinw&;
        auto length() const
          -> const sinw&;
        auto pointer()
          -> ui08*;
        auto pointer() const
          -> const ui08*;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        sinw $length;
        ui08* $pointer;
    };
  }
}
