#!/bin/bash

readonly ARCHIVE="gcd_assignments.zip"
readonly TEST_SCRIPT="test_gcd.sh"

process_file() {
	# Function arguments are $1, $2, etc.
	echo "Processing $1"

	# Extract the name of the file without
	local base=${1%.*}
	
	# cut expected either file input or data on standard in 
	# <<< places the value of the variable on standard in 
	# and is calling a 'here string'
	local first="$(cut -d'_' -f3 <<<$base)" #opening a subshell 
	local last="$(cut -d'_' -f2 <<<$base)"

	echo "Author: $first $last"
	local dirname="$last"_"$first"
	mkdir -p "$dirname"
	mv "$1" "$dirname"
	cp "$TEST_SCRIPT" "$dirname"
	cd "$dirname"
	
	unzip -o "$1"
	rm "$1"
	
	bash "$TEST_SCRIPT" | tee grade.txt	#piping to file grade.txt
	
	
	cd - > /dev/null
	
}
unzip -o "$ARCHIVE"
for f in *.zip; do
	if [ "$f" != "$ARCHIVE" ]; then
		process_file "$f"
	fi
done
