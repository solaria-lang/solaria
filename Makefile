# Makefile usually runs on files, not commands. If a command is not a file,
# it needs to be explicitly market in .PHONY.
.PHONY: compile run build_compilation_db

# when `make` runs with no arguments, the first target is chosen.
# Commonly, the target is called `all`, and it runs the command that builds
# all the necessary archives.
all: compile

compile:
	gcc *.c

run:
	./a.out

build_compilation_db:
	# bear is a compilation database tool for clang tooling.
	bear -- make
