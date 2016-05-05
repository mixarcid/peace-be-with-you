#!/usr/bin/env python

import sys

escape = False
single_quote = False
double_quote = False

for line in sys.stdin:
    for char in line:
        if char == '\'' and not escape:
            single_quote = not single_quote
        elif char == '\"' and not escape:
            double_quote = not double_quote
        elif char == '$' and (single_quote or double_quote):
            sys.stdout.write("\\")
        elif char == '@' and (single_quote or double_quote):
            sys.stdout.write("$")
            continue
        sys.stdout.write(char)
        if char == '\\':
            escape = True
        else:
            escape = False
    sys.stdout.write("\n")
 
