CXXFLAGS = -Wall -Wextra -Wpedantic

TARGET = csgo_weaponid_dumper

OBJ = \
	csgo_weaponid_dumper.o \
	external/android_strings/android_strings.o

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

clean:
	rm $(TARGET) $(OBJ)
