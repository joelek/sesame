#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc1_record.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc1 {
    class archive_t {
      public:
        archive_t();
        archive_t(vector_t<record_t> records);
        archive_t(owned_t<reader_t> reader, sinw& cursor);
        auto records()
          -> vector_t<record_t>&;
        auto records() const
          -> const vector_t<record_t>&;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        vector_t<record_t> $records;
    };
  }
}
