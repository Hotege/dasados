#!/bin/bash

echo "building server..."
docker run --rm -v `pwd`/src:/gcc/$(basename `pwd`)/src -w /gcc/$(basename `pwd`)/src \
-v `pwd`/dist:/gcc/output \
-v `pwd`/../../opensource:/gcc/opensource \
gcc:9.1.0 \
g++ -Wall \
main.cpp \
prepare/notifier.cpp prepare/parser.cpp \
server/server.cpp \
server/luaplugins/luaplugins.cpp \
server/db/database.cpp \
resolve/strings.cpp resolve/resolve.cpp \
-I /gcc/opensource/lua-5.3.5/include \
-I /gcc/opensource/libpqxx/install/include \
-I /usr/include/mysql \
-L /gcc/opensource/lua-5.3.5/lib \
-L /gcc/opensource/libpqxx/install/lib \
-L /usr/lib/x86_64-linux-gnu \
-l:liblua.a -l:libdl.a \
-l:libmariadbclient.a -l:libpthread.a -l:libz.a \
-o /gcc/output/dasados -std=c++11 -s -Os -static #-libgcc -static-libstdc++ -static

upx -9 dist/dasados > /dev/null
