#pragma once

#include "sesame_lib_file.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_reader.hpp"

namespace sesame {
  namespace lib {
    class file_reader_t: public reader_t {
      public:
        file_reader_t();
        file_reader_t(const char* filename);
        auto length() const override
          -> sinw;
        auto read(sinw offset, sinw length, ui08* target) override
          -> void;
      protected:
      private:
        file_t $file;
    };
  }
}
