#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "Viewer.h"

using namespace std;

namespace DDG
{
   // declare static member variables
   Mesh Viewer::mesh;
   Viewer::RenderMode Viewer::mode = renderShaded;
   GLuint Viewer::surfaceDL = 0;
   int Viewer::windowSize[2] = { 512, 512 };
   Camera Viewer::camera;
   NormalScheme Viewer::scheme;
   
   void Viewer :: init( void )
   {
      restoreViewerState();
      initGLUT();
      initGL();
   
      updateDisplayList();
   
      glutMainLoop();
   }
   
   void Viewer :: initGLUT( void )
   {
      int argc = 0;
      vector< vector<char> > argv(1);
   
      // initialize window
      glutInitWindowSize( windowSize[0], windowSize[1] );
      glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
      glutInit( &argc, (char**)&argv );
      glutCreateWindow( "DDG" );
   
      // specify callbacks
      glutDisplayFunc  ( Viewer::display  );
      glutIdleFunc     ( Viewer::idle     );
      glutKeyboardFunc ( Viewer::keyboard );
      glutSpecialFunc  ( Viewer::special  );
      glutMouseFunc    ( Viewer::mouse    );
      glutMotionFunc   ( Viewer::motion   );
   
      // initialize menus
      int viewMenu = glutCreateMenu( Viewer::view );
      glutAddMenuEntry( "[s] Smooth Shaded",  menuSmoothShaded );
      glutAddMenuEntry( "[f] Wireframe",      menuWireframe    );
      glutAddMenuEntry( "[↑] Zoom In",        menuZoomIn       );
      glutAddMenuEntry( "[↓] Zoom Out",       menuZoomOut      );

      int normalsMenu = glutCreateMenu( Viewer::normals );
      glutAddMenuEntry( "[1] Equally Weighted", menuEquallyWeighted );
      glutAddMenuEntry( "[2] Area Weighted",    menuAreaWeighted    );
      glutAddMenuEntry( "[3] Angle Weighted",   menuAngleWeighted   );
      glutAddMenuEntry( "[4] Mean Curvature",   menuMeanCurvature   );
      glutAddMenuEntry( "[5] Sphere Inscribed", menuSphereInscribed );

      int mainMenu = glutCreateMenu( Viewer::menu );
      glutSetMenu( mainMenu );
      glutAddMenuEntry( "[space] Process Mesh", menuProcess   );
      glutAddMenuEntry( "[r] Reset Mesh",       menuResetMesh );
      glutAddMenuEntry( "[w] Write Mesh",       menuWriteMesh );
      glutAddMenuEntry( "[esc] Exit",           menuExit      );
      glutAddSubMenu( "View", viewMenu );
      glutAddSubMenu( "Normals", normalsMenu );
      glutAttachMenu( GLUT_RIGHT_BUTTON );
   }
   
   void Viewer :: initGL( void )
   {
      glClearColor( .5, .5, .5, 1. );
      initLighting();
   }
   
   void Viewer :: initLighting( void )
   {
      GLfloat position[4] = { 20., 30., 40., 0. };
      glLightfv( GL_LIGHT0, GL_POSITION, position );
      glEnable( GL_LIGHT0 );
      glEnable( GL_NORMALIZE );
   }
   
