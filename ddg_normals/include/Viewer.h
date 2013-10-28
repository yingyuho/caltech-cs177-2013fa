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
         static void normals( int value );
         
         // menu functions
         static void mProcess( void );
         static void mResetMesh( void );
         static void mWriteMesh( void );
         static void mExit( void );
         static void mSmoothShaded( void );
         static void mTextured( void );
         static void mWireframe( void );
         static void mZoomIn( void );
         static void mZoomOut( void );
         static void mEquallyWeighted( void );
         static void mAreaWeighted( void );
         static void mAngleWeighted( void );
         static void mMeanCurvature( void );
         static void mSphereInscribed( void );

         // unique identifiers for menus
         enum
         {
            menuProcess,
            menuResetMesh,
            menuWriteMesh,
            menuExit,
            menuSmoothShaded,
            menuWireframe,
            menuZoomIn,
            menuZoomOut,
            menuEquallyWeighted,
            menuAreaWeighted,
            menuAngleWeighted,
            menuMeanCurvature,
            menuSphereInscribed
         };

         // draw state
         enum RenderMode
         {
            renderShaded = 0,
            renderWireframe = 1
         };

         static RenderMode mode;  // current render mode
         static NormalScheme scheme; // method for computing vertex normals

         static void storeViewerState( void );
         static void restoreViewerState( void );
         static int windowSize[2];

         static Camera camera;
         // keeps track of view state

         // viewer data
         static GLuint surfaceDL; // display list for mesh
   };
}

#endif

