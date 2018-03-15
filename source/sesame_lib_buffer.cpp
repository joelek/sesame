#include "sesame_lib_buffer.hpp"

namespace sesame {
  namespace lib {
    buffer_t::buffer_t()
    : $length()
    , $pointer() {

    }

    buffer_t::buffer_t(sinw length)
    : $length(length)
    , $pointer() {
      SESAME_FAIL_IF($length < 0);
      if ($length > 0) {
        $pointer = static_cast<ui08*>(malloc($length));
        SESAME_FAIL_IF($pointer == nullptr);
      }
    }

    buffer_t::buffer_t(sinw length, ui08 value)
    : $length(length)
    , $pointer() {
      SESAME_FAIL_IF($length < 0);
      if ($length > 0) {
        $pointer = static_cast<ui08*>(malloc($length));
        SESAME_FAIL_IF($pointer == nullptr);
        for (auto i = 0; i < $length; i++) {
          $pointer[i] = value;
        }
      }
    }

    buffer_t::buffer_t(owned_t<reader_t> reader)
    : $length(reader->length())
    , $pointer() {
      if ($length > 0) {
        $pointer = static_cast<ui08*>(malloc($length));
        SESAME_FAIL_IF($pointer == nullptr);
        reader->read(0, $length, $pointer);
      }
    }

    buffer_t::buffer_t(const buffer_t& that)
    : $length(that.$length)
    , $pointer() {
      if ($length > 0) {
        $pointer = static_cast<ui08*>(malloc($length));
        SESAME_FAIL_IF($pointer == nullptr);
        for (auto i = 0; i < $length; i++) {
          $pointer[i] = that.$pointer[i];
        }
      }
    }

    buffer_t::buffer_t(buffer_t&& that)
    : $length(pass(that.$length))
    , $pointer(pass(that.$pointer)) {
      new (&that) buffer_t();
    }

    buffer_t::~buffer_t() {
      free($pointer);
    }

    auto buffer_t::operator =(buffer_t that)
      -> buffer_t& {
      return swap(self, that);
    }

    auto buffer_t::length()
      -> sinw& {
      return $length;
    }

    auto buffer_t::length() const
      -> const sinw& {
      return $length;
    }

    auto buffer_t::pointer()
      -> ui08* {
      return $pointer;
    }

    auto buffer_t::pointer() const
      -> const ui08* {
      return $pointer;
    }

    auto buffer_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      writer->write(cursor, $length, $pointer);
      cursor += $length;
    }
  }
}
