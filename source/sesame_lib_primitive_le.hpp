#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_memory.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_system.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class primitive_le_t {
      public:
        primitive_le_t();
        primitive_le_t(T value);
        primitive_le_t(owned_t<reader_t> reader, sinw& cursor);
        primitive_le_t(const ui08 (&data)[size<T>()]);
        primitive_le_t(const char (&cstr)[size<T>() + 1]);
        operator T() const;
        template <signed O, signed L>
        auto decode() const
          -> sinw;
        template <signed O, signed L>
        auto encode(sinw value)
          -> void;
        auto value()
          -> T&;
        auto value() const
          -> const T&;
        auto write(owned_t<writer_t> writer, sinw& cursor) const
          -> void;
      protected:
      private:
        T $value;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    primitive_le_t<T>::primitive_le_t()
    : $value() {

    }

    template <typename T>
    primitive_le_t<T>::primitive_le_t(T value)
    : $value(pass(value)) {

    }

    template <typename T>
    primitive_le_t<T>::primitive_le_t(owned_t<reader_t> reader, sinw& cursor)
    : $value() {
      auto& bytes = memory::reinterpret_as_bytes($value);
      reader->read(cursor, size<T>(), bytes);
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
      cursor += size<T>();
    }

    template <typename T>
    primitive_le_t<T>::primitive_le_t(const ui08 (&data)[size<T>()])
    : $value() {
      auto& bytes = memory::reinterpret_as_bytes($value);
      for (auto i = 0; i < size<T>(); i++) {
        bytes[i] = data[i];
      }
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
    }

    template <typename T>
    primitive_le_t<T>::primitive_le_t(const char (&cstr)[size<T>() + 1])
    : $value() {
      auto& bytes = memory::reinterpret_as_bytes($value);
      for (auto i = 0; i < size<T>(); i++) {
        bytes[i] = cstr[i];
      }
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
    }

    template <typename T>
    primitive_le_t<T>::operator T() const {
      return $value;
    }

    template <typename T>
    template <signed O, signed L>
    auto primitive_le_t<T>::decode() const
      -> sinw {
      static_assert(O >= 0, u8"");
      static_assert(L >= 1, u8"");
      static_assert(L <= size<uinw>() * 8 - 1, u8"");
      static_assert(O + L <= size<T>() * 8, u8"");
      auto value = $value;
      auto& bytes = memory::reinterpret_as_bytes(value);
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
      return memory::decode_bits<size<T>(), O, L>(bytes);
    }

    template <typename T>
    template <signed O, signed L>
    auto primitive_le_t<T>::encode(sinw value)
      -> void {
      static_assert(O >= 0, u8"");
      static_assert(L >= 1, u8"");
      static_assert(L <= size<uinw>() * 8 - 1, u8"");
      static_assert(O + L <= size<T>() * 8, u8"");
      auto max_value = ((((1 << (L - 1)) - 1) << 1) + 1);
      SESAME_FAIL_IF(value < 0);
      SESAME_FAIL_IF(value > max_value);
      auto& bytes = memory::reinterpret_as_bytes($value);
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
      memory::encode_bits<size<T>(), O, L>(bytes, value);
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
    }

    template <typename T>
    auto primitive_le_t<T>::value()
      -> T& {
      return $value;
    }

    template <typename T>
    auto primitive_le_t<T>::value() const
      -> const T& {
      return $value;
    }

    template <typename T>
    auto primitive_le_t<T>::write(owned_t<writer_t> writer, sinw& cursor) const
      -> void {
      auto value = $value;
      auto& bytes = memory::reinterpret_as_bytes(value);
      if (system::uses_big_endian()) {
        memory::swap_byte_order(bytes);
      }
      writer->write(cursor, size<T>(), bytes);
      cursor += size<T>();
    }
  }
}

namespace sesame {
  namespace lib {
    using fp32le = primitive_le_t<fp32>;
    using fp64le = primitive_le_t<fp64>;
    using si08le = primitive_le_t<si08>;
    using si16le = primitive_le_t<si16>;
    using si32le = primitive_le_t<si32>;
    using si64le = primitive_le_t<si64>;
    using ui08le = primitive_le_t<ui08>;
    using ui16le = primitive_le_t<ui16>;
    using ui32le = primitive_le_t<ui32>;
    using ui64le = primitive_le_t<ui64>;
  }
}
