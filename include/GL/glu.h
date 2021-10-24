#ifndef M_PI
#  define M_PI 3.14159265
#endif

void gluPerspective( GLfloat fovy, GLfloat aspect,
		     GLfloat zNear, GLfloat zFar );

typedef struct {
  int draw_style;
} GLUquadricObj;

#define GLU_LINE 0

GLUquadricObj *gluNewQuadric(void);
void gluQuadricDrawStyle(GLUquadricObj *obj, int style);

void gluSphere(GLUquadricObj *qobj,
               float radius,int slices,int stacks);
void gluCylinder( GLUquadricObj *qobj,
                  GLfloat baseRadius, GLfloat topRadius, GLfloat height,
                  GLint slices, GLint stacks );
void gluDisk( GLUquadricObj *qobj,
              GLfloat innerRadius, GLfloat outerRadius,
              GLint slices, GLint loops );

void drawTorus(float rc, int numc, float rt, int numt);
