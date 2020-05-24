#include "LUtil.h"

#define pi 3.142857

//SCORE 1 point everytime you hit
int score = 0;

int arrayX[12][7]; // {-590, -472, -354, -236, -118, 0, 118, 236, 354, 472, 999};
void build_array()
{
    int yCoordinateLocal = yCoordinateGlobal;
    int xCoordinateLocal = xCoordinateGlobal;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            arrayX[i][j] = 1;
        }
    }
    for (int j = 1; j < 7; j++)
    {
        arrayX[0][j] = yCoordinateLocal;
        yCoordinateLocal -= yBrickHeight;
    }
    for (int i = 1; i < 12; i++)
    {
        arrayX[i][0] = xCoordinateLocal;
        xCoordinateLocal += xBrickLength;
    }
    arrayX[0][0] = 0;
}
//int arrayY[] = {-180, -230, -280, -330, -380, 999};

//"gColorMode" controls whether we render a solid cyan square or a multicolored square.
int gColorMode = COLOR_MODE_MULTI;

//"gProjectionScale" controls how large of a coordinate area we want to render.
GLfloat gProjectionScale = 1.f;

//lak_ctr-->left_arrow_key_counter
int lak_ctr = 0;
int rak_ctr = 0;

//left/right wall
int left_wall = -(SCREEN_WIDTH - 50);
int right_wall = SCREEN_WIDTH - 50;

//left/right paddle and factor to move
int factor = 0, paddle_left = 0, paddle_right = 0;

//x and y for ball center
float x = 50, y = SCREEN_HEIGHT - 50 - PADDLE_HEIGHT;
//h and v are factors to hold +ve/-ve signs for movement directions
int h = 1, v = -1;
//to chk if ball is movin up/down or right/left
int flag_down = -1, flag_left = -1; // (h/v)*cor-ord value much give appropriate dir wrt the x/y axes

//util function to generate random number in a given range
int random_number_in_range(int start, int end)
{
    //srand((unsigned) time(0));
    int randomNumber;

    randomNumber = (rand() % end) + start;
    return (randomNumber);
}

//indicating change in x and y directions
//Dictates ball speed
//float delta_y_ = 0.025 * random_number_in_range(0, 3);
//float delta_x_ = 0.025 * random_number_in_range(0, 3);
float delta_x_ = 0.05 + 0.01 * random_number_in_range(1, 3), delta_y_ = 0.1 + 0.01 * random_number_in_range(1, 3);

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT, -SCREEN_HEIGHT, 1, -1);

    //Initialize Model View Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Initialize clear color
    //first argument is red, the second is green, the third is blue, and the last is alpha.
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //Check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        printf("ERROR Initializting OpenGL! %s \n", gluErrorString(error));
        return false;
    }

    return true;
}

void DrawWalls()
{
    //SCREEN BORDERS
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 1);
    glLineWidth(5.0f);

    glVertex2f(-(SCREEN_WIDTH - 50.f), SCREEN_HEIGHT - 50.f);
    glVertex2f(-(SCREEN_WIDTH - 50), -(SCREEN_HEIGHT - 50));
    glVertex2f(SCREEN_WIDTH - 50.f, -(SCREEN_HEIGHT - 50.f));
    glVertex2f(SCREEN_WIDTH - 50.f, SCREEN_HEIGHT - 50.f);

    glEnd();
}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
    glColor3f(0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle
        float x = r * cosf(theta);                                         //calculate the x component
        float y = r * sinf(theta);                                         //calculate the y component
        glVertex2f(x + cx, y + cy);                                        //output vertex
    }
    glEnd();
}

void DrawPaddle()
{
    //Paddle
    glBegin(GL_POLYGON);

    glColor3f(1, 0, 1);
    glLineWidth(5.0f);

    //factor by which the paddle should move right or left
    factor = rak_ctr - lak_ctr;

    //left and right x-coords of the paddle
    paddle_left = -STEP_SIZE + STEP_SIZE * factor;
    paddle_right = STEP_SIZE + STEP_SIZE * factor;

    glVertex2f(paddle_left, SCREEN_HEIGHT - 50);
    glVertex2f(paddle_right, SCREEN_HEIGHT - 50);
    glVertex2f(paddle_right, SCREEN_HEIGHT - 50 - PADDLE_HEIGHT);
    glVertex2f(paddle_left, SCREEN_HEIGHT - 50 - PADDLE_HEIGHT);

    glEnd();
}

