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
EOF

if output=$(cd "$test_dir" && ./terivia questions </dev/null 2>&1); then
    echo "FAIL: an incomplete question was read and accepted from questions file"
    exit 1
fi

printf '%s\n' "$output" | grep -Fq "Couldn't read correct answer to question 1"

if printf '%s\n' "$output" | grep -Eq 'AddressSanitizer|LeakSanitizer|runtime error:'; then
    printf '%s\n' "$output"
    exit 1
fi

printf 'PASS: Incomplete question read from questions file is not accepted\n'
