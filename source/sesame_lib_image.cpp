#include "sesame_lib_image.hpp"

namespace sesame {
  namespace lib {

  }
}

namespace sesame {
  namespace lib {
    image_dynamic_t::image_dynamic_t()
    : $width()
    , $height()
    , $frames()
    , $bpp()
    , $channels()
    , $stride_scanline()
    , $stride_frame()
    , $buffer() {

    }

    image_dynamic_t::image_dynamic_t(sinw width, sinw height, sinw frames, sinw bpp, sinw channels)
    : $width(width)
    , $height(height)
    , $frames(frames)
    , $bpp(bpp)
    , $channels(channels)
    , $stride_scanline(((($channels * $width) << $bpp) + (8 - 1)) >> 3)
    , $stride_frame($stride_scanline * $height)
    , $buffer($stride_frame * $frames) {
      SESAME_FAIL_IF($width < 0);
      SESAME_FAIL_IF($height < 0);
      SESAME_FAIL_IF($frames < 0);
      SESAME_FAIL_IF($bpp < 0);
      SESAME_FAIL_IF($bpp > 3);
      SESAME_FAIL_IF($channels < 1);
      SESAME_FAIL_IF($channels > 4);
    }

    auto image_dynamic_t::bpp() const
      -> sinw {
      return $bpp;
    }

    auto image_dynamic_t::channels() const
      -> sinw {
      return $channels;
    }

    auto image_dynamic_t::frames() const
      -> sinw {
      return $frames;
    }

    auto image_dynamic_t::height() const
      -> sinw {
      return $height;
    }

    auto image_dynamic_t::width() const
      -> sinw {
      return $width;
    }
  }
}
