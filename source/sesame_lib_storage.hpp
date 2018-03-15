#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_sequence.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class storage_t: public sequence_t<T> {
      public:
        storage_t();
        storage_t(sinw length);
        storage_t(const storage_t<T>& that);
        storage_t(storage_t<T>&& that);
        ~storage_t();
        auto operator =(storage_t<T> that)
          -> storage_t<T>&;
        auto at(sinw index) override
          -> T&;
        auto at(sinw index) const override
          -> const T&;
        auto length() const override
          -> sinw;
      protected:
      private:
        sinw $length;
        T* $elements;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    storage_t<T>::storage_t()
    : $length()
    , $elements() {

    }

    template <typename T>
    storage_t<T>::storage_t(sinw length)
    : $length(length)
    , $elements() {
      SESAME_FAIL_IF($length < 0);
      if ($length > 0) {
        $elements = reinterpret_cast<T*>(malloc(size<T>() * $length));
        SESAME_FAIL_IF($elements == nullptr);
      }
    }

    template <typename T>
    storage_t<T>::storage_t(const storage_t<T>& that)
    : $length(that.$length)
    , $elements() {
      if ($length > 0) {
        $elements = reinterpret_cast<T*>(malloc(size<T>() * $length));
        SESAME_FAIL_IF($elements == nullptr);
      }
    }

    template <typename T>
    storage_t<T>::storage_t(storage_t<T>&& that)
    : $length(pass(that.$length))
    , $elements(pass(that.$elements)) {
      new (&that) storage_t<T>();
    }

    template <typename T>
    storage_t<T>::~storage_t() {
      free($elements);
    }

    template <typename T>
    auto storage_t<T>::operator =(storage_t<T> that)
      -> storage_t<T>& {
      return swap(self, that);
    }

    template <typename T>
    auto storage_t<T>::at(sinw index)
      -> T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= $length);
      return $elements[index];
    }

    template <typename T>
    auto storage_t<T>::at(sinw index) const
      -> const T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= $length);
      return $elements[index];
    }

    template <typename T>
    auto storage_t<T>::length() const
      -> sinw {
      return $length;
    }
  }
}
