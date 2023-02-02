Backup daemon for Linux OS:

Creates a copy of any file created or modified in the hot folder

Backup files have the same name of the original file with .bak extension

If the file name is prefixed with 'delete_' it is immediately deleted from the hot folder and backup folder

App keeps a log file of actions taken (file created, altered, backedup or deleted)

Log file can be viewed and filtered from the CLI app.

Log file filters accept filter by [date, filename regex]

The application keeps it's state between reboots

Dependencies:

    make

    gcc (tested on gcc 11)

How to build:

    `make`

Run tests:

    `make test`

Usage:

    `build/./scanner /path/to/source/directory /path/to/target/directory`

    or `make run` (equivalent to `build/./scanner ./test/test_dir/src ./test/test_dir/target`)

