#include "sesame_lib_file.hpp"

namespace sesame {
  namespace lib {
    file_t::file_t()
    : $name()
    , $mode()
    , $handle() {

    }

    file_t::file_t(const char* name, const char* mode)
    : $name(name)
    , $mode(mode)
    , $handle(fopen($name, $mode)) {
      SESAME_FAIL_IF($handle == nullptr);
    }

    file_t::file_t(const file_t& that)
    : $name(that.$name)
    , $mode(that.$mode)
    , $handle(fopen($name, $mode)) {
      SESAME_FAIL_IF($handle == nullptr);
    }

    file_t::file_t(file_t&& that)
    : $name(pass(that.$name))
    , $mode(pass(that.$mode))
    , $handle(pass(that.$handle)) {
      new (&that) file_t();
    }

    file_t::~file_t() {
      SESAME_WARN_IF(fclose($handle) != 0);
    }

    auto file_t::operator =(file_t that)
      -> file_t& {
      return swap(self, that);
    }

    auto file_t::length() const
      -> sinw {
      auto offset = tell();
      SESAME_FAIL_IF(fseek($handle, 0, SEEK_END) != 0);
      auto length = tell();
      SESAME_FAIL_IF(fseek($handle, offset, SEEK_SET) != 0);
      return length;
    }

    auto file_t::read(sinw length, ui08* target) const
      -> void {
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(target == nullptr);
      if (length > 0) {
        SESAME_FAIL_IF(fread(target, length, 1, $handle) != 1);
      }
    }

    auto file_t::seek(sinw offset) const
      -> void {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(fseek($handle, offset, SEEK_SET) != 0);
    }

    auto file_t::tell() const
      -> sinw {
      auto offset = ftell($handle);
      SESAME_FAIL_IF(offset < 0);
      return offset;
    }

    auto file_t::write(sinw length, const ui08* source) const
      -> void {
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(source == nullptr);
      if (length > 0) {
        SESAME_FAIL_IF(fwrite(source, length, 1, $handle) != 1);
      }
    }
  }
}
