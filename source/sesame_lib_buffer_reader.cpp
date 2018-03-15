#include "sesame_lib_buffer_reader.hpp"

namespace sesame {
  namespace lib {
    buffer_reader_t::buffer_reader_t()
    : $buffer() {

    }

    buffer_reader_t::buffer_reader_t(buffer_t buffer)
    : $buffer(pass(buffer)) {

    }

    auto buffer_reader_t::length() const
      -> sinw {
      return $buffer.length();
    }

    auto buffer_reader_t::read(sinw offset, sinw length, ui08* target)
      -> void {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(offset + length > $buffer.length());
      SESAME_FAIL_IF(target == nullptr);
      for (auto i = 0; i < length; i++) {
        target[i] = $buffer.pointer()[offset + i];
      }
    }
  }
}
