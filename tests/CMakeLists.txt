include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_HARNESS_SRC
   ${CMAKE_CURRENT_SOURCE_DIR}/basic_ident.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/fake_tcv.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/fake_hw.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/digital_diag.cpp
   PARENT_SCOPE
)
