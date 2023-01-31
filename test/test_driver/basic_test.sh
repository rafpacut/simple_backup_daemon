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
DEFAULT_OUTPUT_COLOR='\033[0m'

function echo_pass()
{
    echo -e "${GREEN_OUTPUT_COLOR} PASS ${DEFAULT_OUTPUT_COLOR}"
}

function echo_fail()
{
    echo -e "${RED_OUTPUT_COLOR} FAIL ${DEFAULT_OUTPUT_COLOR}"
}

function echo_start()
{
    echo "starting testcase $1"
}

function exec_sut()
{
    ./$binary_path $src_path $target_path
}

function basic_test1()
{
    echo_start "$FUNCNAME"
    clean_test

    touch $src_path/a
    touch $src_path/a.txt
    exec_sut
    sleep 1s
    if [ -f $target_path/a.txt.bak ] && [ -f $target_path/a.bak ]; then
        echo_pass
    else
        echo_fail
    fi
}

function basic_test_dir()
{
    echo_start "$FUNCNAME"
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
        echo_pass
    else
        echo_fail
    fi
}

function symlinks_name_clash_case()
{
    echo_start "$FUNCNAME"
    clean_test

    echo "content_a" > $src_path/a.txt

    echo "content_a_link" > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link.txt

    exec_sut
}

function basic_test_symlinks()
{
    echo_start "$FUNCNAME"
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
        echo_pass
    else
        echo_fail
    fi
}

function clean_tmp_link_dir()
{
    if [ -d $tmp_link_dir ]; then
        rm -rf $tmp_link_dir
        #for i in "$tmp_link_dir/link_dir/*"; do
            #echo "about to delete $i"
            #rm -rf $i;
        #done
        #rm -rf $tmp_link_dir/link_dir/*
    fi
    mkdir -p $tmp_link_dir
}

function backup_chained_symlinks()
{
    echo_start "$FUNCNAME"
    clean_test

    a_content="a_content"
    echo $a_content > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $tmp_link_dir/a_link
    ln -s $tmp_link_dir/a_link $src_path/a_link_link

    exec_sut

    if [ -f $target_path/a_link_link.bak ] \
    && [ $(cat $target_path/a_link_link.bak) = $a_content ]; then
        echo_pass
    else
        echo_fail
    fi
}

function expect_error()
{
    echo "expect_error_placeholder"
}

function fail_on_dangling_symlink()
{
    echo_start "$FUNCNAME"
    clean_test

    touch $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link

    rm $tmp_link_dir/a.txt

    exec_sut

    expect_error
}

function delete_no_effect_case()
{
    echo_start "$FUNCNAME"
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
    echo_start "$FUNCNAME"
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

function basic_delete_test_rename_original_source_file()
{
    echo_start "$FUNCNAME"
    clean_test 

    touch $src_path/a.txt
    exec_sut

    [ -f $target_path/a.txt.bak ]

    mv $src_path/a.txt $src_path/delete_a.txt
    exec_sut

    if [ ! -f $target_path/a.txt.bak ] \
    && [ ! -f $src_path/a.txt ] \
    && [ ! -f $src_path/delete_a.txt ] ; then
        echo_pass
    else
        echo_fail
    fi
}

function basic_delete_test_create_tagged_file()
{
    #GIVEN:
    #./src/file and ./target/file.bak
    #WHEN:
    #touch ./src/delete_file
    #outcome should be:
    #./src empty and ./target/empty

    echo_start "$FUNCNAME"
    clean_test

    touch $src_path/file
    exec_sut

    [ -f $target_path/file.bak ]

    touch $src_path/delete_file
    exec_sut

    if [ "$(ls -A $src_path)" ] \
    && [ "$(ls -A $target_path)" ]; then
        echo_pass
    else
        echo_fail
    fi
}

function backup_modified_files
{
    echo_start "$FUNCNAME"
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
        echo_pass
    else
        echo_fail
    fi
}

function modified_file_toggling_no_backup()
{
    echo_start "$FUNCNAME"
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
        echo_pass
    else
        echo_fail
    fi
}

function clean_test()
{
	rm -rf $target_path/*
	rm -rf $src_path/*
	if [ -f app.log ]; then rm app.log; fi 
    clean_tmp_link_dir
}

function check_app_log()
{
    filename_csv_file_index=2
    message_csv_file_index=3
    while IFS="," read -r date timestamp path message; do
        echo "$path"
        echo "$message"
        if [ ! $(echo $message | grep "*ERR*") = "" ]; then
            echo_fail;
            return;
        fi
    done < <(cat app.log)
}

function log_basic_test1()
{
    basic_test1
    check_app_log
}

basic_test1
#basic_test_dir
#basic_test_symlinks
#basic_delete_test
*basic_delete_test_create_tagged_file
#backup_modified_files
#backup_chained_symlinks
#fail_on_dangling_symlink

#log_basic_test1

#modified_file_toggling_no_backup