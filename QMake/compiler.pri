QMAKE_CXXFLAGS += -DHAVE_STRING_H=1
!msvc:QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

CONFIG(debug, debug|release) {
    msvc:QMAKE_CXXFLAGS += /DEBUG
    !msvc:QMAKE_CXXFLAGS += -O0 -ggdb -g3
}
