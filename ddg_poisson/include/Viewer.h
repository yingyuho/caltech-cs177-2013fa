// -----------------------------------------------------------------------------
// libDDG -- Viewer.h
// -----------------------------------------------------------------------------
//
// Viewer provides a graphical user interface (GUI) for inspecting and
// interacting with a Mesh object.  Viewer methods are static in order
// to make them compatible with GLUT callbacks.
//

#ifndef DDG_VIEWER_H
#define DDG_VIEWER_H

#include <GLUT/glut.h>
#include "Mesh.h"
#include "Camera.h"

namespace DDG
{
   class Viewer
   {
      public:
         static void init( void );
         static Mesh mesh;

      protected:
         // draw routines
         static void initGLUT( void );
         static void initGL( void );
         static void initLighting( void );
         static void drawSurface( void );
         static void drawMesh( void );
         static void setMeshMaterial( void );
         static void updateDisplayList( void );
         static void drawPolygons( void );
         static void drawWireframe( void );
         static void drawIsolatedVertices( void );
         
         // GLUT callbacks
         static void display( void );
         static void idle( void );
         static void keyboard( unsigned char c, int x, int y );
         static void special( int i, int x, int y );
         static void mouse( int button, int state, int x, int y );
         static void motion( int x, int y );
         static void menu( int value );
         static void view( int value );
         
         // menu functions
         static void mComputePotential( void );
         static void mComputeFlow( void );
         static void mResetMesh( void );
         static void mWriteMesh( void );
         static void mExit( void );
         static void mSmoothShaded( void );
         static void mDensity( void );
         static void mPotential( void );
         static void mTextured( void );
         static void mWireframe( void );
         static void mZoomIn( void );
         static void mZoomOut( void );
         static void mScreenshot( void );

         // unique identifiers for menus
         enum
         {
            menuComputePotential,
            menuComputeFlow,
            menuResetMesh,
            menuWriteMesh,
            menuExit,
            menuSmoothShaded,
            menuDensity,
            menuPotential,
            menuWireframe,
            menuZoomIn,
            menuZoomOut,
            menuScreenshot
         };

         // draw state
         enum RenderMode
         {
            renderShaded,
            renderWireframe,
            renderDensity,
            renderPotential
         };

         static RenderMode mode;  // current render mode

         static void storeViewerState( void );
         static void restoreViewerState( void );
         static int windowSize[2];

         static Camera camera;
         // keeps track of view state

         // viewer data
         static GLuint surfaceDL; // display list for mesh

         void updatePotential( void );
         // update the scalar potential determined by the density on vertices

         static double maxDensity;
         // maximum magnitude of the scalar density (used to normalize visualization)

         static double maxPotential;
         // maximum magnitude of the scalar potential (used to normalize visualization)
   };
}

#endif

