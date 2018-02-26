// Pra compilar, roda o script q eu criei:
// ./glcv facecontroller2.cpp; ./facecontroller2

#define CV_NO_BACKWARD_COMPATIBILITY

#include "cv.h"
#include "highgui.h"
#include <cstdlib> 
#include <ctime> 
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cstdio>
#include <signal.h>

#include "imageloader.h"
#include "md2model.h"

#include "texture.h"
#include "types.h"
#include "dcaSeams.h"
#include "loadImage.h"

#define PI 3.14159265

#ifdef _EiC
#define WIN32
#endif

using namespace std;
using namespace cv;

double lookfromx[] = {0,0};
double lookfromy[] = {0,0};
double lookfromz[] = {0,0};
double lookatx[] = {0,0};
double lookaty[] = {0,0};
double lookatz[] = {0,0};

//double guyPos[] = {5.0,1.675,29.0,90.0};
double guyPos[] = {1.5,1.675,22.0,90.0};

String cascadeName = "haarcascade_frontalface_alt.xml";
String nestedCascadeName = "haarcascade_eye_tree_eyeglasses.xml";
int moverEsq = 200;
int moverDir = 400;
int foward = 100;
int backward = 50;
MD2Model* _model;
int _textureId;

double width, height;
vector<Mat> planes;
Mat image, imaged3, imageFloat;
static GLuint texName;

int Window0, Window1, Window2, Window3;
float mapax = -17;
float mapay = -2.75;

int colisao(double i, double j) {
int a = planes[0].at<float>((int)(i * 200), (int)((j * 200)));
//printf("x =  %d  -----   y = %d     --- val: %d \n", (int)(i * 200),  (int)(int)((j * 200)), a );//
        if (a  == 0)   {
   	    //cout << "NAO EH colisao\n";
	    return 0;   
   	}
   	else {
      	   //cout << "EH colisao\n";
	   return 1;
   	}	
}

void init(void) {
  
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	
	glShadeModel (GL_SMOOTH);
	
	//Load the model
	_model = MD2Model::load("palitoAnimado.md2","palito.bmp");
	//_model2 = MD2Model::load("quad.md2","quad.bmp");
	
	if (_model != NULL) {
		_model->setAnimation("walk");
	}
	
	//Load the floor texture
	//Image* image = loadBMP("cube.bmp");
	//_textureId = loadTexture(image);
	//delete image;
	loadGrass();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
  
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
    lookfromx[0] = 1.5;
    lookfromy[0] = 1.675;
    lookfromz[0] = 22.0;
    lookatx[0] = 11.5;
    lookaty[0] = 1.675;
    lookatz[0] = 22.0;
    lookfromx[1] = 8.5;
    lookfromy[1] = 2;
    lookfromz[1] = 27;
    lookatx[1] = 8.5;
    lookaty[1] = 2;
    lookatz[1] = 17;
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}

void draw(void) {
  
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
     GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
     GLfloat lightPos[] = {9.0f, 2.0f, 28.0f, 0.0f};
     glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
     glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
     
		glPushMatrix();
		glTranslatef(guyPos[0], guyPos[1], guyPos[2]);
 		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(guyPos[3], 0.0f, 0.0f, 1.0f);
// 		glScalef(0.8f, 0.8f, 2.0f);
//  		glutWireCube (1.0);
		glScalef(0.25f, 0.25f, 0.25f);
		glTranslatef(0, 0, -1.0);
 		
 		_model->draw();
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(10.9, 2, 39.0);
		glScalef(100.0,100.0,100.0);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		dcaSeamsDraw();
		glPopMatrix();
		
//   		glPushMatrix();
// 		
//  		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
//  		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
//  		_model2->draw();
//   		glPopMatrix();
// 	}
}

