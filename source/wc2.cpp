// [dirty]
#include "sesame.hpp"

using namespace sesame;
using namespace sesame::lib;
using namespace sesame::wc2;

/*
printf("\n");
buffer_t row(this->format->get_width(), 0);
for (auto c = 0; c < 256; c++) {
  printf("character: %i\n", c);
  auto reader = this->format->get_data_reader(c);
  auto cursor = 0;
  for (auto y = 0; y < this->format->get_height(); y++) {
    reader->read(cursor, this->format->get_width(), &row[0]);
    cursor += this->format->get_width();
    for (auto x = 0; x < this->format->get_width(); x++) {
      printf("%s", (row[x] == 0) ? " " : "x");
    }
    printf("\n");
  }
  printf("\n");
}
*/

template <typename A>
void write(owned_t<writer_t> writer, sinw& cursor, const A& value) {
  writer->write(cursor, sizeof(A), reinterpret_cast<const ui08*>(&value));
  cursor += sizeof(A);
}

void export_bmp(sinw width, sinw height, owned_t<reader_t> reader, owned_t<writer_t> writer, owned_t<reader_t> palreader) {
  auto row_stride = ((width + 3) >> 2) << 2;
  auto row_padding = row_stride - width;
  ui08 row_padding_data[3] = {};
  auto magic_number = ui16(19778);
  auto file_size = ui32(14 + 12 + 256 * 3 + row_stride * height);
  auto reserved_a = ui16(0);
  auto reserved_b = ui16(0);
  auto image_data_offset = ui32(14 + 12 + 256 * 3);
  auto cursor = 0;
  write(writer, cursor, magic_number);
  write(writer, cursor, file_size);
  write(writer, cursor, reserved_a);
  write(writer, cursor, reserved_b);
  write(writer, cursor, image_data_offset);
  auto dib_header_size = ui32(12);
  auto bitmap_width = ui16(width);
  auto bitmap_height = ui16(height);
  auto color_planes = ui16(1);
  auto bits_per_pixel = ui16(8);
  write(writer, cursor, dib_header_size);
  write(writer, cursor, bitmap_width);
  write(writer, cursor, bitmap_height);
  write(writer, cursor, color_planes);
  write(writer, cursor, bits_per_pixel);
  for (auto i = 0; i < 256; i++) {
    ui08 rgb[3];
    palreader->read(i*3, 3, &rgb[0]);
    rgb[0] <<= 2;
    rgb[1] <<= 2;
    rgb[2] <<= 2;
    write(writer, cursor, rgb[2]);
    write(writer, cursor, rgb[1]);
    write(writer, cursor, rgb[0]);
  }
  auto buffer = buffer_t(reader->length());
  reader->read(0, reader->length(), buffer.pointer());
  for (auto y = height - 1; y >= 0; y--) {
    writer->write(cursor, width, &buffer.pointer()[y * width]);
    cursor += width;
    writer->write(cursor, row_padding, &row_padding_data[0]);
    cursor += row_padding;
  }
}

const char* archives[] = {
  u8"../INSMAIN.INS",
  u8"../INSSTR.INS",
  u8"../MAINDAT.WAR",
  u8"../MUDDAT.CUD",
  u8"../REZDAT.WAR",
  u8"../SFXDAT.SUD",
  u8"../SNDDAT.WAR",
  u8"../STRDAT.WAR"
};

const char* directories[] = {
  u8"insmain",
  u8"insstr",
  u8"maindat",
  u8"muddat",
  u8"rezdat",
  u8"sfxdat",
  u8"snddat",
  u8"strdat"
};

