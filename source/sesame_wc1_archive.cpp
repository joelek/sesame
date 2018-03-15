#include "sesame_wc1_archive.hpp"
namespace sesame {
  namespace wc1 {
    archive_t::archive_t()
    : $records() {

    }

    archive_t::archive_t(vector_t<record_t> records)
    : $records(pass(records)) {

    }

    archive_t::archive_t(owned_t<reader_t> reader, sinw& cursor)
    : $records() {
      auto identifier = si32le(reader, cursor);
      SESAME_FAIL_IF(identifier != 24);
      auto number_of_records = si32le(reader, cursor);
      auto offset_table = offset_table_t<si32le>(number_of_records, reader, cursor);
      for (const auto& reader : offset_table.readers()) {
        auto cursor = 0;
        $records.push(record_t(reader, cursor));
        SESAME_FAIL_IF(cursor != reader->length());
      }
      cursor = reader->length();
    }

    auto archive_t::records()
      -> vector_t<record_t>& {
      return $records;
    }

    auto archive_t::records() const
      -> const vector_t<record_t>& {
      return $records;
    }

    auto archive_t::write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      auto identifier = si32le(24);
      identifier.write(writer, cursor);
      auto number_of_records = si32le($records.length());
      number_of_records.write(writer, cursor);
      auto cursor_offset = cursor;
      for (auto& record : $records) {
        (void)record;
        auto offset = si32le(0);
        offset.write(writer, cursor);
      }
      for (auto& record : $records) {
        auto offset = si32le(cursor);
        offset.write(writer, cursor_offset);
        record.write(writer, cursor);
      }
    }
  }
}
