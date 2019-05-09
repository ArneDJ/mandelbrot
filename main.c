#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define MAX_ITERATIONS 500

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("mandelbrot", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
			SDL_WINDOW_OPENGL);
	if(window == NULL) {
		printf("could not create SDL2 window: %s\n", SDL_GetError());
		return 1;
	}
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glewInit()) {
		fprintf(stderr, "unable to init glew ... exiting\n");
		return 1;
	}

	struct shaderinfo info[] = {
		{GL_VERTEX_SHADER, "vertex.glsl"},
		{GL_FRAGMENT_SHADER, "fragment.glsl"},
		{GL_NONE, NULL}
	};
	const GLuint program = load_shaders(info);

	const float vertices[] = {
		-1, 1, 0,
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0, 
		1, -1, 0,
		1, 1, 0
	};
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(program);
	glBindVertexArray(vao);

	SDL_Event event;
	struct center {double x; double y;} center;
	center.x = 0.0, center.y = 0.0;
	double zoom = 0.5;
	for(;;) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT) break;

		const Uint8 *keystates = SDL_GetKeyboardState(NULL);
		if(keystates[SDL_SCANCODE_ESCAPE]) break;;
		if(keystates[SDL_SCANCODE_W]) center.y += 0.05 / zoom;
		if(keystates[SDL_SCANCODE_S]) center.y -= 0.05 / zoom;
		if(keystates[SDL_SCANCODE_A]) center.x -= 0.05 / zoom;
		if(keystates[SDL_SCANCODE_D]) center.x += 0.05 / zoom;
		if(keystates[SDL_SCANCODE_Z]) zoom += 0.1 * zoom;
		if(keystates[SDL_SCANCODE_Q]) zoom -= 0.1 * zoom;

		if(zoom < 0.1)
			zoom = 0.1;

		glUseProgram(program);
		glUniform2d(glGetUniformLocation(program, "screen"), SCREEN_WIDTH, SCREEN_HEIGHT);
		glUniform2d(glGetUniformLocation(program, "center"), center.x, center.y);
		glUniform1d(glGetUniformLocation(program, "zoom"), zoom);
		glUniform1i(glGetUniformLocation(program, "itr"), MAX_ITERATIONS);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

