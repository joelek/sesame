// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class character_format_t {
      public:
        character_format_t(owned_t<reader_t> reader, sinw& cursor);
        sinw get_width() const;
        sinw get_height() const;
        sinw get_offset_x() const;
        sinw get_offset_y() const;
      protected:
      private:
        primitive_le_t<ui08> width;
        primitive_le_t<ui08> height;
        primitive_le_t<ui08> offset_x;
        primitive_le_t<ui08> offset_y;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class font_format_t {
      public:
        font_format_t(owned_t<reader_t> reader, sinw cursor);
        sinw get_identifier() const;
        sinw get_first_character() const;
        sinw get_last_character() const;
        sinw get_width() const;
        sinw get_height() const;
        sinw get_character_offset(sinw index) const;
        owned_t<reader_t> get_data_reader(sinw character) const;
      protected:
      private:
        primitive_le_t<ui32> identifier;
        primitive_le_t<ui08> first_character;
        primitive_le_t<ui08> last_character;
        primitive_le_t<ui08> width;
        primitive_le_t<ui08> height;
        vector_t<primitive_le_t<ui32>> character_offsets;
        owned_t<reader_t> reader;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class font_resource_t: public resource_t {
      public:
        font_resource_t(owned_t<reader_t> reader);
        owned_t<font_format_t> get_format() const;
      protected:
      private:
        owned_t<font_format_t> format;
    };
  }
}
