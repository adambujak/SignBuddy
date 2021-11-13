APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: disco

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

nucleo:
	@echo "Compiling SignBudy for nucleo..."
	@./tools/compile.sh NUCLEO

disco:
	@echo "Compiling SignBuddy for discovery..."
	@./tools/compile.sh DISCO

format:
	@echo "Formatting Source Code..."
	@./tools/run_formatter.sh

format_check:
	@echo "Checking Source Code Formatting..."
	@./tools/run_formatter.sh -c

flash_disco:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=DISCO

flash_nucleo:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=NUCLEO

