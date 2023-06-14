cmake_minimum_required(VERSION 3.16)

project(v4 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Svg)

qt_standard_project_setup()

qt_add_executable(appv4
    main.cpp
    drivermodel.h
    drivermodel.cpp
    driverlist.h
    driverlist.cpp
    vehiclemodel.h
    vehiclemodel.cpp
    vehiclelist.h
    vehiclelist.cpp
)

qt_add_qml_module(appv4
    URI v4
    VERSION 1.0
    QML_FILES Main.qml CustomButton.qml CustomMenuItem.qml DriverList.qml VehicleList.qml
    RESOURCES logoshort.png user-solid.svg car-solid.svg gas-pump-solid.svg
)

set_target_properties(appv4 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER stolos.hogent.be
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appv4
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Svg
)

install(TARGETS appv4
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
