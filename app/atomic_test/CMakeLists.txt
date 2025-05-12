aux_source_directory(. ATOMIC_TEST_SOURCES)
add_library(atomic_test STATIC ${ATOMIC_TEST_SOURCES})
target_link_libraries(atomic_test pthread)
