// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class table_format_t {
      public:
        table_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw width() const;
        sinw height() const;
        const matrix_t<primitive_le_t<ui16>>& data() const;
      protected:
      private:
        primitive_le_t<ui16> $width;
        primitive_le_t<ui16> $height;
        matrix_t<primitive_le_t<ui16>> $data;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class table_resource_t: public resource_t {
      public:
        table_resource_t(const owned_t<reader_t>& reader);
        const table_format_t& format() const;
      protected:
        table_format_t parse_format(const owned_t<reader_t>& reader) const;
      private:
        table_format_t $format;
    };
  }
}
