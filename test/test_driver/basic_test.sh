#!/bin/bash
set -e
src_path="./test/test_dir/src"
target_path="./test/test_dir/target"
binary_path="build/scanner"
#temporary
log_file_path="./app.log"
tmp_link_dir="/tmp/link_dir"

function basic_test1()
{
    clean_test

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
    clean_test

    touch $src_path/basic_test_dir_file.txt
    mkdir $src_path/dir
    touch $src_path/dir/basic_test_dir_file.txt
    touch $src_path/dir/basic_test_dir_file2.txt
    mkdir $src_path/dir/dir
    touch $src_path/dir/dir/abc

    ./$binary_path $src_path $target_path
    sleep 1s

    if [ -f $target_path/basic_test_dir_file.txt.bak ] \
        && [ -d $target_path/dir ] \
        && [ -f $target_path/dir/basic_test_dir_file.txt.bak ] \
        && [ -f $target_path/dir/basic_test_dir_file2.txt.bak ] \
        && [ -d $target_path/dir/dir ] \
        && [ -f $target_path/dir/dir/abc.bak ] ; then 
        echo "basic_test_dir pass"
    else
        echo "basic_test_dir fail"
    fi
}

function symlinks_name_clash_case()
{
    clean_test

    echo "content_a" > $src_path/a.txt

    echo "content_a_link" > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link.txt

    ./$binary_path $src_path $target_path
}

function basic_test_symlinks()
{
    clean_test

    echo "content_a" > $src_path/a.txt

    echo "content_b" > /tmp/b.txt
    ln -s /tmp/b.txt $src_path/b_link.txt

    if [ -d /tmp/link_dir ]; then rm -rf /tmp/link_dir; fi
    mkdir -p /tmp/link_dir
    touch /tmp/link_dir/a.txt
    ln -s /tmp/link_dir $src_path/link_dir

    ./$binary_path $src_path $target_path

    #&& [ "content" == $(cat $target_path/abc.txt) ] \
    if [ -f $target_path/a.txt.bak ] \
    && [ -f $target_path/b.txt.bak ] \
    && [ -d $target_path/link_dir ] \
    && [ -f $target_path/link_dir/a.txt.bak ] ; then
        echo "basic symlink test pass"
    else
        echo "basic symlink test fail"
    fi
}

function clean_test()
{
	rm -rf $target_path/*
	rm -rf $src_path/*
	if [ -f app.log ]; then rm app.log; fi 
	#if [ -d $tmp_link_dir ]; then rm $tmp_link_dir/*; fi 
}

basic_test1
basic_test_dir
basic_test_symlinks
