#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_sequence.hpp"

namespace sesame {
  namespace lib {
    template <typename T, signed N>
    class array_t: public sequence_t<T> {
      public:
        array_t();
        template <typename ... A>
        array_t(A&& ... arguments);
        auto at(sinw index) override
          -> T&;
        auto at(sinw index) const override
          -> const T&;
        auto length() const override
          -> sinw;
      protected:
      private:
        T $elements[N];
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T, signed N>
    array_t<T, N>::array_t()
    : $elements() {

    }

    template <typename T, signed N>
    template <typename ... A>
    array_t<T, N>::array_t(A&& ... arguments)
    : $elements() {
      for (auto i = 0; i < N; i++) {
        $elements[i] = T(static_cast<A&&>(arguments)...);
      }
    }

    template <typename T, signed N>
    auto array_t<T, N>::at(sinw index)
      -> T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= N);
      return $elements[index];
    }

    template <typename T, signed N>
    auto array_t<T, N>::at(sinw index) const
      -> const T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= N);
      return $elements[index];
    }

    template <typename T, signed N>
    auto array_t<T, N>::length() const
      -> sinw {
      return N;
    }
  }
}
