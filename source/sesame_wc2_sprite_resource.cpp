// [dirty]
#include "sesame_wc2_sprite_resource.hpp"

namespace sesame {
  namespace wc2 {
    frame_format_t::frame_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $offset_x(reader, cursor)
    , $offset_y(reader, cursor)
    , $width(reader, cursor)
    , $height(reader, cursor)
    , $bits(reader, cursor) {

    }

    sinw frame_format_t::offset_x() const {
      return $offset_x;
    }

    sinw frame_format_t::offset_y() const {
      return $offset_y;
    }

    sinw frame_format_t::width() const {
      return $width;
    }

    sinw frame_format_t::height() const {
      return $height;
    }

    sinw frame_format_t::data_offset() const {
      return $bits.unpack<0, 31>();
    }

    sinw frame_format_t::is_deleted() const {
      return $bits.unpack<31, 1>();
    }
  }
}

namespace sesame {
  namespace wc2 {
    sprite_format_t::sprite_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : number_of_frames(reader, cursor)
    , width(reader, cursor)
    , height(reader, cursor)
    , formats(number_of_frames, reader, cursor)
    , reader(reader) {
      if (this->number_of_frames == 0) {
        SESAME_FAIL_IF(cursor != reader->length());
      }
      for (auto i = 0; i < this->number_of_frames; i++) {
        auto& format = this->formats.at(i);
        SESAME_FAIL_IF(format.offset_x() + format.width() > this->width);
        SESAME_FAIL_IF(format.offset_y() + format.height() > this->height);
        SESAME_FAIL_IF(format.data_offset() < cursor);
        SESAME_FAIL_IF(format.data_offset() >= reader->length());
      }
    }

    sinw sprite_format_t::get_number_of_frames() const {
      return this->number_of_frames;
    }

    sinw sprite_format_t::get_width() const {
      return this->width;
    }

    sinw sprite_format_t::get_height() const {
      return this->height;
    }

    owned_t<reader_t> sprite_format_t::get_data_reader(sinw frame) const {
      SESAME_FAIL_IF(frame < 0 || frame >= this->number_of_frames);
      auto& format = this->formats.at(frame);
      if (!format.is_deleted()) {
        auto width = format.width();
        auto height = format.height();
        auto ox = format.offset_x();
        auto oy = format.offset_y();
        auto reader = this->get_offset_reader(frame);
        auto buffer = buffer_t(this->width * this->height, 0);
        if (reader->length() > 0) {
          auto cursor = 0;
          auto row_offsets = segmented_reader_t<primitive_le_t<ui16>>(height, reader, cursor);
          for (auto y = 0; y < height; y++) {
            auto reader = row_offsets.at(y);
            auto cursor = 0;
            auto x = 0;
            while (x < width) {
              auto control = primitive_le_t<ui08>(reader, cursor);
              auto control_mode = ((control >> 6) & 0x03);
              auto control_length = (control & 0x3F);
              if (control_mode == 0) {
                for (auto i = 0; i < control_length; i++) {
                  auto value = primitive_le_t<ui08>(reader, cursor);
                  buffer.pointer()[(oy + y) * this->width + (ox + x)] = value;
                  x++;
                }
              } else if (control_mode == 1) {
                auto value = primitive_le_t<ui08>(reader, cursor);
                for (auto i = 0; i < control_length; i++) {
                  buffer.pointer()[(oy + y) * this->width + (ox + x)] = value;
                  x++;
                }
              } else if (control_mode == 2) {
                for (auto i = 0; i < control_length; i++) {
                  x++;
                }
              } else if (control_mode == 3) {
                for (auto i = 0; i < control_length; i++) {
                  x++;
                }
                while (x < width) {
                  x++;
                }
              }
            }
            SESAME_FAIL_IF(cursor != reader->length());
          }
        }
        return make<buffer_reader_t>(buffer);
      } else {
        return nullptr;
      }
    }

    owned_t<reader_t> sprite_format_t::get_offset_reader(sinw frame) const {
      SESAME_FAIL_IF(frame < 0 || frame >= this->number_of_frames);
      auto offset = this->formats.at(frame).data_offset();
      if (frame + 1 < this->number_of_frames) {
        auto length = this->formats.at(frame + 1).data_offset() - offset;
        return make<offset_reader_t>(this->reader, offset, length);
      } else {
        auto length = this->reader->length() - offset;
        return make<offset_reader_t>(this->reader, offset, length);
      }
    }
  }
}

namespace sesame {
  namespace wc2 {
    sprite_resource_t::sprite_resource_t(const owned_t<reader_t>& reader)
    : $format(parse_format(reader)) {

    }

    const sprite_format_t& sprite_resource_t::format() const {
      return $format;
    }

    sprite_format_t sprite_resource_t::parse_format(const owned_t<reader_t>& reader) const {
      auto cursor = 0;
      auto format = sprite_format_t(reader, cursor);
      SESAME_FAIL_IF(cursor != reader->length());
      return format;
    }
  }
}
