all:
	gcc draw_a_rectangle_in_a_ppm_file.c -o draw_a_rectangle_in_a_ppm_file.o
	gcc draw_a_circle_in_a_ppm_file.c -lm -o draw_a_circle_in_a_ppm_file.o
