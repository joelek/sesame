#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_offset_reader.hpp"
#include "sesame_lib_owned.hpp"
#include "sesame_lib_reader.hpp"
#include "sesame_lib_vector.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class offset_table_t {
      public:
        offset_table_t();
        offset_table_t(sinw number_of_offsets, owned_t<reader_t> reader, sinw& cursor);
        auto readers()
          -> vector_t<owned_t<reader_t>>&;
        auto readers() const
          -> const vector_t<owned_t<reader_t>>&;
      protected:
      private:
        vector_t<owned_t<reader_t>> $readers;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    offset_table_t<T>::offset_table_t()
    : $readers() {

    }

    template <typename T>
    offset_table_t<T>::offset_table_t(sinw number_of_offsets, owned_t<reader_t> reader, sinw& cursor)
    : $readers(number_of_offsets) {
      SESAME_FAIL_IF(number_of_offsets < 0);
      auto offsets = vector_t<T>(number_of_offsets, reader, cursor);
      auto last_offset = 0 - 1;
      for (const auto& offset : offsets) {
        SESAME_FAIL_IF(offset < cursor);
        SESAME_FAIL_IF(offset >= reader->length());
        SESAME_FAIL_IF(offset <= last_offset);
        last_offset = offset;
      }
      for (auto i = 0; i < offsets.length(); i++) {
        auto offset = offsets.at(i);
        auto length = 0 - offset;
        if (i + 1 < offsets.length()) {
          length += offsets.at(i + 1);
        } else {
          length += reader->length();
        }
        $readers.at(i) = make<offset_reader_t>(reader, offset, length);
      }
    }

    template <typename T>
    auto offset_table_t<T>::readers()
      -> vector_t<owned_t<reader_t>>& {
      return $readers;
    }

    template <typename T>
    auto offset_table_t<T>::readers() const
      -> const vector_t<owned_t<reader_t>>& {
      return $readers;
    }
  }
}
