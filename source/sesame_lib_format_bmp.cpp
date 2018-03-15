#include "sesame_lib_format_bmp.hpp"

namespace sesame {
  namespace lib {
    bitmap_t::bitmap_t()
    : $width()
    , $height()
    , $bpp()
    , $stride()
    , $buffer() {

    }

    bitmap_t::bitmap_t(sinw width, sinw height, sinw bpp)
    : $width(width)
    , $height(height)
    , $bpp(bpp)
    , $stride(($width * $bpp + 7) / 8)
    , $buffer($stride * $height) {
      SESAME_FAIL_IF($width < 0);
      SESAME_FAIL_IF($height < 0);
      SESAME_FAIL_IF($bpp < 0);
    }

    auto bitmap_t::width()
      -> sinw& {
      return $width;
    }

    auto bitmap_t::width() const
      -> const sinw& {
      return $width;
    }

    auto bitmap_t::height()
      -> sinw& {
      return $height;
    }

    auto bitmap_t::height() const
      -> const sinw& {
      return $height;
    }

    auto bitmap_t::bpp()
      -> sinw& {
      return $bpp;
    }

    auto bitmap_t::bpp() const
      -> const sinw& {
      return $bpp;
    }

    auto bitmap_t::stride()
      -> sinw& {
      return $stride;
    }

    auto bitmap_t::stride() const
      -> const sinw& {
      return $stride;
    }

    auto bitmap_t::decode_scanline(sinw y, ui08* target) const
      -> void {
      SESAME_FAIL_IF(y < 0);
      SESAME_FAIL_IF(y >= $height);
      SESAME_FAIL_IF(target == nullptr);

    }

    auto bitmap_t::encode_scanline(sinw y, const ui08* source)
      -> void {
      SESAME_FAIL_IF(y < 0);
      SESAME_FAIL_IF(y >= $height);
      SESAME_FAIL_IF(source == nullptr);

    }
  }
}

namespace sesame {
  namespace lib {
    palette_entry_t::palette_entry_t()
    : $r()
    , $g()
    , $b() {

    }

    palette_entry_t::palette_entry_t(ui08 r, ui08 g, ui08 b)
    : $r(r)
    , $g(g)
    , $b(b) {

    }

    auto palette_entry_t::r()
      -> ui08& {
      return $r;
    }

    auto palette_entry_t::r() const
      -> const ui08& {
      return $r;
    }

    auto palette_entry_t::g()
      -> ui08& {
      return $g;
    }

    auto palette_entry_t::g() const
      -> const ui08& {
      return $g;
    }

    auto palette_entry_t::b()
      -> ui08& {
      return $b;
    }

    auto palette_entry_t::b() const
      -> const ui08& {
      return $b;
    }
  }
}

namespace sesame {
  namespace lib {
    palette_t::palette_t()
    : $entries() {

    }

    palette_t::palette_t(vector_t<palette_entry_t> entries)
    : $entries(pass(entries)) {

    }

    auto palette_t::entries()
      -> vector_t<palette_entry_t>& {
      return $entries;
    }

    auto palette_t::entries() const
      -> const vector_t<palette_entry_t>& {
      return $entries;
    }
  }
}

namespace sesame {
  namespace lib {
    #pragma pack(push, 1)

    struct bmp_header_t {
      friend class format_bmp_t;
      bytes_le_t< 2> identifier;
      bytes_le_t< 4> file_size;
      bytes_le_t< 2> reserved_a;
      bytes_le_t< 2> reserved_b;
      bytes_le_t< 4> image_offset;
      bmp_header_t()
      : identifier("BM")
      , file_size()
      , reserved_a()
      , reserved_b()
      , image_offset() {

      }
      bmp_header_t(owned_t<reader_t> reader, sinw& cursor)
      : identifier(reader, cursor)
      , file_size(reader, cursor)
      , reserved_a(reader, cursor)
      , reserved_b(reader, cursor)
      , image_offset(reader, cursor) {
        SESAME_FAIL_IF(identifier != "BM");
      }
      auto write(owned_t<writer_t> writer, sinw& cursor)
        -> void {
        auto source = reinterpret_cast<ui08*>(&self);
        writer->write(cursor, size<bmp_header_t>(), source);
        cursor += size<bmp_header_t>();
      }
    };