void guess_type(owned_t<resource_loader_t> resource_loader, sinw/*archive_index*/, sinw resource_index) {
  printf("%u:", resource_index);
  auto failed = 0;
  try {
    auto resource = resource_loader->load_bitmap(resource_index);
    printf(" bitmap[%ix%i]", resource->get_format()->get_width(), resource->get_format()->get_height());
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_cursor(resource_index);
    printf(" cursor[%ix%i]", resource->get_format()->get_width(), resource->get_format()->get_height());
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_font(resource_index);
    printf(" font[%ix%i]", resource->get_format()->get_width(), resource->get_format()->get_height());
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_palette(resource_index);
    printf(" palette");
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_sprite(resource_index);
    printf(" sprite[%ix%i][%i]", resource->format().get_width(), resource->format().get_height(), resource->format().get_number_of_frames());
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_strings(resource_index);
    printf(" strings");
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_table(resource_index);
    printf(" table");
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_tiles(resource_index);
    printf(" tiles");
  } catch (...) {
    failed++;
  }
  try {
    auto resource = resource_loader->load_tileset(resource_index);
    printf(" tileset");
  } catch (...) {
    failed++;
  }
  if (failed == 9) {
    auto reader = resource_loader->load_raw(resource_index);
    if (reader->length() > 0) {
      printf(" unknown[%ib]", reader->length());
    } else {
      printf(" -");
    }
  }
  printf("\n");
}

void guess_type(sinw archive_index) {
  printf("%s:\n", directories[archive_index]);
  auto resource_loader = make<resource_loader_t>(archives[archive_index]);
  for (auto resource_index = 0; resource_index < resource_loader->get_number_of_resources(); ++resource_index) {
    guess_type(resource_loader, archive_index, resource_index);
  }
  printf("\n");
}

void guess_type() {
  for (auto archive_index = 0; archive_index < 8; ++archive_index) {
    guess_type(archive_index);
  }
}

void dump(owned_t<archive_t> archive, sinw archive_index, sinw resource_index) {
  printf("%u:\n", resource_index);
  char filename[64];
  snprintf(filename, 64, u8"output/raw/%s/%u.bin", directories[archive_index], resource_index);
  auto reader = archive->get_format()->get_data_reader(resource_index);
  auto writer = make<file_writer_t>(filename);
  if (reader->length() > 0) {
    auto buffer = buffer_t(reader->length());
    reader->read(0, reader->length(), buffer.pointer());
    writer->write(0, reader->length(), buffer.pointer());
  }
}

void dump(sinw archive_index) {
  printf("%s:\n", directories[archive_index]);
  auto archive = make<archive_t>(make<file_reader_t>(archives[archive_index]));
  for (auto resource_index = 0; resource_index < archive->get_format()->get_number_of_records(); ++resource_index) {
    dump(archive, archive_index, resource_index);
  }
  printf("\n");
}

void dump() {
  for (auto archive_index = 0; archive_index < 8; ++archive_index) {
    dump(archive_index);
  }
}


const ui32 riff = 0x46464952;
const ui32 list = 0x5453494C;
const ui32 wave = 0x45564157;
const ui32 fmt_ = 0x20746D66;
const ui32 data = 0x61746164;

void nameof(ui32 id) {
  auto& buffer = reinterpret_cast<ui08 (&)[4]>(id);
  printf("%.*s", 4, buffer);
}

void parse_fmt_(owned_t<reader_t> reader) {
  auto cursor = 0;
  auto compression_code = primitive_le_t<ui16>(reader, cursor);
  auto number_of_channels = primitive_le_t<ui16>(reader, cursor);
  (void)number_of_channels;
  auto samples_per_second = primitive_le_t<ui32>(reader, cursor);
  (void)samples_per_second;
  auto bytes_per_second = primitive_le_t<ui32>(reader, cursor);
  (void)bytes_per_second;
  auto bytes_per_block = primitive_le_t<ui16>(reader, cursor);
  (void)bytes_per_block;
  auto bits_per_sample = primitive_le_t<ui16>(reader, cursor);
  (void)bits_per_sample;
  if (compression_code != 1) {
    auto extra_length = primitive_le_t<ui16>(reader, cursor);
    cursor += extra_length;
  }
  SESAME_FAIL_IF(cursor != reader->length());
}

