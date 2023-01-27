#!/bin/bash
set -e
src_path="./test/test_dir/src"
target_path="./test/test_dir/target"
binary_path="build/scanner"
#temporary
log_file_path="./app.log"

function basic_test1()
{
    touch $src_path/a
    touch $src_path/a.txt
    ./$binary_path $src_path $target_path
    sleep 1s
    if [ -f $target_path/a.txt.bak ] && [ -f $target_path/a.bak ]; then
        echo "basic_test1 pass";
    else
        echo "basic_test1 failed";
    fi
}

function check_app_log()
{
    filename_csv_file_index := 2
    message_csv_file_index := 3
    while read -r line; do
        IFS=',' read -rA ADDR <<< "$line"
        
    done < app.log
}

function log_basic_test1()
{
    basic_test1


}

basic_test1
