#pragma once

#include "sesame_lib.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc1 {
    class tile_t {
      public:
        tile_t();
        tile_t(owned_t<image_t> image);
        tile_t(owned_t<reader_t> reader, sinw& cursor);
        auto image()
          -> owned_t<image_t>&;
        auto image() const
          -> const owned_t<image_t>&;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        owned_t<image_t> $image;
    };
  }
}