void parse_chunk(owned_t<reader_t> reader, sinw& cursor, sinw indent) {
  auto chunk_identifier = primitive_le_t<ui32>(reader, cursor);
  auto chunk_length = primitive_le_t<ui32>(reader, cursor);
  for (auto i = 0; i < indent; i++) {
    printf("  ");
  }
  nameof(chunk_identifier);
  printf(": %u bytes\n", 1*chunk_length);
  if (chunk_identifier == riff || chunk_identifier == list) {
    indent++;
    auto chunks_end = cursor + chunk_length;
    auto chunks_identifier = primitive_le_t<ui32>(reader, cursor);
    for (auto i = 0; i < indent; i++) {
      printf("  ");
    }
    nameof(chunks_identifier);
    printf("\n");
    while (cursor < chunks_end) {
      parse_chunk(reader, cursor, indent);
    }
  } else {
    reader = make<offset_reader_t>(reader, cursor, chunk_length);
    cursor += chunk_length;
    if (chunk_identifier == fmt_) {
      parse_fmt_(reader);
    } else if (chunk_identifier == data) {

    }
  }
  if ((chunk_length & 1) == 1) {
    cursor += 1;
  }
}

void test_riff_parser() {
  auto reader = make<file_reader_t>(u8"output/raw/sfxdat/2.bin");
  auto cursor = 0;
  parse_chunk(reader, cursor, 0);
  SESAME_FAIL_IF(cursor != reader->length());
}























#include <vector>

class riff_data_chunk_t;
class riff_list_chunk_t;

namespace sesame {
  namespace lib {
    class riff_chunk_t: public interface_t {
      public:
        virtual owned_t<riff_data_chunk_t> as_data() const = 0;
        virtual owned_t<riff_list_chunk_t> as_list() const = 0;
        virtual bool is(const char (&fourcc)[5]) const = 0;
        virtual bool is_data() const = 0;
        virtual bool is_list() const = 0;
      protected:
      private:
    };
  }
}





class riff_data_chunk_t: public riff_chunk_t {
  public:
    riff_data_chunk_t(owned_t<reader_t> reader, sinw& cursor);
    ~riff_data_chunk_t();
    owned_t<riff_data_chunk_t> as_data() const override;
    owned_t<riff_list_chunk_t> as_list() const override;
    owned_t<reader_t> get_data_reader() const;
    bool is(const char (&fourcc)[5]) const override;
    bool is_data() const override;
    bool is_list() const override;
  protected:
  private:
    primitive_le_t<ui32> identifier;
    primitive_le_t<ui32> length;
    owned_t<reader_t> data_reader;
};

riff_data_chunk_t::riff_data_chunk_t(owned_t<reader_t> reader, sinw& cursor)
: identifier(reader, cursor)
, length(reader, cursor)
, data_reader(make<offset_reader_t>(reader, cursor, length)) {
  cursor += this->length;
  if ((this->length & 1) == 1) {
    cursor += 1;
  }
}

riff_data_chunk_t::~riff_data_chunk_t() {

}

owned_t<riff_data_chunk_t> riff_data_chunk_t::as_data() const {
  return nullptr;
}

owned_t<riff_list_chunk_t> riff_data_chunk_t::as_list() const {
  SESAME_FAIL_IF(true);
}

owned_t<reader_t> riff_data_chunk_t::get_data_reader() const {
  return this->data_reader;
}

bool riff_data_chunk_t::is(const char (&fourcc)[5]) const {
  return this->identifier == primitive_le_t<ui32>(fourcc);
}

bool riff_data_chunk_t::is_data() const {
  return true;
}

bool riff_data_chunk_t::is_list() const {
  return false;
}















class riff_chunks_t {
  public:
    riff_chunks_t(owned_t<reader_t> reader, sinw& cursor, sinw cursor_end);
    ~riff_chunks_t();
    bool is(const char (&fourcc)[5]) const;
  protected:
  private:
    primitive_le_t<ui32> format;
    std::vector<owned_t<riff_chunk_t>> chunks;
};

