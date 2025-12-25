include_directories("include")

# LIB
ADD_LIBRARY(
   chessmodel STATIC
   src/position.cpp
   src/bitboard.cpp
   src/board.cpp
   src/generate.cpp
   src/stream.cpp
   src/game.cpp
)

# target_link_libraries(chessmodel)

# TEST
enable_testing()
include(GoogleTest)

add_executable(
   chessmodel_test
   test/position_test.cpp
   test/bitboard_test.cpp
   test/board_test.cpp
   test/generate_test.cpp
   test/generate_legal_test.cpp
   test/game_test.cpp
   test/perft.cpp
)

target_link_libraries(
   chessmodel_test
   "chessmodel"
   gtest
   gtest_main
)

gtest_discover_tests(chessmodel_test)