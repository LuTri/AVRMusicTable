#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jsmn.h"

#define N_COLS 14
#define N_ROWS 8

#define MAX_DISTANCE 16.1245f

typedef struct {
	int r;
	int g;
	int b;
} RGB;

typedef struct {
	float h;
	float S;
	float V;
} HSV;

typedef struct {
	float h;
	float s;
	float i;
} HSI;

typedef struct {
	float h;
	float v;
} HV;

HV mood_arr[N_COLS][N_ROWS];

RGB led_arr[N_COLS][N_ROWS];

void snakish_to_coord(int snake, int* x, int* y) {
	if (snake > N_COLS * N_ROWS) {
		*x = 255;
		*y = 255;
	} else {
		*y = snake / N_COLS;
		if (*y % 2) {
			*x = (N_COLS - 1) - (snake - (*y * N_COLS));
		} else {
			*x = snake - (*y * N_COLS);
		}
	}
}

void fill_mood(int pos, HV* root_val, void(*modifier)(float distance, HV* value, HV* root_value)) {
	HV new_val;
	int x, y;
	int idx_x, idx_y;

	snakish_to_coord(pos, &x, &y);

	for (idx_x = 0; idx_x < N_COLS; idx_x++) {
		for (idx_y = 0; idx_y < N_ROWS; idx_y++) {
			float distance = sqrt(pow((x-idx_x),2) + pow((y-idx_y),2));
			(*modifier)(distance, &new_val, root_val);
			mood_arr[idx_x][idx_y].h = new_val.h;
			mood_arr[idx_x][idx_y].v = new_val.v;
		}
	}
}

inline float cycle360(float value) {
	while (value < 0) value += 360.0;
	while (value >= 360) value -= 360.0;
	return value;
}

inline float cycle(float value, float step) {
	while (value < 0) value += step;
	while (value >= step) value -= step;

	return value;
}

void HsiToRgb(HSI* hsi, RGB* rgb) {
	int r, g, b;
	float H, S, I;
	H = cycle360(hsi->h); // cycle H around to 0-360 degrees
	H = 3.14159*H/(float)180; // Convert to radians.
	S = hsi->s;
	I = hsi->i;
		
	// Math! Thanks in part to Kyle Miller.
	if(H < 2.09439) {
		r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
		g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
		b = 255*I/3*(1-S);
	} else if(H < 4.188787) {
		H = H - 2.09439;
		g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
		b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
		r = 255*I/3*(1-S);
	} else {
		H = H - 4.188787;
		b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
		r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
		g = 255*I/3*(1-S);
	}
	rgb->r=r;
	rgb->g=g;
	rgb->b=b;
}

void fast_hsi(float value, RGB* rgb, float intensity) {
//ranges from 0 to 900
	float r_p = 0, g_p = 0, b_p = 0;
	float tmp_val;
		
	value = cycle(value,900.0);
	tmp_val = 100.0 - (
		cycle(value, 300.0) / 300.0 * 100.0);

	if (value >= 0 && value < 300.0) {
		r_p = tmp_val * intensity;
		g_p = (100 - tmp_val) * intensity;
	} else if (value >= 300.0 && value < 600) {
		g_p = tmp_val * intensity;
		b_p = (100 - tmp_val) * intensity;
	} else {
		b_p = tmp_val * intensity;
		r_p = (100 - tmp_val) * intensity;
	}

	rgb->r = 255 * (r_p / 100);
	rgb->b = 255 * (g_p / 100);
	rgb->g = 255 * (b_p / 100);
}

void HsvToRgb(HSV* hsv, RGB* rgb) {
	float H;
	float R, G, B;
	float V = hsv->V;
	float S = hsv->S;


	H = cycle360(hsv->h);

	if (V <= 0) {
		R = G = B = 0;
	}
	else if (S <= 0) {
		R = G = B = V;
	}
	else {
		float hf = H / 60.0;
		int i = hf;
		float f = hf - i;
		float pv = V * (1 - S);
		float qv = V * (1 - S * f);
		float tv = V * (1 - S * (1 - f));
		switch (i) {

			// Red is the dominant color

			case 0:
				R = V;
				G = tv;
				B = pv;
				break;

			// Green is the dominant color

			case 1:
				R = qv;
				G = V;
				B = pv;
				break;
			case 2:
				R = pv;
				G = V;
				B = tv;
				break;

			// Blue is the dominant color

			case 3:
				R = pv;
				G = qv;
				B = V;
				break;
			case 4:
				R = tv;
				G = pv;
				B = V;
				break;

			// Red is the dominant color

			case 5:
				R = V;
				G = pv;
				B = qv;
				break;
			default:
				//LFATAL("i Value error in Pixel conversion, Value is %d", i);
				R = G = B = V; // Just pretend its black/white
				break;
		}
	}
	rgb->r = (int)(R * 255.0);
	rgb->g = (int)(G * 255.0);
	rgb->b = (int)(B * 255.0);
}

void m1(float distance, HV* value, HV* root_value) {
	value->h = root_value->h - distance * 4;
	value->v = 1.0 - (root_value->v * (distance / MAX_DISTANCE));
}

void JsonLeds(void) {
	int i,j;

	printf("{\"values\":{");
	for (i = 0; i < N_COLS; i++) {
		printf("\"%d\": {", i);
		for (j = 0; j < N_ROWS; j++) {
			printf("\"%d\":{\"r\":%i, \"g\":%i, \"b\":%i}", j, led_arr[i][j].r, led_arr[i][j].g, led_arr[i][j].b);
			if (j < N_ROWS - 1) printf(",");
		}
		printf("}");
		if (i < N_COLS -1 ) printf(",");
	}
	printf("}}\n");
}

void HSVTEST(char* arg) {
	HV root_val;

	int idx;
	int x, y;

	HSV my_hsv;
	RGB my_rgb;

	root_val.v = 1;
	root_val.h = 10.0;
	fill_mood(atoi(arg),&root_val,m1);

	for (idx = 0; idx < N_COLS* N_ROWS; idx++) {
		snakish_to_coord(idx,&x, &y);
		my_hsv.h = mood_arr[x][y].h;
		my_hsv.V = mood_arr[x][y].v;
		my_hsv.S = 1.0;

		HsvToRgb(&my_hsv, &my_rgb);

		led_arr[x][y].r = my_rgb.r;
		led_arr[x][y].g = my_rgb.g;
		led_arr[x][y].b = my_rgb.b;
	}

	JsonLeds();
}

void HSITEST(char* arg) {
	int x, y;
	int idx;

	HSI my_hsi;
	RGB my_rgb;

	my_hsi.h = (float)(atoi(arg)) / 1.2;
	my_hsi.s = 1.0;
	my_hsi.i = 1.0;

	fast_hsi((float)atoi(arg), &my_rgb, 1.0);

//	HsiToRgb(&my_hsi, &my_rgb);


	for (idx = 0; idx < N_COLS * N_ROWS; idx++) {
		
		snakish_to_coord(idx, &x, &y);	
		led_arr[x][y].r = my_rgb.r;
		led_arr[x][y].g = my_rgb.g;
		led_arr[x][y].b = my_rgb.b;
	}
	JsonLeds();
}

int main(int argc, char* argv[]) {

	HSITEST(argv[1]);	
	return 0;
}
