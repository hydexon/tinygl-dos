/* sdlGears.c */
/*
 * 3-D gear wheels by Brian Paul. This program is in the public domain.
 *
 * ported to libSDL/TinyGL by Gerald Franz (gfz@o2online.de)
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
//#include <SDL.h>
#include <zbuffer.h>
#include <assert.h>

#include "allegro.h"

#ifndef M_PI
#  define M_PI 3.14159265
#endif

volatile unsigned int ticks = 0;

 int palette[ZB_NB_COLORS];
unsigned char color_indexes[ZB_NB_COLORS];

void timer_function()
{
    ticks++;
}
END_OF_FUNCTION(timer_function)


/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 *         outer_radius - radius at center of teeth
 *         width - width of gear
 *         teeth - number of teeth
 *         tooth_depth - depth of tooth
 */
static void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                 GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2.0;
    r2 = outer_radius + tooth_depth/2.0;

    da = 2.0*M_PI / teeth / 4.0;

    glShadeModel( GL_FLAT );

    glNormal3f( 0.0, 0.0, 1.0 );

    /* draw front face */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
        angle = i * 2.0*M_PI / teeth;
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();

    /* draw front sides of teeth */
    glBegin( GL_QUADS );
    da = 2.0*M_PI / teeth / 4.0;
    for (i=0;i<teeth;i++) {
        angle = i * 2.0*M_PI / teeth;

        glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();

    glNormal3f( 0.0, 0.0, -1.0 );

    /* draw back face */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
        angle = i * 2.0*M_PI / teeth;
        glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    }
    glEnd();

    /* draw back sides of teeth */
    glBegin( GL_QUADS );
    da = 2.0*M_PI / teeth / 4.0;
    for (i=0;i<teeth;i++) {
        angle = i * 2.0*M_PI / teeth;

        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
        glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
    }
    glEnd();

    /* draw outward faces of teeth */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<teeth;i++) {
        angle = i * 2.0*M_PI / teeth;

        glVertex3f( r1*cos(angle),      r1*sin(angle),       width*0.5 );
        glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
        u = r2*cos(angle+da) - r1*cos(angle);
        v = r2*sin(angle+da) - r1*sin(angle);
        len = sqrt( u*u + v*v );
        u /= len;
        v /= len;
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),    width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
        v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
    }

    glVertex3f( r1*cos(0), r1*sin(0), width*0.5 );
    glVertex3f( r1*cos(0), r1*sin(0), -width*0.5 );

    glEnd();


    glShadeModel( GL_SMOOTH );

    /* draw inside radius cylinder */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
        angle = i * 2.0*M_PI / teeth;
        glNormal3f( -cos(angle), -sin(angle), 0.0 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    }
    glEnd();

}


static GLfloat view_rotx=20.0, view_roty=30.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;

void draw() {
    angle += 2.0;
    glPushMatrix();
    glRotatef( view_rotx, 1.0, 0.0, 0.0 );
    glRotatef( view_roty, 0.0, 1.0, 0.0 );
    //glRotatef( view_rotz, 0.0, 0.0, 1.0 );

    glPushMatrix();
    glTranslatef( -3.0, -2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, 1.0 );
    glCallList(gear1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 3.1, -2.0, 0.0 );
    glRotatef( -2.0*angle-9.0, 0.0, 0.0, 1.0 );
    glCallList(gear2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -3.1, 4.2, 0.0 );
    glRotatef( -2.0*angle-25.0, 0.0, 0.0, 1.0 );
    glCallList(gear3);
    glPopMatrix();

    glPopMatrix();
}


void initScene() {
    static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0 };
    static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0 };
    static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0 };
    static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, pos );
    glEnable( GL_CULL_FACE );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_DEPTH_TEST );

    /* make the gears */
    gear1 = glGenLists(1);
    glNewList(gear1, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red );
    gear( 1.0, 4.0, 1.0, 20, 0.7 );
    glEndList();

    gear2 = glGenLists(1);
    glNewList(gear2, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green );
    gear( 0.5, 2.0, 2.0, 10, 0.7 );
    glEndList();

    gear3 = glGenLists(1);
    glNewList(gear3, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue );
    gear( 1.3, 2.0, 0.5, 10, 0.7 );
    glEndList();
    glEnable( GL_NORMALIZE );
}

PALETTE GL_Palette;

int main(int argc, char **argv) {
    // initialize SDL video:
    int winSizeX=320;
    int winSizeY=200;

   //set_uformat(U_ASCII_CP);
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

   BITMAP* buffer = NULL;
   set_color_depth(8);
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   //allegro_message("host color depth: %d", desktop_color_depth());

   int gfx_result =set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   //allegro_message("What?: %s", allegro_error);
    #if 0
   if (set_gfx_mode(GFX_VGA, 320, 200, 0, 0) != 0) {
      if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
        return 1;
      }
   }
   #endif


   for(int i = 0; i < ZB_NB_COLORS; i++)
   {
       color_indexes[i] = i;
   }

   clear_bitmap(screen);
    // initialize TinyGL:

    ZBuffer *frameBuffer = ZB_open( 320, 200, ZB_MODE_INDEX, ZB_NB_COLORS , color_indexes, palette, 0);
    set_palette(desktop_palette);

    glInit( frameBuffer );

    // initialize GL:
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glViewport (0, 0, 320, 200);
    glEnable(GL_DEPTH_TEST);
    GLfloat  h = (GLfloat) 320.0f / (GLfloat) 200.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -h, h, 5.0, 60.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, -45.0 );

    initScene();

    // variables for timing:
    unsigned int frames=0;
    unsigned int tNow=ticks;
    unsigned int tLastFps=tNow;

    // main loop:
    int isRunning=1;
    
    int c = 0;
    
    while(isRunning) {
        //allegro_message(".\n");
        ++frames;
        tNow=ticks;
        
        if(key[KEY_UP]) 
            view_rotx += 5.0;
        else if(key[KEY_DOWN]) 
            view_rotx -= 5.0;
        else if(key[KEY_LEFT])
            view_roty += 5.0;
        else if(key[KEY_RIGHT])
            view_roty -= 5.0;
        else if(key[KEY_1_PAD])
            fade_from(default_palette, desktop_palette,16);
        else if(key[KEY_2_PAD])
            fade_from(desktop_palette, default_palette,16);
        else if(key[KEY_Q] || key[KEY_ESC])
            isRunning = 0;
        else {}
        // draw scene:
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   
        draw();

        // swap buffers:
        ZB_copyFrameBuffer(frameBuffer, screen->line[0], screen->w*2);

        //blit(buffer, screen, 0,0,0,0, 320,200);

        // check for error conditions:

        //update fps:
        if(tNow>=tLastFps+5000) {
            //allegro_message("%i frames in %f secs, %f frames per second.\n",frames,(float)(tNow-tLastFps)*0.001f,(float)frames*1000.0f/(float)(tNow-tLastFps));
            tLastFps=tNow;
            frames=0;
        }
    }
    
    //allegro_message("%i frames in %f secs, %f frames per second.\n",frames,(float)(tNow-tLastFps)*0.001f,(float)frames*1000.0f/(float)(tNow-tLastFps));
    // cleanup:
    allegro_message("i am exiting\n");
    ZB_close(frameBuffer);
    allegro_message("ZB_Close freed framebuffer");
    allegro_exit();
    return 0;
}
END_OF_MAIN();