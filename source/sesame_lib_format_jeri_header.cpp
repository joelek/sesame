#include "sesame_lib_format_jeri_header.hpp"

namespace sesame {
namespace lib {
    format_jeri_header_t::format_jeri_header_t()
    : $identifier("JERI")
    , $image_width()
    , $image_height()
    , $number_of_frames()
    , $bits_per_pixel_per_channel_log2()
    , $number_of_channels_minus_one()
    , $number_of_tables_minus_one()
    , $reserved_a()
    , $reserved_b()
    , $reserved_c()
    , $red_channel_index()
    , $red_table_index()
    , $green_channel_index()
    , $green_table_index()
    , $blue_channel_index()
    , $blue_table_index()
    , $opacity_channel_index()
    , $opacity_table_index() {

    }

    format_jeri_header_t::format_jeri_header_t(owned_t<image_t> image)
    : $identifier("JERI")
    , $image_width(image->width())
    , $image_height(image->height())
    , $number_of_frames(image->frames())
    , $bits_per_pixel_per_channel_log2(image->bpp())
    , $number_of_channels_minus_one(image->channels())
    , $number_of_tables_minus_one()
    , $reserved_a(0xFF)
    , $reserved_b(0xFF)
    , $reserved_c(0xFF)
    , $red_channel_index()
    , $red_table_index()
    , $green_channel_index()
    , $green_table_index()
    , $blue_channel_index()
    , $blue_table_index()
    , $opacity_channel_index()
    , $opacity_table_index() {

    }

    format_jeri_header_t::format_jeri_header_t(owned_t<reader_t> reader, sinw& cursor)
    : $identifier(reader, cursor)
    , $image_width(reader, cursor)
    , $image_height(reader, cursor)
    , $number_of_frames(reader, cursor)
    , $bits_per_pixel_per_channel_log2(reader, cursor)
    , $number_of_channels_minus_one(reader, cursor)
    , $number_of_tables_minus_one(reader, cursor)
    , $reserved_a(reader, cursor)
    , $reserved_b(reader, cursor)
    , $reserved_c(reader, cursor)
    , $red_channel_index(reader, cursor)
    , $red_table_index(reader, cursor)
    , $green_channel_index(reader, cursor)
    , $green_table_index(reader, cursor)
    , $blue_channel_index(reader, cursor)
    , $blue_table_index(reader, cursor)
    , $opacity_channel_index(reader, cursor)
    , $opacity_table_index(reader, cursor)  {
      SESAME_FAIL_IF($identifier != si32le("JERI"));
    }

    auto format_jeri_header_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      $identifier.write(writer, cursor);
      $image_width.write(writer, cursor);
      $image_height.write(writer, cursor);
      $number_of_frames.write(writer, cursor);
      $bits_per_pixel_per_channel_log2.write(writer, cursor);
      $number_of_channels_minus_one.write(writer, cursor);
      $number_of_tables_minus_one.write(writer, cursor);
      $reserved_a.write(writer, cursor);
      $reserved_b.write(writer, cursor);
      $reserved_c.write(writer, cursor);
      $red_channel_index.write(writer, cursor);
      $red_table_index.write(writer, cursor);
      $green_channel_index.write(writer, cursor);
      $green_table_index.write(writer, cursor);
      $blue_channel_index.write(writer, cursor);
      $blue_table_index.write(writer, cursor);
      $opacity_channel_index.write(writer, cursor);
      $opacity_table_index.write(writer, cursor);
    }
  }
}
