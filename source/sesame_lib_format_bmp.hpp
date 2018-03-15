#pragma once

#include "sesame_lib_buffer.hpp"
#include "sesame_lib_bytes_le.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_interface.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_primitive_le.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_vector.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    template <signed B>
    class image_plane_t {
        static_assert((B & 0x03) == B, u8"");
      public:
        image_plane_t()
        : $width()
        , $height()
        , $stride()
        , $buffer() {

        }
        image_plane_t(sinw width, sinw height)
        : $width(width)
        , $height(height)
        , $stride((($width << B) + 7) / 8)
        , $buffer($stride * $height, 0) {
          SESAME_FAIL_IF($width < 0);
          SESAME_FAIL_IF($height < 0);
        }
        auto decode(sinw y, sinw x) const
          -> ui08 {
          SESAME_FAIL_IF(y < 0);
          SESAME_FAIL_IF(y >= $height);
          SESAME_FAIL_IF(x < 0);
          SESAME_FAIL_IF(x >= $width);
          auto mask = ui08((1 << B) - 1);
          auto skip = ui08(8 - B - ((x % (8 / B)) * B));
          const auto& byte = $buffer.pointer()[(y * $stride) + (x / (8 / B))];
          return (byte >> skip) & mask;
        }
        auto encode(sinw y, sinw x, ui08 value)
          -> void {
          SESAME_FAIL_IF(y < 0);
          SESAME_FAIL_IF(y >= $height);
          SESAME_FAIL_IF(x < 0);
          SESAME_FAIL_IF(x >= $width);
          auto mask = ui08((1 << B) - 1);
          auto skip = ui08(8 - B - ((x % (8 / B)) * B));
          SESAME_FAIL_IF((value & mask) != mask);
          auto& byte = $buffer.pointer()[(y * $stride) + (x / (8 / B))];
          byte = (value << skip) | (byte & ~(mask << skip));
        }
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void {
          $buffer.write(writer, cursor);
        }
      protected:
      private:
        sinw $width;
        sinw $height;
        sinw $stride;
        buffer_t $buffer;
    };
  }
}

namespace sesame {
  namespace lib {
    class bitmap_t {
      public:
        bitmap_t();
        bitmap_t(sinw width, sinw height, sinw bpp);
        auto width()
          -> sinw&;
        auto width() const
          -> const sinw&;
        auto height()
          -> sinw&;
        auto height() const
          -> const sinw&;
        auto bpp()
          -> sinw&;
        auto bpp() const
          -> const sinw&;
        auto stride()
          -> sinw&;
        auto stride() const
          -> const sinw&;
        auto decode_scanline(sinw y, ui08* target) const
          -> void;
        auto encode_scanline(sinw y, const ui08* source)
          -> void;
      protected:
      private:
        sinw $width;
        sinw $height;
        sinw $bpp;
        sinw $stride;
        buffer_t $buffer;
    };
  }
}

namespace sesame {
  namespace lib {
    class palette_entry_t {
      public:
        palette_entry_t();
        palette_entry_t(ui08 r, ui08 g, ui08 b);
        auto r()
          -> ui08&;
        auto r() const
          -> const ui08&;
        auto g()
          -> ui08&;
        auto g() const
          -> const ui08&;
        auto b()
          -> ui08&;
        auto b() const
          -> const ui08&;
      protected:
      private:
        ui08 $r;
        ui08 $g;
        ui08 $b;
    };
  }
}

namespace sesame {
  namespace lib {
    class palette_t {
      public:
        palette_t();
        palette_t(vector_t<palette_entry_t> entries);
        auto entries()
          -> vector_t<palette_entry_t>&;
        auto entries() const
          -> const vector_t<palette_entry_t>&;
      protected:
      private:
        vector_t<palette_entry_t> $entries;
    };
  }
}

namespace sesame {
  namespace lib {
    class format_bmp_t {
      public:
        format_bmp_t();
        format_bmp_t(bitmap_t bitmap);
        format_bmp_t(bitmap_t bitmap, palette_t palette);
        format_bmp_t(owned_t<reader_t> reader, sinw& cursor);
        auto bitmap()
          -> bitmap_t&;
        auto bitmap() const
          -> const bitmap_t&;
        auto palette()
          -> palette_t&;
        auto palette() const
          -> const palette_t&;
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
        auto compute_scanline_stride(sinw absolute_width, sinw bpp) const
          -> sinw;
        auto read_bitmap_1_0(owned_t<reader_t> reader, sinw& cursor, sinw width, sinw height) const
          -> bitmap_t;
        auto read_palette(owned_t<reader_t> reader, sinw& cursor, sinw bpp, sinw colors_used) const
          -> palette_t;
      private:
        bitmap_t $bitmap;
        palette_t $palette;
    };
  }
}
