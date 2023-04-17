# +------------------------------------------------------------------+
# | Copyright (C) 2023 Roger P. Johnson, roger0080@netscape.net      |
# |                                                                  |
# | This file is part of libbst.                                     |
# |                                                                  |
# | libbst is free software: you can redistribute it and/or modify   |
# | it under the terms of the GNU Lessor General Public License as   |
# | published by the Free Software Foundation, either version 3 of   |
# | the License, or (at your option) any later version.              |
# |                                                                  |
# | libbst is distributed in the hope that it will be useful, but    |
# | WITHOUT ANY WARRANTY; without even the implied warranty of       |
# | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the     |
# | GNU Lessor General Public License for more details.              |
# |                                                                  |
# | You should have received a copy of the GNU Lessor General Public |
# | License along with libbst. If not, see www.gnu.org/licenses.     |
# +------------------------------------------------------------------+

#  Makefile file for managing the bst library and programs.

# OS information
OPSYS := $(shell uname -s)

SHELL = /bin/sh
ECHO = /bin/echo     # (SunOS 4.x) use Sys V 'echo' for '\c' to work right.

#CC= /opt/SUNWspro/bin/cc
#CC= cc
CC = gcc

ifeq ($(CC),gcc)
  #$(error is gcc platform)
  # GCC compilier tools
  #MAKE   = gmake
  #CC     = gcc
  AR     = ar
  RANLIB = ranlib
  STRIP  = strip
else
  #$(error is Sun Studio platform)
  # Sun Studio compiler tools
  #MAKE   = /usr/ccs/bin/make
  #CC     = cc
  #CC     = /opt/SUNWspro/bin/cc
  AR     = /usr/ccs/bin/ar
  RANLIB = /usr/ccs/bin/ranlib
  STRIP  = /usr/ccs/bin/strip
endif

# use gmake on all platforms
MAKE = gmake

#
# Depends
# 

# The command to generate file dependencies - Can use gcc, makedepend (if you go get build it), or others; set here:
DEPEND = $(CC)

# uncomment if you want a more friendly updating depends message other than "... not found ..."
MAKE_DEPEND_VERBOSE = 1

# Depend flags - Set according to what you use for $(DEPEND) above
ifeq ($(CC),gcc)
  # gcc on all platforms
  # use only local files; do reference system includes as dependencies
  #DEPEND_ARGS = -M
  DEPEND_ARGS = -MM
else
  # platform specific compilier being used
  ifeq ($(OPSYS),$(filter $(OPSYS),SunOS Linux))
    # implies Sun Studio on Solaris or Linux:
    # use only local files; do reference system includes as dependencies
    #DEPEND_ARGS = -xM
    DEPEND_ARGS = -xM1
  else
    # unknown OS/compilier
    #DEPEND_ARGS = -xM1
    $(error Unknown platform: you need to set 'DEPEND_ARGS' for the 'CC' you are using)
  endif
endif

#
# library DEBUG/TRACE build flags
#

# Enable/Disable program and library trace and debug output: pick one of each:
# Note:
#   If you are going to run test.c with ARRSIZ > 10 or do benchmark testing, do not
#   enable these DEBUG_LIB_* defines, it generates a lot of output to the screen.

# Enable/disable informative messages as routines in the library execute (for production and test usage)
# only used one place, not much info:
DEBUG_LIB_WITH_LIBCALL_TRACING = -DDEBUG_TRACE
DEBUG_LIB_WITH_LIBCALL_TRACING = 

# Enable/disable the showing of ASCII art graph boxes on allocation of nodes w/addresses from the library routines:
DEBUG_LIB_SHOW_LIBCALL_MALLOC_GRAPHS = -DDEBUG_SHOWGRAPHS
DEBUG_LIB_SHOW_LIBCALL_MALLOC_GRAPHS = 

# Enable/disable the showing of allocation/deallocation malloc'd memory w/addresses from the library routines:
DEBUG_LIB_SHOW_LIBCALL_MALLOC_INFO = -DDEBUG_MALLAC_USAGE
DEBUG_LIB_SHOW_LIBCALL_MALLOC_INFO = 

# Enable/disable AVL tree rebalance info from the library routines:
DEBUG_LIB_SHOW_LIBCALL_TREE_REBALANCE = -DDEBUG_SHOWREBALANCE
DEBUG_LIB_SHOW_LIBCALL_TREE_REBALANCE = 

