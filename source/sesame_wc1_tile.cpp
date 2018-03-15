#include "sesame_wc1_tile.hpp"

namespace sesame {
  namespace wc1 {
    tile_t::tile_t()
    : $image() {

    }

    tile_t::tile_t(owned_t<image_t> image)
    : $image(pass(image)) {
      SESAME_FAIL_IF($image->bpp() != 3);
      SESAME_FAIL_IF($image->channels() != 1);
      SESAME_FAIL_IF($image->frames() != 1);
      SESAME_FAIL_IF($image->height() != 8);
      SESAME_FAIL_IF($image->width() != 8);
    }

    tile_t::tile_t(owned_t<reader_t>, sinw&)
    : $image(make<image_dynamic_t>(8, 8, 1, 3, 1)) {

    }

    auto tile_t::image()
      -> owned_t<image_t>& {
      return $image;
    }

    auto tile_t::image() const
      -> const owned_t<image_t>& {
      return $image;
    }

    auto tile_t::write(owned_t<writer_t>, sinw&)
      -> void {

    }
  }
}
