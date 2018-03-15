#pragma once

#include "sesame_lib_header.hpp"
#include "sesame_lib_owned.fwd"

namespace sesame {
  namespace lib {
    template <typename T>
    class borrowed_t {
        template <typename U>
        friend class borrowed_t;
        template <typename U>
        friend class owned_t;
      public:
        borrowed_t();
        borrowed_t(decltype(nullptr) null);
        template <typename U>
        borrowed_t(borrowed_t<U> that);
        template <typename U>
        borrowed_t(owned_t<U> that);
        borrowed_t(const borrowed_t<T>& that);
        borrowed_t(borrowed_t<T>&& that);
        ~borrowed_t();
        explicit operator bool() const;
        auto operator =(borrowed_t<T> that)
          -> borrowed_t<T>&;
        auto operator ->()
          -> T*;
        auto operator ->() const
          -> const T*;
        auto operator *()
          -> T&;
        auto operator *() const
          -> const T&;
      protected:
        auto number_of_borrowers()
          -> sinw&;
        auto number_of_borrowers() const
          -> const sinw&;
        auto number_of_owners()
          -> sinw&;
        auto number_of_owners() const
          -> const sinw&;
        auto pointer()
          -> T*;
        auto pointer() const
          -> const T*;
      private:
        ui08* $storage;
    };
  }
}

namespace sesame {
  namespace lib {
    template <typename T>
    borrowed_t<T>::borrowed_t()
    : $storage() {

    }

    template <typename T>
    borrowed_t<T>::borrowed_t(decltype(nullptr) null)
    : $storage(null) {

    }

    template <typename T>
    template <typename U>
    borrowed_t<T>::borrowed_t(borrowed_t<U> that)
    : $storage(that.$storage) {
      (void)static_cast<T*>(static_cast<U*>(nullptr));
      that.$storage = nullptr;
    }

    template <typename T>
    template <typename U>
    borrowed_t<T>::borrowed_t(owned_t<U> that)
    : $storage(that.$storage) {
      (void)static_cast<T*>(static_cast<U*>(nullptr));
      if ($storage != nullptr) {
        auto& number_of_borrowers = number_of_borrowers();
        number_of_borrowers += 1;
      }
    }

    template <typename T>
    borrowed_t<T>::borrowed_t(const borrowed_t<T>& that)
    : $storage(that.$storage) {
      if ($storage != nullptr) {
        auto& number_of_borrowers = number_of_borrowers();
        number_of_borrowers += 1;
      }
    }

    template <typename T>
    borrowed_t<T>::borrowed_t(borrowed_t<T>&& that)
    : $storage(pass(that.$storage)) {
      new (&that) borrowed_t<T>();
    }

    template <typename T>
    borrowed_t<T>::~borrowed_t() {
      if ($storage != nullptr) {
        auto& number_of_borrowers = number_of_borrowers();
        if (--number_of_borrowers == 0) {
          auto& number_of_owners = number_of_owners();
          if (number_of_owners == 0) {
            free($storage);
          }
        }
      }
    }

    template <typename T>
    borrowed_t<T>::operator bool() const {
      if ($storage == nullptr) {
        return false;
      }
      if (number_of_owners() == 0) {
        return false;
      }
      return true;
    }

    template <typename T>
    auto borrowed_t<T>::operator =(borrowed_t<T> that)
      -> borrowed_t<T>& {
      return swap(self, that);
    }

    template <typename T>
    auto borrowed_t<T>::operator ->()
      -> T* {
      SESAME_FAIL_IF(!self);
      return pointer();
    }

    template <typename T>
    auto borrowed_t<T>::operator ->() const
      -> const T* {
      SESAME_FAIL_IF(!self);
      return pointer();
    }

    template <typename T>
    auto borrowed_t<T>::operator *()
      -> T& {
      SESAME_FAIL_IF(!self);
      return *pointer();
    }

    template <typename T>
    auto borrowed_t<T>::operator *() const
      -> const T& {
      SESAME_FAIL_IF(!self);
      return *pointer();
    }

    template <typename T>
    auto borrowed_t<T>::number_of_borrowers()
      -> sinw& {
      return reinterpret_cast<sinw*>($storage)[0];
    }

    template <typename T>
    auto borrowed_t<T>::number_of_borrowers() const
      -> const sinw& {
      return reinterpret_cast<sinw*>($storage)[0];
    }

    template <typename T>
    auto borrowed_t<T>::number_of_owners()
      -> sinw& {
      return reinterpret_cast<sinw*>($storage)[1];
    }

    template <typename T>
    auto borrowed_t<T>::number_of_owners() const
      -> const sinw& {
      return reinterpret_cast<sinw*>($storage)[1];
    }

    template <typename T>
    auto borrowed_t<T>::pointer()
      -> T* {
      return reinterpret_cast<T*>(&$storage[size<sinw[2]>()]);
    }

    template <typename T>
    auto borrowed_t<T>::pointer() const
      -> const T* {
      return reinterpret_cast<T*>(&$storage[size<sinw[2]>()]);
    }
  }
}
