#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc1_tile.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc1 {
    class tiles_t {
      public:
        tiles_t();
        tiles_t(vector_t<tile_t> tiles);
        tiles_t(owned_t<reader_t> reader, sinw& cursor);
        auto tiles()
          -> vector_t<tile_t>&;
        auto tiles() const
          -> const vector_t<tile_t>&;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        vector_t<tile_t> $tiles;
    };
  }
}
