// [dirty]
#pragma once

#include "sesame_lib.hpp"
#include "sesame_wc2_archive.hpp"
#include "sesame_wc2_bitmap_resource.hpp"
#include "sesame_wc2_cursor_resource.hpp"
#include "sesame_wc2_font_resource.hpp"
#include "sesame_wc2_palette_resource.hpp"
#include "sesame_wc2_sprite_resource.hpp"
#include "sesame_wc2_strings.hpp"
#include "sesame_wc2_table.hpp"
#include "sesame_wc2_tiles.hpp"
#include "sesame_wc2_tileset.hpp"

using namespace sesame::lib;

namespace sesame {
  namespace wc2 {
    class resource_loader_t {
      public:
        resource_loader_t(const char* cstr);
        sinw get_number_of_resources() const;
        owned_t<bitmap_resource_t> load_bitmap(sinw index) const;
        owned_t<cursor_resource_t> load_cursor(sinw index) const;
        owned_t<font_resource_t> load_font(sinw index) const;
        owned_t<palette_resource_t> load_palette(sinw index) const;
        owned_t<reader_t> load_raw(sinw index) const;
        owned_t<sprite_resource_t> load_sprite(sinw index) const;
        owned_t<strings_resource_t> load_strings(sinw index) const;
        owned_t<table_resource_t> load_table(sinw index) const;
        owned_t<tiles_resource_t> load_tiles(sinw index) const;
        owned_t<tileset_resource_t> load_tileset(sinw index) const;
      protected:
      private:
        owned_t<archive_t> archive;
    };
  }
}
