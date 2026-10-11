#!/bin/sh
set -eu

test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
cp ./terivia "$test_dir/terivia"

cat > "$test_dir/questions" <<'EOF'
What is the capital of Sweden?
Helsinki
Oslo
Stockholm
Medellin
c
EOF

output=$(cd "$test_dir" && printf 'notavalidanswer\nc\nn\n' | ./terivia)

printf '%s\n' "$output" | grep -Fq 'Answer should be a, b, c or d.'
printf '%s\n' "$output" | grep -Fq 'Your score: 1 out of 1'
