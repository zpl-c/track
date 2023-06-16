#!/bin/bash

cd "$(dirname $0)"

echo "track.h"
echo "zpl_hedley.h"

pushd ../code > /dev/null
    find header -type f
    find source -type f
popd > /dev/null