    struct dib_header_t {
      friend class format_bmp_t;
      dib_header_t()
      : header_size(40)
      , image_width()
      , image_height()
      , color_planes(1)
      , bits_per_pixel()
      , compression_method()
      , image_size()
      , horizontal_resolution(2835)
      , vertical_resolution(2835)
      , image_colors()
      , important_colors() {

      }
      dib_header_t(owned_t<reader_t> reader, sinw& cursor)
      : header_size(reader, cursor)
      , image_width(reader, cursor)
      , image_height(reader, cursor)
      , color_planes(reader, cursor)
      , bits_per_pixel(reader, cursor)
      , compression_method(reader, cursor)
      , image_size(reader, cursor)
      , horizontal_resolution(reader, cursor)
      , vertical_resolution(reader, cursor)
      , image_colors(reader, cursor)
      , important_colors(reader, cursor) {
        SESAME_FAIL_IF(header_size != size<dib_header_t>());
        SESAME_FAIL_IF(color_planes != 1);
      }
      auto write(owned_t<writer_t> writer, sinw& cursor)
        -> void {
        auto source = reinterpret_cast<ui08*>(&self);
        writer->write(cursor, size<dib_header_t>(), source);
        cursor += size<dib_header_t>();
      }
      bytes_le_t< 4> header_size;
      bytes_le_t< 4> image_width;
      bytes_le_t< 4> image_height;
      bytes_le_t< 2> color_planes;
      bytes_le_t< 2> bits_per_pixel;
      bytes_le_t< 4> compression_method;
      bytes_le_t< 4> image_size;
      bytes_le_t< 4> horizontal_resolution;
      bytes_le_t< 4> vertical_resolution;
      bytes_le_t< 4> image_colors;
      bytes_le_t< 4> important_colors;
    };

    struct channel_masks_t {
      friend class format_bmp_t;
      bytes_le_t< 4> r;
      bytes_le_t< 4> g;
      bytes_le_t< 4> b;
      auto write(owned_t<writer_t> writer, sinw& cursor)
        -> void {
        auto source = reinterpret_cast<ui08*>(&self);
        writer->write(cursor, size<channel_masks_t>(), source);
        cursor += size<channel_masks_t>();
      }
    };

    struct bmp_palette_entry_t {
      friend class format_bmp_t;
      bytes_le_t< 1> b;
      bytes_le_t< 1> g;
      bytes_le_t< 1> r;
      bytes_le_t< 1> p;
      bmp_palette_entry_t()
      : b()
      , g()
      , r()
      , p() {

      }
      bmp_palette_entry_t(owned_t<reader_t> reader, sinw& cursor)
      : b(reader, cursor)
      , g(reader, cursor)
      , r(reader, cursor)
      , p(reader, cursor) {
        SESAME_FAIL_IF(p != 0);
      }
      auto write(owned_t<writer_t> writer, sinw& cursor)
        -> void {
        auto source = reinterpret_cast<ui08*>(&self);
        writer->write(cursor, size<bmp_palette_entry_t>(), source);
        cursor += size<bmp_palette_entry_t>();
      }
    };

    #pragma pack(pop)

    format_bmp_t::format_bmp_t()
    : $bitmap()
    , $palette() {

    }

    format_bmp_t::format_bmp_t(bitmap_t bitmap)
    : $bitmap(pass(bitmap))
    , $palette() {
      auto bpp = $bitmap.bpp();
      SESAME_FAIL_IF(bpp != 16 && bpp != 24 && bpp != 32);
    }

    format_bmp_t::format_bmp_t(bitmap_t bitmap, palette_t palette)
    : $bitmap(pass(bitmap))
    , $palette(pass(palette)) {
      SESAME_FAIL_IF((1 << $bitmap.bpp()) != $palette.entries().length());
      auto bpp = $bitmap.bpp();
      SESAME_FAIL_IF(bpp != 1 && bpp != 4 && bpp != 8);
    }

