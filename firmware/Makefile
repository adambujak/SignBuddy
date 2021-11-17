APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: disco_l4

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

nucleo_l0:
	@echo "Compiling SignBudy for nucleo_l0..."
	@./tools/compile.sh NUCLEO_L0

nucleo_l4:
	@echo "Compiling SignBudy for nucleo_l4..."
	@./tools/compile.sh NUCLEO_L4

disco_l4:
	@echo "Compiling SignBuddy for discovery_l4..."
	@./tools/compile.sh DISCO_L4

format:
	@echo "Formatting Source Code..."
	@./tools/run_formatter.sh

format_check:
	@echo "Checking Source Code Formatting..."
	@./tools/run_formatter.sh -c

flash_disco_l4:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=DISCO_L4

flash_nucleo_l4:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=NUCLEO_L4

flash_nucleo_l0:
	@make -C $(APPLICATION_ROOT) flash BOARD_VERSION=NUCLEO_L0
