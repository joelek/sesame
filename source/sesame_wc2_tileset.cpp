// [dirty]
#include "sesame_wc2_tileset.hpp"

namespace sesame {
  namespace wc2 {
    tileset_format_tile_entry_t::tileset_format_tile_entry_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $bits(reader, cursor) {

    }

    sinw tileset_format_tile_entry_t::is_inverted_y() const {
      return $bits.unpack<0, 1>();
    }

    sinw tileset_format_tile_entry_t::is_inverted_x() const {
      return $bits.unpack<1, 1>();
    }

    sinw tileset_format_tile_entry_t::index() const {
      return $bits.unpack<2, 14>();
    }
  }
}

namespace sesame {
  namespace wc2 {
    tileset_format_tile_t::tileset_format_tile_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $entries(4, 4, reader, cursor) {

    }

    const matrix_t<tileset_format_tile_entry_t>& tileset_format_tile_t::entries() const {
      return $entries;
    }
  }
}

namespace sesame {
  namespace wc2 {
    tileset_format_t::tileset_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $tiles(((reader->length() - cursor) / (4 * 4 * 2)), reader, cursor) {

    }

    const vector_t<tileset_format_tile_t>& tileset_format_t::tiles() const {
      return $tiles;
    }
  }
}

namespace sesame {
  namespace wc2 {
    tileset_resource_t::tileset_resource_t(const owned_t<reader_t>& reader)
    : $format(parse_format(reader)) {

    }

    const tileset_format_t& tileset_resource_t::format() const {
      return $format;
    }

    tileset_format_t tileset_resource_t::parse_format(const owned_t<reader_t>& reader) const {
      SESAME_FAIL_IF((reader->length() % (4 * 4 * 2)) != 0);
      auto cursor = 0;
      auto format = tileset_format_t(reader, cursor);
      SESAME_FAIL_IF(cursor != reader->length());
      return format;
    }
  }
}
