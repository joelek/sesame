if [ $# -eq 1 ] && ([ $1 = "debug" ] || [ $1 = "release" ]); then
  echo "[sesame]"
else
  echo "usage: make <debug|release>"
  exit 1
fi

COMPILER_OPTIONS="-std=c++11 -shared-libgcc -pedantic -Wall -Wextra -Werror -O3"

if [ $1 = "debug" ]; then
  echo "[configuration: debug]"
  COMPILER_OPTIONS+=" -D DEBUG"
fi

if [ $1 = "release" ]; then
  echo "[configuration: release]"
  COMPILER_OPTIONS+=" -s"
fi

SOURCES=(
  "sesame"
  "sesame_lib"
  "sesame_lib_array"
  "sesame_lib_bitmap"
  "sesame_lib_borrowed"
  "sesame_lib_buffer"
  "sesame_lib_buffer_reader"
  "sesame_lib_file"
  "sesame_lib_file_reader"
  "sesame_lib_file_writer"
  "sesame_lib_format_jeri"
  "sesame_lib_format_jeri_header"
  "sesame_lib_header"
  "sesame_lib_image"
  "sesame_lib_interface"
  "sesame_lib_lzss_reader"
  "sesame_lib_memory"
  "sesame_lib_offset_reader"
  "sesame_lib_offset_table"
  "sesame_lib_owned"
  "sesame_lib_primitive_be"
  "sesame_lib_primitive_le"
  "sesame_lib_reader"
  "sesame_lib_sequence"
  "sesame_lib_sequence_iterator"
  "sesame_lib_storage"
  "sesame_lib_system"
  "sesame_lib_table"
  "sesame_lib_vector"
  "sesame_lib_writer"
  "sesame_wc1"
  "sesame_wc1_archive"
  "sesame_wc1_record"
  "sesame_wc1_tile"
  "sesame_wc1_tiles"
)

TARGETS=(
  "main"
  "wc1"
)

mkdir -p build/objects
mkdir -p build/targets

rm -rf build/objects/*
rm -rf build/targets/*

OBJECTS=()

for i in ${SOURCES[@]}; do
  OBJECTS+=" build/objects/$i.o"
done

echo "[phase: sources]"

for i in ${SOURCES[@]}; do
  echo "[compiling: source/$i.cpp]"
  gcc $COMPILER_OPTIONS -c source/$i.cpp -o build/objects/$i.o
  RETURN_CODE=$?
  if [ $RETURN_CODE -gt 0 ]; then
    exit 1;
  fi
done

echo "[phase: targets]"

for i in ${TARGETS[@]}; do
  echo "[compiling: source/$i.cpp]"
  gcc $COMPILER_OPTIONS ${OBJECTS[@]} source/$i.cpp -o build/targets/$i -l stdc++
  RETURN_CODE=$?
  if [ $RETURN_CODE -gt 0 ]; then
    exit 1;
  fi
done
