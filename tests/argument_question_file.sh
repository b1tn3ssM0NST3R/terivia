#!/bin/sh
set -eu

test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
cp ./terivia "$test_dir/terivia"

cat > "$test_dir/other_questions" <<'EOF'
What is the capital of Sweden?
Helsinki
Oslo
Stockholm
Medellin
c
EOF

output=$(cd "$test_dir" && printf 'c\nn\n' | ./terivia other_questions)

printf '%s\n' "$output" | grep -Fq 'Your score: 1 out of 1'

printf 'PASS: Passing question files through command line arguments work\n'
