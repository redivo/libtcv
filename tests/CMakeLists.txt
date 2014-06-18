include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_HARNESS_SRC
   ${CMAKE_CURRENT_SOURCE_DIR}/basic_ident.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/mock_tcv.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/mock_hw.cpp
   PARENT_SCOPE
)