DEBUG_LIB_DEFINES = $(DEBUG_LIB_WITH_LIBCALL_TRACING) $(DEBUG_LIB_SHOW_LIBCALL_MALLOC_GRAPHS) $(DEBUG_LIB_SHOW_LIBCALL_MALLOC_INFO) $(DEBUG_LIB_SHOW_LIBCALL_TREE_REBALANCE)

# library routines include dir files:
LIB_INC_DIR = inc
LIB_INCLUDES = -I $(LIB_INC_DIR)

# Compilier Options for library objects and executables
# If development or release, select your CFLAGS to use: *_DEV or *_REL
ifeq ($(CC),gcc)
  # gcc on all platforms
  # code uses 0x%-5x to print pointer addresses but creates warnings
  # -Wno-format is equivalent to -Wformat=0,
  #
  # valgrind: valgrind --leak-check=yes ./demo
  GCC_CFLAGS_VAL = -g -Wno-format -O0

  # development
  GCC_CFLAGS_DEV = -g -Wno-format

  # release/production
  GCC_CFLAGS_REL = -O3 -Wno-format

  CC_FLAGS = $(GCC_CFLAGS_DEV)
else
  # platform specific compilier being used
  ifeq ($(OPSYS),$(filter $(OPSYS),SunOS Linux))
    # implies Sun Studio on Solaris or Linux:
    #     -xdryrun or -# will show what -fast expands to
    #     -fd warn about non-K&R styles
    SS_CFLAGS_DEV = -fd -g -xtransition
    SS_CFLAGS_REL = -fast -xO4

    CC_FLAGS = $(SS_CFLAGS_DEV)
  else
    # unknown OS/compilier
    SS_CFLAGS_DEV = 
    SS_CFLAGS_REL = 

    CC_FLAGS = $(SS_CFLAGS_DEV)
    $(error Unknown platform: you need to set 'CC_FLAGS' for the 'CC' you are using)
  endif
endif

LIB_CFLAGS =  $(CC_FLAGS) $(DEBUG_LIB_DEFINES) $(LIB_INCLUDES)

# Additional library defines:
#LIB_DFLAGS = -DBSD        # SVR3 or SVR4 (tid.c), or BSD  environment. 
LIB_DFLAGS = -DSVR4       # SVR3 or SVR4 (tid.c), or BSD  environment. 

#
# demo program build flags
#
# enable use of/show additional commands that use normally code abstracted/hidden from user
# (not for production use, just interactive demo usage)
DEBUG_DEMO_PVT_TREE_HDR = 
DEBUG_DEMO_PVT_TREE_HDR = -DDEBUG_EXPLOIT_TREE_HDR
DEBUG_DEMO_DEFINES = $(DEBUG_DEMO_PVT_TREE_HDR)

DEMO_DFLAGS =
PROG_INCLUDES = -I./
DEMO_CFLAGS = $(CC_FLAGS) $(DEBUG_DEMO_DEFINES) $(PROG_INCLUDES)

#
# test program build flags
#
# enable use of/show additional commands that use normally code abstracted/hidden from user
# (not for production use, just interactive demo usage)
DEBUG_TEST_PVT_TREE_HDR = 
DEBUG_TEST_PVT_TREE_HDR = -DDEBUG_EXPLOIT_TREE_HDR
DEBUG_TEST_DEFINES = $(DEBUG_TEST_PVT_TREE_HDR)

TEST_DFLAGS =
PROG_INCLUDES = -I./ -I $(LIB_INC_DIR)
TEST_CFLAGS = $(CC_FLAGS) $(DEBUG_TEST_DEFINES) $(PROG_INCLUDES)


#-------------------------------------------------------------------------------
# Object and Depend Directories
#-------------------------------------------------------------------------------
# With objects and depends, you can easily choose the layout where you want each to reside.
# There are 3 cases (object files for example):
#      i. At the same level as src (ie ../obj).
#            Set OBJDIRPFX to a path you want (i.e. ../) to move OBJDIR up one level.
#     ii. Into a directory in your current directory (ie ./obj).
#            Leave OBJDIRPFX blank, and let ODJDIR specify the dir name.
#    iii. No segregation, just create the .o's in your current directory "."
#            Leave OBJDIRPFX and OBJDIR blank.


# Object Directory: where to relocate the .o files: same dir as build or other:
#OBJDIRPFX     = temp/
OBJDIRPFX     = 
OBJDIR        = obj/

# for depend generation below, shell escaped versions of OBJDIRPFX and OBJDIR:
#OBJDIRPFX_SED = temp\/
OBJDIRPFX_SED =
OBJDIR_SED    = obj\/

