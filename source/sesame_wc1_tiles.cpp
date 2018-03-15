#include "sesame_wc1_tiles.hpp"

namespace sesame {
  namespace wc1 {
    tiles_t::tiles_t()
    : $tiles() {

    }

    tiles_t::tiles_t(vector_t<tile_t> tiles)
    : $tiles(pass(tiles)) {

    }

    tiles_t::tiles_t(owned_t<reader_t> reader, sinw& cursor)
    : $tiles() {
      auto length_left = (reader->length() - cursor);
      auto number_of_tiles = (length_left / (8 * 8));
      $tiles = vector_t<tile_t>(number_of_tiles, reader, cursor);
    }

    auto tiles_t::tiles()
      -> vector_t<tile_t>& {
      return $tiles;
    }

    auto tiles_t::tiles() const
      -> const vector_t<tile_t>& {
      return $tiles;
    }

    auto tiles_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      for (auto& tile : $tiles) {
        tile.write(writer, cursor);
      }
    }
  }
}
