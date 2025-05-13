# Copyright [2024] FaCAD

find_package(RTTR CONFIG REQUIRED Core)

# Check if RTTR::Core target exists
if(TARGET RTTR::Core)
    message(STATUS "RTTR::Core found")
else()
    message(FATAL_ERROR "RTTR::Core not found")
endif()

# Use RTTR's install location from its config
if(RTTR_DIR)
    install(FILES "${RTTR_DIR}/../../debug/bin/rttr_core_d.dll"  # Adjust for platform
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime
    )
else()
    message(FATAL_ERROR "RTTR_DIR not defined")
endif()