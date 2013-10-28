// -----------------------------------------------------------------------------
// libDDG -- Camera.h
// -----------------------------------------------------------------------------
//
// Camera is used by Viewer to keep track of the view state; it also
// handles mouse input related to camera manipulation.
//

#ifndef DDG_CAMERA_H
#define DDG_CAMERA_H

#include "Quaternion.h"

#ifdef __CYGWIN__
#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

namespace DDG
{
   class Camera
   {
      public:
         Camera( void );

         Quaternion clickToSphere( int x, int y );
         void setView( void ) const;
         void mouse( int button, int state, int x, int y );
         void motion( int x, int y );
         void idle( void );
         void zoomIn( void );
         void zoomOut( void );

         Quaternion pClick;   // mouse coordinates of current click
         Quaternion pDrag;    // mouse coordinates of current drag
         Quaternion pLast;    // mouse coordinates of previous drag
         Quaternion rLast;    // previous camera rotation
         Quaternion momentum; // camera momentum
         int tLast;           // time of previous drag
         double zoom, vZoom;  // zoom and zoom velocity
   };
}

#endif

