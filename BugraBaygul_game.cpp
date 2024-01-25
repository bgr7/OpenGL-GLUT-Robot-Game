#include <GL/glut.h>
#include <cmath>
#include <cstdlib> 
#include <SOIL/SOIL.h>
//https://stackoverflow.com/questions/30227615/mapping-a-texture-with-opengl-using-soil
//https://stackoverflow.com/questions/18886598/how-do-i-install-soil-simple-opengl-image-loader#:~:text=Installing%20SOIL%20by%20%22sudo%20apt,(%23include%20%3CSOIL%2FSOIL.
#include <cstdio>
//I run that code in ubuntu by using that compile code :
//g++ BugraBaygul_hw2p4.cpp -o deneme -lGL -lGLU -lglut -lSOIL
//./deneme
//all the references that I used in that assignment is given below just after the related code.
//####################################INSTRUCTIONS#########################################
/* 
   Use "f" and "b" or the up and down arrow keys to move forward and backward.
   Use "r" and "l" or the right and left arrow keys rotate the camera around its own center.
   Press "0" for make Night
   Press "1" for make Day
   Press "2" for make Emissive on
   Press "3" for make Emissive off
   Press "w" to move robot forward.                                                                   
   Press "s" to move robot backward.                                                         
   Press "a" to move robot left.
   Press "d" to move robot right.
   
##########################################################################################
*/

float coneRotate = 0.0;
float robotX = 10.0;
float robotZ = 150.0;
float robotY= 30;
float coneX = 10;
float coneY = 30;
float coneZ = 47;
float eye_x, eye_y, eye_z;
float initial_cam_x = 20.0, initial_cam_y = 80.0, initial_cam_z = 220.0;
float initial_lx = 0.0, initial_lz = -1.0;
float cam_x = 20.0, cam_y = 80.0, cam_z = 220.0;
float cam_yaw = 0.0;
float lx = 0.0, lz = -1.0;
float coinPositions[5][3];
void resetCoin(int index);
int findCoinIndex(float x, float z);
bool checkCollision(float x, float z);
int collectedCoins = 0;
float riverWidth = 40.0;
bool alreadychecked=false;
bool playerwins =false;

bool DayOn = true;  //oyun sabah başlıyo.

void ChangeDay() {
    DayOn = !DayOn;
    
    
}

GLuint texPhoto;

void drawTexture(const char *imagePath) {
    int width, height;
    unsigned char *image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image) {
        printf("Error: %s\n", SOIL_last_result());
        exit(1);
    }

    glGenTextures(1, &texPhoto);
    glBindTexture(GL_TEXTURE_2D, texPhoto);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void drawTexBoundary() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texPhoto);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-55.0, 21.0, 0.0);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-25.0, 21.0, 0.0);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(-25.0, 61.0, 0.0);

    glTexCoord2f(1.0, 1.0);
    glVertex3f(-55.0, 61.0, 0.0);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); 
    glVertex3f(45.0, 21.0, 0.0);

    glTexCoord2f(1.0, 0.0); 
    glVertex3f(75.0, 21.0, 0.0);

    glTexCoord2f(1.0, 1.0); 
    glVertex3f(75.0, 61.0, 0.0);

    glTexCoord2f(0.0, 1.0); 
    glVertex3f(45.0, 61.0, 0.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
}



