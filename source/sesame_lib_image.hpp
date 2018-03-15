#pragma once

#include "sesame_lib_buffer.hpp"
#include "sesame_lib_header.hpp"
#include "sesame_lib_interface.hpp"

namespace sesame {
  namespace lib {
    class image_t: public interface_t {
      public:
        virtual auto bpp() const
          -> sinw = 0;
        virtual auto channels() const
          -> sinw = 0;
        virtual auto frames() const
          -> sinw = 0;
        virtual auto height() const
          -> sinw = 0;
        virtual auto width() const
          -> sinw = 0;
      protected:
      private:
    };
  }
}

namespace sesame {
  namespace lib {
    class image_dynamic_t: public image_t {
      public:
        image_dynamic_t();
        image_dynamic_t(sinw width, sinw height, sinw frames, sinw bpp, sinw channels);
        auto bpp() const override
          -> sinw;
        auto channels() const override
          -> sinw;
        auto frames() const override
          -> sinw;
        auto height() const override
          -> sinw;
        auto width() const override
          -> sinw;
      protected:
      private:
        sinw $width;
        sinw $height;
        sinw $frames;
        sinw $bpp;
        sinw $channels;
        sinw $stride_scanline;
        sinw $stride_frame;
        buffer_t $buffer;
    };
  }
}
