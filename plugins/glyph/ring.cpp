#include <cmath>

#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>

#include <tulip/Graph.h>
#include <tulip/GlGraph.h>

using namespace std;
using namespace tlp;

class Ring : public Glyph {
public:
  Ring(GlyphContext *gc=NULL);
  virtual ~Ring();
  virtual string getName() {return string("Ring");}
  virtual void draw(node n);

protected:
  GLuint LList;
  bool listOk;
  void drawRing();
  void drawRingBorder();
};
//=====================================================
GLYPHPLUGIN(Ring, "2D - Ring", "David Auber", "09/07/2002", "Textured Ring", "1", "1", 15);
//===================================================================================
Ring::Ring(GlyphContext *gc): Glyph(gc),listOk(false) {
}
//=====================================================
Ring::~Ring() {
  if (listOk)
    if (glIsList(LList)) glDeleteLists(LList, 2);
}
//=====================================================
void Ring::draw(node n) {
  setMaterial(glGraph->elementColor->getNodeValue(n));
  string texFile = glGraph->elementTexture->getNodeValue(n);
  if (texFile != "") {
    if (glGraph->activateTexture(texFile))
      setMaterial(Color(255,255,255,0));
  }
  if (!listOk) {
    LList = glGenLists( 2 );
    glNewList( LList, GL_COMPILE ); 
    drawRing();
    glEndList();
    glNewList( LList + 1, GL_COMPILE ); 
    drawRingBorder();
    glEndList();
    listOk=true;
  }
  glCallList(LList);
  ColorProperty *border = glGraph->getGraph()->getProperty<ColorProperty>("viewBorderColor");
  glGraph->desactivateTexture();
  Color c = border->getNodeValue(n);
  //  setMaterial(c);
  glLineWidth(2);
  glDisable(GL_LIGHTING);
  glColor4ub(c[0],c[1],c[2],c[3]);
  glCallList(LList + 1);
  glEnable(GL_LIGHTING);
}
//=====================================================
void Ring::drawRing() {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);  
  gluQuadricOrientation(quadratic, GLU_OUTSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluQuadricOrientation(quadratic, GLU_INSIDE);
  gluDisk(quadratic, 0.2f, 0.5f, 30, 1);
  gluDeleteQuadric(quadratic);
}

void Ring::drawRingBorder() {
  glBegin(GL_LINE_LOOP);
  double alpha = M_PI / 2.;
  double delta = 2. * M_PI/30.0;
  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.5*cos(alpha), 0.5 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
  glBegin(GL_LINE_LOOP);
  alpha = M_PI / 2.;
  for (unsigned int i = 0; i < 30; ++i) {
    glVertex3f(0.2*cos(alpha), 0.2 * sin(alpha), 0.0);
    alpha += delta;
  }
  glEnd();
}
