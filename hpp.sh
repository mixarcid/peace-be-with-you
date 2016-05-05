#!/bin/bash

EXTENSION=pp
HEADERS=`find src src/Standard -name "*.h"`

for h in $HEADERS
do
    cp $h $h$EXTENSION
done