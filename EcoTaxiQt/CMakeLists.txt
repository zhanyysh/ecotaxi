cmake_minimum_required(VERSION 3.5)

project(EcoTaxiQt VERSION 0.4.17.10 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        classes/kwhperiod.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EcoTaxiQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        classes/driver.h classes/driver.cpp
        classes/investor.h classes/investor.cpp
        classes/event.h classes/event.cpp
        classes/charge.h classes/charge.cpp
        pages/login.h pages/login.cpp pages/login.ui
        managers/dbmanager.h managers/dbmanager.cpp
        managers/stylemanager.h managers/stylemanager.cpp
        managers/operations.h managers/operations.cpp
        Inter.ttf
        classes/car.h classes/car.cpp
        classes/location.h classes/location.cpp
        classes/type.h classes/type.cpp
        pages/settingspage.h pages/settingspage.cpp pages/settingspage.ui
        managers/navigationmanager.h managers/navigationmanager.cpp managers/navigationmanager.ui
        managers/nm.h
        pages/addupdatewindow.h pages/addupdatewindow.cpp pages/addupdatewindow.ui
        enums/setting.h
        pages/customdialog.h pages/customdialog.cpp pages/customdialog.ui
        managers/usersession.h managers/usersession.cpp
        pages/eventpage.h pages/eventpage.cpp pages/eventpage.ui
        enums/Events.h
        pages/addupdatewindowevents.h pages/addupdatewindowevents.cpp pages/addupdatewindowevents.ui
        pages/calendarpage.h pages/calendarpage.cpp pages/calendarpage.ui
        managers/backupmanager.h managers/backupmanager.cpp
        pages/generalreport.h pages/generalreport.cpp pages/generalreport.ui
        enums/Report.h
        managers/reportoperations.h managers/reportoperations.cpp
        pages/reportpage.h pages/reportpage.cpp pages/reportpage.ui
        managers/pdfmanager.h managers/pdfmanager.cpp
        icon.rc
        classes/user.h classes/user.cpp
        scripts/hash.h scripts/hash.cpp
        pages/removedb.h pages/removedb.cpp pages/removedb.ui
        pages/nointernet.h pages/nointernet.cpp pages/nointernet.ui
        managers/logger.h managers/logger.cpp
        managers/encryptionmanager.h managers/encryptionmanager.cpp
        scripts/greenbackgrounddelegate.h scripts/greenbackgrounddelegate.cpp
        pages/editablereport.h pages/editablereport.cpp pages/editablereport.ui
        pages/addupdaterepair.h pages/addupdaterepair.cpp pages/addupdaterepair.ui
        scripts/columnselectiondialog.h scripts/columnselectiondialog.cpp
        enums/eSetting.h
        scripts/awss3uploader.h scripts/awss3uploader.cpp
        managers/excelmanager.h managers/excelmanager.cpp
        pages/paymenthistory.h pages/paymenthistory.cpp pages/paymenthistory.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EcoTaxiQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EcoTaxiQt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EcoTaxiQt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(EcoTaxiQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# add gui library
find_package(Qt6 REQUIRED COMPONENTS Gui)
target_link_libraries(EcoTaxiQt PRIVATE Qt6::Gui)

# add print library
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)
target_link_libraries(EcoTaxiQt PRIVATE Qt6::PrintSupport)

# add sql library
find_package(Qt6 REQUIRED COMPONENTS Sql)
target_link_libraries(EcoTaxiQt PRIVATE Qt6::Sql)

# add network library
find_package(Qt6 REQUIRED COMPONENTS Network)
target_link_libraries(EcoTaxiQt PRIVATE Qt6::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EcoTaxiQt)
endif()
set_target_properties(EcoTaxiQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS EcoTaxiQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    # set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
    # qt_add_executable(EcoTaxi main.cpp ${app_icon_resource_windows})
    qt_finalize_executable(EcoTaxiQt)
endif()
