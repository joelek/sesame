#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_image.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    namespace {
      template <signed B> constexpr
      auto mask()
        -> signed {
        return ((1 << B) - 1);
      }

      template <signed C, signed B, signed W> constexpr
      auto pack()
        -> signed {
        return ((((C * W) << B) + (8 - 1)) >> 3);
      }

      template <signed C, signed B, signed X> constexpr
      auto skip()
        -> signed {
        return (8 - ((((X * C) & (8 >> B)) + 1) << B));
      }
    }
  }
}

namespace sesame {
  namespace lib {
    template <signed F, signed C, signed B, signed W, signed H>
    class bitmap_t {
        static_assert(F >= 1, u8"");
        static_assert((C >= 1 && C <= 4), u8"");
        static_assert((B >= 0 && B <= 3), u8"");
        static_assert(W >= 1, u8"");
        static_assert(H >= 1, u8"");
      public:
        bitmap_t();
        bitmap_t(owned_t<reader_t> reader, sinw& cursor);
        auto data()
          -> ui08 (&)[F][H][pack<C, B, W>()];
        auto data() const
          -> const ui08 (&)[F][H][pack<C, B, W>()];
        auto write(owned_t<writer_t> writer, sinw& cursor)
          -> void;
      protected:
      private:
        ui08 $data[F][H][pack<C, B, W>()];
    };
  }
}

namespace sesame {
  namespace lib {
    template <signed F, signed C, signed B, signed W, signed H>
    bitmap_t<F, C, B, W, H>::bitmap_t()
    : $data() {

    }

    template <signed F, signed C, signed B, signed W, signed H>
    bitmap_t<F, C, B, W, H>::bitmap_t(owned_t<reader_t> reader, sinw& cursor)
    : $data() {
      auto length = size($data);
      reader->read(cursor, length, &$data[0][0][0]);
      cursor += length;
    }

    template <signed F, signed C, signed B, signed W, signed H>
    auto bitmap_t<F, C, B, W, H>::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      auto length = size($data);
      writer->write(cursor, length, &$data[0][0][0]);
      cursor += length;
    }
  }
}

namespace sesame {
  namespace lib {
    template <signed W, signed H>
    using bitmap_1bit_t = bitmap_t<1, 1, 0, W, H>;
    template <signed W, signed H>
    using bitmap_2bit_t = bitmap_t<1, 1, 1, W, H>;
    template <signed W, signed H>
    using bitmap_4bit_t = bitmap_t<1, 1, 2, W, H>;
    template <signed W, signed H>
    using bitmap_8bit_t = bitmap_t<1, 1, 3, W, H>;
    template <signed W, signed H>
    using bitmap_24bit_t = bitmap_t<1, 3, 3, W, H>;
    template <signed W, signed H>
    using bitmap_32bit_t = bitmap_t<1, 4, 3, W, H>;
  }
}
