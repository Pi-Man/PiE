#include "Shader.h"

Shader::Shader() {}

int Shader::buildShader(const char * vertFile, const char * fragFile, const char * geomFile) {

	ID = glCreateProgram();
	vertID = glCreateShader(GL_VERTEX_SHADER);
	fragID = glCreateShader(GL_FRAGMENT_SHADER);

	FILE *file;
	size_t size;
	char *fileContents = (char*)SDL_malloc(1);
	errno_t error;
	if (!(error = fopen_s(&file, vertFile, "r"))) {

		size = 10; while (fgetc(file) != EOF) { size++; }
		fseek(file, 0, SEEK_SET);
		fileContents = (char*)SDL_realloc(fileContents, size * sizeof(char));

		size_t s = fread_s(fileContents, size, sizeof(char), size, file);
		fileContents[s] = 0;

		GL_FATAL(glShaderSource(vertID, 1, &fileContents, NULL));
		GL_SERROR(glCompileShader(vertID), vertID, fileContents);

		fclose(file);
	}
	else {
		printf("ERROR: could not open file: %s\nERROR: %ul\n", vertFile, error);
	}

	if (!(error = fopen_s(&file, fragFile, "r"))) {

		size = 10; while (fgetc(file) != EOF) { size++; }
		fseek(file, 0, SEEK_SET);
		fileContents = (char*)SDL_realloc(fileContents, size * sizeof(char));

		size_t s = fread_s(fileContents, size, sizeof(char), size, file);
		fileContents[s] = 0;

		GL_FATAL(glShaderSource(fragID, 1, &fileContents, NULL));
		GL_SERROR(glCompileShader(fragID), fragID, fileContents);

		fclose(file);
	}
	else {
		printf("ERROR: could not open file: %s\nERROR: %ul\n", fragFile, error);
	}

	if (geomFile) {
		if (!(error = fopen_s(&file, geomFile, "r"))) {

			GL_FATAL(geomID = glCreateShader(GL_GEOMETRY_SHADER));

			size = 10; while (fgetc(file) != EOF) { size++; }
			fseek(file, 0, SEEK_SET);
			fileContents = (char*)SDL_realloc(fileContents, size * sizeof(char));

			size_t s = fread_s(fileContents, size, sizeof(char), size, file);
			fileContents[s] = 0;

			GL_FATAL(glShaderSource(geomID, 1, &fileContents, NULL));
			GL_SERROR(glCompileShader(geomID), geomID, fileContents);

			fclose(file);
		}
		else {
			printf("ERROR: could not open file: %s\nERROR: %ul\n", geomFile, error);
		}
	}

	SDL_free(fileContents);

	GL_FATAL(glAttachShader(ID, vertID));
	GL_FATAL(glAttachShader(ID, fragID));
	if (geomFile) GL_FATAL(glAttachShader(ID, geomID));
	GL_FATAL(glLinkProgram(ID));
	GL_FATAL(glDeleteShader(vertID));
	GL_FATAL(glDeleteShader(fragID));
	if (geomFile) GL_FATAL(glDeleteShader(geomID));

	return 0;
}

Shader::~Shader() {}
