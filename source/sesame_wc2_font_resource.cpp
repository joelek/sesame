// [dirty]
#include "sesame_wc2_font_resource.hpp"

namespace sesame {
  namespace wc2 {
    character_format_t::character_format_t(owned_t<reader_t> reader, sinw& cursor)
    : width(reader, cursor)
    , height(reader, cursor)
    , offset_x(reader, cursor)
    , offset_y(reader, cursor) {

    }

    sinw character_format_t::get_width() const {
      return this->width;
    }

    sinw character_format_t::get_height() const {
      return this->height;
    }

    sinw character_format_t::get_offset_x() const {
      return this->offset_x;
    }

    sinw character_format_t::get_offset_y() const {
      return this->offset_y;
    }
  }
}
/*
namespace sesame {
  namespace wc2 {
    character_stream_t::character_stream_t(owned_t<reader_t> reader, sinw cursor, sinw width, sinw height)
    : format(reader, cursor)
    , reader(reader)
    , cursor(cursor)
    , width(width)
    , height(height)
    , x(0)
    , y(0)
    , encoding_length(0)
    , encoding_color(0) {
      SESAME_FAIL_IF(width < 0);
      SESAME_FAIL_IF(height < 0);
    }

    sinw character_stream_t::length() const {
      return this->width * this->height;
    }

    void character_stream_t::read(sinw length, ui08* target) {
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(this->y * this->width + this->x + length > this->length());
      SESAME_FAIL_IF(target == nullptr);
      for (auto i = 0; i < length; i++) {
        auto pad = false;
        pad = pad || (this->x < this->format.get_offset_x());
        pad = pad || (this->y < this->format.get_offset_y());
        pad = pad || (this->x >= this->format.get_offset_x() + this->format.get_width());
        pad = pad || (this->y >= this->format.get_offset_y() + this->format.get_height());
        if (pad) {
          target[i] = 0;
        } else {
          if (this->encoding_length == 0) {
            auto encoding = primitive_le_t<ui08>(this->reader, this->cursor);
            this->encoding_length = (encoding >> 3) + 1;
            this->encoding_color = (encoding & 0x07);
          }
          if (this->encoding_length == 1) {
            target[i] = this->encoding_color;
          } else {
            target[i] = 0;
          }
          this->encoding_length -= 1;
        }
        this->x += 1;
        if (this->x >= this->width) {
          this->y += 1;
          this->x = 0;
        }
      }
    }
  }
}
*/
namespace sesame {
  namespace wc2 {
    font_format_t::font_format_t(owned_t<reader_t> reader, sinw cursor)
    : identifier(reader, cursor)
    , first_character(reader, cursor)
    , last_character(reader, cursor)
    , width(reader, cursor)
    , height(reader, cursor)
    , character_offsets(last_character - first_character + 1, reader, cursor)
    , reader(reader) {
      SESAME_FAIL_IF(identifier != primitive_le_t<ui32>(u8"FONT"));
      for (auto i = 0; i < this->last_character - this->first_character + 1; i++) {
        cursor = this->character_offsets.at(i);
        SESAME_FAIL_IF(cursor >= reader->length());
        if (cursor > 0) {
          auto format = character_format_t(reader, cursor);
          SESAME_FAIL_IF(format.get_offset_x() + format.get_width() > this->width);
          SESAME_FAIL_IF(format.get_offset_y() + format.get_height() > this->height);
        }
      }
    }

    sinw font_format_t::get_identifier() const {
      return this->identifier;
    }

    sinw font_format_t::get_first_character() const {
      return this->first_character;
    }

    sinw font_format_t::get_last_character() const {
      return this->last_character;
    }

    sinw font_format_t::get_width() const {
      return this->width;
    }

    sinw font_format_t::get_height() const {
      return this->height;
    }

    sinw font_format_t::get_character_offset(sinw index) const {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= this->last_character - this->first_character + 1);
      return this->character_offsets.at(index);
    }

    owned_t<reader_t> font_format_t::get_data_reader(sinw character) const {
      SESAME_FAIL_IF(character < 0);
      SESAME_FAIL_IF(character > 255);
      if (character >= this->first_character && character <= this->last_character) {
        auto cursor = this->character_offsets.at(character - this->first_character);
        if (cursor > 0) {
          //auto stream = make<character_stream_t>(this->reader, cursor, this->width, this->height);
          //return make<buffered_reader_t>(stream);
          return nullptr;
        }
      }
      return nullptr;
    }
  }
}

namespace sesame {
  namespace wc2 {
    font_resource_t::font_resource_t(owned_t<reader_t> reader)
    : format(make<font_format_t>(reader, 0)) {

    }

    owned_t<font_format_t> font_resource_t::get_format() const {
      return this->format;
    }
  }
}
