#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_system.hpp"

namespace sesame {
  namespace lib {
    namespace memory {
      template <signed N, signed O, signed L>
      auto decode_bits(const ui08 (&bytes)[N])
        -> uinw;
      template <signed N, signed O, signed L>
      auto encode_bits(ui08 (&bytes)[N], uinw value)
        -> void;
      template <typename T>
      auto reinterpret_as_bytes(T& value)
        -> ui08 (&)[size<T>()];
      template <typename T>
      auto reinterpret_as_bytes(const T& value)
        -> const ui08 (&)[size<T>()];
      template <signed N>
      auto swap_byte_order(ui08 (&bytes)[N])
        -> void;
    }
  }
}

namespace sesame {
  namespace lib {
    namespace memory {
      template <signed N, signed O, signed L>
      auto decode_bits(const ui08 (&bytes)[N])
        -> uinw {
        static_assert(O >= 0, u8"");
        static_assert(L >= 1, u8"");
        static_assert(L <= size<uinw>() * 8, u8"");
        static_assert((O + L) <= (N * 8), u8"");
        auto lower_bit = (O);
        auto upper_bit = (O + L - 1);
        auto lower_byte = (lower_bit / 8);
        auto upper_byte = (upper_bit / 8);
        lower_bit -= (lower_byte * 8);
        upper_bit -= (upper_byte * 8);
        auto lower_mask = ~((1 << lower_bit) - 1);
        auto upper_mask = ((2 << upper_bit) - 1);
        auto value = uinw(0);
        for (auto i = lower_byte; i <= upper_byte; i++) {
          auto mask = ui08(~0);
          if (i == lower_byte) {
            mask &= lower_mask;
          }
          if (i == upper_byte) {
            mask &= upper_mask;
          }
          auto byte = (bytes[i] & mask);
          auto shift_l = ((i - lower_byte) * 8);
          auto shift_r = (lower_bit);
          if (shift_r > shift_l) {
            value |= (byte >> (shift_r - shift_l));
          } else {
            value |= (byte << (shift_l - shift_r));
          }
        }
        return value;
      }

      template <signed N, signed O, signed L>
      auto encode_bits(ui08 (&bytes)[N], uinw value)
        -> void {
        static_assert(O >= 0, u8"");
        static_assert(L >= 1, u8"");
        static_assert(L <= size<uinw>() * 8, u8"");
        static_assert((O + L) <= (N * 8), u8"");
        auto lower_bit = (O);
        auto upper_bit = (O + L - 1);
        auto lower_byte = (lower_bit / 8);
        auto upper_byte = (upper_bit / 8);
        lower_bit -= (lower_byte * 8);
        upper_bit -= (upper_byte * 8);
        auto lower_mask = ~((1 << lower_bit) - 1);
        auto upper_mask = ((2 << upper_bit) - 1);
        for (auto i = lower_byte; i <= upper_byte; i++) {
          auto mask = ui08(~0);
          if (i == lower_byte) {
            mask &= lower_mask;
          }
          if (i == upper_byte) {
            mask &= upper_mask;
          }
          auto byte = (bytes[i] & ~mask);
          auto shift_l = (lower_bit);
          auto shift_r = ((i - lower_byte) * 8);
          if (shift_r > shift_l) {
            byte |= ((value >> (shift_r - shift_l)) & mask);
          } else {
            byte |= ((value << (shift_l - shift_r)) & mask);
          }
          bytes[i] = byte;
        }
      }

      template <typename T>
      auto reinterpret_as_bytes(T& value)
        -> ui08 (&)[size<T>()] {
        return reinterpret_cast<ui08 (&)[size<T>()]>(value);
      }

      template <typename T>
      auto reinterpret_as_bytes(const T& value)
        -> const ui08 (&)[size<T>()] {
        return reinterpret_cast<const ui08 (&)[size<T>()]>(value);
      }

      template <signed N>
      auto swap_byte_order(ui08 (&bytes)[N])
        -> void {
        for (auto a = 0, b = N - 1; a < b; a++, b--) {
          bytes[a] ^= bytes[b];
          bytes[b] ^= bytes[a];
          bytes[a] ^= bytes[b];
        }
      }
    }
  }
}
