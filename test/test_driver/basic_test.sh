#!/bin/bash
set -e
src_path="./test/test_dir/src"
target_path="./test/test_dir/target"
binary_path="build/scanner"
#temporary
log_file_path="./app.log"
tmp_link_dir="/tmp/link_dir"

RED_OUTPUT_COLOR='\033[0;31m'
GREEN_OUTPUT_COLOR='\033[0;32m'

function echo_pass()
{
    test_name=$1
    echo -e "${GREEN_OUTPUT_COLOR} $test_name PASS"
}

function echo_fail()
{
    test_name=$1
    echo -e "${RED_OUTPUT_COLOR} $test_name FAIL"
}

function exec_sut()
{
    ./$binary_path $src_path $target_path
}

function basic_test1()
{
    clean_test

    touch $src_path/a
    touch $src_path/a.txt
    exec_sut
    sleep 1s
    if [ -f $target_path/a.txt.bak ] && [ -f $target_path/a.bak ]; then
        echo_pass basic_test1
    else
        echo_fail basic_test1
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

    exec_sut
    sleep 1s

    if [ -f $target_path/basic_test_dir_file.txt.bak ] \
        && [ -d $target_path/dir ] \
        && [ -f $target_path/dir/basic_test_dir_file.txt.bak ] \
        && [ -f $target_path/dir/basic_test_dir_file2.txt.bak ] \
        && [ -d $target_path/dir/dir ] \
        && [ -f $target_path/dir/dir/abc.bak ] ; then 
        echo_pass "basic_test_dir"
    else
        echo_fail "basic_test_dir"
    fi
}

function symlinks_name_clash_case()
{
    clean_test

    echo "content_a" > $src_path/a.txt

    echo "content_a_link" > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link.txt

    exec_sut
}

function basic_test_symlinks()
{
    clean_test

    mkdir -p $tmp_link_dir/link_dir
    if [ -d $tmp_link_dir/link_dir ]; then rm -rf $tmp_link_dir/link_dir/*; fi

    touch $src_path/a.txt

    touch $tmp_link_dir/b.txt
    ln -s $tmp_link_dir/b.txt $src_path/b_link.txt

    touch $tmp_link_dir/link_dir/a.txt
    ln -s $tmp_link_dir/link_dir $src_path/link_dir

    exec_sut

    if [ -f $target_path/a.txt.bak ] \
    && [ -f $target_path/b_link.txt.bak ] \
    && [ -d $target_path/link_dir ] \
    && [ -f $target_path/link_dir/a.txt.bak ] ; then
        echo_pass "basic_test_symlinks" 
    else
        echo_fail "basic_test_symlinks" 
    fi
}

function clean_tmp_link_dir()
{
    if [ -d $tmp_link_dir/link_dir ]; then 
        rm -rf $tmp_link_dir/link_dir/*
    else
        mkdir -p $tmp_link_dir
    fi
}

function backup_chained_symlinks()
{
    clean_test

    a_content="a_content"
    echo $a_content > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $tmp_link_dir/a_link
    ln -s $tmp_link_dir/a_link $src_path/a_link_link

    exec_sut

    if [ -f $target_path/a_link_link.bak ] \
    && [ $(cat $target_path/a_link_link.bak) = $a_content ]; then
        echo_pass "$0"
    else
        echo_fail "$0"
    fi
}

function fail_on_dangling_symlink()
{
    clean_test

    touch $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link

    rm $tmp_link_dir/a.txt

    exec_sut
}

function delete_no_effect_case()
{
    clean_test

    touch $src_path/a.txt
    exec_sut

    [ -f $target_path/a.txt.bak ]

    touch $src_path/delete_a.txt
    exec_sut
    #the way the fs tree is iterated rn
    #the deletion of $src_path/a.txt might not get observed
    #by fs::recursive_iterator and this non-existing file
}

function delete_symlink_case()
{
    clean_test

    mkdir -p $/tmp_link_dir
    touch $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/alink

    exec_sut
    
    echo "foo"
    [ -f $target_path/a.txt.bak ]
    echo "bar"

    mv $src_path/alink $src_path/delete_alink

    exec_sut

    #What then?

}

function basic_delete_test()
{
    clean_test 

    touch $src_path/a.txt
    exec_sut

    [ -f $target_path/a.txt.bak ]

    mv $src_path/a.txt $src_path/delete_a.txt
    exec_sut

    if [ ! -f $target_path/a.txt.bak ] \
    && [ ! -f $src_path/a.txt ] \
    && [ ! -f $src_path/delete_a.txt ] ; then
        echo_pass "$0" 
    else
        echo_fail "$0" 
    fi
}

function backup_modified_files
{
    clean_test

    a_content="content_a_1"
    b_content="content_b_1"
    echo $a_content > $src_path/a.txt
    echo $b_content> $src_path/b.txt

    exec_sut

    
    [ -f $target_path/a.txt.bak ]
    [ -f $target_path/b.txt.bak ]

    sleep 0.1s
    b_content_new="content_b_2"
    echo $b_content_new > $src_path/b.txt

    exec_sut

    if [ -f $target_path/a.txt.bak ] \
    && [ -f $target_path/b.txt.bak ] \
    && [ $(cat $target_path/a.txt.bak) = $a_content ] \
    && [ $(cat $target_path/b.txt.bak) = $b_content_new ]; then
        echo_pass "$0"
    else
        echo_fail "$0"
    fi
}

function modified_file_toggling_no_backup()
{
    clean_test

    pass_number=0
    fail_number=0
    for _ in {1..100}; do
        echo "a1" > $src_path/a.txt
        exec_sut
        #passes only with sleep command below 
        #sleep 0.1s
        echo "a2" > $src_path/a.txt
        exec_sut
        if [ $(cat $target_path/a.txt.bak) = "a2" ]; then
            pass_number=$(($pass_number+1))
        else
            fail_number=$(($fail_number+1))
        fi
    done
    echo "pass/fail: $pass_number/$fail_number"

    if [ $fail_number -eq 0 ]; then
        echo_pass "$0"
    else
        echo_fail "$0"
    fi
}

function clean_test()
{
	rm -rf $target_path/*
	rm -rf $src_path/*
	if [ -f app.log ]; then rm app.log; fi 
    clean_tmp_link_dir
}

basic_test1
#basic_test_dir
#basic_test_symlinks
#basic_delete_test
#backup_modified_files
#modified_file_toggling_no_backup
#backup_chained_symlinks
#fail_on_dangling_symlink