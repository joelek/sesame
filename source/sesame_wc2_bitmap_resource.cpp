// [dirty]
#include "sesame_wc2_bitmap_resource.hpp"

namespace sesame {
  namespace wc2 {
    bitmap_format_t::bitmap_format_t(owned_t<reader_t> reader, sinw cursor)
    : width(reader, cursor)
    , height(reader, cursor)
    , data_reader(make<offset_reader_t>(reader, cursor, width * height)) {
      cursor += this->width * this->height;
      SESAME_FAIL_IF(cursor != reader->length());
    }

    sinw bitmap_format_t::get_width() const {
      return this->width;
    }

    sinw bitmap_format_t::get_height() const {
      return this->height;
    }

    owned_t<reader_t> bitmap_format_t::get_data_reader() const {
      return this->data_reader;
    }
  }
}

namespace sesame {
  namespace wc2 {
    bitmap_resource_t::bitmap_resource_t(owned_t<reader_t> reader)
    : format(make<bitmap_format_t>(reader, 0)) {

    }

    owned_t<bitmap_format_t> bitmap_resource_t::get_format() const {
      return this->format;
    }
  }
}
