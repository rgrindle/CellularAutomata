# Add include directory
include_directories(../include)

# Find all the *.cpp files
file(GLOB SRCS *.cpp)

# Make the executable based on all the .cpp files.
add_executable(CellularAutomaton ${SRCS})

# Build the executable in /bin
# Use this by typing cmake install.
install(TARGETS CellularAutomaton DESTINATION ${CMAKE_BINARY_DIR}/../bin)
