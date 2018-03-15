// [dirty]
#include "sesame_wc2_archive.hpp"

namespace sesame {
  namespace wc2 {
    archive_format_t::archive_format_t(owned_t<reader_t> reader, sinw cursor)
    : reader(make<offset_reader_t>(reader, cursor))
    , magic_number(reader, cursor)
    , number_of_records(reader, cursor)
    , archive_id(reader, cursor)
    , record_offsets(number_of_records) {
      SESAME_FAIL_IF(this->magic_number != 25);
      for (auto i = 0; i < this->number_of_records; i++) {
        this->record_offsets.at(i) = primitive_le_t<ui32>(reader, cursor);
        SESAME_FAIL_IF(this->record_offsets.at(i) >= this->reader->length());
      }
    }

    sinw archive_format_t::get_magic_number() const {
      return this->magic_number;
    }

    sinw archive_format_t::get_number_of_records() const {
      return this->number_of_records;
    }

    sinw archive_format_t::get_archive_id() const {
      return this->archive_id;
    }

    sinw archive_format_t::get_record_offset(sinw index) const {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= this->number_of_records);
      return this->record_offsets.at(index);
    }

    owned_t<reader_t> archive_format_t::get_data_reader(sinw index) const {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= this->number_of_records);
      auto reader = this->get_offset_reader(index);
      auto cursor = 0;
      if (reader->length() == 1) {
        auto data_encoding = primitive_le_t<ui08>(reader, cursor);
        SESAME_FAIL_IF(data_encoding != 0x00 && data_encoding != 0x20);
        return make<offset_reader_t>(reader, cursor, 0);
      } else {
        auto data = primitive_le_t<ui32>(reader, cursor);
        auto data_size = (data & 0x00FFFFFF);
        auto data_encoding = ((data >> 24) & 0xFF);
        SESAME_FAIL_IF(data_encoding != 0x00 && data_encoding != 0x20);
        if (data_encoding == 0x00) {
          return make<offset_reader_t>(reader, cursor, data_size);
        } else {
          auto offset_reader = make<offset_reader_t>(reader, cursor);
          return make<lzss_reader_t>(offset_reader, data_size);
        }
      }
    }

    owned_t<reader_t> archive_format_t::get_offset_reader(sinw index) const {
      SESAME_FAIL_IF(index < 0);
      SESAME_FAIL_IF(index >= this->number_of_records);
      auto offset = this->record_offsets.at(index);
      if (index + 1 < this->number_of_records) {
        auto length = this->record_offsets.at(index + 1) - offset;
        return make<offset_reader_t>(this->reader, offset, length);
      } else {
        auto length = this->reader->length() - offset;
        return make<offset_reader_t>(this->reader, offset, length);
      }
    }
  }
}

namespace sesame {
  namespace wc2 {
    archive_t::archive_t(owned_t<reader_t> reader)
    : format(make<archive_format_t>(reader, 0)) {

    }

    owned_t<archive_format_t> archive_t::get_format() const {
      return this->format;
    }
  }
}
