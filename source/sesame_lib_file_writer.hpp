#pragma once

#include "sesame_lib_file.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_writer.hpp"

namespace sesame {
  namespace lib {
    class file_writer_t: public writer_t {
      public:
        file_writer_t();
        file_writer_t(const char* filename);
        auto length() const override
          -> sinw;
        auto write(sinw offset, sinw length, const ui08* source) override
          -> void;
      protected:
      private:
        file_t $file;
    };
  }
}
