#ifndef _MACROS
#define _MACROS

#define GL_FATAL(a) a; {GLenum b = glGetError(); if(b) {fprintf(stderr, "ERROR: %s in file %s on line: %d\n", (char*)glewGetErrorString(b), __FILE__, __LINE__); return b;}}
#define GL_ERROR(a) a; {GLenum b = glGetError(); if(b) fprintf(stderr, "ERROR: %s in file %s on line: %d\n", (char*)glewGetErrorString(b), __FILE__, __LINE__);}
#define GL_INFO(a) a; {GLenum b = glGetError(); if(b) fprintf(stdout, "ERROR: %s in file %s on line: %d\n", (char*)glewGetErrorString(b), __FILE__, __LINE__);}

#define RED(r) r & 255
#define GREEN(g) (g >> 8) & 255
#define BLUE(b) (b >> 16) & 255
#define ALPHA(a) (a >> 24) & 255
#define SPLIT_COLOR(color) RED(color), GREEN(color), BLUE(color)
#define SPLIT_COLOR_A(color) RED(color), GREEN(color), BLUE(color), ALPHA(color)
#define SPLIT_COLOR_F(color) (RED(color)) / 255.0f, (GREEN(color)) / 255.0f, (BLUE(color)) / 255.0f
#define SPLIT_COLOR_F_A(color) (RED(color)) / 255.0f, (GREEN(color)) / 255.0f, (BLUE(color)) / 255.0f, (ALPHA(color)) / 255.0f
#define COLOR(r, g, b) ((unsigned int)((r & 255) | (g & 255) << 8 | (b & 255) << 16))
#define COLOR_A(r, g, b, a) ((unsigned int)((r & 255) | (g & 255) << 8 | (b & 255) << 16 | (a & 255) << 24))

#endif