# Make lldb-dotest a custom target.
add_custom_target(lldb-dotest)
add_dependencies(lldb-dotest lldb-test-deps)
set_target_properties(lldb-dotest PROPERTIES FOLDER "lldb utils")

get_property(LLDB_DOTEST_ARGS GLOBAL PROPERTY LLDB_DOTEST_ARGS_PROPERTY)
set(LLDB_LIBS_DIR "${LLVM_LIBRARY_OUTPUT_INTDIR}")

llvm_canonicalize_cmake_booleans(
  LLDB_BUILD_INTEL_PT
)

# Generate lldb-dotest Python driver script for each build mode.
if(LLDB_BUILT_STANDALONE)
  set(config_types ".")
  if(CMAKE_CONFIGURATION_TYPES)
    set(config_types ${CMAKE_CONFIGURATION_TYPES})
  endif()
  foreach(config_type ${config_types})
    # In paths to our build-tree, replace CMAKE_CFG_INTDIR with our actual configuration names.
    string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} config_runtime_output_dir ${LLVM_RUNTIME_OUTPUT_INTDIR})
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_DOTEST_ARGS_CONFIGURED "${LLDB_DOTEST_ARGS}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_SOURCE_DIR_CONFIGURED "${LLDB_SOURCE_DIR}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_FRAMEWORK_DIR_CONFIGURED "${LLDB_FRAMEWORK_DIR}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_BUILD_DIRECTORY_CONFIGURED "${LLDB_TEST_BUILD_DIRECTORY}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_EXECUTABLE_CONFIGURED "${LLDB_TEST_EXECUTABLE}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_COMPILER_CONFIGURED "${LLDB_TEST_COMPILER}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_DSYMUTIL_CONFIGURED "${LLDB_TEST_DSYMUTIL}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_FILECHECK_CONFIGURED "${LLDB_TEST_FILECHECK}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_YAML2OBJ_CONFIGURED "${LLDB_TEST_YAML2OBJ}")
    string(REPLACE ${LLVM_RUNTIME_OUTPUT_INTDIR} ${config_runtime_output_dir} LLDB_TEST_SERVER_CONFIGURED "${LLDB_TEST_SERVER}")

    # Remaining ones must be paths to the provided LLVM build-tree.
    if(${config_type} IN_LIST LLVM_CONFIGURATION_TYPES)
      # Multi-configuration generator like Xcode (with a matching config).
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_DOTEST_ARGS_CONFIGURED "${LLDB_DOTEST_ARGS}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_SOURCE_DIR_CONFIGURED "${LLDB_SOURCE_DIR}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_FRAMEWORK_DIR_CONFIGURED "${LLDB_FRAMEWORK_DIR}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_BUILD_DIRECTORY_CONFIGURED "${LLDB_TEST_BUILD_DIRECTORY}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_EXECUTABLE_CONFIGURED "${LLDB_TEST_EXECUTABLE}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_COMPILER_CONFIGURED "${LLDB_TEST_COMPILER}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_DSYMUTIL_CONFIGURED "${LLDB_TEST_DSYMUTIL}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_FILECHECK_CONFIGURED "${LLDB_TEST_FILECHECK}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_YAML2OBJ_CONFIGURED "${LLDB_TEST_YAML2OBJ}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_TEST_SERVER_CONFIGURED "${LLDB_TEST_SERVER}")
      string(REPLACE ${CMAKE_CFG_INTDIR} ${config_type} LLDB_LIBS_DIR_CONFIGURED "${LLDB_LIBS_DIR}")
    else()
      # Single-configuration generator like Ninja.
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_DOTEST_ARGS_CONFIGURED "${LLDB_DOTEST_ARGS}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_SOURCE_DIR_CONFIGURED "${LLDB_SOURCE_DIR}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_FRAMEWORK_DIR_CONFIGURED "${LLDB_FRAMEWORK_DIR}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_BUILD_DIRECTORY_CONFIGURED "${LLDB_TEST_BUILD_DIRECTORY}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_EXECUTABLE_CONFIGURED "${LLDB_TEST_EXECUTABLE}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_COMPILER_CONFIGURED "${LLDB_TEST_COMPILER}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_DSYMUTIL_CONFIGURED "${LLDB_TEST_DSYMUTIL}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_FILECHECK_CONFIGURED "${LLDB_TEST_FILECHECK}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_YAML2OBJ_CONFIGURED "${LLDB_TEST_YAML2OBJ}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_TEST_SERVER_CONFIGURED "${LLDB_TEST_SERVER}")
      string(REPLACE ${CMAKE_CFG_INTDIR} "." LLDB_LIBS_DIR_CONFIGURED "${LLDB_LIBS_DIR}")
    endif()

    configure_file(
      lldb-dotest.in
      ${config_runtime_output_dir}/lldb-dotest @ONLY
    )
  endforeach()
