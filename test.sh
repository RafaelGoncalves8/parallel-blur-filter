#!/bin/sh

for filename in data/*.jpg; do
    echo "Processing file $filename..."
    echo ""
    ./show_time.sh $filename
    echo ""
    echo "...Done."
    echo ""
done

