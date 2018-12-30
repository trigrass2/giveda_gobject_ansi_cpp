#
# Copyright (C) 2019  明心  <imleizhang@qq.com>
# All rights reserved.
# 
# This program is an open-source software; and it is distributed in the hope 
# that it will be useful, but WITHOUT ANY WARRANTY; without even the 
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
# PURPOSE. 
# This program is not free software; so you can not redistribute it and/or 
# modify it without my authorization. If you only need use it for personal
# study purpose(no redistribution, and without any  commercial behavior), 
# you should accept and follow the GNU AGPL v3 license, otherwise there
# will be your's credit and legal risks.  And if you need use it for any 
# commercial purpose, you should first get commercial authorization from
# me, otherwise there will be your's credit and legal risks. 
#
#
CXX = g++
LINK = g++
CXXFLAGS = -std=c++98
LFLAGS =
INCPATH = -I.
LIBS = 
TARGET = hello_giveda
SRC = $(wildcard *.cpp)
OBJECTS = $(SRC:%.cpp=%.o)

.PHONY: all
all: $(OBJECTS) $(TARGET)

$(OBJECTS): %.o:%.cpp

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)

.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(TARGET)

####### Implicit rules

.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
