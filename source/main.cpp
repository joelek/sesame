#include "sesame.hpp"

using namespace sesame;
using namespace sesame::lib;

template <signed L, signed U>
class integer_t {
  public:
    integer_t()
    : $value() {
      SESAME_FAIL_IF($value < L);
      SESAME_FAIL_IF($value > U);
    }
    template <signed L2, signed U2>
    integer_t(const integer_t<L2, U2>& that)
    : $value(that) {
      SESAME_FAIL_IF($value < L);
      SESAME_FAIL_IF($value > U);
    }
    integer_t(sinw value)
    : $value(pass(value)) {
      SESAME_FAIL_IF($value < L);
      SESAME_FAIL_IF($value > U);
    }
    operator sinw() const {
      return $value;
    }
    template <signed L2, signed U2>
    auto operator <<(const integer_t<L2, U2>& that) const
      -> integer_t<L << L2, U << U2> {
      return integer_t<L << L2, U << U2>($value << that);
    }
  protected:
  private:
    sinw $value;
};

// mlbm = multi-layered bitmap

class image_instance_t {
  public:
    image_instance_t(integer_t<0, 32767> width, integer_t<0, 32767> height, integer_t<0, 3> bpplog2, integer_t<1, 4> channels, integer_t<1, 4> tables, integer_t<0, 32767> layers)
    : $width(pass(width))
    , $height(pass(height))
    , $bpplog2(pass(bpplog2))
    , $bpp(integer_t<1, 1>(1) << $bpplog2)
    , $indices(integer_t<1, 1>(1) << $bpp)
    , $channels(pass(channels))
    , $tables(pass(tables))
    , $layers(pass(layers)) {

    }
  protected:
  private:
    integer_t<0, 32767> $width;
    integer_t<0, 32767> $height;
    integer_t<0, 3> $bpplog2;
    integer_t<1, 8> $bpp;
    integer_t<1, 256> $indices;
    integer_t<1, 4> $channels;
    integer_t<1, 4> $tables;
    integer_t<0, 32767> $layers;
};







template <signed N, signed BE>
class bits_t {
  public:
    bits_t()
    : $bytes() {

    }
    bits_t(sinw value)
    : $bytes() {
      SESAME_FAIL_IF(value < 0);
      for (auto i = 0; i < size($bytes); i++) {
        $bytes[BE ? size($bytes) - 1 - i : i] = static_cast<ui08>(value);
        value >>= 8;
      }
      SESAME_FAIL_IF(static_cast<sinw>(self) != value);
    }
    bits_t(const ui08 (&data)[(N + 7) >> 3])
    : $bytes() {
      for (auto i = 0; i < size($bytes); i++) {
        $bytes[i] = data[i];
      }
      if ((N & (8 - 1)) != 0) {
        auto byte = $bytes[BE ? 0 : size($bytes) - 1];
        auto mask = static_cast<ui08>((1 << (N & (8 - 1))) - 1);
        SESAME_FAIL_IF((byte & mask) != byte);
      }
    }
    bits_t(const char (&cstr)[((N + 7) >> 3) + 1])
    : $bytes() {
      for (auto i = 0; i < size($bytes); i++) {
        $bytes[i] = cstr[i];
      }
      if ((N & (8 - 1)) != 0) {
        auto byte = $bytes[BE ? 0 : size($bytes) - 1];
        auto mask = static_cast<ui08>((1 << (N & (8 - 1))) - 1);
        SESAME_FAIL_IF((byte & mask) != byte);
      }
    }
    bits_t(owned_t<reader_t> reader, sinw& cursor)
    : $bytes() {
      reader->read(cursor, size($bytes), &$bytes[0]);
      cursor += size($bytes);
      if ((N & (8 - 1)) != 0) {
        auto byte = $bytes[BE ? 0 : size($bytes) - 1];
        auto mask = static_cast<ui08>((1 << (N & (8 - 1))) - 1);
        SESAME_FAIL_IF((byte & mask) != byte);
      }
    }
    operator sinw() const {
      auto value = 0;

// N = 40 => bytes is 5 and int is 4 bytes on 32bit
// the extra byte must be zero and bit 32 must be zreo

      if (size($bytes) > size(value)) {
        for (auto i = 0; i < size(value); i++) {
          value <<= 8;
          value |= $bytes[BE ? i : size($bytes) - 1 - i];
        }
      } else {
        for (auto i = 0; i < size($bytes); i++) {
          value <<= 8;
          value |= $bytes[BE ? i : size($bytes) - 1 - i];
        }
      }
      return value;
    }
    auto at(sinw offset)
      -> ui08& {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(offset >= N);
      return $bytes[offset];
    }
    auto at(sinw offset) const
      -> const ui08& {
      SESAME_FAIL_IF(offset < 0);
      SESAME_FAIL_IF(offset >= N);
      return $bytes[offset];
    }
    auto write(owned_t<writer_t> writer, sinw& cursor)
      -> void {
      writer->write(cursor, size($bytes), &$bytes[0]);
      cursor += size($bytes);
    }
  protected:
  private:
    ui08 $bytes[(N + 7) >> 3];
};

template <signed N>
using le_t = bits_t<N, 0>;

template <signed N>
using be_t = bits_t<N, 1>;

auto test_bits_t()
  -> void {
  auto writer = make<file_writer_t>(u8"games/test.bin");
  auto cursor = 0;
  auto le30 = le_t<40>("\xFF\xFF\xFF\xFF\xFF");
  auto be30 = be_t<40>("\xFF\xFF\xFF\xFF\xFF");
  le30.write(writer, cursor);
  be30.write(writer, cursor);
}

const char* archives[] = {
  u8"aud",
  u8"conquer",
  u8"desert",
  u8"general",
  u8"local",
  u8"scores",
  u8"setup",
  u8"sounds",
  u8"speech",
  u8"temperat",
  u8"transit",
  u8"winter",
  u8"zounds"
};

struct ccentry {
  le_t<32> checksum;
  le_t<32> offset;
  le_t<32> length;
  ccentry(owned_t<reader_t> reader, sinw& cursor)
  : checksum(reader, cursor)
  , offset(reader, cursor)
  , length(reader, cursor) {

  }
};

auto make_archive(sinw index)
  -> void {
  char filename1[256];
  snprintf(filename1, 256, u8"games/cc/archives/%s.mix", archives[index]);
  auto reader = make<file_reader_t>(filename1);
  auto cursor = 0;
  auto number_of_records = le_t<16>(reader, cursor);
  auto data_size = le_t<32>(reader, cursor);
  printf("%i\n", sinw(number_of_records));
  printf("%i\n", sinw(data_size));
  auto entries = vector_t<ccentry>(number_of_records, reader, cursor);
  for (auto i = 0; i < entries.length(); i++) {
    auto checksum = entries.at(i).checksum;
    auto offset = cursor + (sinw)entries.at(i).offset;
    auto length = (sinw)entries.at(i).length;
    printf("%i\n", (sinw)checksum);
    printf("%i\n", offset);
    printf("%i\n", length);
    auto buffer = buffer_t(make<offset_reader_t>(reader, offset, length));
    char filename[256];
    snprintf(filename, 256, u8"games/cc/archives/%s/%i.bin", archives[index], i);
    auto writer = make<file_writer_t>(filename);
    auto wcursor = 0;
    buffer.write(writer, wcursor);
  }
}

auto main()
  -> sinw {
  test_bits_t();
  for (auto i = 0; i < size(archives); i++) {
    make_archive(i);
  }
}
