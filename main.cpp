#include "LUtil.h"

void runMainLoop(int val);
/*
Pre Condition:
 -Initialized freeGLUT
Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
Side Effects:
 -Sets glutTimerFunc
*/
int game_state = 0;

int main( int argc , char* args[] )
{
    //Initialize FreeGLUT
    glutInit(&argc,args);

    
    /* 
    In this application, we're going to use a double buffered window.
    What this means is that we have two buffers, a front and back buffer. 
    The front buffer is what the user sees. The back buffer is in memory. 
    When we make rendering calls in a double buffered window, they are rendered to the back buffer. 
    After we're done rendering what we want to show to the user,we swap the front buffer with 
    the back buffer so the user sees what we originally rendered to the back buffer.
    Now in a single buffered window, everything is draw directly to the front buffer.
    This means the user will see geometry as it's being rendered, which means we may see unfinished 
    rendering and tearing.
    */

    glutInitDisplayMode( GLUT_SINGLE );

    glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    glutCreateWindow("Breakout");

    if(!initGL())
    {
        printf("Unable to initalize graphics library!\n");
        return 1;
    }


    //Set keyboard handler
    glutKeyboardFunc(handleKeys);

    //Set rendering function
    // if(game_state == 1)
    //     {
    //         glutDisplayFunc(render);
    //         glutIdleFunc(update);
    //     }
    // if(game_state==0)
    glutDisplayFunc(welcomeDisplay);
        
    //Setting up Idle func to update
    

    //Set main loop
    runMainLoop(game_state);

    //glutTimerFunc(5,runMainLoop,0);

    //Start GLUT main loop
    glutMainLoop();//This runs freeGLUT's internal main loop not our main loop

    return 0;
}