ifneq ( $(OBJDIRPFX)$(OBJDIR),)
  MKOBJDIR := $(shell mkdir -p  $(OBJDIRPFX)$(OBJDIR) )
endif


# Dependency Directory: where to relocate the .d files: same dir as build or other:
#DEPENDDIRPFX     = temp/
DEPENDDIRPFX     = 
DEPENDDIR        = dep/
# for depend generation below, shell escaped versions of DEPENDDIRPFX and DEPENDDIR:
#DEPENDDIRPFX_SED = temp\/
DEPENDDIRPFX_SED = 
DEPENDDIR_SED    = dep\/

ifneq ( $(DEPENDDIRPFX)$(DEPENDDIR),)
  MKDEPDIR := $(shell mkdir -p  $(DEPENDDIRPFX)$(DEPENDDIR) )
endif


# Finished Library Name
LIBNAME = libbst.a

# Library Modules/Components
LIBOBJECTS = \
        $(OBJDIRPFX)$(OBJDIR)globals.o     \
        $(OBJDIRPFX)$(OBJDIR)defined.o     \
        $(OBJDIRPFX)$(OBJDIR)msgs.o        \
        $(OBJDIRPFX)$(OBJDIR)count.o       \
        $(OBJDIRPFX)$(OBJDIR)remove.o      \
        $(OBJDIRPFX)$(OBJDIR)fnode.o       \
        $(OBJDIRPFX)$(OBJDIR)rprint.o      \
        $(OBJDIRPFX)$(OBJDIR)tid.o         \
        $(OBJDIRPFX)$(OBJDIR)tident.o      \
        $(OBJDIRPFX)$(OBJDIR)graphics.o    \
        $(OBJDIRPFX)$(OBJDIR)tstat.o       \
        $(OBJDIRPFX)$(OBJDIR)delete.o      \
        $(OBJDIRPFX)$(OBJDIR)get.o         \
        $(OBJDIRPFX)$(OBJDIR)print.o       \
        $(OBJDIRPFX)$(OBJDIR)release.o     \
        $(OBJDIRPFX)$(OBJDIR)rebalance.o   \
        $(OBJDIRPFX)$(OBJDIR)tmem.o        \
        $(OBJDIRPFX)$(OBJDIR)tcp.o         \
        $(OBJDIRPFX)$(OBJDIR)tequal.o      \
        $(OBJDIRPFX)$(OBJDIR)create.o      \
        $(OBJDIRPFX)$(OBJDIR)empty.o       \
        $(OBJDIRPFX)$(OBJDIR)node.o        \
        $(OBJDIRPFX)$(OBJDIR)put.o         \
        $(OBJDIRPFX)$(OBJDIR)fheader.o     \
        $(OBJDIRPFX)$(OBJDIR)insnode.o     \
        $(OBJDIRPFX)$(OBJDIR)twalk.o       \
        $(OBJDIRPFX)$(OBJDIR)tdispose.o    \
        $(OBJDIRPFX)$(OBJDIR)qfind.o       \
        $(OBJDIRPFX)$(OBJDIR)fheaderlist.o

###################
#  t a r g e t s  #
###################
what:
	@printf "CC                   = [$(CC)]\n"
	@printf "Objects relocated to = [$(OBJDIRPFX)$(OBJDIR)]\n"
	@printf "Depends relocated to = [$(DEPENDDIRPFX)$(DEPENDDIR)]\n"
	@printf "Library relocated to = [$(LIBDIRPFX)$(LIBDIR)]\n"
	@printf "\n"
	@echo "QUICK USAGE: $(MAKE) all; ./demo or ./test"
	@printf "\n"
	@echo "Targets to make:"
	@echo "  $(MAKE) all          - build all targets: library $(LIBNAME), executables: demo test"
	@echo "  $(MAKE) lib          - build just the archive library $(LIBNAME)"
	@echo "  $(MAKE) strip        - strip debugging symbol tables from executables"
	@echo "  $(MAKE) clean        - delete compiled .o object files"
	@echo "  $(MAKE) realclean    - delete compiled .o object files AND their dependency .d files"
	@echo "  $(MAKE) clobber      - delete compiled .o object files AND their dependency .d files and executables and libraries"

all: lib demo test
	@echo "all built."

strip: demo test
	$(STRIP) demo test

clean:
	rm -f demo test  $(OBJDIRPFX)$(OBJDIR)demo.o $(OBJDIRPFX)$(OBJDIR)test.o $(OBJDIRPFX)$(OBJDIR)$(LIBNAME) $(LIBOBJECTS)

