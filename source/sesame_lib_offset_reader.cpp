#include "sesame_lib_offset_reader.hpp"

namespace sesame {
  namespace lib {
    offset_reader_t::offset_reader_t()
    : $reader()
    , $offset()
    , $length() {

    }

    offset_reader_t::offset_reader_t(owned_t<reader_t> reader, sinw offset)
    : $reader(reader)
    , $offset(offset)
    , $length(reader->length() - offset) {
      SESAME_FAIL_IF($offset < 0);
      SESAME_FAIL_IF($length < 0);
      SESAME_FAIL_IF($offset + $length > $reader->length());
    }

    offset_reader_t::offset_reader_t(owned_t<reader_t> reader, sinw offset, sinw length)
    : $reader(reader)
    , $offset(offset)
    , $length(length) {
      SESAME_FAIL_IF($offset < 0);
      SESAME_FAIL_IF($length < 0);
      SESAME_FAIL_IF($offset + $length > $reader->length());
    }

    auto offset_reader_t::length() const
      -> sinw {
      return $length;
    }

    auto offset_reader_t::read(sinw offset, sinw length, ui08* target)
      -> void {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(offset + length > $length);
      SESAME_FAIL_IF(target == nullptr);
      $reader->read($offset + offset, length, target);
    }
  }
}
