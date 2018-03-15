// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class frame_format_t {
      public:
        frame_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw offset_x() const;
        sinw offset_y() const;
        sinw width() const;
        sinw height() const;
        sinw data_offset() const;
        sinw is_deleted() const;
      protected:
      private:
        primitive_le_t<ui08> $offset_x;
        primitive_le_t<ui08> $offset_y;
        primitive_le_t<ui08> $width;
        primitive_le_t<ui08> $height;
        primitive_le_t<ui32> $bits;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class sprite_format_t {
      public:
        sprite_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw get_number_of_frames() const;
        sinw get_width() const;
        sinw get_height() const;
        owned_t<reader_t> get_data_reader(sinw frame) const;
      protected:
        owned_t<reader_t> get_offset_reader(sinw frame) const;
      private:
        primitive_le_t<ui16> number_of_frames;
        primitive_le_t<ui16> width;
        primitive_le_t<ui16> height;
        vector_t<frame_format_t> formats;
        owned_t<reader_t> reader;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class sprite_resource_t: public resource_t {
      public:
        sprite_resource_t(const owned_t<reader_t>& reader);
        const sprite_format_t& format() const;
      protected:
        sprite_format_t parse_format(const owned_t<reader_t>& reader) const;
      private:
        sprite_format_t $format;
    };
  }
}
