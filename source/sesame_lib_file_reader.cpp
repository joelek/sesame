#include "sesame_lib_file_reader.hpp"

namespace sesame {
  namespace lib {
    file_reader_t::file_reader_t()
    : $file() {

    }

    file_reader_t::file_reader_t(const char* filename)
    : $file(filename, u8"rb") {

    }

    auto file_reader_t::length() const
      -> sinw {
      return $file.length();
    }

    auto file_reader_t::read(sinw offset, sinw length, ui08* target)
      -> void {
      $file.seek(offset);
      $file.read(length, target);
    }
  }
}