riff_chunks_t::riff_chunks_t(owned_t<reader_t> reader, sinw& cursor, sinw cursor_end)
: format(reader, cursor)
, chunks() {
  while (cursor < cursor_end) {
    auto cursor_copy = cursor;
    auto identifier = primitive_le_t<ui32>(reader, cursor_copy);
    if (identifier == primitive_le_t<ui32>(u8"LIST")) {
      this->chunks.push_back(make<riff_list_chunk_t>(reader, cursor));
    } else {
      this->chunks.push_back(make<riff_data_chunk_t>(reader, cursor));
    }
  }
  SESAME_FAIL_IF(cursor != cursor_end);
}

riff_chunks_t::~riff_chunks_t() {

}

bool riff_chunks_t::is(const char (&fourcc)[5]) const {
  return this->format == primitive_le_t<ui32>(fourcc);
}















class riff_list_chunk_t: public riff_chunk_t {
  public:
    riff_list_chunk_t(owned_t<reader_t> reader, sinw& cursor);
    ~riff_list_chunk_t();
    owned_t<riff_data_chunk_t> as_data() const override;
    owned_t<riff_list_chunk_t> as_list() const override;
    riff_chunks_t& get_chunks();
    const riff_chunks_t& get_chunks() const;
    bool is(const char (&fourcc)[5]) const override;
    bool is_data() const override;
    bool is_list() const override;
  protected:
  private:
    primitive_le_t<ui32> identifier;
    primitive_le_t<ui32> length;
    riff_chunks_t chunks;
};

riff_list_chunk_t::riff_list_chunk_t(owned_t<reader_t> reader, sinw& cursor)
: identifier(reader, cursor)
, length(reader, cursor)
, chunks(reader, cursor, cursor + length) {
  SESAME_FAIL_IF(this->identifier != primitive_le_t<ui32>(u8"LIST"));
  if ((this->length & 1) == 1) {
    cursor += 1;
  }
}

riff_list_chunk_t::~riff_list_chunk_t() {

}

owned_t<riff_data_chunk_t> riff_list_chunk_t::as_data() const {
  SESAME_FAIL_IF(true);
}

owned_t<riff_list_chunk_t> riff_list_chunk_t::as_list() const {
  return nullptr;
}

riff_chunks_t& riff_list_chunk_t::get_chunks() {
  return this->chunks;
}

const riff_chunks_t& riff_list_chunk_t::get_chunks() const {
  return this->chunks;
}

bool riff_list_chunk_t::is(const char (&fourcc)[5]) const {
  return this->identifier == primitive_le_t<ui32>(fourcc);
}

bool riff_list_chunk_t::is_data() const {
  return false;
}

bool riff_list_chunk_t::is_list() const {
  return true;
}











class riff_format_t {
  public:
    riff_format_t(owned_t<reader_t> reader, sinw cursor = 0);
    ~riff_format_t();
  protected:
  private:
    primitive_le_t<ui32> identifier;
    primitive_le_t<ui32> length;
    riff_chunks_t chunks;
};

riff_format_t::riff_format_t(owned_t<reader_t> reader, sinw cursor)
: identifier(reader, cursor)
, length(reader, cursor)
, chunks(reader, cursor, cursor + length) {
  SESAME_FAIL_IF(this->identifier != primitive_le_t<ui32>(u8"RIFF"));
  SESAME_FAIL_IF(cursor != reader->length());
}

riff_format_t::~riff_format_t() {

}

void dump_sprite() {
  auto palreader = make<file_reader_t>(u8"output/raw/maindat/2.bin");
  auto palformat = make<palette_resource_t>(palreader)->get_format();
  palreader = palformat->get_data_reader();
  auto reader = make<file_reader_t>(u8"output/raw/maindat/33.bin");
  auto format = make<sprite_resource_t>(reader)->format();
  for (auto i = 0; i < format.get_number_of_frames(); i++) {
    printf("frame: %i\n", i);
    char filename[128];
    snprintf(filename, 128, "output/raw/maindat/33[%u].bmp", i);
    auto reader = format.get_data_reader(i);
    auto writer = make<file_writer_t>(filename);
    if (reader) {
      export_bmp(format.get_width(), format.get_height(), reader, writer, palreader);
    }
  }
}

void parse_riff() {
  auto reader = make<file_reader_t>(u8"output/raw/sfxdat/2.bin");
  auto riff = make<riff_format_t>(reader);
}

