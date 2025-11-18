# Makefile para Ejercicio Individual 3 (refactor)
# Usa variables y reglas patrón. Diseñado para ejecutarse desde Git Bash / MSYS (cp, rm, mkdir -p).

# --- Variables ---
CXX := g++
INCLUDES := -I./include \
            -I./external/spdlog/include \
            -I./external/GLM/include \
            -I./external/SDL2/include \
			-I./external/SDL2/include/SDL2 \
            -I./external/SDL2_image/include \
			-I./external/SDL2_image/include/SDL2 \
            -I./external/SDL2_ttf/include \
			-I./external/SDL2_ttf/include/SDL2
CXXFLAGS := -std=gnu++17 -Wall -mconsole -DSPDLOG_WIN_NO_FILE_FLUSH -DGLM_ENABLE_EXPERIMENTAL $(INCLUDES)
LDFLAGS := -L./lib -L./external/SDL2/lib -L./external/SDL2_image/lib -L./external/SDL2_ttf/lib
# Incluye tu libspdlog.a en ./lib (libspdlog.a)
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lspdlog \
        -lwinmm -limm32 -lversion -lole32 -loleaut32 -lsetupapi -lshell32 -luser32 -lgdi32 -luuid

# Fuentes y objetos
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,build/%.o,$(SRCS))

# Output
BIN_DIR := bin
BUILD_DIR := build
TARGET := $(BIN_DIR)/juego.exe

# --- Reglas principales ---
all: $(TARGET) copy_dlls

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Enlazando ejecutable..."
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
	@echo "----------------------------------------"
	@echo "¡PROYECTO COMPILADO EXITOSAMENTE!"
	@echo "----------------------------------------"

# Crear carpetas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla patrón para compilar .cpp -> .o
build/%.o: src/%.cpp | $(BUILD_DIR)
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copia de DLLs (asume Git Bash/MSYS)
copy_dlls: | $(BIN_DIR)
	@echo "Copiando DLLs necesarias a $(BIN_DIR)/"
	cp -n external/SDL2/bin/SDL2.dll $(BIN_DIR)/ || true
	cp -n external/SDL2_image/bin/SDL2_image.dll $(BIN_DIR)/ || true
	cp -n external/SDL2_ttf/bin/SDL2_ttf.dll $(BIN_DIR)/ || true

# Ejecutar
run:
	@echo "Ejecutando $(TARGET)"
	$(TARGET)

clean:
	@echo "Limpiando..."
	rm -rf $(BUILD_DIR)/*.o || true
	rm -rf $(BIN_DIR)/*.exe || true
	rm -rf $(BIN_DIR)/*.dll || true

.PHONY: all copy_dlls run clean
