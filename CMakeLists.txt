cmake_minimum_required(VERSION 3.27)
project(C___Car_rental_command_line_application)

set(CMAKE_CXX_STANDARD 17)



add_library(sqlite3 sqlite3/sqlite3.c)
target_include_directories(sqlite3 PUBLIC sqlite3)

# SQLite library
add_library(sqlitecpp
        sqlitecpp/src/Backup.cpp
        sqlitecpp/src/Column.cpp
        sqlitecpp/src/Database.cpp
        sqlitecpp/src/Exception.cpp
        sqlitecpp/src/Statement.cpp
        sqlitecpp/src/Transaction.cpp

)

target_include_directories(sqlitecpp PUBLIC sqlitecpp/include)
target_link_libraries(sqlitecpp sqlite3)

if (UNIX)
    target_link_libraries(sqlite3 pthread dl)
endif (UNIX)

add_executable(C___Car_rental_command_line_application main.cpp
)
target_link_libraries(C___Car_rental_command_line_application sqlite3 sqlitecpp)