#include "sesame.hpp"

using namespace sesame;
using namespace sesame::lib;
using namespace sesame::wc1;

const char* archives[] = {
  u8"games/wc1/archives/data.war"
};

auto make_archive(sinw index)
  -> archive_t {
  auto reader = make<file_reader_t>(archives[index]);
  auto cursor = 0;
  auto archive = archive_t(reader, cursor);
  SESAME_FAIL_IF(cursor != reader->length());
  return archive;
}

auto extract()
  -> void {
  try {
    auto archive = make_archive(0);
    auto reader = archive.records().at(193).reader();
    auto cursor = 0;
    auto tiles = tiles_t(reader, cursor);
    SESAME_FAIL_IF(cursor != reader->length());
  } catch (...) {}
}

auto repack()
  -> void {
  try {
    auto archive = make_archive(0);
    auto writer = make<file_writer_t>(u8"games/wc1/data.war");
    auto cursor = 0;
    archive.write(writer, cursor);
  } catch (...) {}
}

auto unpack()
  -> void {
  try {
    auto archive = make_archive(0);
    auto counter = 0;
    for (auto& record : archive.records()) {
      try {
        char filename[128];
        snprintf(filename, 128, u8"games/wc1/archives/data/%u.bin", counter);
        auto buffer = buffer_t(record.reader());
        auto writer = make<file_writer_t>(filename);
        auto cursor = 0;
        buffer.write(writer, cursor);
      } catch (...) {}
      counter += 1;
    }
  } catch (...) {}
}

auto main()
  -> sinw {
  repack();
}
