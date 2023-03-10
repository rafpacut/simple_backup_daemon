#!/bin/bash
set -e
test_dir="./test/test_dir"
src_path="$test_dir/src"
target_path="$test_dir/target"
target_expected_path="./test/test_dir/target_expected"
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
    debug_run_symbol="d"
    ./$binary_path $src_path $target_path $debug_run_symbol
}

function expect_error()
{
    if [ "$(cat $log_file_path| grep ERR | wc -l)" -gt 0 ]; then
        return 0;
    else
        return 1;
    fi
}

function expect_source_path_empty()
{
    if [ -z "$(ls -A $src_path)" ]; then
        return 0;
    else
        return 1;
    fi
}

function compare_target_with_expected()
{
    if [ -z "$(diff -qr $target_path $target_expected_path)" ]; then
        echo_pass
    else
        echo_fail
    fi
}

function clean_tmp_link_dir()
{
    if [ -d $tmp_link_dir ]; then
        rm -rf $tmp_link_dir
    fi
    mkdir -p $tmp_link_dir
}

function clean_test()
{
	rm -rf $target_path/*
	rm -rf $src_path/*
	if [ -f app.log ]; then rm app.log; fi 
    clean_tmp_link_dir
    rm -rf $target_expected_path/*
}

function basic_test1()
{
    #GIVEN:
    #   src
    #   ├── a
    #   └── a.txt
    #WHEN:
    #   binary executed
    #THEN:
    #   target
    #   ├── a.bak
    #   └── a.txt.bak


    echo_start "$FUNCNAME"
    clean_test

    #setup
    echo "a" > $src_path/a
    echo "aa" > $src_path/a.txt

    #expected
    echo "a" > $target_expected_path/a.bak
    echo "aa" > $target_expected_path/a.txt.bak

    exec_sut

    compare_target_with_expected
}

function basic_test_dir()
{
    #GIVEN:
    #   src
    #   └── dir
    #       ├── a.txt
    #       ├── b.txt
    #       └── dir
    #           └── abc
    #WHEN:
    #   binary executed
    #THEN:
    #   target
    #   └── dir
    #       ├── a.txt.bak
    #       ├── b.txt.bak
    #       └── dir
    #           └── abc.bak

    echo_start "$FUNCNAME"
    clean_test

    mkdir -p $src_path/dir/dir
    echo "a" > $src_path/dir/a.txt
    echo "b_content" > $src_path/dir/b.txt
    echo "content_abc" > $src_path/dir/dir/abc

    exec_sut
    mkdir -p $target_expected_path/dir/dir
    echo "a" > $target_expected_path/dir/a.txt.bak
    echo "b_content" > $target_expected_path/dir/b.txt.bak
    echo "content_abc" > $target_expected_path/dir/dir/abc.bak

    compare_target_with_expected
}

function basic_test_symlinks()
{
    #GIVEN:
    #   src
    #   ├── a_link.txt -> /tmp/link_dir/a.txt
    #   └── link_dir -> /tmp/link_dir/link_dir
    #   where
    #   /tmp/link_dir/link_dir
    #   └── b.txt
    #WHEN:
    #   binary executed
    #THEN:
    #   target
    #   ├── a_link.txt.bak
    #   └── link_dir
    #       └── b.txt.bak

    echo_start "$FUNCNAME"
    clean_test

    mkdir -p $tmp_link_dir/link_dir
    if [ -d $tmp_link_dir/link_dir ]; then rm -rf $tmp_link_dir/link_dir/*; fi

    #setup
    echo "content_a" > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link.txt

    echo "content_b" > $tmp_link_dir/link_dir/b.txt
    ln -s $tmp_link_dir/link_dir $src_path/link_dir

    #expected target
    echo "content_a" > $target_expected_path/a_link.txt.bak
    mkdir -p $target_expected_path/link_dir
    echo "content_b" > $target_expected_path/link_dir/b.txt.bak

    exec_sut

    compare_target_with_expected
}

function backup_chained_symlinks()
{
    #GIVEN:
    #   src
    #   └── a_link_link -> /tmp/link_dir/a_link -> /tmp/link_dir/a.txt
    #WHEN:
    #   binary executed
    #THEN:
    #   target
    #   └── a_link_link.bak
    
    echo_start "$FUNCNAME"
    clean_test

    #setup
    a_content="a_content"
    echo $a_content > $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $tmp_link_dir/a_link
    ln -s $tmp_link_dir/a_link $src_path/a_link_link

    #expected
    echo $a_content > $target_expected_path/a_link_link.bak

    exec_sut

    compare_target_with_expected
}

function fail_on_dangling_symlink()
{
    #GIVEN:
    #   ./src contains a link whose target doesn't exist
    #WHEN:
    #   binary executed
    #THEN:
    #   target should be empty,
    #   error should be present in the app log

    echo_start "$FUNCNAME"
    clean_test

    #setup
    touch $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/a_link
    rm $tmp_link_dir/a.txt

    #expected
    #empty $target_expected_path

    exec_sut

    if expect_error; then
        compare_target_with_expected
    else
        echo_fail
    fi
}

function delete_symlink_test()
{
    #GIVEN:
    #   src
    #   └── alink -> /tmp/link_dir/a.txt
    #   and
    #   target
    #   └── alink.bak
    #WHEN:
    #   mv alink delete_alink
    #   binary executed
    #THEN:
    #   target should be empty
    #   src should be empty

    echo_start "$FUNCNAME"
    clean_test

    #setup
    mkdir -p $tmp_link_dir
    touch $tmp_link_dir/a.txt
    ln -s $tmp_link_dir/a.txt $src_path/alink

    exec_sut
    
    mv $src_path/alink $src_path/delete_alink

    #expected
    #target_expected directory is empty 

    exec_sut

    if expect_source_path_empty; then
        compare_target_with_expected;
    else
        echo_fail
    fi
}

function basic_delete_test_rename_original_source_file()
{
    #GIVEN:
    #src
    #└── a.txt
    #and
    #target
    #└── a.txt.bak
    #WHEN:
    #mv ./src/a.txt ./src/delete_a.txt
    #THEN
    #./src empty and ./target empty

    echo_start "$FUNCNAME"
    clean_test 

    #setup
    touch $src_path/a.txt
    exec_sut

    mv $src_path/a.txt $src_path/delete_a.txt
    exec_sut

    #expected
    mkdir -p $target_expected_path
    if expect_source_path_empty; then
        compare_target_with_expected 
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

    #setup
    touch $src_path/file
    exec_sut

    touch $src_path/delete_file

    #expected
    mkdir -p $target_expected_path

    exec_sut

    if expect_source_path_empty; then
        compare_target_with_expected 
    else
        echo_fail
    fi
}

function backup_modified_files
{
    #GIVEN:
    #src
    #├── a.txt (containing "content_a_1")
    #└── b.txt (containing "content_b_1")
    #and
    #target
    #├── a.txt.bak (containing "content_a_1")
    #└── b.txt.bak (containing "content_b_1")
    #WHEN:
    #./src/b.txt changes content to "content_b_2"
    #THEN:
    #backup modified b.txt
    #target
    #├── a.txt.bak (contaninig "content_a_1")
    #└── b.txt.bak (contaninig "content_b_2")


    echo_start "$FUNCNAME"
    clean_test

    #setup
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

    #expected
    echo $a_content > $target_expected_path/a.txt.bak
    echo $b_content_new > $target_expected_path/b.txt.bak

    compare_target_with_expected
}

function resume_interrupted_copy()
{
    #GIVEN:
    #src
    #└── a.txt
    #and
    #target
    #└── a.txt.bak.tmp
    #WHEN:
    #binary executed
    #SHOULD:
    #target_expected
    #└── a.txt.bak

    echo_start "$FUNCNAME"
    clean_test

    #setup
    touch $src_path/a.txt
    touch $target_path/a.txt.bak.tmp

    #expected
    touch $target_expected_path/a.txt.bak

    exec_sut

    compare_target_with_expected
}


if [ ! -d "$test_dir" ]; then mkdir -p $test_dir; fi
if [ ! -d "$src_path" ]; then mkdir -p $src_path; fi
if [ ! -d "$target" ]; then mkdir -p $target_path; fi
if [ ! -d "$target_expected_path" ]; then mkdir -p $target_expected_path; fi
if [ ! -d "$tmp_link_dir" ]; then mkdir -p $tmp_link_dir; fi


basic_test1
basic_test_dir
basic_test_symlinks
basic_delete_test_rename_original_source_file
delete_symlink_test
basic_delete_test_create_tagged_file
backup_modified_files
backup_chained_symlinks
fail_on_dangling_symlink
resume_interrupted_copy