// [dirty]
#include "sesame_wc2_resource_loader.hpp"

namespace sesame {
  namespace wc2 {
    resource_loader_t::resource_loader_t(const char* cstr)
    : archive(make<archive_t>(make<file_reader_t>(cstr))) {

    }

    sinw resource_loader_t::get_number_of_resources() const {
      return this->archive->get_format()->get_number_of_records();
    }

    owned_t<bitmap_resource_t> resource_loader_t::load_bitmap(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<bitmap_resource_t>(reader);
    }

    owned_t<cursor_resource_t> resource_loader_t::load_cursor(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<cursor_resource_t>(reader);
    }

    owned_t<font_resource_t> resource_loader_t::load_font(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<font_resource_t>(reader);
    }

    owned_t<palette_resource_t> resource_loader_t::load_palette(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<palette_resource_t>(reader);
    }

    owned_t<reader_t> resource_loader_t::load_raw(sinw index) const {
      return this->archive->get_format()->get_data_reader(index);
    }

    owned_t<sprite_resource_t> resource_loader_t::load_sprite(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<sprite_resource_t>(reader);
    }

    owned_t<strings_resource_t> resource_loader_t::load_strings(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<strings_resource_t>(reader);
    }

    owned_t<table_resource_t> resource_loader_t::load_table(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<table_resource_t>(reader);
    }

    owned_t<tiles_resource_t> resource_loader_t::load_tiles(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<tiles_resource_t>(reader);
    }

    owned_t<tileset_resource_t> resource_loader_t::load_tileset(sinw index) const {
      auto reader = this->archive->get_format()->get_data_reader(index);
      return make<tileset_resource_t>(reader);
    }
  }
}
