#!/bin/sh
set -eu

test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
cp ./terivia "$test_dir/terivia"

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21; do
cat >> "$test_dir/questions" <<'EOF'
What is the capital of Sweden?
Helsinki
Oslo
Stockholm
Medellin
c
EOF
done

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21; do
	printf "c\n" >> "$test_dir/answers"
done
printf "n\n" >> "$test_dir/answers"

output=$(cd "$test_dir" && ./terivia questions < answers)

printf '%s\n' "$output" | grep -Fq "Your score: 21 out of 21"

printf 'PASS: Storage for questions grow correctly as more questions are added\n'
