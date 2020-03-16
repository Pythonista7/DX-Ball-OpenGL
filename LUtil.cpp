#include "LUtil.h"


//"gColorMode" controls whether we render a solid cyan square or a multicolored square.
int gColorMode = COLOR_MODE_MULTI;

//"gProjectionScale" controls how large of a coordinate area we want to render.
GLfloat gProjectionScale = 1.f; 


//lak_ctr-->left_arrow_key_counter 
int lak_ctr=0;
int rak_ctr=0;

//left/right wall
int left_wall=-(SCREEN_WIDTH-50);
int right_wall=SCREEN_WIDTH-50;

//left/right paddle and factor to move
int factor=0,paddle_left=0,paddle_right=0;


bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_WIDTH,SCREEN_WIDTH,SCREEN_HEIGHT,-SCREEN_HEIGHT,1,-1);

    //Initialize Model View Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Initialize clear color
    //first argument is red, the second is green, the third is blue, and the last is alpha.
    glClearColor(0.f,0.f,0.f,1.f);

    //Check for error
    GLenum error=glGetError();

    if(error != GL_NO_ERROR)
    {
        printf("ERROR Initializting OpenGL! %s \n",gluErrorString(error));
        return false;
    }

    return true;
}

void update()
{

}

void render()
{
    //Clear color buffer and set it to the color specified in glClearColor() in initGL()
    glClear(GL_COLOR_BUFFER_BIT);

    //Reset Model View Matrix
    //projection matrix controls how the geometry is viewed
    //modelview matrix tranformations control how geometry is placed in the rendering world.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
   

    //Render Quad
    //if( gColorMode == COLOR_MODE_CYAN )
    //{
        //SCREEN BORDERS
        glBegin(GL_LINE_STRIP);
            glColor3f(0,1,1);
            glLineWidth(5.0f);
            
            glVertex2f( -(SCREEN_WIDTH-50.f) ,  SCREEN_HEIGHT-50.f );
            glVertex2f( -(SCREEN_WIDTH-50) , -(SCREEN_HEIGHT-50) );
            glVertex2f(  SCREEN_WIDTH-50.f , -(SCREEN_HEIGHT-50.f) );
            glVertex2f(  SCREEN_WIDTH-50.f ,  SCREEN_HEIGHT-50.f );
            
            
        glEnd();
        glutSwapBuffers();


        //Moveable Paddle
        
        glBegin(GL_LINE_LOOP);

            glColor3f(1,0,1);
            glLineWidth(5.0f);
        
            //factor by which the paddle should move right or left 
            factor=rak_ctr-lak_ctr;

            //left and right x-coords of the paddle
            paddle_left  = -STEP_SIZE + STEP_SIZE * factor;
            paddle_right =  STEP_SIZE + STEP_SIZE * factor; 


            glVertex2f(  paddle_left  , SCREEN_HEIGHT-50 );
            glVertex2f(  paddle_right , SCREEN_HEIGHT-50);
            glVertex2f(  paddle_right , SCREEN_HEIGHT-50-PADDLE_HEIGHT );
            glVertex2f(  paddle_left  , SCREEN_HEIGHT-50-PADDLE_HEIGHT );

    
        glEnd();
        glutSwapBuffers();



    //}


    glutSwapBuffers();//same as glflush() in single buffer mode

}

void runMainLoop(int val)
{
    //Frame Logic
    update();
    render();

    //Run frame one more time
    glutTimerFunc(1000/SCREEN_FPS ,runMainLoop , val );
}

void handleKeys(unsigned char key,int x ,int y)
{
    
    //if left wall is hit stay there until right arrow is pressed
    if( paddle_left <= -(SCREEN_WIDTH-100))
    {
       if(key=='d')
            rak_ctr++;
    }

    //if right wall is hit stay there until left arrow is pressed
    else if( paddle_right >= SCREEN_WIDTH-100)
    {
        if(key=='a')
            lak_ctr++;
    }

    else
    {   
        //there is enough space on either side so feel free to move 
          
        if(key=='a')
        {
            lak_ctr++;
        }

        if(key=='d')
        {
            rak_ctr++;
        }

    }





}