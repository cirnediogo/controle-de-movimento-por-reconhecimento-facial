//#define GRASS_TEX_SIZE1 640
//#define GRASS_TEX_SIZE2 480
#define GRASS_TEX_SIZE1 2200
#define GRASS_TEX_SIZE2 400
//#define GRASS_TEX_SIZE1 640
//#define GRASS_TEX_SIZE2 480

GLubyte grama[GRASS_TEX_SIZE1][GRASS_TEX_SIZE2][3];

void loadGrass() {
	FILE *arq;
	
	//arq = fopen("grass3DCA.pnm", "r");
	arq = fopen("mapinha4.pnm", "r");
	//arq = fopen("labirintot.pnm", "r");
	//arq = fopen("grass3.pnm", "r");
	
	char formato[3];
	int w, h, d;
	fscanf(arq, "%s %d %d %d", formato, &w, &h, &d);
	for(int i = 0; i < GRASS_TEX_SIZE1; i++)
		for(int j = 0; j < GRASS_TEX_SIZE2; j++) {
			int a, b, c;
			fscanf(arq, "%d %d %d", &a, &b, &c);
			grama[i][j][0] = (GLubyte) a;
			grama[i][j][1] = (GLubyte) b;
			grama[i][j][2] = (GLubyte) c;
		}
	fclose(arq);
	glBindTexture(GL_TEXTURE_2D, 2000);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GRASS_TEX_SIZE1, GRASS_TEX_SIZE2, 0, GL_RGB, GL_UNSIGNED_BYTE, grama);
}