void parse_table(owned_t<reader_t> reader) {
  auto cursor = 0;
  auto width = primitive_le_t<ui16>(reader, cursor);
  auto height = primitive_le_t<ui16>(reader, cursor);
  auto values = vector_t<primitive_le_t<ui16>>(width * height, reader, cursor);
  printf("width: %i\n", 1*width);
  printf("height: %i\n", 1*height);
  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      printf("%5i ", 1*values.at(y * width + x));
    }
    printf("\n");
  }
  SESAME_FAIL_IF(cursor != reader->length());
}

void parse_table() {
  parse_table(make<file_reader_t>(u8"output/raw/maindat/6.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/7.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/8.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/14.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/15.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/16.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/22.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/23.bin"));
  parse_table(make<file_reader_t>(u8"output/raw/maindat/24.bin"));
}

void parse_tileset(sinw palette_index, sinw tileset_index, sinw tiles_index) {
  char filename[128];
  snprintf(filename, 128, "output/raw/maindat/%i.bin", palette_index);
  auto palette = make<file_reader_t>(filename);
  snprintf(filename, 128, "output/raw/maindat/%i.bin", tileset_index);
  auto tileset = make<file_reader_t>(filename);
  snprintf(filename, 128, "output/raw/maindat/%i.bin", tiles_index);
  auto tiles = make<file_reader_t>(filename);
  auto number_of_tiles = (tileset->length() >> 5);
  SESAME_FAIL_IF((number_of_tiles << 5) != tileset->length());
  auto cursor = 0;
  for (auto i = 0; i < number_of_tiles; i++) {
    auto values = vector_t<primitive_le_t<ui16>>(4 * 4, tileset, cursor);
    auto buffer = buffer_t(4 * 8 * 4 * 8);
    for (auto c = 0; c < 4 * 4; c++) {
      auto mode = (values.at(c) & 3);
      auto offset = ((values.at(c) >> 2) << 6);
      SESAME_FAIL_IF(offset >= tiles->length());
      auto tile = make<offset_reader_t>(tiles, offset, 8 * 8);
      auto tx = (c & 3);
      auto ty = (c >> 2);
      if (mode == 0) {
        for (auto y = 0; y < 8; y++) {
          tile->read(y * 8, 8, &buffer.pointer()[(ty * 8 + y) * 4 * 8 + (tx * 8)]);
        }
      } else if (mode == 1) {
        for (auto y = 0; y < 8; y++) {
          tile->read((7 - y) * 8, 8, &buffer.pointer()[(ty * 8 + y) * 4 * 8 + (tx * 8)]);
        }
      } else if (mode == 2) {
        for (auto y = 0; y < 8; y++) {
          for (auto x = 0; x < 8; x++) {
            tile->read(y * 8 + (7 - x), 1, &buffer.pointer()[(ty * 8 + y) * 4 * 8 + (tx * 8) + x]);
          }
        }
      } else {
        for (auto y = 0; y < 8; y++) {
          for (auto x = 0; x < 8; x++) {
            tile->read((7 - y) * 8 + (7 - x), 1, &buffer.pointer()[(ty * 8 + y) * 4 * 8 + (tx * 8) + x]);
          }
        }
      }
    }
    snprintf(filename, 128, "output/raw/maindat/%i[%i].bmp", tileset_index, i);
    auto writer = make<file_writer_t>(filename);
    export_bmp(32, 32, make<buffer_reader_t>(buffer), writer, palette);
  }
}

void parse_tileset() {
  parse_tileset(2, 3, 4);
  parse_tileset(10, 11, 12);
  parse_tileset(18, 19, 20);
}

sinw main() {
/*
  auto reader = file_reader_t(u8"output/raw/maindat/6.bin");
  auto table = table_resource_t(reader).format().data();
  for (auto y = 0; y < table.height(); y++) {
    for (auto x = 0; x < table.width(); x++) {
      printf(" %i", table.at(y, x) * 1);
    }
    printf("\n");
  }
*/
}