void display1(void) {
  
    glutSetWindow(Window1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();             /* clear the matrix */
    gluLookAt (lookfromx[0], lookfromy[0], lookfromz[0], lookatx[0], lookaty[0], lookatz[0], 0.0, 1.0, 0.0);
    
    
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   gluLookAt (8.0, 2.5, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//    glLightfv (GL_LIGHT0, GL_POSITION, position);

   glTranslated (8.0, 2.5, 25);
   glDisable (GL_LIGHTING);
   glColor3f (1.0, 1.0, 1.0);
   glEnable (GL_LIGHTING);
   glPopMatrix ();
   
    draw();
    glutSwapBuffers();
    
    glutSetWindow(Window2);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();             /* clear the matrix */
    gluLookAt (lookfromx[1], lookfromy[1], lookfromz[1], lookatx[1], lookaty[1], lookatz[1], 0.0, 1.0, 0.0);
    draw();
    glutSwapBuffers();
    
    glutSetWindow(Window3);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);
    glBindTexture(GL_TEXTURE_2D, 2000); //ADD
    glBegin(GL_QUADS); //ADD
	glTexCoord2f(0.0, 0.0); glVertex3f(-17, -2.75, 0.0);
	glTexCoord2f(0.0, 1); glVertex3f(-17, 2.75, 0.0);
	glTexCoord2f(1, 1); glVertex3f(17, 2.75, 0.0);
	glTexCoord2f(1, 0.0); glVertex3f(17, -2.75, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f,0.0f,0.0f);
	
    glPushMatrix();
    //printf("%f \n",(0.45)+mapay);
	   glTranslatef(((lookfromz[0]*7/22)+mapax),((lookfromx[0]*0.45/1.5)+mapay),0.0);
	   //glTranslatef(-10,-2.3,0.0);
	  // glTranslatef(lookfromz[0]/10,lookfromx[0]/10 ,0.0);
	glutSolidSphere(0.2,30,30);
	   
    glPopMatrix();
    
    glutSwapBuffers();
}


int dtime = 1;
int time_foward = 0;
int time_backward = 26;

void walk_foward(int value) {
// 	_angle += 0.7f;
// 	if (_angle > 360) {
// 		_angle -= 360;
// 	}
	
	//Advance the animation
	if (_model != NULL && time_foward <= 26) {
		_model->foward(time_foward);
		time_foward += dtime;
		glutPostRedisplay();
		glutTimerFunc(50, walk_foward,0);
		return;
	}
	if (time_foward > 26) time_foward = 0;
}

void walk_backward(int value) {
	if (_model != NULL && time_backward >= 0) {
		_model->backward(time_backward);
		time_backward -= dtime;
		glutPostRedisplay();
		glutTimerFunc(50, walk_backward, 0);
		return;
	}
	if (time_backward < 0) time_backward = 26;
	
}

void reshape (int w, int h) {
  
  glEnable(GL_TEXTURE_2D);
//   if (glutGetWindow() == Window0)
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-0.05, 0.05, -0.05, 0.05, 0.05, 50.0);
    //gluPerspective(45.0, (float)w / (float)h, 1.0, 50.0);
    glMatrixMode (GL_MODELVIEW);
    
    if (glutGetWindow() == Window0) {
      
    glutSetWindow(Window1);
    glutPositionWindow(0, 0);
    glutReshapeWindow(w/2, h);
    glutPostRedisplay();
    glutSetWindow(Window2);
    glutPositionWindow(w/2, 0);
    glutReshapeWindow(w/2, h);
    glutPostRedisplay();
    glutSetWindow(Window3);
    glutPositionWindow((w/2)-220, 0);
    }
}

void reshape1 (int w, int h) {
 glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (0.0, 0.0, -5.0);
	glutPostRedisplay();
	
    
}

void carregar_texturas()
{
	//string a = "/home/daniel/Basketball";
    //a * = "./home/daniel/Basketball.png";
   png_texture((char*) "texturaNovoDCA.png");

}

void carregar_texturas1()
{
	//string a = "/home/daniel/Basketball";
    //a * = "./home/daniel/Basketball.png";
   png_texture((char*) "texturaNovoDCAa.png");

}

double dx;
double dz;
int ang1 = 0;
int ang2 = 180;

