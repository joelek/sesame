#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_image.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_primitive_le.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    class format_jeri_header_t {
        friend class format_jeri_t;
      public:
        format_jeri_header_t();
        format_jeri_header_t(owned_t<image_t> image);
        format_jeri_header_t(owned_t<reader_t> reader, sinw& cursor);
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        si32le $identifier;
        si16le $image_width;
        si16le $image_height;
        si16le $number_of_frames;
        ui08le $bits_per_pixel_per_channel_log2;
        ui08le $number_of_channels_minus_one;
        ui08le $number_of_tables_minus_one;
        ui08le $reserved_a;
        ui08le $reserved_b;
        ui08le $reserved_c;
        ui08le $red_channel_index;
        ui08le $red_table_index;
        ui08le $green_channel_index;
        ui08le $green_table_index;
        ui08le $blue_channel_index;
        ui08le $blue_table_index;
        ui08le $opacity_channel_index;
        ui08le $opacity_table_index;
    };
  }
}
