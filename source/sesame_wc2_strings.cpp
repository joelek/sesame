// [dirty]
#include "sesame_wc2_strings.hpp"

namespace sesame {
  namespace wc2 {
    strings_format_t::strings_format_t(const owned_t<reader_t>& reader, sinw& cursor)
    : $number_of_strings(reader, cursor)
    , $strings($number_of_strings, reader, cursor) {
      if ($number_of_strings == 0) {
        SESAME_FAIL_IF(cursor != reader->length());
      } else {
        for (auto i = 0; i < $number_of_strings; i++) {
          auto reader = $strings.at(i);
          auto value = ui08(0);
          reader->read(reader->length() - 1, 1, &value);
          SESAME_FAIL_IF(value != 0);
        }
      }
      cursor = reader->length();
    }

    sinw strings_format_t::number_of_strings() const {
      return $number_of_strings;
    }

    owned_t<reader_t> strings_format_t::get_string(sinw index) const {
      return $strings.at(index);
    }
  }
}

namespace sesame {
  namespace wc2 {
    strings_resource_t::strings_resource_t(const owned_t<reader_t>& reader)
    : $format(parse_format(reader)) {

    }

    const strings_format_t& strings_resource_t::format() const {
      return $format;
    }

    strings_format_t strings_resource_t::parse_format(const owned_t<reader_t>& reader) const {
      auto cursor = 0;
      auto format = strings_format_t(reader, cursor);
      SESAME_FAIL_IF(cursor != reader->length());
      return format;
    }
  }
}
