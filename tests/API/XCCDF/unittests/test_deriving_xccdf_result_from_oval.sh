#!/bin/bash

set -e
set -o pipefail

name=$(basename $0 .sh)

result=$(mktemp -t ${name}.out.XXXXXX)
stderr=$(mktemp -t ${name}.out.XXXXXX)

$OSCAP xccdf eval --results $result $srcdir/${name}.xccdf.xml 2> $stderr

echo "Stderr file = $stderr"
echo "Result file = $result"
[ -f $stderr ]; [ ! -s $stderr ]; rm -rf $stderr

$OSCAP xccdf validate-xml $result

assert_exists() { [ "$(xpath $result 'count('"$2"')')" == "$1" ]; }
assert_exists 8 '//rule-result'
assert_exists 8 '//rule-result/result'
assert_exists 8 '//rule-result/result[text()="pass"]'
assert_exists 8 '//rule-result/check'
assert_exists 8 '//rule-result/check[@system="http://oval.mitre.org/XMLSchema/oval-definitions-5"]'
assert_exists 4 '//rule-result/check[@negate="true"]'
assert_exists 8 '//rule-result/check/check-content-ref'
assert_exists 4 '//rule-result/check/check-content-ref[@href="test_deriving_xccdf_result_from_oval_fail.oval.xml"]'
assert_exists 4 '//rule-result/check/check-content-ref[@href="test_deriving_xccdf_result_from_oval_pass.oval.xml"]'
assert_exists 0 '//message'

# TODO investigate the given score and assert for this.

rm -rf $result