#!/usr/bin/env python3

import sys
from enum import Enum

class Quote(Enum):
    SINGLE = 0
    DOUBLE = 1

quote_stack = []

for line in sys.stdin:
    escape = False
    for char in line:
        if sys.platform == "darwin":
            if char == '\'' and not escape:
                if len(quote_stack) > 0 and quote_stack[-1] == Quote.SINGLE:
                    quote_stack.pop()
                else:
                    quote_stack.append(Quote.SINGLE)
            elif char == '\"' and not escape:
                if len(quote_stack) > 0 and quote_stack[-1] == Quote.DOUBLE:
                    quote_stack.pop()
                else:
                    quote_stack.append(Quote.DOUBLE)
            elif char == '$' and len(quote_stack) > 0:
                sys.stdout.write("\\")
            elif char == '@' and len(quote_stack) > 0:
                sys.stdout.write("$")
                continue
            
            sys.stdout.write(char)
            if char == '\\':
                escape = True
            else:
                escape = False
        else:
            if char == '@':
                sys.stdout.write('$')
            else:
                sys.stdout.write(char)
    sys.stdout.write("\n")
 
