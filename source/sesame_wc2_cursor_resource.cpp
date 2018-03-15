// [dirty]
#include "sesame_wc2_cursor_resource.hpp"

namespace sesame {
  namespace wc2 {
    cursor_format_t::cursor_format_t(owned_t<reader_t> reader, sinw cursor)
    : origin_x(reader, cursor)
    , origin_y(reader, cursor)
    , width(reader, cursor)
    , height(reader, cursor)
    , data_reader(make<offset_reader_t>(reader, cursor, width * height)) {
      SESAME_FAIL_IF(this->origin_x >= this->width);
      SESAME_FAIL_IF(this->origin_y >= this->height);
      cursor += this->width * this->height;
      SESAME_FAIL_IF(cursor != reader->length());
    }

    sinw cursor_format_t::get_origin_x() const {
      return this->origin_x;
    }

    sinw cursor_format_t::get_origin_y() const {
      return this->origin_y;
    }

    sinw cursor_format_t::get_width() const {
      return this->width;
    }

    sinw cursor_format_t::get_height() const {
      return this->height;
    }

    owned_t<reader_t> cursor_format_t::get_data_reader() const {
      return this->data_reader;
    }
  }
}

namespace sesame {
  namespace wc2 {
    cursor_resource_t::cursor_resource_t(owned_t<reader_t> reader)
    : format(make<cursor_format_t>(reader, 0)) {

    }

    owned_t<cursor_format_t> cursor_resource_t::get_format() const {
      return this->format;
    }
  }
}
