// adapted from GLFW, glfw.sf.net
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <zbuffer.h>

#include <allegro.h>

int palette[ZB_NB_COLORS];
unsigned char color_indexes[ZB_NB_COLORS];
PALETTE GL_Palette;
unsigned int ticks =0;

void timer_function()
{
    ticks++;
}
END_OF_FUNCTION(timer_function)

char fps_text[16];

int main(int argc, char **argv) {
    // initialize SDL video:
    int winSizeX=320;
    int winSizeY=200;
   srand(time(NULL));

    if(allegro_init() != 0) {
        allegro_message("ERROR: cannot initialize Allegro\n");
        return 1;
    }
   install_keyboard();
   install_timer();

   LOCK_FUNCTION(timer_function);
   LOCK_VARIABLE(ticks);
   install_int_ex(timer_function, BPS_TO_TIMER(1000));
    set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
    // initialize TinyGL:
    /*
    unsigned int pitch;
    int	mode;
    switch( screen->format->BitsPerPixel ) {
    case  8:
        fprintf(stderr,"ERROR: Palettes are currently not supported.\n");
        return 1;
    case 16:
        pitch = screen->pitch;
        mode = ZB_MODE_5R6G5B;
        break;
    case 24:
        pitch = ( screen->pitch * 2 ) / 3;
        mode = ZB_MODE_RGB24;
        break;
    case 32:
        pitch = screen->pitch / 2;
        mode = ZB_MODE_RGBA;
        break;
    default:
        return 1;
        break;
    }*/
   for(int i = 0; i < ZB_NB_COLORS; i++)
   {
       color_indexes[i] = i;
   }

    ZBuffer *frameBuffer = ZB_open( winSizeX, winSizeY, ZB_MODE_INDEX, ZB_NB_COLORS,color_indexes, palette, 0);
    glInit( frameBuffer );

    // set viewport
    glViewport( 0, 0, winSizeX, winSizeY);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // main loop:
    int frames=0;
    int x=0;
    float  t, t0, fps = 0;
    char    titlestr[ 200 ];
    int running = GL_TRUE;
    t0 = (float)ticks/1000.0;
    while( running ) {
        // calculate and display FPS (frames per second):
        t = (float)ticks/1000.0;
        if( (t-t0) > 1.0 || frames == 0 ) {
            fps = (float)frames / (t-t0);
            t0 = t;
            frames = 0;
        }
        ++frames;

        // Clear color buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Select and setup the projection matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 65.0f, (GLfloat)winSizeX/(GLfloat)winSizeY, 1.0f, 100.0f );

        // Select and setup the modelview matrix
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glRotatef(-90, 1,0,0);
        glTranslatef(0,0,-1.0f);

        // Draw a rotating colorful triangle
        glTranslatef( 0.0f, 14.0f, 0.0f );
        glRotatef( 0.3*(GLfloat)x + (GLfloat)t*100.0f, 0.0f, 0.0f, 1.0f );
        glBegin( GL_TRIANGLES );
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( -5.0f, 0.0f, -4.0f );
        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( 5.0f, 0.0f, -4.0f );
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex3f( 0.0f, 0.0f, 6.0f );
        glEnd();

        // swap buffers:
        ZB_copyFrameBuffer(frameBuffer, screen->line[0], screen->w*2);
        // check if the ESC key was pressed or the window was closed:
        if(key[KEY_Q] || key[KEY_ESC])
            running = 0;
    }
    // cleanup:
    ZB_close(frameBuffer);
    allegro_exit();
    //float avgfps = (float)(totalfps/(float)fpscount);
    return 0;
}
END_OF_MAIN();