elseif(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
  foreach(LLVM_BUILD_MODE ${CMAKE_CONFIGURATION_TYPES})
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_DOTEST_DIR ${LLVM_RUNTIME_OUTPUT_INTDIR})
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_DOTEST_ARGS_CONFIGURED "${LLDB_DOTEST_ARGS}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_SOURCE_DIR_CONFIGURED "${LLDB_SOURCE_DIR}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_FRAMEWORK_DIR_CONFIGURED "${LLDB_FRAMEWORK_DIR}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_BUILD_DIRECTORY_CONFIGURED "${LLDB_TEST_BUILD_DIRECTORY}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_EXECUTABLE_CONFIGURED "${LLDB_TEST_EXECUTABLE}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_COMPILER_CONFIGURED "${LLDB_TEST_COMPILER}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_DSYMUTIL_CONFIGURED "${LLDB_TEST_DSYMUTIL}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_FILECHECK_CONFIGURED "${LLDB_TEST_FILECHECK}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_YAML2OBJ_CONFIGURED "${LLDB_TEST_YAML2OBJ}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_TEST_SERVER_CONFIGURED "${LLDB_TEST_SERVER}")
    string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} LLDB_LIBS_DIR_CONFIGURED "${LLDB_LIBS_DIR}")

    configure_file(
      lldb-dotest.in
      ${LLDB_DOTEST_DIR}/lldb-dotest
      )
  endforeach()
else()
  set(LLDB_DOTEST_ARGS_CONFIGURED "${LLDB_DOTEST_ARGS}")
  set(LLDB_SOURCE_DIR_CONFIGURED "${LLDB_SOURCE_DIR}")
  set(LLDB_FRAMEWORK_DIR_CONFIGURED "${LLDB_FRAMEWORK_DIR}")
  set(LLDB_TEST_BUILD_DIRECTORY_CONFIGURED "${LLDB_TEST_BUILD_DIRECTORY}")
  set(LLDB_TEST_EXECUTABLE_CONFIGURED "${LLDB_TEST_EXECUTABLE}")
  set(LLDB_TEST_COMPILER_CONFIGURED "${LLDB_TEST_COMPILER}")
  set(LLDB_TEST_DSYMUTIL_CONFIGURED "${LLDB_TEST_DSYMUTIL}")
  set(LLDB_TEST_FILECHECK_CONFIGURED "${LLDB_TEST_FILECHECK}")
  set(LLDB_TEST_YAML2OBJ_CONFIGURED "${LLDB_TEST_YAML2OBJ}")
  set(LLDB_TEST_SERVER_CONFIGURED "${LLDB_TEST_SERVER}")
  set(LLDB_LIBS_DIR_CONFIGURED "${LLDB_LIBS_DIR}")

  configure_file(
    lldb-dotest.in
    ${LLVM_RUNTIME_OUTPUT_INTDIR}/lldb-dotest
    )
endif()
