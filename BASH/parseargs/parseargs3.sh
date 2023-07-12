#!/bin/bash
###############################################################################
# parseargs3.sh
# Takes in a list of files and optional -p and -s flags.
# Prints the files that exist.
# If -p is provided, the permissions of the file are printed.
# If -s <bytes> is provided, only files of up to <bytes> bytes are displayed.
# Example: ./parseargs3.sh -p -s 10000 ~/*
# Passes shellcheck with no warnings.
###############################################################################

permissions_flag=0
size_flag=0
max_size=0

while getopts ":ps:" option; do
    case "$option" in
        p) permissions_flag=1
           ;;
        s) size_flag=1
           max_size=$OPTARG
           ;;
        ?) echo "Error: Unknown option '-$OPTARG'." >&2
           exit 1
           ;;
    esac
done

if ! [ "$max_size" -eq "$max_size" ] 2>/dev/null; then
    echo "Error: Invalid value '$max_size' for size option." >&2
    exit 1
fi

# Skip over all the arguments with flags processed in the loop above.
shift "$((OPTIND-1))"

# How many flags are there?
count=$(( permissions_flag + size_flag ))

if [ $size_flag -eq 1 ]; then
    echo "Listing all regular files whose size is at most $max_size bytes."
fi

print_size=0

# Loop over all positional arguments.
for f in "$@"; do
    if [ -f "$f" ]; then
        if [ $count -eq 0 ]; then
            echo "$f"
            continue
        fi

        file_listing=$(ls -l "$f" 2>/dev/null)
        if [ $size_flag -eq 1 ]; then
            file_size=$(cut -d' ' -f5 <<< "$file_listing")
            if [ "$file_size" -le "$max_size" ]; then
                print_size=1
            else
                print_size=0
            fi
        fi

        if [[ $size_flag -eq 0 || $print_size -eq 1 ]]; then
            echo -n "$f: "
            if [ $permissions_flag -eq 1 ]; then
                # Use cut to split the string into pieces.
                permissions=$(cut -d' ' -f1 <<< "$file_listing")
                if [ $count -eq 1 ]; then
                    echo "$permissions"
                else
                    echo -n "$permissions, "
                fi
            fi
            if [ $print_size -eq 1 ]; then
                echo "$file_size bytes"
            fi
        fi
    fi
done

exit 0
