#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// _J <===> Julia
// _M <===> Mandelbrot

int min(int a, int b) {
	return (a < b) ? a : b;
}

// structura pentru un numar complex
typedef struct _complex {
	double a, b;
} complex;

// structura pentru parametrii unei rulari
typedef struct _params {
	int is_julia, iterations;
	double x_min, x_max, y_min, y_max, resolution;
	complex c_julia;
} params;

// numele fisierelor de intrare si de iesire
char *in_filename_J;
char *in_filename_M;
char *out_filename_J;
char *out_filename_M;

// dimensiunile imaginilor
int width_J, height_J;
int width_M, height_M;

// numarul de thread-uri pe care vom rula programul
int P;

// matricea corespunzatoare imaginii dorite
unsigned char** result;
// range(unsigned char) = [0, 255]

// parametrii de generare a imaginilor
params par_J, par_M;

// bariera pentru thread-uri
pthread_barrier_t barrier;

void get_args(int argc, char** argv) {
	if (argc != 6) {
		puts("Acest executabil trebuie rulat cu 6 parametrii.");
		exit(-1);
	}

	in_filename_J = argv[1];
	out_filename_J = argv[2];
	in_filename_M = argv[3];
	out_filename_M = argv[4];
	P = atoi(argv[5]);
}

// citirea parametrilor din fisierul de intrare dat
void read_input_file(char* in_filename, params *par) {
	FILE *file = fopen(in_filename, "r");
	if (!file) {
		puts("Nu se poate deschide fisierul de intrare!");
		exit(-2);
	}

	fscanf(file, "%d", &par->is_julia);
	fscanf(file, "%lf %lf %lf %lf",
			&par->x_min, &par->x_max,
			&par->y_min, &par->y_max);
	fscanf(file, "%lf", &par->resolution);
	fscanf(file, "%d", &par->iterations);

	if (par->is_julia)
		fscanf(file, "%lf %lf", &par->c_julia.a, &par->c_julia.b);

	fclose(file);
}

// sem e folosit pentru fractalul dorit (0 pentru Julia, 1 pentru Mandelbrot)
void write_output_file(char* out_filename, unsigned char sem) {
	FILE* file = fopen(out_filename, "w");
	if(!file) {
		puts("Nu se poate deschide fisierul de iesire!");
		exit(-3);
	}

	int width = (sem == 0) ? width_J : width_M;
	int height = (sem == 0) ? height_J : height_M;
	
	int i, j;
	fprintf(file, "P2\n%d %d\n255\n", width, height);
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j)
			fprintf(file, "%u ", result[height - 1 - i][j]);
		fprintf(file, "\n");
	}
	fclose(file);
}

void* thread_function(void* arg) {
	int thread_id = *(int*)arg;

///////////////////////////////////////////////////////////////////////////////
//                              JULIA                                        //
///////////////////////////////////////////////////////////////////////////////

	// Pasul 1 : Initializare Julia + Alocare matrice	
	if (thread_id == 0)
		result = malloc(height_J * sizeof(unsigned char*));
	pthread_barrier_wait(&barrier);

	// Pasul 2 : Alocarea liniilor + Algoritmul de generare Julia
	int start_J = thread_id * height_J / P;
	int end_J = min(height_J, (thread_id + 1) * height_J / P);

	int h, w, step, i;
	complex z, z_aux, c;
	c.a = (par_J.c_julia).a;
	c.b = (par_J.c_julia).b;

	for (h = start_J; h < end_J; ++h) {
		result[h] = calloc(width_J, sizeof(unsigned char));

		for (w = 0; w < width_J; ++w) {
			z.a = w * par_J.resolution + par_J.x_min;
			z.b = h * par_J.resolution + par_J.y_min;

			for (step = 0; pow(z.a, 2.0) + pow(z.b, 2.0) < 4.0
			&& step < par_J.iterations; ++step) {
				z_aux.a = z.a;
				z_aux.b = z.b;

				z.a = pow(z_aux.a, 2.0) - pow(z_aux.b, 2.0) + c.a;
				z.b = 2 * z_aux.a * z_aux.b + c.b;
			}

			result[h][w] = step & 0xFF;
		}
	}
	pthread_barrier_wait(&barrier);

	// Pasul 3 : Scrierea in fisier Julia
	if (thread_id == 0)
		write_output_file(out_filename_J, 0);
	pthread_barrier_wait(&barrier);

	// Pasul 4 : Eliberea rezultatului de la Julia
	for (i = start_J; i < end_J; ++i)
		free(result[i]);

	pthread_barrier_wait(&barrier);

	if (thread_id == 0)
		free(result);

	pthread_barrier_wait(&barrier);

///////////////////////////////////////////////////////////////////////////////
//                              MANDELBROT                                   //
///////////////////////////////////////////////////////////////////////////////

	// Pasul 5 : Initializare Mandelbrot + Alocare matrice
	int start_M = thread_id * height_M / P;
	int end_M = min(height_M, (thread_id + 1) * height_M / P);

	if (thread_id == 0)
		result = malloc(height_M * sizeof(unsigned char*));
	pthread_barrier_wait(&barrier);

	// Pasul 6 : Alocare linii + Algoritmul de generare Mandelbrot
	for (h = start_M; h < end_M; ++h) {
		result[h] = calloc(width_M, sizeof(unsigned char));
		for (w = 0; w < width_M; ++w) {
			z.a = 0;
			z.b = 0;

			c.a = w * par_M.resolution + par_M.x_min;
			c.b = h * par_M.resolution + par_M.y_min;

			for (step = 0; pow(z.a, 2.0) + pow(z.b, 2.0) < 4.0
			&& step < par_M.iterations; ++step) {
				z_aux.a = z.a;
				z_aux.b = z.b;

				z.a = pow(z_aux.a, 2.0) - pow(z_aux.b, 2.0) + c.a;
				z.b = 2 * z_aux.a * z_aux.b + c.b;
			}

			result[h][w] = step & 0xFF;
		}
	}
	pthread_barrier_wait(&barrier);

	// Pasul 7 : Scrierea in fisier Mandelbrot
	if (thread_id == 0)
		write_output_file(out_filename_M, 1);
	pthread_barrier_wait(&barrier);

	// Pasul 8 : Eliberea rezultatului de la Mandelbrot
	for (i = start_M; i < end_M; ++i)
		free(result[i]);
	pthread_barrier_wait(&barrier);

	if (thread_id == 0)
		free(result);

	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	// citirea parametrilor din CLI
	get_args(argc, argv);

	// citirea parametrilor din fisierele de intrare
	read_input_file(in_filename_J, &par_J);
	read_input_file(in_filename_M, &par_M);
	
	// setarea dimensiunilor imaginilor de output (Julia)
	width_J = (int)((par_J.x_max - par_J.x_min) / par_J.resolution);
	height_J = (int)((par_J.y_max - par_J.y_min) / par_J.resolution);

	// setarea dimensiunilor imaginilor de output (Mandelbrot)
	width_M = (int)((par_M.x_max - par_M.x_min) / par_M.resolution);
	height_M = (int)((par_M.y_max - par_M.y_min) / par_M.resolution);

	pthread_t tid[P];
	int thread_id[P];

	pthread_barrier_init(&barrier, NULL, P);

	int i;
	for (i = 0; i < P; ++i) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	for (i = 0; i < P; ++i)
		pthread_join(tid[i], NULL);

	pthread_barrier_destroy(&barrier);

	return 0;
}