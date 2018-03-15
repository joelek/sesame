#pragma once

#include "sesame_lib_borrowed.fwd"
#include "sesame_lib_header.hpp"

namespace sesame {
  namespace lib {
    template <typename T>
    class owned_t {
        template <typename U>
        friend class borrowed_t;
        template <typename U>
        friend class owned_t;
        template <typename U, typename ... A>
        friend auto make(A&& ... arguments)
          -> owned_t<U>;
      public:
        owned_t();
        owned_t(decltype(nullptr) null);
        template <typename U>
        owned_t(borrowed_t<U> that);
        template <typename U>
        owned_t(owned_t<U> that);
        owned_t(const owned_t<T>& that);
        owned_t(owned_t<T>&& that);
        ~owned_t();
        explicit operator bool() const;
        auto operator =(owned_t<T> that)
          -> owned_t<T>&;
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
    owned_t<T>::owned_t()
    : $storage() {

    }

    template <typename T>
    owned_t<T>::owned_t(decltype(nullptr) null)
    : $storage(null) {

    }

    template <typename T>
    template <typename U>
    owned_t<T>::owned_t(borrowed_t<U> that)
    : $storage(that.$storage) {
      (void)static_cast<T*>(static_cast<U*>(nullptr));
      if ($storage != nullptr) {
        auto& owners = number_of_owners();
        if (owners == 0) {
          $storage = nullptr;
        } else {
          owners += 1;
        }
      }
    }

    template <typename T>
    template <typename U>
    owned_t<T>::owned_t(owned_t<U> that)
    : $storage(that.$storage) {
      (void)static_cast<T*>(static_cast<U*>(nullptr));
      that.$storage = nullptr;
    }

    template <typename T>
    owned_t<T>::owned_t(const owned_t<T>& that)
    : $storage(that.$storage) {
      if ($storage != nullptr) {
        auto& owners = number_of_owners();
        owners += 1;
      }
    }

    template <typename T>
    owned_t<T>::owned_t(owned_t<T>&& that)
    : $storage(pass(that.$storage)) {
      that.$storage = nullptr;
    }

    template <typename T>
    owned_t<T>::~owned_t() {
      if ($storage != nullptr) {
        auto& owners = number_of_owners();
        if (--owners == 0) {
          auto& borowers = number_of_borrowers();
          if (borowers == 0) {
            pointer()->~T();
            free($storage);
          } else {
            pointer()->~T();
          }
        }
      }
    }

    template <typename T>
    owned_t<T>::operator bool() const {
      if ($storage == nullptr) {
        return false;
      }
      if (number_of_owners() == 0) {
        return false;
      }
      return true;
    }

    template <typename T>
    auto owned_t<T>::operator =(owned_t<T> that)
      -> owned_t<T>& {
      return swap(self, that);
    }

    template <typename T>
    auto owned_t<T>::operator ->()
      -> T* {
      SESAME_FAIL_IF(!self);
      return pointer();
    }

    template <typename T>
    auto owned_t<T>::operator ->() const
      -> const T* {
      SESAME_FAIL_IF(!self);
      return pointer();
    }

    template <typename T>
    auto owned_t<T>::operator *()
      -> T& {
      SESAME_FAIL_IF(!self);
      return *pointer();
    }

    template <typename T>
    auto owned_t<T>::operator *() const
      -> const T& {
      SESAME_FAIL_IF(!self);
      return *pointer();
    }

    template <typename T>
    auto owned_t<T>::number_of_borrowers()
      -> sinw& {
      return reinterpret_cast<sinw*>($storage)[0];
    }

    template <typename T>
    auto owned_t<T>::number_of_borrowers() const
      -> const sinw& {
      return reinterpret_cast<sinw*>($storage)[0];
    }

    template <typename T>
    auto owned_t<T>::number_of_owners()
      -> sinw& {
      return reinterpret_cast<sinw*>($storage)[1];
    }

    template <typename T>
    auto owned_t<T>::number_of_owners() const
      -> const sinw& {
      return reinterpret_cast<sinw*>($storage)[1];
    }

    template <typename T>
    auto owned_t<T>::pointer()
      -> T* {
      return reinterpret_cast<T*>($storage + size<sinw[2]>());
    }

    template <typename T>
    auto owned_t<T>::pointer() const
      -> const T* {
      return reinterpret_cast<T*>($storage + size<sinw[2]>());
    }
  }
}

namespace sesame {
  namespace lib {
    template <typename T, typename ... A>
    owned_t<T> make(A&& ... arguments) {
      owned_t<T> instance;
      instance.$storage = static_cast<ui08*>(malloc(size<sinw[2]>() + size<T>()));
      SESAME_FAIL_IF(instance.$storage == nullptr);
      try {
        new (instance.pointer()) T(static_cast<A&&>(arguments)...);
        instance.number_of_owners() = 1;
        instance.number_of_borrowers() = 0;
      } catch (...) {
        free(instance.$storage);
        instance.$storage = nullptr;
        SESAME_FAIL_IF(true);
      }
      return instance;
    }
  }
}