void init(void) {
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);

    glEnable(GL_LIGHTING);
    if (DayOn){
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {eye_x, eye_y, eye_z, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat sunLightPosition[] = {80.0, 140.0, 20.0, 1.0};
    GLfloat sunLightAmbient[] = {0.02, 0.02, 0.2, 1.0}; 
    GLfloat sunLightDiffuse[] = {0.9, 0.9, 0.1, 1.0};  
    GLfloat sunLightSpecular[] = {0.8, 0.8, 0.8, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, sunLightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunLightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunLightSpecular);
}
    if (!DayOn) {
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
   
    
    GLfloat lampLightPosition[] = {60.0, 28.0, 30.0, 1.0};
    GLfloat lampLightAmbient[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat lampLightDiffuse[] = {1.0, 1.0, 1.0, 1.0};  
    GLfloat lampLightSpecular[] = {1.0, 1.0, 1.0, 1.0};
    
    glLightfv(GL_LIGHT1, GL_POSITION, lampLightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lampLightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lampLightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lampLightSpecular);
    
    GLfloat castleLightPosition[] = {10.0, 1000.0, 0.0, 1.0}; //30
    GLfloat castleLightAmbient[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat castleLightDiffuse[] = {1.0, 0.5, 0.0, 1.0};  
    GLfloat castleLightSpecular[] = {1.0, 1.0, 0.0, 1.0};
    
    glLightfv(GL_LIGHT2, GL_POSITION, castleLightPosition);
    glLightfv(GL_LIGHT2, GL_AMBIENT, castleLightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, castleLightDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, castleLightSpecular);

    
}
    srand(40);
    for (int i = 0; i < 5; ++i) {
        float xOffset = rand() % 300 - 150;
        float zOffset = rand() % 40 + 100;

        coinPositions[i][0] = xOffset;
        coinPositions[i][1] = 25.0;
        coinPositions[i][2] = zOffset;
    }

    glClearColor(0.5, 0.8, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1.0, 50.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    //gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
    gluLookAt(cam_x, cam_y, cam_z, cam_x + lx, cam_y, cam_z + lz, 0.0, 1.0, 0.0);

    
     

    
}
void setDullMaterialProperties() {
    GLfloat dullAmbient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat dullDiffuse[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat dullSpecular[] = {0.0, 0.0, 0.0, 1.0};  

    glMaterialfv(GL_FRONT, GL_AMBIENT, dullAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dullDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dullSpecular);
}

void drawings() {
    glPushMatrix(); 
//REF1: https://community.khronos.org/t/performance-slowing-down-over-time/55040
//REF2: https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwjd-cX_-O2CAxXvVfEDHeNVDLoQFnoECAsQAQ&url=https%3A%2%2www.gamedev.net%2forums%2topic%2224338-timer-movements-amp-glut_elapsed_time%2&usg=AOvVaw2EQ3oEuuP2eqlKAwcvHdtr&opi=89978449
    float rotAngle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 60.0;
    glRotatef(rotAngle, 0.0, 1.0, 0.0);

    glTranslatef(80.0, 95.0, 0.0);
    GLfloat teapot_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, teapot_diffuse);
    glColor3f(0.0, 0.0, 1.0);
    setDullMaterialProperties();
    glutSolidTeapot(10.0);

    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(110.0, 25.0, -20.0);
    glColor3f(0.1, 0.0, 0.0); 
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix();                   //ağac koku
    glPushMatrix(); 
    glTranslatef(110.0, 30.0, -20.0);
    glColor3f(0.1, 0.0, 0.0); 
    setDullMaterialProperties();
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix(); 
    
    glPushMatrix(); 
    glTranslatef(110.0, 40.0, -20.0);
    glColor3f(0, 0.5, 0.0); 
    setDullMaterialProperties();
    glutWireSphere(10, 20, 20);
    
    glutSolidSphere(10.0,20,20);
    glPopMatrix();
    
    glPushMatrix(); 
    glTranslatef(110.0, 25.0, 20.0);
    glColor3f(0.1, 0.0, 0.0); 
    setDullMaterialProperties();
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix();                   //ağac wire form
    glPushMatrix(); 
    glTranslatef(110.0, 30.0, 20.0);
    glColor3f(0.1, 0.0, 0.0); 
    setDullMaterialProperties();
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix(); 
    
    glPushMatrix(); 
    glTranslatef(110.0, 40.0, 20.0);
    glColor3f(0, 0.5, 0.0); 
    setDullMaterialProperties();
    glutWireSphere(10.0,20,20);
    glPopMatrix();
    
    glPushMatrix(); 
    glTranslatef(110.0, 25.0,60.0);
    glColor3f(0.1, 0.0, 0.0);
    setDullMaterialProperties(); 
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix();                   //ağac wire+solid
    glPushMatrix(); 
    glTranslatef(110.0, 30.0, 60.0);
    glColor3f(0.1, 0.0, 0.0);
    setDullMaterialProperties(); 
    glutSolidCube(5.0); 
    setDullMaterialProperties();
    glutWireCube(5.0);
    glPopMatrix(); 
    
    glPushMatrix(); 
    glTranslatef(110.0, 40.0, 60.0);
    glColor3f(0, 0.5, 0.0); 
    setDullMaterialProperties();
    glutSolidSphere(10.0,20,20);
    glColor3f(1.0,1.0,1.0);
    setDullMaterialProperties();
    glutWireSphere(10, 20, 20);
    
    glPopMatrix();
    
    
   
   
   
    
   
}


void drawStars() {
    if (!DayOn) {
        
        glPushMatrix();
        GLfloat star_diffuse[] = {0.0, 0.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, star_diffuse);
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(80.0, 140.0, 20.0);
        setDullMaterialProperties();
        glutSolidSphere(1.0, 20, 20);
        glPopMatrix();

        srand(42);  // Random kullanınca oluşan hatayı sabit random kullanarak değiştirdim. !!

        for (int i = 0; i < 600; ++i) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);

            
            float xOffset = rand() % 300 - 5;  
            float yOffset = rand() % 300 - 5;
            float zOffset = rand() % 300 - 5;

            glTranslatef(-120.0 + xOffset, 140.0 + yOffset, -60.0 + zOffset);
            setDullMaterialProperties();
            glutSolidSphere(0.5, 10, 10);  
            glPopMatrix();
        }
    }
}

void drawSun() {
    if (DayOn) {
        
        glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(80.0, 140.0, 20.0);
        setDullMaterialProperties();
        glutSolidSphere(10.0, 20, 20);
        glPopMatrix();
    }
}
void drawWoods(){
    for (int i =0;i<3;i++){
    	glPushMatrix();
    	float woodX = -80;
    	float woodY = 25;
    	float woodZ = -20+ i*40;
    	
    	glTranslatef(woodX,woodY,woodZ);
    	glColor3f(0.1,0.0,0.0);
    	glutSolidCube(5.0); 
    	glPopMatrix();
    	glPushMatrix();
    	glTranslatef(woodX,woodY+5,woodZ);
    	glColor3f(0.1,0.0,0.0);
    	setDullMaterialProperties();
    	glutSolidCube(5.0); 
    	glPopMatrix();
    	
    	
    	}
    	
    	
    	
    
    }                         





void drawTrees() {
    for (int i = 0; i < 3; ++i) {
        glPushMatrix(); 
        float treeX = -80.0;
        float treeY = 40.0;
        float treeZ = -20.0 +i*40;

        glTranslatef(treeX, treeY, treeZ);

        
        glColor3f(0.0, 0.5, 0.0); 
        setDullMaterialProperties();
        glutSolidSphere(10.0, 20, 20);
        glColor3f(0.1, 0.0, 0.0); 
        glutWireSphere(10.0, 20, 20);
        

        glPopMatrix(); 
    }
}




void drawCastle() {
    
    glColor3f(2.0, 0.5, 1.0);
    glPushMatrix();
    glTranslatef(10.0, 35.0, 0.0);
    setDullMaterialProperties();
    glutSolidCube(40.0);
    glPopMatrix();
    glColor3f(0.8, 0.0, 0.0); 
    glPushMatrix();
    glTranslatef(10.0, 25.0, 14.0);
    setDullMaterialProperties();
    glutSolidCube(15.0);
    glPopMatrix();
    
    

    
    glColor3f(1.0, 0.35, 1.0); 
    for (float y = 60.0; y >= 0.0; y -= 10.0) {          //kuleler
        glPushMatrix();
        glTranslatef(30.0, y, 20.0);
        setDullMaterialProperties();
        glutSolidCube(10.0);
        glutWireCube(10.0);
        glPopMatrix();
    }

    for (float y = 60.0; y >= 0.0; y -= 10.0) {
        glPushMatrix();
        glTranslatef(30.0, y, -20.0);
        setDullMaterialProperties();
        glutSolidCube(10.0);
        glPopMatrix();
    }

    for (float y = 60.0; y >= 0.0; y -= 10.0) {
        glPushMatrix();
        glTranslatef(-10.0, y, 20.0);
        setDullMaterialProperties();
        glutSolidCube(10.0);
        glPopMatrix();
    }

    for (float y = 60.0; y >= 0.0; y -= 10.0) {
        glPushMatrix();
        glTranslatef(-10.0, y, -20.0);
        setDullMaterialProperties();
        glutSolidCube(10.0);
        glPopMatrix();
    }

    
    glColor3f(0.5, 1.0, 1.0); 
    glPushMatrix();                           // kulenin ust kısmı
    glTranslatef(30.0, 65.0, 20.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCone(5.0, 12.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30.0, 65.0, -20.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCone(5.0, 12.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0, 65.0, 20.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCone(5.0, 12.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0, 65.0, -20.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCone(5.0, 12.0, 20, 20);
    glPopMatrix();
    
   
    
    glColor3f(0.5, 0.5, 0.5); 
    glPushMatrix();
    glTranslatef(coneX,coneY,coneZ); 
    glRotatef(coneRotate, 0.0, 1.0, 0.0);  //hareket eden koni deneme
    glRotatef(90, 1.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCone(10.0, 12.0, 20, 20);
    glColor3f(1.0, 1.0, 1.0); 
    glutWireCone(10.0,12.0,8,20);
    
    //glutWireCone(8.0, 12.0, 20, 20);
    glPopMatrix();
    
    
    
   
}
void setMaterialProperties1(); // I defined it before drawRobot because it doesn't work otherwise.
void drawRobot(){

    glPushMatrix(); 
    glTranslatef(robotX, robotY-5, robotZ);
    glColor3f(0.5, 0.5, 0.5); 
    setMaterialProperties1();
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix();                          
    glPushMatrix(); 
    glTranslatef(robotX, robotY, robotZ);
    glColor3f(0.5, 0.5, 0.5); 
    setMaterialProperties1();
    glutSolidCube(5.0);
    glutWireCube(5.0);
    glPopMatrix(); 
    glPushMatrix(); 
    glTranslatef(robotX+5, robotY, robotZ);
    glColor3f(0.5, 0.5, 0.5); 
    setMaterialProperties1();                 //kollar
    glutSolidCube(2.0);
    glutWireCube(2.0);
    glPopMatrix(); 
    glPushMatrix(); 
    glTranslatef(robotX-5, robotY, robotZ);
    glColor3f(0.5, 0.5, 0.5); 
    setMaterialProperties1();
    glutSolidCube(2.0);
    glutWireCube(2.0);
    glPopMatrix(); 
    
    glPushMatrix(); 
    glTranslatef(robotX, robotY+5, robotZ);  //kafa
    glColor3f(0.5, 0.5, 0.5); 
    setMaterialProperties1();
    glutSolidSphere(3.0,20,20);
    glPopMatrix();

}
void setMaterialProperties1() {
    GLfloat materialAmbient[] = {0.4, 0.4, 0.4, 1.0};
    GLfloat materialDiffuse[] = {0.7, 0.7, 0.7, 1.0};
    GLfloat materialSpecular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess = 50.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void drawStreetLamp() {
    glPushMatrix();
    glTranslatef(60.0, 20.0, 30.0);
    glColor3f(0.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCube(2.0);
    glutWireCube(2.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(60.0, 22.0, 30.0);
    glColor3f(0.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCube(2.0);
    glutWireCube(2.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(60.0, 24.0, 30.0);
    glColor3f(0.0, 0.0, 0.0);
    setDullMaterialProperties();
    glutSolidCube(2.0);
    glutWireCube(2.0);
    glPopMatrix();
    if (!DayOn) {
        glPushMatrix();
        glTranslatef(60.0, 28.0, 30.0);
        glColor3f(1.0, 1, 0.0);
        setDullMaterialProperties();
        glutSolidSphere(4, 10, 10);
        glPopMatrix();
    }
    if (DayOn) {
        glPushMatrix();
        glTranslatef(60.0, 28.0, 30.0);
        glColor3f(1.0, 1.0, 1.0);
        setDullMaterialProperties();
        glutWireSphere(4, 10, 10);
        glPopMatrix();
    }
    
}
void resetCoin(int coinIndex) {
    srand(40);

    float xOffset = rand() % 300 - 150;
    float zOffset = rand() % 40 + 100;

    coinPositions[coinIndex][0] = xOffset;
    coinPositions[coinIndex][1] = 25.0;
    coinPositions[coinIndex][2] = zOffset;
}
bool checkCollisionRiver(float robotX, float robotZ) {
    
    if ((robotX >= -150.0 && robotX <= -2.0) || (robotX >= 18.0 && robotX <= 150.0)) {
        if (robotZ >= 70.0 && robotZ <= 90.0) {
            return true;  
            
        }
    }
    return false; 
    
}




bool checkCollisionCone(float coneX, float coneZ) {
    float distance = sqrt(pow(robotX - coneX, 2) + pow(robotZ - coneZ, 2));
    if (distance < 5.0) {
        //ı created that function to check if the robot reaches a cone or not
       
        return true;
    }
    return false;
}

bool checkCollision(float coinX, float coinZ) {
    float distance = sqrt(pow(robotX - coinX, 2) + pow(robotZ - coinZ, 2));
    if (distance < 5.0) {
        //I created that function to check collision in between coin and robot
        resetCoin(findCoinIndex(coinX, coinZ));
        collectedCoins++;
        return true;
    }
    return false;
}

bool checkCollisionBridge(float robotX, float robotZ, int collectedCoins) {
    //check collision in between robot and bridge with respect to the collectedCoins
    if (robotX >= -2.0 && robotX <= 18.0) {
        
        if (collectedCoins == 0 && robotZ > 70.0 && robotZ < 90.0) {
            return true;
        } else if (collectedCoins == 1 && robotZ > 70.0 && robotZ < 86.0) {
            return true;
        } else if (collectedCoins == 2 && robotZ > 70.0 && robotZ < 82.0) {
            return true;
        } else if (collectedCoins == 3 && robotZ > 70.0 && robotZ < 78.0) {
            return true;
        } else if (collectedCoins == 4 && robotZ > 70.0 && robotZ < 74.0) {
            return true;
        }
    }
    return false;
}



int findCoinIndex(float coinX, float coinZ) {
    for (int i = 0; i < 5; ++i) {
        if (coinPositions[i][0] == coinX && coinPositions[i][2] == coinZ) {
            return i;
        }
    }
    return -1; 
}


void drawCoin() {
    for (int i = 0; i < 5; ++i) {
        if (coinPositions[i][0] != -1000.0) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 0.0);

            glTranslatef(coinPositions[i][0], 25.0, coinPositions[i][2]);
            setMaterialProperties1();
            glutSolidSphere(3.0, 10, 10);
            glPopMatrix();
        }
    }
}

void drawWin(){

    glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 100.0; y -= 4.0) {          //w ilk cubuk
        glPushMatrix();
        glTranslatef(-40.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
    
    glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 100.0; y -= 4.0) {          //w son cubuk
        glPushMatrix();
        glTranslatef(-10.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
    
    glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 100.0; y -= 4.0) {          //i cubuk
        glPushMatrix();
        glTranslatef( 10.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
    
    
    	glPushMatrix();
    	glTranslatef(-40.0, 100, 0.0);
	glRotatef(-30.0, 0.0, 0.0, 1.0); 
	glTranslatef(40.0, -100, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for (float y = 128.0; y >= 100.0; y -= 4.0) {     // w ilk rotate
    	glPushMatrix();
    	glTranslatef(-40.0, y, 0.0);
    	setDullMaterialProperties();
    	glutSolidCube(4.0);
    	glPopMatrix();
	}
	glPopMatrix(); 
	
	
	glPushMatrix();
    	glTranslatef(-10.0, 100, 0.0);
	glRotatef(30.0, 0.0, 0.0, 1.0); 
	glTranslatef(10.0, -100, 0.0);
	glColor3f(1.0, 1.0, 1.0);                            //w 2.rotate
	for (float y = 128.0; y >= 100.0; y -= 4.0) {
    	glPushMatrix();
    	glTranslatef(-10.0, y, 0.0);
    	setDullMaterialProperties();
    	glutSolidCube(4.0);
    	glPopMatrix();
	}
	glPopMatrix(); 
	
	
	
	
	glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 100.0; y -= 4.0) {          //n cubuk
        glPushMatrix();
        glTranslatef( 30.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
	
	glPushMatrix();
    	glTranslatef(45.0, 100, 0.0);
	glRotatef(30.0, 0.0, 0.0, 1.0); 
	glTranslatef(-45.0, -100, 0.0);
	glColor3f(1.0, 1.0, 1.0);                            //n rotate
	for (float y = 128.0; y >= 100.0; y -= 4.0) {
    	glPushMatrix();
    	glTranslatef(45.0, y, 0.0);
    	setDullMaterialProperties();
    	glutSolidCube(4.0);
    	glPopMatrix();
	}
	glPopMatrix(); 
	
	
	glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 100.0; y -= 4.0) {          //n son cubuk
        glPushMatrix();
        glTranslatef( 45.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
    
    glColor3f(1.0, 1.0, 1.0); 
    for (float y = 128.0; y >= 106.0; y -= 4.0) {          //unlem 
        glPushMatrix();
        glTranslatef( 65.0, y, 0.0);
        setDullMaterialProperties();
        glutSolidCube(4.0);
       
        glPopMatrix();
    }
    
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(65.0,100,0);
    setDullMaterialProperties();
    glutSolidCube(4.0);
    glPopMatrix();
	
	


}
bool animationInProgress = false;
int animationStep = 0;

void animateRobotConePosition(int value) {
    
    if ((robotY < 70)||(coneY < 65)) {
        robotY += 1;
        coneY +=1;
       
        glutPostRedisplay();  
        glutTimerFunc(25, animateRobotConePosition, 0); 
    } 
    else if ((robotX < 10)||(coneX < 10)) {
        robotX+= 1;
        coneX+=1;
         
        glutPostRedisplay();  
        glutTimerFunc(25, animateRobotConePosition, 0);  
    }
    else if ((robotZ >0)||(coneZ >0)) {
        robotZ -= 1;
        coneZ -=1;
         
        glutPostRedisplay();  
        glutTimerFunc(25, animateRobotConePosition, 0);  
    }
    
    else {
       
        animationInProgress = false;
    }
}






void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glLoadIdentity();
    

    gluLookAt(cam_x, cam_y, cam_z, cam_x + lx, cam_y, cam_z + lz, 0.0, 1.0, 0.0);
    GLfloat light_position[] = {cam_x, cam_y, cam_z, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    


    if (DayOn) {
        
        glClearColor(0.5, 0.8, 1.0, 1.0); 
    } else {
        
        glClearColor(0, 0, 0.0, 1.0); 
    }
   
    glColor3f(1.0, 1.0, 1.0);
    drawTexBoundary();
    
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);                                                   //CİMEN
    glVertex3f(-150.0, 20.0, -150.0);
    glVertex3f(-150.0, 20.0, 150.0);
    glVertex3f(150.0, 20.0, 150.0);
    glVertex3f(150.0, 20.0, -150.0);
    glEnd();
    
    glColor3f(0.0, 0.0, 1.0);                                      // göl
    glBegin(GL_QUADS);
    glVertex3f(-150.0, 21.0, 70.0);  
    glVertex3f(-150.0, 21.0, 90.0);  
    glVertex3f(150.0, 21.0, 90.0);   
    glVertex3f(150.0, 21.0, 70.0);   
    glEnd();
    
    
    
    
   
    
    glColor3f(0.5, 0.5, 0.5); 
    for (int i = 0; i < collectedCoins; ++i) {
        glPushMatrix();
        float blockZ = 88.0 - i * 4.0;                             // tas kopru
        glTranslatef(0.0, 20.0, blockZ);
        setMaterialProperties1();
        glutSolidCube(4.0);
        glPopMatrix();
        
        glPushMatrix();                                 
        glTranslatef(4.0, 20.0, blockZ);
        setMaterialProperties1();
        glutSolidCube(4.0);
        glPopMatrix();
        
        glPushMatrix();                               
        glTranslatef(8.0, 20.0, blockZ);
        setMaterialProperties1();
        glutSolidCube(4.0);
        glPopMatrix();
        glPushMatrix();                               
        glTranslatef(12.0, 20.0, blockZ);
        setMaterialProperties1();
        glutSolidCube(4.0);
        glPopMatrix();
        glPushMatrix();                               
        glTranslatef(16.0, 20.0, blockZ);
        setMaterialProperties1();
        glutSolidCube(4.0);
        glPopMatrix();
        
    }

    
    drawCastle();
    drawings();
    drawTrees();
    drawWoods();
    drawSun();
    drawStars();
    drawStreetLamp();
   if (!alreadychecked) {
    if (checkCollisionCone(coneX, coneZ)) {
        alreadychecked = true;
        animationInProgress = true;
        glutTimerFunc(25, animateRobotConePosition, 0);
        
        // robotY = 70;
        // robotX = 10;
        // robotZ = 0;
        //coneX = 10;
        //coneY = 65;
       // coneZ = 0;
        playerwins = true;
    }
}




    
    if (checkCollisionRiver(robotX,robotZ)) {
            
            robotX =10 ; 
            robotY = 30 ;
            robotZ = 150 ;
            
            
           
        }
    if (checkCollisionBridge(robotX,robotZ,collectedCoins))
     {
            robotX =10 ; 
            robotY = 30 ;
            robotZ = 150 ;
    
    }
    

    
    if(playerwins){
    drawWin();
    }
    
    drawRobot();
    for (int i = 0; i < 5; ++i) {
        if (checkCollision(coinPositions[i][0], coinPositions[i][2])) {
            
            coinPositions[i][0] = -1000.0; 
        }
    }
    
    

    drawCoin();
    glFlush();
    
    
    
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, (GLfloat)w / (GLfloat)h, 50.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void kybrd(unsigned char key, int x,int y) {
float fraction = 2.0;
    switch (key) {    
        case 'x':
        case 'X':
            exit(0);  
            break;
        case 'u':
        case 'U':
            cam_y += 5.0;  
            break;
        case 'w':
        case 'W':
            robotZ -= 1.0;  
            break;
        case 'a':
        case 'A':
            robotX -= 1.0;  
            break;
        case 's':
        case 'S':
            robotZ += 1.0;  
            break;
        case 'd':
        case 'D':
            robotX += 1.0;  
            break;
        case '1':
            DayOn=true;  
            break;
        case '0':
            DayOn=false;  
            break;
        case '2':
            glEnable(GL_LIGHTING);  
            break;
        case '3':
            glDisable(GL_LIGHTING);  
            break;
        case 'f':
        case 'F':
            cam_x += lx * fraction; cam_z += lz * fraction;  
            break;
        case 'b':
        case 'B':
            cam_x -= lx * fraction; cam_z -= lz * fraction; 
            break;
        case 'r':
        case 'R':
            cam_yaw += 0.02; lx = sin(cam_yaw); lz = -cos(cam_yaw);  
            break;
        case 'l':
        case 'L':
            cam_yaw -= 0.02; lx = sin(cam_yaw); lz = -cos(cam_yaw);
            break;
            
    }
  }
void specialKeys(int key, int x, int y) {
    float fraction = 2.0; //sağa sola hızlı gitmesi için arttırdım. 
    switch (key) {
        case GLUT_KEY_LEFT:  cam_yaw -= 0.02; lx = sin(cam_yaw); lz = -cos(cam_yaw); break;
        case GLUT_KEY_RIGHT: cam_yaw += 0.02; lx = sin(cam_yaw); lz = -cos(cam_yaw); break;
        case GLUT_KEY_UP:    cam_x += lx * fraction; cam_z += lz * fraction; break;
        case GLUT_KEY_DOWN:  cam_x -= lx * fraction; cam_z -= lz * fraction; break;
       
    }
    glutPostRedisplay();
}

void movetrial(int value) {

    coneRotate += 1.0; 
   // robotX += 0.1;
//REF3: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#:~:text=glutTimerFunc%20registers%20a%20timer%20callback,a%20specified%20numfber%20of%20milliseconds.&text=glutTimerFunc%20registers%20the%20timer%20callback,the%20value%20parameter%20to%20glutTimerFunc.
    glutPostRedisplay();
    
//REF4: https://stackoverflow.com/questions/32197737/glut-animation-with-glutpostredisplay
    glutTimerFunc(25, movetrial, 0); 
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 500);
    glutInitWindowSize(800, 600);
    glutCreateWindow("BugraBaygul_HW2P4");
    glEnable(GL_DEPTH_TEST);

    drawTexture("foto1.jpeg");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(kybrd);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(25, movetrial, 0);
    glutMainLoop();

    return 0;
}

