#!/bin/bash

if [ ! -d "logs" ]
then
    echo "Creating log folder"
    mkdir logs
else
    echo "Log folder already present; skipping"
fi
