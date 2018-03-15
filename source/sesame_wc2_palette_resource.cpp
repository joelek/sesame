// [dirty]
#include "sesame_wc2_palette_resource.hpp"

namespace sesame {
  namespace wc2 {
    palette_format_t::palette_format_t(owned_t<reader_t> reader, sinw cursor)
    : data_reader(make<offset_reader_t>(reader, cursor, 768)) {
      cursor += 768;
      SESAME_FAIL_IF(cursor != reader->length());
    }

    owned_t<reader_t> palette_format_t::get_data_reader() const {
      return this->data_reader;
    }
  }
}

namespace sesame {
  namespace wc2 {
    palette_resource_t::palette_resource_t(owned_t<reader_t> reader)
    : format(make<palette_format_t>(reader, 0)) {

    }

    owned_t<palette_format_t> palette_resource_t::get_format() const {
      return this->format;
    }
  }
}
