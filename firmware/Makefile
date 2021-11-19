APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: nucleo

.PHONY: test clean format format_check flash_nucleo

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

nucleo:
	@echo "Compiling SignBudy for nucleo..."
	@./tools/compile.sh NUCLEO

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
