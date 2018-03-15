// [dirty]
#pragma once

#include "sesame_lib.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class archive_format_t {
        friend class archive_t;
      public:
        archive_format_t(owned_t<reader_t> reader, sinw cursor);
        sinw get_magic_number() const;
        sinw get_number_of_records() const;
        sinw get_archive_id() const;
        sinw get_record_offset(sinw index) const;
        owned_t<reader_t> get_data_reader(sinw index) const;
      protected:
        owned_t<reader_t> get_offset_reader(sinw index) const;
      private:
        owned_t<reader_t> reader;
        primitive_le_t<ui32> magic_number;
        primitive_le_t<ui16> number_of_records;
        primitive_le_t<ui16> archive_id;
        vector_t<primitive_le_t<ui32>> record_offsets;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class archive_t {
      public:
        archive_t(owned_t<reader_t> reader);
        owned_t<archive_format_t> get_format() const;
      protected:
      private:
        owned_t<archive_format_t> format;
    };
  }
}
