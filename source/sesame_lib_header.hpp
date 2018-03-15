#pragma once

#include <cstdbool>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <new>

#ifdef DEBUG
  #define SESAME_FAIL_IF(trigger)                                              \
    if (trigger) {                                                             \
      printf("[FAILURE:%s:%u] %s\n", __FILE__, __LINE__, #trigger);            \
      throw EXIT_FAILURE;                                                      \
    }

  #define SESAME_WARN_IF(trigger)                                              \
    if (trigger) {                                                             \
      printf("[WARNING:%s:%u] %s\n", __FILE__, __LINE__, #trigger);            \
    }
#else
  #define SESAME_FAIL_IF(trigger)                                              \
    if (trigger) {                                                             \
      throw EXIT_FAILURE;                                                      \
    }

  #define SESAME_WARN_IF(trigger)                                              \
    if (trigger) {                                                             \
    }
#endif

#define self (*this)

namespace sesame {
  namespace lib {
    using fp32 = float;
    using fp64 = double;
    using si08 = int8_t;
    using si16 = int16_t;
    using si32 = int32_t;
    using si64 = int64_t;
    using sinw = intptr_t;
    using ui08 = uint8_t;
    using ui16 = uint16_t;
    using ui32 = uint32_t;
    using ui64 = uint64_t;
    using uinw = uintptr_t;

    template <typename T> constexpr
    auto size()
      -> signed {
      return sizeof(T);
    }

    template <typename T> constexpr
    auto size(const T&)
      -> signed {
      return sizeof(T);
    }

    template <typename T> constexpr
    auto pass(T& source)
      -> T&& {
      return static_cast<T&&>(source);
    }

    template <typename T>
    auto copy(T& target, const T& source)
      -> T& {
      auto target_address = &target;
      auto source_address = &source;
      if (target_address != source_address) {
        target_address->~T();
        new (target_address) T(source);
      }
      return target;
    }

    template <typename T>
    auto swap(T& target, T& source)
      -> T& {
      auto target_address = &target;
      auto source_address = &source;
      if (target_address != source_address) {
        auto temporary = T(pass(target));
        target_address->~T();
        new (target_address) T(pass(source));
        source_address->~T();
        new (source_address) T(pass(temporary));
      }
      return target;
    }
  }
}
