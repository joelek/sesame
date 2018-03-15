#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_sequence.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class sequence_iterator_t {
      public:
        sequence_iterator_t(sequence_t<T>& sequence, sinw index);
        sequence_iterator_t(const sequence_t<T>& sequence, sinw index);
        auto operator *()
          -> T&;
        auto operator *() const
          -> const T&;
        auto operator ++()
          -> sequence_iterator_t<T>&;
        auto operator !=(const sequence_iterator_t<T>& that) const
          -> bool;
      private:
        sequence_t<T>& $sequence;
        sinw $index;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    sequence_iterator_t<T>::sequence_iterator_t(sequence_t<T>& sequence, sinw index)
    : $sequence(sequence)
    , $index(index) {

    }

    template <typename T>
    sequence_iterator_t<T>::sequence_iterator_t(const sequence_t<T>& sequence, sinw index)
    : $sequence(const_cast<sequence_t<T>&>(sequence))
    , $index(index) {

    }

    template <typename T>
    auto sequence_iterator_t<T>::operator *()
      -> T& {
      return $sequence.at($index);
    }

    template <typename T>
    auto sequence_iterator_t<T>::operator *() const
      -> const T& {
      return $sequence.at($index);
    }

    template <typename T>
    auto sequence_iterator_t<T>::operator ++()
      -> sequence_iterator_t<T>& {
      $index += 1;
      return self;
    }

    template <typename T>
    auto sequence_iterator_t<T>::operator !=(const sequence_iterator_t<T>& that) const
      -> bool {
      if ($index != that.$index) {
        return true;
      }
      return false;
    }
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    auto begin(sequence_t<T>& sequence)
      -> sequence_iterator_t<T> {
      return sequence_iterator_t<T>(sequence, 0);
    }

    template <typename T>
    auto end(sequence_t<T>& sequence)
      -> sequence_iterator_t<T> {
      return sequence_iterator_t<T>(sequence, sequence.length());
    }

    template <typename T>
    auto begin(const sequence_t<T>& sequence)
      -> const sequence_iterator_t<T> {
      return sequence_iterator_t<T>(sequence, 0);
    }

    template <typename T>
    auto end(const sequence_t<T>& sequence)
      -> const sequence_iterator_t<T> {
      return sequence_iterator_t<T>(sequence, sequence.length());
    }
  }
}
