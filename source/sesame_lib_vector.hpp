#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_sequence.hpp"
#include "sesame_lib_storage.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class vector_t: public sequence_t<T> {
      public:
        vector_t();
        template <typename ... A>
        vector_t(sinw length, A&& ... arguments);
        vector_t(const vector_t<T>& that);
        vector_t(vector_t<T>&& that);
        ~vector_t();
        auto operator =(vector_t<T> that)
          -> vector_t<T>&;
        auto at(sinw index) override
          -> T&;
        auto at(sinw index) const override
          -> const T&;
        auto length() const override
          -> sinw;
        template <typename U>
        auto map(U (*transform)(const T&)) const
          -> vector_t<U>;
        auto push(T element)
          -> vector_t<T>&;
      protected:
      private:
        storage_t<T> $storage;
        sinw $length;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    vector_t<T>::vector_t()
    : $storage()
    , $length() {

    }

    template <typename T>
    template <typename ... A>
    vector_t<T>::vector_t(sinw length, A&& ... arguments)
    : $storage(length)
    , $length(length) {
      SESAME_FAIL_IF($length < 0);
      for (auto i = 0; i < $length; i++) {
        try {
          new (&$storage.at(i)) T(static_cast<A&&>(arguments)...);
        } catch (...) {
          for (i--; i >= 0; i--) {
            (&$storage.at(i))->~T();
          }
          SESAME_FAIL_IF(true);
        }
      }
    }

    template <typename T>
    vector_t<T>::vector_t(const vector_t<T>& that)
    : $storage(that.$storage)
    , $length(that.$length) {
      for (auto i = 0; i < $length; i++) {
        try {
          new (&$storage.at(i)) T(that.at(i));
        } catch (...) {
          for (i--; i >= 0; i--) {
            (&$storage.at(i))->~T();
          }
          SESAME_FAIL_IF(true);
        }
      }
    }

    template <typename T>
    vector_t<T>::vector_t(vector_t<T>&& that)
    : $storage(pass(that.$storage))
    , $length(pass(that.$length)) {
      new (&that) vector_t<T>();
    }

    template <typename T>
    vector_t<T>::~vector_t() {
      for (auto i = 0; i < $length; i++) {
        (&$storage.at(i))->~T();
      }
    }

    template <typename T>
    auto vector_t<T>::operator =(vector_t<T> that)
      -> vector_t<T>& {
      return swap(self, that);
    }

    template <typename T>
    auto vector_t<T>::at(sinw index)
      -> T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= $length);
      return $storage.at(index);
    }

    template <typename T>
    auto vector_t<T>::at(sinw index) const
      -> const T& {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= $length);
      return $storage.at(index);
    }

    template <typename T>
    auto vector_t<T>::length() const
      -> sinw {
      return $length;
    }

    template <typename T>
    template <typename U>
    auto vector_t<T>::map(U (*transform)(const T&)) const
      -> vector_t<U> {
      SESAME_FAIL_IF(transform == nullptr);
      auto that = vector_t<U>($length);
      for (auto i = 0; i < $length; i++) {
        that.at(i) = transform(at(i));
      }
      return that;
    }

    template <typename T>
    auto vector_t<T>::push(T element)
      -> vector_t<T>& {
      if ($length >= $storage.length()) {
        auto capacity = ($length > 0) ? $length * 2 : 8;
        auto storage = storage_t<T>(capacity);
        for (auto i = 0; i < $length; i++) {
          new (&storage.at(i)) T(pass(at(i)));
        }
        swap($storage, storage);
      }
      new (&$storage.at($length)) T(pass(element));
      $length += 1;
      return self;
    }
  }
}
