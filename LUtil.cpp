#include "LUtil.h"

#define pi 3.142857 

//SCORE 1 point everytime you hit 
int score=0;

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



//x and y for ball center
float x=50,y=SCREEN_HEIGHT-50-PADDLE_HEIGHT ;
//h and v are factors to hold +ve/-ve signs for movement directions
int h=1,v=-1;
//to chk if ball is movin up/down or right/left
int flag_down=-1,flag_left=-1; // (h/v)*cor-ord value much give appropriate dir wrt the x/y axes

//util function to generate random number in a given range
int random_number_in_range(int start,int end) 
{
  //srand((unsigned) time(0));
  int randomNumber;
  
  randomNumber = (rand() % end) + start;
  return(randomNumber);
}


//indicating change in x and y directions
//Dictates ball speed
float delta_x_= 10+random_number_in_range(0,5),delta_y_= 15+random_number_in_range(0,5);


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

void DrawCircle(float cx, float cy, float r, int num_segments) {
    glColor3f(0,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
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


        //Game Physics - Bricks



        //Moveable Paddle
        
        glBegin(GL_POLYGON);

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


       

        
        //Game Physics - Ball

        //speeding up the ball
       
        DrawCircle(x,y,BALL_RADII,8);
        //Adding randomness to the ball movement speed
        
        x=x+float(delta_x_ * h) ;
        y=y+float(delta_y_ * v) ;

        

      
            int i=0;
            
            //BOUNDRY CONDITIONS

            //CHECK RIGHT WALL IMPACT 
            if(x>=right_wall && flag_left==-1)
            {
                flag_left=1;
                h=-1;
             
            }
            //CHECK LEFT WALL IMPACT
            if(x<=left_wall && flag_left==1) 
            {
                flag_left=-1;
                h=1;
                
            }

            //CHECK TOP WALL IMPACT
            if(y<=-(SCREEN_HEIGHT-50) && flag_down==-1)//0
            {
                flag_down=1;
                v=1;
               
            }


            //CHECK BOTTOME PADDLE IMPACT
            if( (x > paddle_left && x < paddle_right) && y>=SCREEN_HEIGHT-50-PADDLE_HEIGHT && flag_down==1 )
            {   
                // printf("%d",random_number_in_range(0,3));
                delta_y_+=random_number_in_range(0,3);
                delta_x_+=random_number_in_range(0,3);
                //printf("delta_y=%d \t",delta_y_);
                //printf("delta_x=%d \t HIT\n",delta_x_);
                score++;
                flag_down=-1;
                v=-1;
            }

            //Mainly for debugging
            //printf("flag_down = %d\n",flag_down);
            //if(y>SCREEN_HEIGHT-200)
            //{
            //    printf("%d  \t %d  \t %d\n",(x > paddle_left && x < paddle_right),y==SCREEN_HEIGHT-50-PADDLE_HEIGHT,flag_down==1 );
            //    printf("y = %d \t paddle_y= %d \n",y,SCREEN_HEIGHT-50-PADDLE_HEIGHT);
            //}
            //PADDLE IMPACT CHECK
            //x < paddle_left && x > paddle_right &&

            if( y>SCREEN_HEIGHT)//condition when ball wont be within the reach of the paddle
            {
               //printf("x=%d  \t  paddle_left=%d \t paddle_right=%d\n",x,paddle_left,paddle_right);
               printf("GAME OVER\n");
               printf("SCORE : %d\n",score);
               exit(0);
            }

            
 
        


    glutSwapBuffers(); //same as glflush() in single buffer mode

}

void runMainLoop(int val)
{
  
    //Frame Logic
    update();
    render();
    
    //Run frame one more time
    glutTimerFunc(10,runMainLoop,val);//1000/SCREEN_FPS ,runMainLoop , val );
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