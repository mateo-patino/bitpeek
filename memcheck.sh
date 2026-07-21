#!/usr/bin/env bash

# This shell script tests for memory leaks in bitpeek. It must be ran in a Linux-based environment.
if ! command -v valgrind >/dev/null 2>&1; then
    cat << end >&2
Error: valgrind was not found.
Use a Linux-based machine or another kind of environment where valgrind is supported.
end
    exit 1
fi

# Check bitpeek executable exists
if [ ! -f "bitpeek" ]; then
    echo "No 'bitpeek' executable was found. Build 'bitpeek' executable before memory checking." >&2
    exit 1
fi

# Table of expressions to feed bitpeek
expressions=(
    "1 + 1",
    "1 + 0x1 + 01 + 0b1"
)

# Total and pass counters
total_tests="${#expressions[@]}"
pass=0

# Calls valgrind on a single expression and increments the mem_pass counter accordingly.
# Takes ONE argument only: the string expression fed into bitpeek
runMemoryCheck() {
    local expr="$1"
    success=1

    if ./valgrind \
        --quiet \
        --leak-check=full \
        --error-exitcode=1 \
        ./bitpeek "$expr"
    then
        ((pass++))
        success=0 # Global variable checked by the caller
    fi
}

line="------------"
printf "%sMEMORY TESTS%s\n" "$line" "$line"

# Iterate through expression table
for expr in "${expressions[@]}"; do
    runMemoryCheck "expr"
    if success
    then
        printf "./bitpeek %s PASS\n" "$expr"
    else 
        printf "./bitpeek %s FAIL\n" "$expr" >&2
    fi
done

# Overall summary
echo ""
printf "%sSUMMARY%s\n" "$line" "$line"
printf "Total tests: %i\n" "$total_tests"
printf "Successful: %i\n" "$pass"
printf "Failed: %i\n" $(( total_tests-pass ))
