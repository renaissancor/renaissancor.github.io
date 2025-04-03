#!/opt/homebrew/bin/bash
# MacOS
#!/bin/bash
# Linux

# Flock usage, only one process write to file at same time
exec 200>counter.txt.lock      # Create or open lock file
flock -n 200 || exit 1         # Get lock, exit if locked

# If file does not exist create starting with 0
if [ ! -f counter.txt ]; then
    echo 0 > counter.txt
fi

# Read the last number from the file
last_num=$(tail -n 1 counter.txt)

# Create new number incremented one from last number
new_num=$((last_num + 1))

# Append new number to result
echo $new_num >> counter.txt