void move (char key) {
  switch(key) {
	case 'a':
	  ang1 = (ang1 + 3) % 360;
	  lookatx[0] = 10*cos(ang1*PI/180) + lookfromx[0];
	  lookatz[0] = -10*sin(ang1*PI/180) + lookfromz[0];
	  guyPos[3] = ang1 + 90.0;
	  break;
	case 'd':
	  ang1 = (ang1 - 3) % 360;
	  lookatx[0] = 10*cos(ang1*PI/180) + lookfromx[0];
	  lookatz[0] = -10*sin(ang1*PI/180) + lookfromz[0];
	  guyPos[3] = ang1 + 90.0;
	  break;
	case 'w':
	    dx = (lookatx[0]-lookfromx[0])/100;
	    dz = (lookatz[0]-lookfromz[0])/100;
	    //dx = (lookatx[0]-lookfromx[0])/100;
	    //dz = (lookatz[0]-lookfromz[0])/100;
	    //if (colisao(lookfromx[0] + dx, lookfromz[0] + dz) == 0) {
	    if (colisao(guyPos[0] + dx, guyPos[2] + dz) == 0) {
	      lookfromx[0] = lookfromx[0] + dx;
	      lookfromz[0] = lookfromz[0] + dz;
	      lookatx[0] = lookatx[0] + dx;
	      lookatz[0] = lookatz[0] + dz;
	      guyPos[0] = guyPos[0] + dx;
	      guyPos[2] = guyPos[2] + dz;
	      if (time_foward == 0)
	      glutTimerFunc(0, walk_foward, 0);
	    }
	    //printf("posicao = %d | lookfromx = %f | lookfromz = %f | lookatx = %f | lookatz = %f\n", posicao, lookfromx, lookfromz, lookatx, lookatz);
	    break;
	case 'x':
	    lookfromy[0] = lookfromy[0] - 0.1;
	    lookaty[0] = lookaty[0] - 0.1;
	    break;
	case 's':
	    dx = (lookatx[0]-lookfromx[0])/100;
	    dz = (lookatz[0]-lookfromz[0])/100;
	    if ( colisao(lookfromx[0] - dx, lookfromz[0] - dz) == 0){
	    	lookfromx[0] = lookfromx[0] - dx;
	    	lookfromz[0] = lookfromz[0] - dz;
	    	lookatx[0] = lookatx[0] - dx;
	    	lookatz[0] = lookatz[0] - dz;
	    	guyPos[0] = guyPos[0] - dx;
	    	guyPos[2] = guyPos[2] - dz;
	    	if (time_backward == 26)
	    	glutTimerFunc(0, walk_backward, 0);
            }
	    //printf("posicao = %d | lookfromx = %f | lookfromz = %f | lookatx = %f | lookatz = %f\n", posicao, lookfromx, lookfromz, lookatx, lookatz);
	    break;
	case 'j':
	  ang2 = (ang2 + 5) % 360;
	  lookatx[1] = 10*cos(ang2*PI/180) + lookfromx[1];
	  lookatz[1] = -10*sin(ang2*PI/180) + lookfromz[1];
	  break;
	case 'l':
	  ang2 = (ang2 - 5) % 360;
	  lookatx[1] = 10*cos(ang2*PI/180) + lookfromx[1];
	  lookatz[1] = -10*sin(ang2*PI/180) + lookfromz[1];
	  break;
	case 'i':
	    dx = (lookatx[1]-lookfromx[1])/100;
	    dz = (lookatz[1]-lookfromz[1])/100;
	    lookfromx[1] = lookfromx[1] + dx;
	    lookfromz[1] = lookfromz[1] + dz;
	    lookatx[1] = lookatx[1] + dx;
	    lookatz[1] = lookatz[1] + dz;
	    break;
	case 'k':
	    dx = (lookatx[1]-lookfromx[1])/100;
	    dz = (lookatz[1]-lookfromz[1])/100;
	    lookfromx[1] = lookfromx[1] - dx;
	    lookfromz[1] = lookfromz[1] - dz;
	    lookatx[1] = lookatx[1] - dx;
	    lookatz[1] = lookatz[1] - dz;
	    break;
	case 27:
	    exit(0);
	    break;
	default:
	  break;
    }
//      printf("ang = %d | lookfromx = %f | lookfromz = %f | lookatx = %f | lookatz = %f\n", ang1, lookfromx[0], lookfromz[0], lookatx[0], lookatz[0]);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    move(key);
}

//int firsttime = 1;

struct drawParam{
//   int idThread;
  int x;
  int y;
  int raio;
};

//int x_inicial, y_inicial, raio_inicial;

void teste () {
  glutPostRedisplay();
}

void* moveThread(void* param) {
//   int id = ((struct dataParam *) param)->idThread;
  int x = ((struct drawParam *) param)->x;
  int y = ((struct drawParam *) param)->y;
  int raio = ((struct drawParam *) param)->raio;
  if (x < moverEsq) move('a');
  if (x > moverDir) move('d');
  if (raio > foward) move('w');
  if (raio < backward) move('s');
  
  
  pthread_exit(NULL);
}

