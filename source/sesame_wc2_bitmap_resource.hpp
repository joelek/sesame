// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class bitmap_format_t {
      public:
        bitmap_format_t(owned_t<reader_t> reader, sinw cursor);
        sinw get_width() const;
        sinw get_height() const;
        owned_t<reader_t> get_data_reader() const;
      protected:
      private:
        primitive_le_t<ui16> width;
        primitive_le_t<ui16> height;
        owned_t<reader_t> data_reader;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class bitmap_resource_t: public resource_t {
      public:
        bitmap_resource_t(owned_t<reader_t> reader);
        owned_t<bitmap_format_t> get_format() const;
      protected:
      private:
        owned_t<bitmap_format_t> format;
    };
  }
}
