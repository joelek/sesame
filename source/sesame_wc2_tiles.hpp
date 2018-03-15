// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_resource.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class tiles_format_t {
      public:
        tiles_format_t(const owned_t<reader_t>& reader, sinw& cursor);
        sinw number_of_tiles() const;
        owned_t<reader_t> get_tile(sinw index) const;
      protected:
      private:
        sinw $number_of_tiles;
        owned_t<reader_t> $reader;
    };
  }
}

namespace sesame {
  namespace wc2 {
    class tiles_resource_t: public resource_t {
      public:
        tiles_resource_t(const owned_t<reader_t>& reader);
        const tiles_format_t& format() const;
      protected:
        tiles_format_t parse_format(const owned_t<reader_t>& reader) const;
      private:
        tiles_format_t $format;
    };
  }
}
