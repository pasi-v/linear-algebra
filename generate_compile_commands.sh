#!/bin/bash

# Define some basic compilation flags
CXX=clang++
CXXFLAGS="-std=c++11 -Wall -Wextra -g"

# Output file
OUT_FILE="compile_commands.json"

# Get absolute path to the current directory
PROJECT_DIR="$(pwd)"

# Start JSON array
echo "[" > "$OUT_FILE"

# Loop over all .cpp files in the current directory
FIRST=1
for FILE in *.cpp; do
  # Skip if no .cpp files are found
  [ -e "$FILE" ] || continue

  # Add comma between entries, but not before the first one
  if [ $FIRST -eq 0 ]; then
    echo "," >> "$OUT_FILE"
  fi

  # Write one entry
  echo "  {" >> "$OUT_FILE"
  echo "    \"directory\": \"${PROJECT_DIR}\"," >> "$OUT_FILE"
  echo "    \"command\": \"${CXX} ${CXXFLAGS} -c ${FILE}\"," >> "$OUT_FILE"
  echo "    \"file\": \"${FILE}\"" >> "$OUT_FILE"
  echo -n "  }" >> "$OUT_FILE"

  FIRST=0
done

# Close JSON array
echo "" >> "$OUT_FILE"
echo "]" >> "$OUT_FILE"

echo "✅ Generated compile_commands.json with $(ls *.cpp 2>/dev/null | wc -l) entries."
