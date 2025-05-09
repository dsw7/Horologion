.PHONY = help format compile clean lint

BUILD_DIR = build

define HELP_LIST_TARGETS
To format code:
    $$ make format
To compile binary:
    $$ make compile
To remove build directory:
    $$ make clean
To run cppcheck linter:
    $$ make lint
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

format:
	@clang-format -i --verbose --style=file Horologion/src/*.cpp Horologion/src/*.hpp

compile:
	@cmake -S Horologion -B $(BUILD_DIR)
	@make --jobs=12 --directory=$(BUILD_DIR)

clean:
	@rm -rfv $(BUILD_DIR)

lint:
	@cppcheck Horologion --enable=all
