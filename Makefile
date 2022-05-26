CC=gcc
DEPS=./src/Application.c ./src/Camera.c ./src/Renderer.c ./src/Shader.c ./src/Window.c ./src/LightObject.c ./src/Primitives.c ./Dependencies/src/glad.c ./Dependencies/src/FastNoiseLite.c
HEADERS=./src/Application.h ./src/Camera.h ./src/Renderer.h ./src/Shader.h ./src/Window.h ./src/LightObject.h ./src/Primitives.h
INCLUDE=-I./Dependencies/include/
MAIN=./src/main.c

SDL_FLAGS=-lSDL2main -lSDL2
CGLM_FLAGS=-L./Dependencies/src/ -l:libcglm.a
STD=-std=c99
DEFAULT_FLAGS=-Wall -Wextra -Werror -fsanitize=address -ggdb

FLAGS=${DEFAULT_FLAGS} ${SDL_FLAGS} ${CGLM_FLAGS} -lm ${STD} ${INCLUDE}

all: ${DEPS} ${MAIN} ${HEADERS}
	${CC} ${MAIN} ${DEPS} ${FLAGS} -o ./build/main.o