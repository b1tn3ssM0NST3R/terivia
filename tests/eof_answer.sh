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

output=$(cd "$test_dir" && ./terivia questions </dev/null)

if printf '%s\n' "$output" | grep -Fq 'Your score:'; then
    echo "FAIL: score displayed after eof was input"
    exit 1
fi

printf 'PASS: No score is display for eof answer inputs\n'
