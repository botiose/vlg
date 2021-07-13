#!/bin/bash

# cat cur-leaks.log | sed -n "s/.*used_suppression: *[0-9]* *\([^ ]*\) etc\/valgrind.supp:[0-9]* suppressed: \(.*\) in.*/\1: \2/p"
# cat cur-leaks.log | sed -n "\$s/.*suppressed: \([0-9]*\) from \([0-9]*\).*/\1\/\2/p"
grep -A 4 'LEAK SUMMARY' cur-leaks.log | sed 's/==.*== \(.*\)/\1/g'
