APPLICATION_ROOT = applications/SignBuddy
OUTPUT = _build

default: build

clean: 
	rm -rf $(APPLICATION_ROOT)/$(OUTPUT)

build:
	@echo "Compiling SignBuddy..."
	@./compile.sh	

flash:
	@make -C $(APPLICATION_ROOT) flash

