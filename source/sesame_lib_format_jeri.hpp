#pragma once

#include "sesame_lib_format_jeri_header.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_image.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    class format_jeri_t {
      public:
        format_jeri_t();
        format_jeri_t(owned_t<image_t> image);
        format_jeri_t(owned_t<reader_t> reader, sinw& cursor);
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        owned_t<image_t> $image;
    };
  }
}
