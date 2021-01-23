#!/bin/bash
find src -name \*.h | xargs grep -h  "void lv" | sed 's/(/ /g' | awk '{print $2}'
