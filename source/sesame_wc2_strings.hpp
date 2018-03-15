// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class strings_format_t {
      public:
        strings_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw number_of_strings() const;
        owned_t<reader_t> get_string(sinw index) const;
      protected:
      private:
        primitive_le_t<ui16> $number_of_strings;
        segmented_reader_t<primitive_le_t<ui16>> $strings;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class strings_resource_t: public resource_t {
      public:
        strings_resource_t(const owned_t<reader_t>& reader);
        const strings_format_t& format() const;
      protected:
        strings_format_t parse_format(const owned_t<reader_t>& reader) const;
      private:
        strings_format_t $format;
    };
  }
}
