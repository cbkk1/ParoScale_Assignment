PROJECT := niova-candidate-test-driver
MODULE  := niova-candidate
GO      := go

OS   := $(shell go env GOOS)
ARCH := $(shell go env GOARCH)

# Windows needs .exe suffix
EXT  := $(if $(filter $(OS),windows),.exe,)

OUTPUT := $(PROJECT)_$(OS)$(EXT)

all: mod build

mod:
	@if [ ! -f go.mod ]; then \
		echo "Initializing Go module: $(MODULE)"; \
		$(GO) mod init $(MODULE); \
	fi
	@$(GO) mod tidy

build:
	@echo "Building $(OUTPUT)..."
	$(GO) build -o $(OUTPUT) ./...

install: build
	@cp $(OUTPUT) ../test-driver-bin/
	@echo "Installed $(OUTPUT) to ../test-driver-bin/"

clean:
	@echo "Cleaning up..."
	rm -f $(PROJECT)_linux $(PROJECT)_windows.exe $(PROJECT)_darwin
