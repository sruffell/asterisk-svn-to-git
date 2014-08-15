# Copyright 2014 Shaun Ruffell <sruffell@digium.com>

CFLAGS:=-Wall -Os --std=c++11

log-check: log-check.cpp
	$(CXX) $(CFLAGS) -o $@ $< -lboost_regex

.PHONY: clean
clean:
	@rm -fr log-check log-check.o
