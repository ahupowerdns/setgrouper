#!/bin/sh
./grouper testdata/een testdata/twee testdata/drie > testdata/real.1
cmp testdata/real.1 testdata/test.1
return $?

