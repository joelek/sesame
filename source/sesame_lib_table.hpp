#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_vector.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class table_t {
      public:
        table_t();
        template <typename ... A>
        table_t(sinw width, sinw height, A&& ... arguments);
        auto at(sinw y, sinw x)
          -> T&;
        auto at(sinw y, sinw x) const
          -> const T&;
        auto width() const
          -> sinw;
        auto height() const
          -> sinw;
      protected:
      private:
        sinw $width;
        sinw $height;
        vector_t<T> $vector;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    table_t<T>::table_t()
    : $width()
    , $height()
    , $vector() {

    }

    template <typename T>
    template <typename ... A>
    table_t<T>::table_t(sinw width, sinw height, A&& ... arguments)
    : $width(width)
    , $height(height)
    , $vector((width * height), static_cast<A&&>(arguments)...) {
      SESAME_FAIL_IF(width < 0);
      SESAME_FAIL_IF(height < 0);
    }

    template <typename T>
    auto table_t<T>::at(sinw y, sinw x)
      -> T& {
      SESAME_FAIL_IF(y < 0);
      SESAME_FAIL_IF(y >= $height);
      SESAME_FAIL_IF(x < 0);
      SESAME_FAIL_IF(x >= $width);
      return $vector.at((y * $width) + x);
    }

    template <typename T>
    auto table_t<T>::at(sinw y, sinw x) const
      -> const T& {
      SESAME_FAIL_IF(y < 0);
      SESAME_FAIL_IF(y >= $height);
      SESAME_FAIL_IF(x < 0);
      SESAME_FAIL_IF(x >= $width);
      return $vector.at((y * $width) + x);
    }

    template <typename T>
    auto table_t<T>::width() const
      -> sinw {
      return $width;
    }

    template <typename T>
    auto table_t<T>::height() const
      -> sinw {
      return $height;
    }
  }
}
