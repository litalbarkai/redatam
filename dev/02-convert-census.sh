#!/bin/bash

for file in downloads/*.zip; do
  filename=$(basename "$file" .zip)
  # mkdir -p "downloads/$filename"
  if [ ! -d "downloads/$filename" ]; then
    unzip "$file" -d "downloads/$filename"
  fi
done

for file in $(find downloads -type f -name "*.dicx"); do
  # skip corrupt files
  if [[ "$file" == *CP2011URY* ]]; then
    echo "Skipping $file..."
    continue
  fi

  dirname=$(dirname "$file")
  dirname=$(echo "$dirname" | cut -d'/' -f1,2)
  dirname="$dirname/open-redatam-cpp/"
  mkdir -p "$dirname"
  
  if [ $(ls -1 "$dirname" | wc -l) -eq 0 ]; then
    echo "Converting $file..."
    ./redatam "$file" "$dirname"
  fi
done

for file in $(find downloads -type f -name "*.dic"); do
  # skip corrupt files
  if [[ "$file" == *CP2011URY* ]]; then
    echo "Skipping $file..."
    continue
  fi

  dirname=$(dirname "$file")
  dirname=$(echo "$dirname" | cut -d'/' -f1,2)
  dirname="$dirname/open-redatam-cpp/"
  mkdir -p "$dirname"
  
  if [ $(ls -1 "$dirname" | wc -l) -eq 0 ]; then
    echo "Converting $file..."
    ./redatam "$file" "$dirname"
  fi
done
