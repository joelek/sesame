#include "sesame_lib_file_writer.hpp"

namespace sesame {
  namespace lib {
    file_writer_t::file_writer_t()
    : $file() {

    }

    file_writer_t::file_writer_t(const char* filename)
    : $file(filename, u8"wb") {

    }

    auto file_writer_t::length() const
      -> sinw {
      return $file.length();
    }

    auto file_writer_t::write(sinw offset, sinw length, const ui08* source)
      -> void {
      $file.seek(offset);
      $file.write(length, source);
    }
  }
}