    format_bmp_t::format_bmp_t(owned_t<reader_t> reader, sinw& cursor)
    : $bitmap()
    , $palette() {
      auto bmp = bmp_header_t(reader, cursor);
      auto dib = dib_header_t(reader, cursor);
      auto image_offset = bmp.image_offset.as<sinw>();
      auto bpp = dib.bits_per_pixel.as<sinw>();
      auto cm = dib.compression_method.as<sinw>();
      auto width = dib.image_width.as<sinw>();
      auto height = dib.image_height.as<sinw>();
      auto colors_used = dib.image_colors.as<sinw>();
      if (bpp == 1) {
        $palette = read_palette(reader, cursor, bpp, colors_used);
        SESAME_FAIL_IF(image_offset < cursor);
        cursor = image_offset;
        if (cm == 0) {
          $bitmap = read_bitmap_1_0(reader, cursor, width, height);
        } else {
          SESAME_FAIL_IF(true);
        }
      } else if (bpp == 4) {
        $palette = read_palette(reader, cursor, bpp, colors_used);
        SESAME_FAIL_IF(image_offset < cursor);
        cursor = image_offset;
        if (cm == 0) {

        } else if (cm == 2) {

        } else {
          SESAME_FAIL_IF(true);
        }
      } else if (bpp == 8) {
        $palette = read_palette(reader, cursor, bpp, colors_used);
        SESAME_FAIL_IF(image_offset < cursor);
        cursor = image_offset;
        if (cm == 0) {

        } else if (cm == 1) {

        } else {
          SESAME_FAIL_IF(true);
        }
      } else if (bpp == 24) {
        SESAME_FAIL_IF(image_offset < cursor);
        cursor = image_offset;
        if (cm == 0) {

        } else {
          SESAME_FAIL_IF(true);
        }
      } else if (bpp == 16 || bpp == 32) {
        auto channel_masks = channel_masks_t {
          { reader, cursor },
          { reader, cursor },
          { reader, cursor }
        };
        SESAME_FAIL_IF(image_offset < cursor);
        cursor = image_offset;
        if (cm == 3) {

        } else {
          SESAME_FAIL_IF(true);
        }
      } else {
        SESAME_FAIL_IF(true);
      }
/*
rle_8:

auto control_byte = le_t<ui08>(reader, cursor);
if (control_byte == 0) {
  if (next_byte == 0) {
    end of scanline
  } else if (next_byte == 1) {
    end of rle, stop
  } else if (next_byte == 2) {
    auto x = read byte
    auto y = read byte
    jump to right x and down y
  } else {
    auto unenc_length = read byte;
    for (auto i = 0; i < unenc_length; i++) {
      auto byte = read byte;
      write(byte);
    }
    if (unec_length & 1) {
      read byte;
    }
  }
} else {
  auto byte = read byte;
  for (auto i = 0; i < control_byte; i++) {
    write(byte);
  }
}

rle_4:
same controls
each literal byte is high nibble, low nibble
byte padding

*/
    }

    auto format_bmp_t::bitmap()
      -> bitmap_t& {
      return $bitmap;
    }

    auto format_bmp_t::bitmap() const
      -> const bitmap_t& {
      return $bitmap;
    }

    auto format_bmp_t::palette()
      -> palette_t& {
      return $palette;
    }

    auto format_bmp_t::palette() const
      -> const palette_t& {
      return $palette;
    }