   void Viewer :: menu( int value )
   {
      switch( value )
      {
         case( menuProcess ):
            mProcess();
            break;
         case( menuResetMesh ):
            mResetMesh();
            break;
         case( menuWriteMesh ):
            mWriteMesh();
            break;
         case( menuExit ):
            mExit();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: view( int value )
   {
      switch( value )
      {
         case( menuSmoothShaded ):
            mSmoothShaded();
            break;
         case( menuWireframe ):
            mWireframe();
            break;
         case( menuZoomIn ):
            mZoomIn();
            break;
         case( menuZoomOut ):
            mZoomOut();
            break;
         default:
            break;
      }
   }

   void Viewer :: normals( int value )
   {
      switch( value )
      {
         case( menuEquallyWeighted ):
            mEquallyWeighted();
            break;
         case( menuAreaWeighted ):
            mAreaWeighted();
            break;
         case( menuAngleWeighted ):
            mAngleWeighted();
            break;
         case( menuMeanCurvature ):
            mMeanCurvature();
            break;
         case( menuSphereInscribed ):
            mSphereInscribed();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: mProcess( void )
   {
      // TODO process geometry here!

      updateDisplayList();
   }
   
   void Viewer :: mResetMesh( void )
   {
      mesh.reload();
      updateDisplayList();
   }
   
   void Viewer :: mWriteMesh( void )
   {
      mesh.write( "out.obj" );
   }
   
   void Viewer :: mExit( void )
   {
      storeViewerState();
      exit( 0 );
   }
   
   void Viewer :: mSmoothShaded( void )
   {
      mode = renderShaded;
      updateDisplayList();
   }
   
   void Viewer :: mWireframe( void )
   {
      mode = renderWireframe;
      updateDisplayList();
   }

   void Viewer :: mZoomIn( void )
   {
      camera.zoomIn();
   }

   void Viewer :: mZoomOut( void )
   {
      camera.zoomOut();
   }

   void Viewer :: mEquallyWeighted( void )
   {
      scheme = nsEquallyWeighted;
      updateDisplayList();
   }
   
   void Viewer :: mAreaWeighted( void )
   {
      scheme = nsAreaWeighted;
      updateDisplayList();
   }
   
   void Viewer :: mAngleWeighted( void )
   {
      scheme = nsAngleWeighted;
      updateDisplayList();
   }
   
   void Viewer :: mMeanCurvature( void )
   {
      scheme = nsMeanCurvature;
      updateDisplayList();
   }
   
   void Viewer :: mSphereInscribed( void )
   {
      scheme = nsSphereInscribed;
      updateDisplayList();
   }
   
   void Viewer :: keyboard( unsigned char c, int x, int y )
   {
      switch( c )
      {
         case 's':
            mSmoothShaded();
            break;
         case 'f':
            mWireframe();
            break;
         case 'w':
            mWriteMesh();
            break;
         case 'r':
            mResetMesh();
            break;
         case ' ':
            mProcess();
            break;
         case GLUT_KEY_UP:
            camera.zoomIn();
            break;
         case GLUT_KEY_DOWN:
            camera.zoomOut();
            break;
         case '1':
            mEquallyWeighted();
            break;
         case '2':
            mAreaWeighted();
            break;
         case '3':
            mAngleWeighted();
            break;
         case '4':
            mMeanCurvature();
            break;
         case '5':
            mSphereInscribed();
            break;
         case 27:
            mExit();
            break;
         default:
            break;
      }
   }

   void Viewer :: special( int i, int x, int y )
   {
      switch( i )
      {
         case GLUT_KEY_UP:
            camera.zoomIn();
            break;
         case GLUT_KEY_DOWN:
            camera.zoomOut();
            break;
         case 27:
            mExit();
            break;
         default:
            break;
      }
   }
   
   void Viewer :: display( void )
   {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
   
      GLint viewport[4];
      glGetIntegerv( GL_VIEWPORT, viewport );
      double aspect = (double) viewport[2] / (double) viewport[3];
      const double fovy = 50.;
      const double clipNear = .01;
      const double clipFar = 1000.;
      gluPerspective( fovy, aspect, clipNear, clipFar );
   
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();
      gluLookAt( 0., 0., -2.5*camera.zoom,
                 0., 0., 0.,
                 0., 1., 0. );
   
      camera.setView();
   
      drawSurface();
   
      glutSwapBuffers();
   }

   void Viewer :: setMeshMaterial( void )
   {
      GLfloat  diffuse[4] = { .8, .5, .3, 1. };
      GLfloat specular[4] = { .3, .3, .3, 1. };
      GLfloat  ambient[4] = { .2, .2, .5, 1. };
   
      glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse  );
      glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient  );
      glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, 16.      );
   }
   
   void Viewer :: drawSurface( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glEnable( GL_DEPTH_TEST );
      glEnable( GL_LIGHTING );
   
      glCallList( surfaceDL );
   
      glPopAttrib();
   }
   
   void Viewer :: drawMesh( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glEnable( GL_POLYGON_OFFSET_FILL );
      glPolygonOffset( 1., 1. );
   
      drawPolygons();
   
      glDisable( GL_POLYGON_OFFSET_FILL );
   
      if( mode == renderWireframe )
      {
         drawWireframe();
      }

      drawIsolatedVertices();

      glPopAttrib();
   }

   void Viewer :: drawPolygons( void )
   {
      for( FaceCIter f  = mesh.faces.begin();
                     f != mesh.faces.end();
                     f ++ )
      {
         if( f->isBoundary() ) continue;

         glBegin( GL_POLYGON );
         if( mode == renderWireframe )
         {
            Vector N = f->normal();
            glNormal3dv( &N[0] );
         }

         HalfEdgeCIter he = f->he;
         do
         {
            if( mode != renderWireframe )
            {
               Vector N = he->vertex->normal( scheme );
               glNormal3dv( &N[0] );
            }

            glVertex3dv( &he->vertex->position[0] );

            he = he->next;
         }
         while( he != f->he );
         glEnd();
      }
   }

   void Viewer :: drawWireframe( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glDisable( GL_LIGHTING );
      glColor4f( 0., 0., 0., .5 );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      glBegin( GL_LINES );
      for( EdgeCIter e  = mesh.edges.begin();
            e != mesh.edges.end();
            e ++ )
      {
         glVertex3dv( &e->he->vertex->position[0] );
         glVertex3dv( &e->he->flip->vertex->position[0] );
      }
      glEnd();

      glPopAttrib();
   }

   void Viewer :: drawIsolatedVertices( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      // draw with big, round, red dots
      glPointSize( 5 );
      glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
      glEnable( GL_POINT_SMOOTH );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glColor4f( 1., 0., 0., 1. ); // red

      glBegin( GL_POINTS );
      for( VertexCIter v  = mesh.vertices.begin();
                       v != mesh.vertices.end();
                       v ++ )
      {
         if( v->isIsolated() )
         {
            glVertex3dv( &v->position[0] );
         }
      }
      glEnd();

      glPopAttrib();
   }
   
   void Viewer :: updateDisplayList( void )
   {
      if( surfaceDL )
      {
         glDeleteLists( surfaceDL, 1 );
         surfaceDL = 0;
      }
   
      surfaceDL = glGenLists( 1 );
   
      setMeshMaterial();
   
      glNewList( surfaceDL, GL_COMPILE );
      drawMesh();
      glEndList();
   }
   
   void Viewer :: mouse( int button, int state, int x, int y )
   {
      camera.mouse( button, state, x, y );
   }

   void Viewer :: motion( int x, int y )
   {
      camera.motion( x, y );
   }
   
   void Viewer :: idle( void )
   {
      camera.idle();
      glutPostRedisplay();
   }

   void Viewer :: storeViewerState( void )
   {
      ofstream out( ".viewer_state.txt" );

      out << camera.rLast[0] << endl;
      out << camera.rLast[1] << endl;
      out << camera.rLast[2] << endl;
      out << camera.rLast[3] << endl;

      GLint view[4];
      glGetIntegerv( GL_VIEWPORT, view );
      out << view[2] << endl;
      out << view[3] << endl;

      out << (int) mode << endl;
   }

   void Viewer :: restoreViewerState( void )
   {
      ifstream in( ".viewer_state.txt" );

      if( !in.is_open() ) return;

      in >> camera.rLast[0];
      in >> camera.rLast[1];
      in >> camera.rLast[2];
      in >> camera.rLast[3];

      in >> windowSize[0];
      in >> windowSize[1];

      int m;
      in >> m;
      mode = (RenderMode) m;
   }
}

