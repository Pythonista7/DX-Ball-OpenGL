#include "LUtil.h"

float startx = -(640.0 - 60.0);
float starty = 140.0;

void runMainLoop(int val);
/*
Pre Condition:
 -Initialized freeGLUT
Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
Side Effects:NULL;
 -Sets glutTimerFunc
*/
//linked List Part
X createNodeX()
{
    X temp = NULL;                         // declare a node
    temp = (X)malloc(sizeof(struct Xval)); // allocate memory using malloc()
    temp->next = NULL;                     // make next point to NULL
    return temp;                           //return the new node
}
Y createNodeY()
{
    Y temp = NULL;                         // declare a node
    temp = (Y)malloc(sizeof(struct Yval)); // allocate memory using malloc()
    temp->next = NULL;                     // make next point to NULL
    return temp;                           //return the new node
}

X addNodeX(X head, int value)
{
    X temp, p;            // declare two nodes temp and p
    temp = createNodeX(); //createNode will return a new node with data = value and next pointing to NULL.
    temp->data = value;   // add element's value to data part of node
    if (head == NULL)
    {
        head = temp; //when linked list is empty
    }
    else
    {
        p = head; //assign head to p
        while (p->next != NULL)
        {
            p = p->next; //traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp; //Point the previous last node to the new node created.
    }
    return head;
}
Y addNodeY(Y head, int value)
{
    Y temp, p;            // declare two nodes temp and p
    temp = createNodeY(); //createNode will return a new node with data = value and next pointing to NULL.
    temp->data = value;   // add element's value to data part of node
    if (head == NULL)
    {
        head = temp; //when linked list is empty
    }
    else
    {
        p = head; //assign head to p
        while (p->next != NULL)
        {
            p = p->next; //traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp; //Point the previous last node to the new node created.
    }
    return head;
}
//Linked List ends here

int main(int argc, char *args[])
{
    //figuring last line
    while (startx != 580.0)
    {
        addNodeX(xAxisHead, startx);
        addNodeY(yAxisHead, starty);
        startx += 232;
    }
    //Initialize FreeGLUT
    glutInit(&argc, args);

    //Create OpenGL 2.1 context <OPTIONAL> {comment out if error pops up}
    glutInitContextVersion(2, 1);

    /* 
    In this application, we're going to use a double buffered window.
    What this means is that we have two buffers, a front and back buffer. 
    The front buffer is what the user sees. The back buffer is in memory. 
    When we make rendering calls in a double buffered window, they are rendered to the back buffer. 
    After we're done rendering what we want to show to the user,we swap the front buffer with 
    the back buffer so the user sees what we originally rendered to the back buffer.
    Now in a single buffered window, everything is draw directly to the front buffer.
    This means the user will see geometry as it's being rendered, which means they may see unfinished 
    rendering and tearing.
    From main.cpp
    */
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Ashwin.M.S");

    if (!initGL())
    {
        printf("Unable to initalize graphics library!\n");
        return 1;
    }

    //Set keyboard handler
    glutKeyboardFunc(handleKeys);

    //Set rendering function
    glutDisplayFunc(render);

    //Set main loop
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

    //Start GLUT main loop
    glutMainLoop(); //This runs freeGLUT's internal main loop not our main loop

    return 0;
}