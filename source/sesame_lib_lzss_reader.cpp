#include "sesame_lib_lzss_reader.hpp"

namespace sesame {
  namespace lib {
    lzss_reader_t::lzss_reader_t()
    : $reader()
    , $length()
    , $length_unpacked()
    , $buffer()
    , $history()
    , $history_position()
    , $cursor() {

    }

    lzss_reader_t::lzss_reader_t(owned_t<reader_t> reader, sinw length)
    : $reader(reader)
    , $length(length)
    , $length_unpacked()
    , $buffer($length)
    , $history()
    , $history_position()
    , $cursor() {
      SESAME_FAIL_IF($length < 0);
    }

    auto lzss_reader_t::length() const
      -> sinw {
      return $length;
    }

    auto lzss_reader_t::read(sinw offset, sinw length, ui08* target)
      -> void {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(length < 0);
      SESAME_FAIL_IF(offset + length > $length);
      SESAME_FAIL_IF(target == nullptr);
      while ($length_unpacked < offset + length) {
        auto control_byte = ui08le($reader, $cursor).value();
        for (auto i = 0; i < 8; i++) {
          if (control_byte & 1) {
            append(ui08le($reader, $cursor).value());
          } else {
            auto data = ui16le($reader, $cursor);
            auto offset = data.decode<0, 12>();
            auto length = data.decode<12, 4>() + 3;
            for (auto j = offset; j < offset + length; j++) {
              append($history[j % (1 << 12)]);
            }
          }
          if ($length_unpacked >= $length) {
            break;
          }
          control_byte >>= 1;
        }
      }
      for (auto i = 0; i < length; i++) {
        target[i] = $buffer.pointer()[offset + i];
      }
    }

    auto lzss_reader_t::append(ui08 byte)
      -> void {
      $history[$history_position] = byte;
      $history_position += 1;
      $history_position %= 1 << 12;
      $buffer.pointer()[$length_unpacked] = byte;
      $length_unpacked += 1;
    }
  }
}
