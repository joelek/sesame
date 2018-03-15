#include "sesame_wc1_record.hpp"

namespace sesame {
  namespace wc1 {
    record_t::record_t()
    : $reader() {

    }

    record_t::record_t(owned_t<reader_t> reader)
    : $reader(pass(reader)) {

    }

    record_t::record_t(owned_t<reader_t> reader, sinw& cursor)
    : $reader() {
      auto header = si32le(reader, cursor);
      auto length = header.decode<0, 20>();
      auto unknown_a = header.decode<20, 9>();
      auto is_compressed = header.decode<29, 1>();
      auto unknown_b = header.decode<30, 2>();
      SESAME_FAIL_IF(unknown_a != 0);
      SESAME_FAIL_IF(unknown_b != 0);
      if (is_compressed) {
        auto offset_reader = make<offset_reader_t>(reader, cursor);
        $reader = make<lzss_reader_t>(offset_reader, length);
      } else {
        $reader = make<offset_reader_t>(reader, cursor, length);
      }
      cursor = reader->length();
    }

    auto record_t::reader()
      -> owned_t<reader_t>& {
      return $reader;
    }

    auto record_t::reader() const
      -> const owned_t<reader_t>& {
      return $reader;
    }

    auto record_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      auto header = si32le();
      header.encode<0, 20>($reader->length());
      header.write(writer, cursor);
      auto buffer = buffer_t($reader);
      buffer.write(writer, cursor);
    }
  }
}
