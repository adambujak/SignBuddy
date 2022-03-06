APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: buddy

.PHONY: test clean format format_check flash_nucleo flash_buddy

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

nucleo:
	@echo "Compiling SignBudy for nucleo..."
	@./tools/compile.sh NUCLEO

buddy:
	@echo "Compiling SignBudy for buddy..."
	@./tools/compile.sh BUDDY

proto:
	@echo "Generating proto source..."
	@./tools/docker_build_proto.sh

test:
	@echo "Running tests..."
	@./tools/docker_run_tests.sh

format:
	@echo "Formatting Source Code..."
	@./tools/docker_run_formatter.sh

format_check:
	@echo "Checking Source Code Formatting..."
	@./tools/docker_run_formatter.sh -c

flash_nucleo:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=NUCLEO

flash_nucleo_jlink:
	@make -C $(APPLICATION_ROOT) flash_jlink BOARD_VERSION=NUCLEO

flash_buddy:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=BUDDY

flash_buddy_jlink:
	@make -C $(APPLICATION_ROOT) flash_jlink BOARD_VERSION=BUDDY
