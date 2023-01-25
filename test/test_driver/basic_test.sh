#!/bin/bash
src_path="./test/test_dir/src"
target_path="./test/test_dir/target"
binary_path="build/main.out"

#function basic_test1()
#{
    rm $target_path/*;

    touch $src_path/a
    touch $src_path/a.txt
    ./$binary_path $src_path $target_path
    sleep 1s
    if [ -f $target_path/a.txt.bak ] && [ -f $target_path/a.bak ]; then
        echo "basic_test1 pass";
    else
        echo "basic_test1 failed";
    fi
#}

#basic_test1()
