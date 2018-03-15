// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class palette_format_t {
      public:
        palette_format_t(owned_t<reader_t> reader, sinw cursor);
        owned_t<reader_t> get_data_reader() const;
      protected:
      private:
        owned_t<reader_t> data_reader;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class palette_resource_t: public resource_t {
      public:
        palette_resource_t(owned_t<reader_t> reader);
        owned_t<palette_format_t> get_format() const;
      protected:
      private:
        owned_t<palette_format_t> format;
    };
  }
}
