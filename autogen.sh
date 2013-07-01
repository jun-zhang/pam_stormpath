#!/bin/sh
set -e
autoreconf --install --force
rm -rf autom4te.cache
