BUILD_DIR=build
# Adjust T3D_INST to point to your Tiny3D directory
T3D_INST=$(shell realpath ../..)

include $(N64_INST)/include/n64.mk
include $(T3D_INST)/t3d.mk

N64_CFLAGS += -std=gnu2x -O2

PROJECT_NAME=my_menu_game

src = main.c

# These folders are required for your rules to work
assets_png = $(wildcard assets/*.png)
assets_ttf = $(wildcard assets/*.ttf)

# Conversion logic from your template
assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
              $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64)))

all: $(PROJECT_NAME).z64

# Rule for sprites
filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	$(N64_MKSPRITE) -o filesystem "$<"

# Rule for fonts
filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	$(N64_MKFONT) -s 9 -o filesystem "$<"

# Linking the ROM
$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(assets_conv)
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(src:%.c=$(BUILD_DIR)/%.o)

$(PROJECT_NAME).z64: N64_ROM_TITLE="My N64 Menu"
$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

clean:
	rm -rf $(BUILD_DIR) *.z64 filesystem

.PHONY: all clean
