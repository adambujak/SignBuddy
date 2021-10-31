APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: build

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

build:
	@echo "Compiling SignBuddy..."
	@./tools/compile.sh

format:
	@echo "Formatting Source Code..."
	@./tools/run_formatter.sh

flash:
	@make -C $(APPLICATION_ROOT) flash

