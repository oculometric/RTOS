TOOLPATH 	:= $(HOME)/opt/cross/bin/
PREFIX 		:= $(TOOLPATH)i386-elf-

AS			:= nasm
CC			:= $(PREFIX)g++
LD			:= $(PREFIX)ld

AS_FLAGS		:= -f elf
CC_FLAGS		:= -ffreestanding -m32 -g -masm=intel -Wall -Wextra -Wpedantic -Wno-unused-parameter -mno-red-zone -std=c++20 -fno-exceptions -s -fno-ident -fno-asynchronous-unwind-tables -O3
LD_FLAGS		:= -T linker.ld -s

BIN				= bin
SRC				= src
RES				= res

BOOT_OUT		= $(BIN)/boot.bin
KERN_OUT		= $(BIN)/kernel.bin


CC_DIR 			= $(SRC)
CC_INCLUDE		= $(SRC)/include
AS_DIR			= $(SRC)
BL_DIR			= $(SRC)/boot
OBJ_DIR			= $(BIN)/obj
BINRES_DIR		= $(BIN)/res

CC_FILES_IN		= $(wildcard $(CC_DIR)/*.cpp)
CC_FILES_OUT	= $(patsubst $(CC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CC_FILES_IN))

AS_FILES_IN		= $(wildcard $(AS_DIR)/*.asm)
AS_FILES_OUT	= $(patsubst $(AS_DIR)/%.asm, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

BL_FILES_IN		= $(wildcard $(BL_DIR)/*.asm)
BL_FILES_OUT	= $(patsubst $(BL_DIR)/%.asm, $(OBJ_DIR)/%.bin, $(BL_FILES_IN))

MESH_FILES		= $(wildcard $(RES)/*.obj)
REENCODED_MESHES= $(patsubst $(RES)/%.obj, $(BINRES_DIR)/%.binmesh, $(MESH_FILES))
BMP_FILES		= $(wildcard $(RES)/*.bmp)
REENCODED_BMPS  = $(patsubst $(RES)/%.bmp, $(BINRES_DIR)/%.binbmp, $(BMP_FILES))
EMBED_FILES		= $(REENCODED_MESHES) $(REENCODED_BMPS)
EMBED_FILES_OUT = $(patsubst $(BINRES_DIR)/%, $(BINRES_DIR)/%.o, $(EMBED_FILES))

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BINRES_DIR):
	@mkdir -p $(BINRES_DIR)

$(OBJ_DIR)/%.o: $(CC_DIR)/%.cpp $(OBJ_DIR)
	@echo "Compiling" $<
	@$(CC) $(CC_FLAGS) -I$(CC_INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(AS_DIR)/%.asm $(OBJ_DIR)
	@echo "Assembling" $<
	@$(AS) $(AS_FLAGS) $< -o $@

$(OBJ_DIR)/%.bin: $(BL_DIR)/%.asm $(OBJ_DIR)
	@echo "Assembling raw" $<
	@$(AS) -f bin $< -o $@

$(BINRES_DIR)/%.o: $(BINRES_DIR)/% $(BINRES_DIR)
	@echo "Copying resource" $<
	@$(PREFIX)objcopy -I binary -O elf32-i386 -B i386 $< $@
	@echo _binary_$(subst /,_,$(subst .,_,$(patsubst %.o,%,$@)))_start=_res_$(subst .,_,$(patsubst $(BINRES_DIR)/%,%,$<))_start
	@$(PREFIX)objcopy -I binary -O elf32-i386 -B i386 $< $@ \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$(patsubst %.o,%,$@)))_start=_res_$(subst .,_,$(patsubst $(BINRES_DIR)/%,%,$<))_start \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$(patsubst %.o,%,$@)))_end=_res_$(subst .,_,$(patsubst $(BINRES_DIR)/%,%,$<))_end \
		--redefine-sym _binary_$(subst /,_,$(subst .,_,$(patsubst %.o,%,$@)))_size=_res_$(subst .,_,$(patsubst $(BINRES_DIR)/%,%,$<))_size

$(BINRES_DIR)/%.binmesh: $(RES)/%.obj $(BINRES_DIR)
	@echo "Reencoding" $< "to" $@
	@$(RES)/obj_to_binmesh $< $@

$(BINRES_DIR)/%.binbmp: $(RES)/%.bmp $(BINRES_DIR)
	@echo "Reencoding" $< "to" $@
	@echo "NOT IMPLEMENTED"

reencode-meshes: $(REENCODED_MESHES)

build: $(BL_FILES_OUT) $(CC_FILES_OUT) $(AS_FILES_OUT) $(EMBED_FILES_OUT)
	@echo "Linking" $(KERN_OUT)
	@$(LD) $(LD_FLAGS) -o $(KERN_OUT) $(AS_FILES_OUT) $(CC_FILES_OUT) $(EMBED_FILES_OUT)
	@echo "Stupid concatenation hack" $(BOOT_OUT)
	@cat $(BL_FILES_OUT) $(KERN_OUT) > $(BOOT_OUT)
	@echo "Done, size in bytes:"
	@stat -c%s $(BOOT_OUT)

emulate:
	qemu-system-x86_64 -drive file=$(BOOT_OUT),format=raw,index=0,media=disk -m 32M -monitor stdio -serial file:log/output.log

disassemble:
	objdump -m i8086 -M intel -b binary -D $(BOOT_OUT)

dump:
	hd $(BOOT_OUT)

clean:
	@rm -fr $(BIN)