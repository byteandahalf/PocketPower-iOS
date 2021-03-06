# Name of the project being built
PROJECT := pocketpower

# Name of process the tweak is loaded into
PROCESS := minecraftpe

# Local IP Address of device to SSH into
DEVICEIP := 192.168.1.5

# Directory of the iOS SDK on MacOS
SDKROOT := /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk

# Build using libc++ and C++11 support (for 12/24-byte strings and vectors)
# This should be build on a Mac, since the iOS build environment does not work correctly when linking to libc++
override CXXFLAGS += -stdlib=libc++ -std=c++11 -miphoneos-version-min=7.0 -isysroot $(SDKROOT)
override LDFLAGS += -stdlib=libc++ -std=c++11 -miphoneos-version-min=7.0 -isysroot $(SDKROOT)


# Names of the tweak library, substrate filter, and debian package
TWEAK := $(PROJECT).dylib
FILTER := $(PROJECT).plist
DEB := $(PROJECT).deb
# Directory for build products like executables, object files, and dependency files
BUILD := build
# Directory for debian package filesystem layout
LAYOUT := layout
# Directory for temporary staging of the debian package structure while creating the package
STAGING := $(BUILD)/deb
SUBSTRATE := $(STAGING)/Library/MobileSubstrate/DynamicLibraries
# Name of the unstripped version
UNSTRIPPED := $(BUILD)/$(TWEAK:.dylib=_unstripped.dylib)
# List of source files and their corresponding object file paths
SRCS := $(shell find . -type f -name '*.cpp')
OBJS := $(addprefix $(BUILD)/,$(SRCS:.cpp=.o))
# Supported architectures
ARCHS := armv7 arm64
ARCHFLAGS := $(addprefix -arch ,$(ARCHS))
# Frameworks for linking
FRAMEWORKS := CydiaSubstrate Forklift
override LDFLAGS += $(addprefix -framework ,$(FRAMEWORKS))

# Compiler and linker
CLANG := clang $(ARCHFLAGS)
CLANGXX := clang++ $(ARCHFLAGS)
CC := $(CLANG)
CXX := $(CLANGXX)
LD := $(CLANGXX)
STRIP := strip

# When invoked as "make VERBOSE=1", every command gets printed as it runs
ifdef VERBOSE
_v :=
else
_v := @
endif


### Build rules ###

all: $(TWEAK)

.PHONY: all

$(TWEAK): $(UNSTRIPPED)
	@echo 'Strip $@'
	$(_v)$(STRIP) -x -o $@ $<

%.dylib: $(OBJS)
	@echo 'Link $@'
	$(_v)$(LD) $(LDFLAGS) -dynamiclib -o $@ $^

%.cpp: $(BUILD)/%.d

$(BUILD)/%.o: %.cpp | $(BUILD)/.dir
	@echo 'Compile $@'
	$(_v)$(CXX) $(CXXFLAGS) -MD -MF $(BUILD)/$*.d -c -o $@ $<

.SECONDARY: $(BUILD)/.dir

-include $(BUILD)/*.d


### Package rules ###

package: $(DEB)

.PHONY: package

$(DEB): $(TWEAK) $(FILTER)
	@echo 'Stage $@'
	$(_v)rm -rf $(STAGING)
	$(_v)mkdir -p $(STAGING)
	$(_v)cp -R $(LAYOUT)/* $(STAGING)
	$(_v)mkdir -p $(SUBSTRATE)
	$(_v)cp $^ $(SUBSTRATE)
	$(_v)chown -R root:wheel $(STAGING)
	$(_v)chmod 0755 $(SUBSTRATE)/$(TWEAK)
	$(_v)chmod 0644 $(SUBSTRATE)/$(FILTER)
	@echo 'Package $@'
	$(_v)dpkg-deb -Zgzip -b $(STAGING) $@
	$(_v)rm -rf $(STAGING)


### Install rules ###

install: $(DEB)
	@echo 'Install $(DEB)'
	scp $(DEB) root@$(DEVICEIP):/var/tmp/
	@echo ‘Run dpkg -i /var/tmp/$(DEB) to install.’
	ssh root@$(DEVICEIP)

.PHONY: install

### Clean rules ###

clean:
	@echo 'Remove $(BUILD)'
	$(_v)rm -rf $(BUILD)

.PHONY: clean


### General rules ###

%/.dir:
	@echo 'Create directory $*/'
	$(_v)mkdir -p $* && touch $@