void startCam() {
      
      CvCapture* capture = 0;
      Mat frame, frameCopy, image;
      const String scaleOpt = "--scale=";
      size_t scaleOptLen = scaleOpt.length();
      const String cascadeOpt = "--cascade=";
      size_t cascadeOptLen = cascadeOpt.length();
      const String nestedCascadeOpt = "--nested-cascade";
      size_t nestedCascadeOptLen = nestedCascadeOpt.length();
      String inputName;

      CascadeClassifier cascade, nestedCascade;
      double scale = 1;

      if( !cascade.load( cascadeName ) )
      {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=\"<cascade_path>\"]\n"
            "   [--nested-cascade[=\"nested_cascade_path\"]]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" ;
        return;
      }

      capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
      cvNamedWindow( "Camera", 1 );
      cvResizeWindow("Camera",50,50);

      pthread_t new_thread;
      struct drawParam param;
      int statusRetorno;
      
      if( capture )
      {
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = iplImg;
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            //else
                flip( frame, frameCopy, 1 );
		
	    vector<Rect> faces;
	    Mat gray, smallImg( cvRound (frameCopy.rows/scale), cvRound(frameCopy.cols/scale), CV_8UC1 );
	    cvtColor( frameCopy, gray, CV_BGR2GRAY );
	    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
	    cascade.detectMultiScale(smallImg,faces,1.3,3,0
	      |CV_HAAR_FIND_BIGGEST_OBJECT
	      //|CV_HAAR_DO_ROUGH_SEARCH
	      //|CV_HAAR_SCALE_IMAGE
	      ,Size(10, 10));
	    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++)
	    {
	      Point center;
	      int radius;
	      center.x = cvRound((r->x + r->width*0.5)*scale);
	      center.y = cvRound((r->y + r->height*0.5)*scale);
	      radius = cvRound((r->width + r->height)*0.25*scale);
	      circle( frameCopy, center, radius, CV_RGB(255,0,0), 3, 9, 0 );
// 	      printf( "x = %d | y = %d | raio = %d \n", center.x, center.y, radius);
	      param.x = center.x;
	      param.y = center.y;
	      param.raio = radius;
	      statusRetorno = pthread_create(&new_thread, NULL, moveThread, (void *)&param);
	      if (statusRetorno){
// 		printf("Erro ao criar a thread: %d\n", statusRetorno);
		exit(-1);
	      }
	      
	    }
	    cv::imshow( "Camera", frameCopy );

            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }

        waitKey(0);
_cleanup_:
        cvReleaseCapture( &capture );
      }
      
      cvDestroyWindow("Camera");
}

struct dataParam{
   int  idThread;
   int  argc;
   char** argv;
};

void* startThreads(void* param) {
  
  int id = ((struct dataParam *) param)->idThread;
  int argc = ((struct dataParam *) param)->argc;
  char** argv = ((struct dataParam *) param)->argv;
  if (id == 0){
    const char* filename =  "mapaFinal.png";
  image = imread(filename,-1);
  width= image.size().width;
  height=image.size().height;
  cout << "w  " << width << "   h " << height << "\n";
  cvtColor(image, image, CV_BGR2YCrCb);
  image.convertTo(imageFloat,CV_32F);
  split(imageFloat, planes);
  imshow("a", planes[0]);
  
    srand((unsigned)time(0)); 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   
    
    glutInitWindowSize (1000, 500);
    glutInitWindowPosition (0, 0);
    
    Window0 = glutCreateWindow ("Computacao Grafica - Projeto Final - Diogo, Daniel e Gwena");
    glutReshapeFunc(reshape);
    
    Window1 = glutCreateSubWindow (Window0,0,0,500,500);
    carregar_texturas();     
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    init();
    
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    Window2 = glutCreateSubWindow (Window0,500,0,500,500);
    
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    init();
    
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    Window3 = glutCreateSubWindow (Window0,280,0,440,80);
    
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape1);
    glutKeyboardFunc(keyboard);
    init();
    
    glutIdleFunc(teste);
    
    glutMainLoop();
  }
    if (id == 1) {
      startCam();
    }
  pthread_exit(NULL);
}

int main(int argc, char** argv)
{ 
    pthread_t vetorThreads[2];

    struct dataParam vetorParam[2];

    int statusRetorno;

    for(int i=0; i < 2; i++){
      //Inicializando o vetor de parametros
      vetorParam[i].idThread = i;
      vetorParam[i].argc = argc;
      vetorParam[i].argv = argv;

      //printf("Criando a thread: %d\n", i);
      //Criar a thread i
      statusRetorno = pthread_create(&vetorThreads[i], NULL, startThreads, (void *)&vetorParam[i]);
      //Verificando a exist�ncia de erros na cria��o
      if (statusRetorno){
         printf("Erro ao criar a thread: %d\n", statusRetorno);
         exit(-1);
      }
   }
   
   pthread_exit(NULL);
}
