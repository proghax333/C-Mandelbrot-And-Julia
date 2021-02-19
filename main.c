
#include <stdio.h>
#include <stdlib.h>

typedef struct Complex {
	double real, imag;
} Complex;

inline static Complex complex_mult(Complex* a, Complex* b) {
	Complex result;
	result.real = (a->real * b->real - a->imag * b->imag);
	result.imag = (a->real * b->imag + a->imag * b->real);
	return result;
}

inline static Complex complex_add(Complex* a, Complex* b) {
	Complex result;
	result.real = a->real + b->real;
	result.imag = a->imag + b->imag;
	
	return result;
}

inline static double len_square(Complex* a) {
	return a->real * a->real + a->imag * a->imag;
}

inline static int fractal(Complex* z, Complex* c, int maxIter) {
    int n = 0;
    while(len_square(z) <= 4 && n < maxIter) {
	    *z = complex_mult(z, z);
	    *z = complex_add(z, c);
	    
	    n += 1;
	}
    return n;
}

void mandelbrot(char* filename, int width, int height, int maxIter) {
	if(filename) {
		char endline = '\n';
		
		FILE* outputImage = fopen(filename, "wb");
		
		if(!outputImage) {
			return;
		}
		fprintf(outputImage, "P2\n"); // Writing Magic Number to the File
		fprintf(outputImage, "%d %d\n", width, height); // Writing Width and Height into the file
		fprintf(outputImage, "%d\n", maxIter); // Writing the maximum gray value
		
		float step_height = 4.0/(double)height;
		float step_width = 4.0/(double)width;
		Complex* z = (Complex*) calloc(1, sizeof(Complex));
		Complex* c = (Complex*) calloc(1, sizeof(Complex));
		
		int color;
		int** image = (int**) calloc(height, sizeof(int*));
		for(int i = 0; i < height; ++i) {
			image[i] = (int*) calloc(width, sizeof(int));
		}
		
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < width; k++)
			{
				z->real = 0;
				z->imag = 0;
				
				c->real = (((double)k * step_width) - 2.0);
				c->imag = (((double)j * step_height) - 2.0);
				
				color = fractal(z, c, maxIter);
				image[j][k] = maxIter - color;
				
				fprintf(outputImage, "%d ", image[j][k]);
			}
			fprintf(outputImage, "%c", endline);
		}
		
		for(int i = 0; i < height; ++i) {
			free(image[i]);
		}
		free(image);
		free(z);
		free(c);
		fclose(outputImage);
	}
}

void julia(char* filename, int width, int height, int maxIter, Complex c) {
	char endline = '\n';
	FILE* outputImage = fopen(filename, "wb");
	
	if(!outputImage) {
		return;
	}
	// Header
	fprintf(outputImage, "P2\n"); // Writing Magic Number to the File
	fprintf(outputImage, "%d %d\n", width, height); // Writing Width and Height into the file
	fprintf(outputImage, "%d\n", maxIter); // Writing the maximum gray value
	
	float step_height = 4.0/(double)height;
	float step_width = 4.0/(double)width;
	Complex* z = (Complex*) calloc(1, sizeof(Complex));
	
	int color;
	int** image = (int**) calloc(height, sizeof(int*));
	for(int i = 0; i < height; ++i) {
		image[i] = (int*) calloc(width, sizeof(int));
	}
	
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < width; k++)
		{
			z->real = (((double)k * step_width) - 2.0);
			z->imag = (((double)j * step_height) - 2.0);
			
			color = fractal(z, &c, maxIter);
			image[j][k] = maxIter - color;
			
			fprintf(outputImage, "%d ", image[j][k]);
		}
		fprintf(outputImage, "%c", endline);
	}
	
	for(int i = 0; i < height; ++i) {
		free(image[i]);
	}
	free(image);
	free(z);
	fclose(outputImage);
}

int main() {
	int width, height;
	
	printf("Please enter the output image width: > ");
	scanf("%d", &width);
	
	printf("Please enter the output image height: > ");
	scanf("%d", &height);
	
	int maxIter = 400;
	
	Complex c = { -0.8, 0.156 };
	
	mandelbrot("mandelbrot.pgm", width, height, maxIter);
	julia("julia.pgm", width, height, maxIter, c);
	
	return 0;
}

