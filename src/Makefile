# I usually use CMake, but it is overkill for this task.
# As we are not doing dependency management, and this is a coding challenge.
# So I am sticking to make for now.

# I normally use clang, but it still misses some features from C++23.
CXX = g++

# Extra warning flags to snoop out more errors.
CXX_EXTRA_WARNING_FLAGS = \
	-Wunused -Wunused-parameter -Wunused-variable -Wunused \
	-Wunused-function -Wunused-value -Wunused-label -Wunreachable-code \
	-Wreturn-type -Wshadow -Wconversion -Wlogical-op -Wfloat-equal \
	-Wcast-qual -Wcast-align -Wmissing-declarations \
	-Wmissing-field-initializers -Wnull-dereference -Wundef \
	-Wunused-but-set-variable -Wvla -Wwrite-strings

# -Werror is essential for production.
CXX_WARNING_FLAGS = -Werror -Wall -Wextra -pedantic $(CMAKE_EXTRA_WARNING_FLAGS)

# TODO: Conditionally enable debug flags, using env var.
CXX_DEBUG_FLAGS = -ggdb -gdwarf

# We set LTO and O2 for optimization.
CXX_FLAGS = -std=c++23 -O2 -flto -I./ $(CXX_WARNING_FLAGS) $(CXX_DEBUG_FLAGS)

TARGET = parser.out

# Lazy implementation:
OBJECTS = main.o \
	pcap/pcap_buffer.o \
	simba/sbe.o simba/sbe_messages.o simba/sbe_types.o \
	simba/simba.o simba/net.o simba/simba_extractor.o \
	simba/data_field.o

# Default rule:
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $<

pcap/%.o: pcap/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

simba/%.o: simba/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET)
	find . -name '*.o' -exec rm -rf {} \;

.PHONY: clean
