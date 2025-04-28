#!/usr/bin/bash

# Rename all sections from .sect.* to .user_sect.*
# This is used to appropriately link files from the userspace

# Input and output object files passed as args
INPUT_FILE=$1
OUTPUT_FILE=$2
TOOLCHAIN=$3

for section in $($TOOLCHAIN-objdump -h $INPUT_FILE | awk '/^[ ]+[0-9]+/ {print $2}'); do
    if [[ $section =~ ^\. ]]; then
        new_section=".user_${section:1}"
        $TOOLCHAIN-objcopy --rename-section $section=$new_section $INPUT_FILE $OUTPUT_FILE
    fi
done
