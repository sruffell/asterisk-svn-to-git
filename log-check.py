#!/usr/bin/python
# Copyright 2014 Shaun Ruffell <sruffell@digium.com>

import sys
import re
import functools

def load_patterns(filename):
    patterns = []
    for line in open(filename, "r"):
        line = line.strip()
        if line.startswith("#") or len(line) == 0:
            continue
        patterns.append(re.compile(line, re.IGNORECASE))
    return patterns
    
def main():
    authors = set()

    patterns = load_patterns("patterns.txt")

    for line in sys.stdin:
        for pat in patterns:
            match = pat.search(line)
            if None != match:
                authors.add(match.group(1).strip(" \r\n\t."))

    def mycmp(lhs, rhs):
        value = cmp(lhs.lower(), rhs.lower())
        if 0 == value:
            return cmp(lhs, rhs)
        return value
    for author in sorted(authors, key=functools.cmp_to_key(mycmp)):
        sys.stdout.write(author + "\n");

    return 0

if "__main__" == __name__:
    sys.exit(main())
