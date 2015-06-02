#!/bin/bash
valgrind --leak-check=full bin/peace $@
