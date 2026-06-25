#!/usr/bin/env bash
# size-report.sh
# Analyzes the ELF file to report flash and RAM usage.

set -e

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path-to-elf>"
    exit 1
fi

ELF_FILE="$1"

if [ ! -f "$ELF_FILE" ]; then
    echo "Error: ELF file not found: $ELF_FILE"
    exit 1
fi

echo "Size report for: $ELF_FILE"
size "$ELF_FILE"
