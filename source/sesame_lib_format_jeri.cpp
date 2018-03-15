#include "sesame_lib_format_jeri.hpp"

namespace sesame {
  namespace lib {
    format_jeri_t::format_jeri_t()
    : $image() {

    }

    format_jeri_t::format_jeri_t(owned_t<image_t> image)
    : $image(pass(image)) {

    }

    format_jeri_t::format_jeri_t(owned_t<reader_t> reader, sinw& cursor)
    : $image() {
      auto header = format_jeri_header_t(reader, cursor);
      auto width = header.$image_width;
      auto height = header.$image_height;
      auto frames = header.$number_of_frames;
      auto bpp = header.$bits_per_pixel_per_channel_log2;
      auto channels = header.$number_of_channels_minus_one;
      $image = make<image_dynamic_t>(width, height, frames, bpp, channels + 1);
    }

    auto format_jeri_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      auto header = format_jeri_header_t($image);
      header.write(writer, cursor);
    }
  }
}