    auto format_bmp_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      auto cursor_start = cursor;
      auto bmp = bmp_header_t();
      auto dib = dib_header_t();
      bmp.write(writer, cursor);
      dib.write(writer, cursor);
      dib.image_width = $bitmap.width();
      dib.image_height = $bitmap.height();
      dib.bits_per_pixel = $bitmap.bpp();
      if ($bitmap.bpp() == 32) {
        auto channel_masks = channel_masks_t();
        channel_masks.r = 0xFF000000;
        channel_masks.g = 0x00FF0000;
        channel_masks.b = 0x0000FF00;
        channel_masks.write(writer, cursor);
        dib.compression_method = 3;
      } else if ($bitmap.bpp() == 24) {
      } else if ($bitmap.bpp() == 16) {
        auto channel_masks = channel_masks_t();
        channel_masks.r = 0xF8000000;
        channel_masks.g = 0x07C00000;
        channel_masks.b = 0x003E0000;
        channel_masks.write(writer, cursor);
        dib.compression_method = 3;
      } else {
        for (const auto& palette_entry : $palette.entries()) {
          auto bmp_palette_entry = bmp_palette_entry_t();
          bmp_palette_entry.r = palette_entry.r();
          bmp_palette_entry.g = palette_entry.g();
          bmp_palette_entry.b = palette_entry.b();
          bmp_palette_entry.write(writer, cursor);
        }
      }
      bmp.image_offset = cursor - cursor_start;
      // pixels
      bmp.file_size = cursor - cursor_start;
      dib.image_size = bmp.file_size.as<sinw>() - bmp.image_offset.as<sinw>();
      auto cursor_end = cursor;
      cursor = cursor_start;
      bmp.write(writer, cursor);
      dib.write(writer, cursor);
      cursor = cursor_end;
    }

    auto format_bmp_t::compute_scanline_stride(sinw absolute_width, sinw bpp) const
      -> sinw {
      SESAME_FAIL_IF(absolute_width < 0);
      SESAME_FAIL_IF(bpp < 0);
      return (((absolute_width * bpp) + 31) / 32) * 4;
    }

    auto format_bmp_t::read_bitmap_1_0(owned_t<reader_t> reader, sinw& cursor, sinw width, sinw height) const
      -> bitmap_t {
      auto absolute_width = (width < 0) ? 0 - width : width;
      auto absolute_height = (height < 0) ? 0 - height : height;
      auto bitmap = bitmap_t(absolute_width, absolute_height, 1);
      auto buffer = buffer_t(compute_scanline_stride(absolute_width, 1));
      auto scanline = buffer_t(absolute_width * 1);
      auto mask = ui08((1 << 1) - 1);
      auto skip = ui08(8 - 1 - ((8 % (8 / 1)) * 1));
      for (auto h = 0; h < absolute_height; h++) {
        reader->read(cursor, buffer.length(), buffer.pointer());
        cursor += buffer.length();
        for (auto p = 0; p < scanline.length(); p++) {
          auto pixel = (buffer.pointer()[p / (8 / 1)] >> skip) & mask;
          auto x = (width < 0) ? scanline.length() - 1 - p : p;
          scanline.pointer()[x] = pixel;
        }
        auto y = (height < 0) ? h : height - 1 - h;
        bitmap.encode_scanline(y, scanline.pointer());
      }
      return bitmap;
    }

    auto format_bmp_t::read_palette(owned_t<reader_t> reader, sinw& cursor, sinw bpp, sinw colors_used) const
      -> palette_t {
      SESAME_FAIL_IF(bpp < 0);
      SESAME_FAIL_IF(colors_used < 0);
      auto colors_max = (1 << bpp);
      SESAME_FAIL_IF(colors_used > colors_max);
      auto colors_to_read = (colors_used == 0) ? colors_max : colors_used;
      auto bmp_palette_entries = vector_t<bmp_palette_entry_t>(colors_to_read, reader, cursor);
      auto palette_entries = vector_t<palette_entry_t>(colors_max);
      for (auto i = 0; i < colors_to_read; i++) {
        const auto& bmp_palette_entry = bmp_palette_entries.at(i);
        auto& palette_entry = palette_entries.at(i);
        palette_entry.r() = bmp_palette_entry.r.as<ui08>();
        palette_entry.g() = bmp_palette_entry.g.as<ui08>();
        palette_entry.b() = bmp_palette_entry.b.as<ui08>();
      }
      return palette_t(pass(palette_entries));
    }
  }
}
