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
        echo "basic_test1 fail";
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

function basic_test_dir()
{
    touch $src_path/basic_test_dir_file.txt
    mkdir $src_path/dir
    touch $src_path/dir/basic_test_dir_file.txt
    touch $src_path/dir/basic_test_dir_file2.txt

    ./$binary_path $src_path $target_path
    sleep 1s

    if [ -f $target_path/basic_test_dir_file.txt.bak ] \
        && [ -d $target_path/dir ] \
        && [ -f $target_path/dir/basic_test_dir_file.txt.bak ] \
        && [ -f $target_path/dir/basic_test_dir_file2.txt.bak ]; then 
        echo "basic_test_dir pass"
    else
        echo "basic_test_dir fail"
    fi
}

function clean_test()
{
	rm -rf $target_path/*
	rm -rf $src_path/*
	if [ -f app.log ]; then rm app.log; fi 
}

clean_test
basic_test1
clean_test
basic_test_dir