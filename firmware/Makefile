APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: nucleo

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

nucleo:
	@echo "Compiling SignBudy for nucleo..."
	@./tools/compile.sh NUCLEO

format:
	@echo "Formatting Source Code..."
	@./tools/run_formatter.sh

format_check:
	@echo "Checking Source Code Formatting..."
	@./tools/run_formatter.sh -c

flash_nucleo:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=NUCLEO