realclean: clean
	rm -f $(addprefix $(DEPENDDIRPFX)$(DEPENDDIR), $(notdir $(LIBOBJECTS:.o=.d)))

clobber: realclean
	rm -f demo test $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)

###################################
#  l i b r a r y   t a r g e t s  #
###################################
# How to make the main library archive file $(LIBNAME)

# THIS STYLE RULES COMPILES *ALL* THE LIBRARY OBJECTS *FIRST*, THEN CREATES THE ARCHIVE.
# Other style is to add each object to the archive library for each object as it is compiled.

# Library Directory: where to relocate the .a files: same dir as build or other:
LIBDIRPFX     = 
LIBDIR        = lib/

ifneq ( $(LIBDIRPFX)$(LIBDIR),)
  MKLIBDIR := $(shell mkdir -p  $(LIBDIRPFX)$(LIBDIR) )
endif


lib:    $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)
	@echo "info: $(LIBDIRPFX)$(LIBDIR)$(LIBNAME) OK"

# How to put an object (.o) into the archive file
$(LIBDIRPFX)$(LIBDIR)$(LIBNAME): $(LIBOBJECTS)
	$(AR) r $@ $?
#	rm -f $(OBJDIRPFX)$(OBJDIR)$*.o

# How to compile a library module (.o)
# -DMY_MAKE_LIB_CC_CMD_COMPILE is a visual indicator that gmake is using my rule to compile a library object and not a gmake implicit rule being used.
$(OBJDIRPFX)$(OBJDIR)%.o: $(notdir $(OBJDIRPFX)$(OBJDIR)%.c)
	$(CC) -DMY_MAKE_LIB_CC_CMD_COMPILE $(LIB_CFLAGS) $(LIB_DFLAGS) -c $(notdir $(@:.o=.c)) -o $(OBJDIRPFX)$(OBJDIR)$*.o

###################################
#  p r o g r a m   t a r g e t s  #
###################################
demo :  $(OBJDIRPFX)$(OBJDIR)demo.o
	$(CC) -DMY_MAKE_DEMO_CC_CMD_LINK $(DEMO_CFLAGS) $(DEMO_DFLAGS) -o $@  $(OBJDIRPFX)$(OBJDIR)demo.o $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)

$(OBJDIRPFX)$(OBJDIR)demo.o: demo.c bstpkg.h leaf.h $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)
	$(CC) -DMY_MAKE_DEMO_CC_CMD_COMPILE $(DEMO_CFLAGS) $(DEMO_DFLAGS) -o $@ -c $<


test :  $(OBJDIRPFX)$(OBJDIR)test.o
	$(CC) -DMY_MAKE_TEST_CC_CMD_LINK $(TEST_CFLAGS) $(TEST_DFLAGS) -o $@  $(OBJDIRPFX)$(OBJDIR)test.o $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)

$(OBJDIRPFX)$(OBJDIR)test.o: test.c bstpkg.h leaf.h  $(LIBDIRPFX)$(LIBDIR)$(LIBNAME)
	$(CC) -DMY_MAKE_TEST_CC_CMD_COMPILE $(TEST_CFLAGS) $(TEST_DFLAGS) -o $@ -c $<

#######################################################
#  a u t o - c r e a t e d   d e p e n d e n c i e s  #
#######################################################
# This generates your .d include dependencies and stores them where specified in the DEPENDDIR option:
# gcc places all one one line and Sun Studio has one line per dependency.
# This sed'ing works for both compiliers:
$(DEPENDDIRPFX)$(DEPENDDIR)%.d: $(notdir $(LIBOBJECTS:.o=.c))
	@$(SHELL) -ec 'if [ ! -z "$(MAKE_DEPEND_VERBOSE)" ]; then echo "<updating dependency file $@>"; fi'
	@$(SHELL) -ec '$(DEPEND) $(DEPEND_ARGS) -I$(LIB_INC_DIR) $(LIB_DFLAGS) $(notdir $(@:.d=.c))    \
                | sed -e '\''s/$*\.o */& $(DEPENDDIRPFX_SED)$(DEPENDDIR_SED)$(notdir $@) /g'\'' -e '\''s/^.*\.o /$(OBJDIRPFX_SED)$(OBJDIR_SED)&/'\'' > $@'

# Include all the generated .d dependencies from the rule above:
include $(addprefix $(DEPENDDIRPFX)$(DEPENDDIR), $(notdir $(LIBOBJECTS:.o=.d)))
