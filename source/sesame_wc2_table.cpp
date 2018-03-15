// [dirty]
#include "sesame_wc2_table.hpp"

namespace sesame {
  namespace wc2 {
    table_format_t::table_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $width(reader, cursor)
    , $height(reader, cursor)
    , $data($width, $height, reader, cursor) {

    }

    sinw table_format_t::width() const {
      return $width;
    }

    sinw table_format_t::height() const {
      return $height;
    }

    const matrix_t<primitive_le_t<ui16>>& table_format_t::data() const {
      return $data;
    }
  }
}

namespace sesame {
  namespace wc2 {
    table_resource_t::table_resource_t(const owned_t<reader_t>& reader)
    : $format(parse_format(reader)) {

    }

    const table_format_t& table_resource_t::format() const {
      return $format;
    }

    table_format_t table_resource_t::parse_format(const owned_t<reader_t>& reader) const {
      auto cursor = 0;
      auto format = table_format_t(reader, cursor);
      SESAME_FAIL_IF(cursor != reader->length());
      return format;
    }
  }
}
