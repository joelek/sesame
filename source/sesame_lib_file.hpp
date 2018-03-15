#pragma once

#include "sesame_lib_header.hpp"

namespace sesame {
  namespace lib {
    class file_t {
      public:
        file_t();
        file_t(const char* name, const char* mode);
        file_t(const file_t& that);
        file_t(file_t&& that);
        ~file_t();
        auto operator =(file_t that)
          -> file_t&;
        auto length() const
          -> sinw;
        auto read(sinw length, ui08* target) const
          -> void;
        auto seek(sinw offset) const
          -> void;
        auto tell() const
          -> sinw;
        auto write(sinw length, const ui08* source) const
          -> void;
      protected:
      private:
        const char* $name;
        const char* $mode;
        FILE* $handle;
    };
  }
}
