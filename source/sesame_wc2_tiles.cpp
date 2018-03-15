// [dirty]
#include "sesame_wc2_tiles.hpp"

namespace sesame {
  namespace wc2 {
    tiles_format_t::tiles_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $number_of_tiles((reader->length() - cursor) / (8 * 8))
    , $reader(make<offset_reader_t>(reader, cursor, ($number_of_tiles * (8 * 8)))) {
      cursor += $reader->length();
    }

    sinw tiles_format_t::number_of_tiles() const {
      return $number_of_tiles;
    }

    owned_t<reader_t> tiles_format_t::get_tile(sinw index) const {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= $number_of_tiles);
      return make<offset_reader_t>($reader, (index * (8 * 8)), (8 * 8));
    }
  }
}

namespace sesame {
  namespace wc2 {
    tiles_resource_t::tiles_resource_t(const owned_t<reader_t>& reader)
    : $format(parse_format(reader)) {

    }

    const tiles_format_t& tiles_resource_t::format() const {
      return $format;
    }

    tiles_format_t tiles_resource_t::parse_format(const owned_t<reader_t>& reader) const {
      SESAME_FAIL_IF((reader->length() % (8 * 8)) != 0);
      auto cursor = 0;
      auto format = tiles_format_t(reader, cursor);
      SESAME_FAIL_IF(cursor != reader->length());
      return format;
    }
  }
}
