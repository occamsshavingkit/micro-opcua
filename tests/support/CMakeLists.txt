# tests/support/CMakeLists.txt
add_library(muc_opcua_test_support STATIC
    fake_platform.c
    service_builders.c
)

target_include_directories(muc_opcua_test_support PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(muc_opcua_test_support PUBLIC muc_opcua)
