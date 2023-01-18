cmake_minimum_required(VERSION 3.21)    # been there

project(NetSim)                         # been there

set(CMAKE_CXX_STANDARD 17)              # been there

# Dodaj flagi kompilacji wymuszające pisanie kodu zgodnego ze standardem.
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

# Dodaj katalogi z plikami nagłówkowymi dla wszystkich konfiguracji.
include_directories(
        include
)

# Ustaw zmienną `SOURCE_FILES`, która będzie przechowywać ścieżki do
# tych plików źródłowych, które będą niezbędne w każdej konfiguracji.
set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        src/types.cpp
        )

# Dodaj konfigurację typu `Debug`.
add_executable(NetSim_debug ${SOURCE_FILES} main.cpp)



# == Unit testing using Google Testing Framework ==

# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.
set(SOURCES_FILES_TESTS
<<<<<<< HEAD
##        test/main_gtest.cpp       TODO: Delete this particular line of code
#        test/test_Factory.cpp
#        test/test_factory_io.cpp
#        test/test_nodes.cpp
#        test/test_package.cpp
=======
#        test/main_gtest.cpp
#        test/test_Factory.cpp
#        test/test_factory_io.cpp
#        test/test_nodes.cpp
        test/test_package.cpp
>>>>>>> main
#        test/test_reports.cpp
#        test/test_simulate.cpp
        test/test_storage_types.cpp
        )

# Dodaj konfigurację typu `Test`.
add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master)

# Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
target_include_directories(NetSim_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

# Podlinkuj bibliotekę o identyfikatorze `gmock` (w pliku CMake) wyłącznie do konkretnej
# konfiguracji (tu: `Test`).
target_link_libraries(NetSim_test gmock)

# Ta linijka jest do ewentualnych zmian
# EXCERCISE_ID znajdziesz w config.hpp TODO testy różne
target_compile_definitions(NetSim_test PUBLIC EXERCISE_ID=EXERCISE_ID_PACKAGES)
