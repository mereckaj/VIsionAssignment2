#!/usr/bin/env make

#
# Simple Makefile for OpenCV projects.
#
# Author: Basil L. Contovounesios <contovob@tcd.ie>
#

target := main
source := $(target).cpp
depens := $(filter-out $(source), $(wildcard *.cpp))

# Emulates an if-else function for equating flags/text.
# Usage: $(call ifflag, <flag>, <comparison>, <equal-value>, <unequal-value>)
ifflag = $(strip $(if $(filter $(2), $(1)), $(3), $(4)))

#
# Make options controlling compilation flags and their defaults.
#
# For example, the following will disable optimisations and compiler warnings:
#
#   make glue OPT=false WARN=false
#

# Use the 2011 ISO C++ standard plus amendments
override CXXFLAGS += $(call ifflag, $(GNU), true, -std=gnu++11, -std=c++11)
# Optimise
override CXXFLAGS += $(call ifflag, $(OPT), false, -O0, -O3)
# Issue warnings
override CXXFLAGS += $(call ifflag, $(WARN), false, -w, -pedantic -Wall)
# Issue coloured text output
override CXXFLAGS += $(call ifflag, $(COLOUR), false, , -fdiagnostics-color)

# Change shared object directory to search according to installation
override LDFLAGS  += -L/usr/local/lib

# Remove if using OpenCV 2
override LDLIBS   += -lopencv_imgcodecs
# OpenCV libraries needed
override LDLIBS   += -lopencv_core -lopencv_highgui

.PHONY: clean

# Compile the target
$(target): $(source) $(depens)

# Remove the target binary
clean:
	$(RM) $(target)
