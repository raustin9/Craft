#!/usr/bin/bash

go-build() {
    echo "Building everything..."
    make -f "Makefile.linux.mak" all
    errorlevel=$?
    if [ $errorlevel -ne 0 ]
    then
        echo "Error: $errorlevel" && exit
    fi

    echo "All assemblies built"
}

go-rebuild() {
    echo "Rebuilding everything..."
    make -f "Makefile.linux.mak" clean
    make -f "Makefile.linux.mak" all
    errorlevel=$?
    if [ $errorlevel -ne 0 ]
    then
        echo "Error: $errorlevel" && exit
    fi

    echo "All assemblies built"
}


go-clean() {
    make -f "Makefile.linux.mak" clean
    
    echo "Done"
}

go-valgrind() {
    valgrind\
        --leak-check=full\
        --show-leak-kinds=all\
        --track-origins=yes\
        --verbose\
        --log-file=valgrind-out.txt\
        ./bin/compiler\
    ##
}

go-run() {
    ./bin/compiler
}

# Run the command(s)
"go-$@"