void collisionDetection()
{
    if (y < -180)
    {
        for (int i = 1; i < 11; i++)
        {
            for (int j = 1; j < 6; j++)
                if (y > arrayX[0][j + 1] && y < arrayX[0][j] && x < arrayX[i + 1][0] && x > arrayX[i][0])
                {
                    if (arrayX[i][j] != 0)
                    {
                        score++;
                        v = -1 * v;
                        flag_down = -1*flag_down;
                        //flag_left = -1*random_number_in_range(0,1)*flag_left;
                    }
                    arrayX[i][j] = 0;
                    //reflect_ball_logic
                    // v = -1;
                    // DrawBricks();
                    // glutPostRedisplay();
                    //re-render screen
                    // for (int i = 0; i < 11; i++)
                    // {
                    //     for (int j = 0; j < 6; j++)
                    //     {
                    //         printf("%d\t", arrayX[i][j]);
                    //     }
                    //     printf("\n");
                    // }
                }
        }
    }

    //BOUNDRY CONDITIONS
    //CHECK RIGHT WALL IMPACT
    if (x >= right_wall && flag_left == -1)
    {
        flag_left = 1;
        h = -1;
    }
    //CHECK LEFT WALL IMPACT
    if (x <= left_wall && flag_left == 1)
    {
        flag_left = -1;
        h = 1;
    }

    //CHECK TOP WALL IMPACT
    if (y <= -(SCREEN_HEIGHT - 50) && flag_down == -1) //0
    {
        flag_down = 1;
        v = 1;
    }

    //CHECK BOTTOME PADDLE IMPACT
    if ((x > paddle_left && x < paddle_right) && y >= SCREEN_HEIGHT - 50 - PADDLE_HEIGHT && flag_down == 1)
    {
        // printf("%d",random_number_in_range(0,3));
        if (score % 3 == 0)
        {
            delta_y_ += 0.025 * random_number_in_range(0, 3);
            delta_x_ += 0.025 * random_number_in_range(0, 3);
        }
        //printf("delta_y=%d \t",delta_y_);
        //printf("delta_x=%d \t HIT\n",delta_x_);
        score++;
        flag_down = -1;
        v = -1;
    }
}

void DrawBricks()
{
    //logic to render bricks here , add logic to update bricks below the function call in update()

    glColor3f(0.4, 0.9, 0.5);
    glLineWidth(5.0f);
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 6; j++)
        {
            if (arrayX[i][j] != 0)
            {

                glRectd(arrayX[i][0], arrayX[0][j] - (yBrickHeight - 1), arrayX[i][0] + (xBrickLength - 1), arrayX[0][j]); //-1 both places to show borders

                // glBegin(GL_QUADS);

                // glVertex2f(arrayX[i][0], arrayX[0][j]);
                // glVertex2f(arrayX[i][0] + 117, arrayX[0][j]);
                // glVertex2f(arrayX[i][0] + 117, arrayX[0][j] - 49);
                // glVertex2f(arrayX[i][0], arrayX[0][j] - 49);

                // glEnd();
            }
        }
    }
}

void update()
{
    //Testing
    DrawCircle(x, y, BALL_RADII, 8);
    DrawWalls();
    DrawPaddle();

    //Adding randomness to the ball movement speed

    x = x + float(delta_x_ * h);
    y = y + float(delta_y_ * v);

    int i = 0;

    //Mainly for debugging
    //printf("flag_down = %d\n",flag_down);
    //if(y>SCREEN_HEIGHT-200)
    //{
    //    printf("%d  \t %d  \t %d\n",(x > paddle_left && x < paddle_right),y==SCREEN_HEIGHT-50-PADDLE_HEIGHT,flag_down==1 );
    //    printf("y = %d \t paddle_y= %d \n",y,SCREEN_HEIGHT-50-PADDLE_HEIGHT);
    //}
    //PADDLE IMPACT CHECK
    //x < paddle_left && x > paddle_right &&

    collisionDetection();

    DrawBricks();

    glutPostRedisplay();

    if (y > SCREEN_HEIGHT) //condition when ball wont be within the reach of the paddle
    {
        //printf("x=%d  \t  paddle_left=%d \t paddle_right=%d\n",x,paddle_left,paddle_right);
        printf("GAME OVER\n");
        printf("SCORE : %d\n", score);
        exit(0);
    }
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

    //drawing all game objects
    DrawCircle(x, y, BALL_RADII, 8);
    DrawWalls();
    DrawPaddle();
    DrawBricks();
    glFlush();
}

void handleKeys(unsigned char key, int x, int y)
{

    //if left wall is hit stay there until right arrow is pressed
    if (paddle_left <= -(SCREEN_WIDTH - 100))
    {
        if (key == 'd')
            rak_ctr++;
    }

    //if right wall is hit stay there until left arrow is pressed
    else if (paddle_right >= SCREEN_WIDTH - 100)
    {
        if (key == 'a')
            lak_ctr++;
    }

    else
    {
        //there is enough space on either side so feel free to move

        if (key == 'a')
        {
            lak_ctr++;
        }

        if (key == 'd')
        {
            rak_ctr++;
        }
    }
}

void runMainLoop(int val)
{

    //Frame Logic
    if (val == 0)
    {
        build_array();
        val = 1;
    }
    render();
    update();
    glutIdleFunc(update);

    //Run frame one more time
    //glutTimerFunc(25,runMainLoop,val);//1000/SCREEN_FPS ,runMainLoop , val );
}