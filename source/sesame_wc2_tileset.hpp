// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class tileset_format_tile_entry_t {
      public:
        tileset_format_tile_entry_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw is_inverted_y() const;
        sinw is_inverted_x() const;
        sinw index() const;
      protected:
      private:
        primitive_le_t<ui16> $bits;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class tileset_format_tile_t {
      public:
        tileset_format_tile_t(const owned_t<reader_t>& reader, sinw& cursor);
        const matrix_t<tileset_format_tile_entry_t>& entries() const;
      protected:
      private:
        matrix_t<tileset_format_tile_entry_t> $entries;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class tileset_format_t {
      public:
        tileset_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        const vector_t<tileset_format_tile_t>& tiles() const;
      protected:
      private:
        vector_t<tileset_format_tile_t> $tiles;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class tileset_resource_t: public resource_t {
      public:
        tileset_resource_t(const owned_t<reader_t>& reader);
        const tileset_format_t& format() const;
      protected:
        tileset_format_t parse_format(const owned_t<reader_t>& reader) const;
      private:
        tileset_format_t $format;
    };
  }
}
