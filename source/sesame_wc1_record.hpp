#pragma once

#include "sesame_lib.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc1 {
    class record_t {
      public:
        record_t();
        record_t(owned_t<reader_t> reader);
        record_t(owned_t<reader_t> reader, sinw& cursor);
        auto reader()
          -> owned_t<reader_t>&;
        auto reader() const
          -> const owned_t<reader_t>&;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        owned_t<reader_t> $reader;
    };
  }
}
