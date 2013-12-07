#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "Viewer.h"
#include "Image.h"
#include "Application.h"

namespace DDG
{
   // declare static member variables
   Mesh Viewer::mesh;
   GLuint Viewer::surfaceDL = 0;
   int Viewer::windowSize[2] = { 512, 512 };
   Camera Viewer::camera;
   Shader Viewer::shader;
   bool Viewer::renderWireframe = false;
   bool Viewer::render3D = false;
   bool Viewer::renderQuasiConformal = false;
   
   void Viewer :: init( void )
   {
      restoreViewerState();
      initGLUT();
      setGL();
      initGLSL();
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
      glutSetMenu( viewMenu );
      glutAddMenuEntry( "[f] Wireframe",  menuWireframe    );
      glutAddMenuEntry( "[s] Switch 2D/3D",  menuRender3D  );
      glutAddMenuEntry( "[q] Quasi Conformal Error",  menuQuasiConformal );
      glutAddMenuEntry( "[↑] Zoom In",    menuZoomIn       );
      glutAddMenuEntry( "[↓] Zoom Out",   menuZoomOut      );
      
      int mainMenu = glutCreateMenu( Viewer::menu );
      glutSetMenu( mainMenu );
      glutAddMenuEntry( "[space] Process Mesh", menuProcess    );
      glutAddMenuEntry( "[r] Reset Mesh",       menuResetMesh  );
      glutAddMenuEntry( "[w] Write Mesh",       menuWriteMesh  );
      glutAddMenuEntry( "[\\] Screenshot",      menuScreenshot );
      glutAddMenuEntry( "[esc] Exit",           menuExit       );
      glutAddSubMenu( "View", viewMenu );
      glutAttachMenu( GLUT_RIGHT_BUTTON );
   }
   
   void Viewer :: initGLSL( void )
   {
      shader.loadVertex( "shaders/vertex.glsl" );
      shader.loadFragment( "shaders/fragment.glsl" );
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
         case( menuScreenshot ):
            mScreenshot();
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
         case( menuQuasiConformal ):
            mQuasiConformal();
            break;
         case( menuRender3D ):
            mRender3D();
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
   
   void Viewer :: keyboard( unsigned char c, int x, int y )
   {
      switch( c )
      {
         case 'q':
            mQuasiConformal();
            break;
         case 's':
            mRender3D();
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
         case '\\':
            mScreenshot();
            break;
         case ' ':
            mProcess();
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

   void Viewer :: mouse( int button, int state, int x, int y )
   {
      if( ( glutGetModifiers() & GLUT_ACTIVE_SHIFT) and state == GLUT_UP )
         pickVertex(x, y);
      else
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
   }

   void Viewer :: mProcess( void )
   {
      Application app;
      app.run(mesh);
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
      //storeViewerState();
      exit( 0 );
   }
   
   void Viewer :: mWireframe( void )
   {
      renderWireframe = !renderWireframe;
      updateDisplayList();
   }

   void Viewer :: mRender3D( void )
   {
      render3D = !render3D;
      updateDisplayList();
   }
   
   void Viewer :: mQuasiConformal( void )
   {
      renderQuasiConformal = !renderQuasiConformal;
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
   
   void Viewer :: mScreenshot( void )
   {
      static int index = 0;
      
      // get window width and height
      GLint view[4];
      glGetIntegerv( GL_VIEWPORT, view );
      int w = view[2];
      int h = view[3];
      
      // get pixels
      Image image( w, h );
      glReadPixels( 0, 0, w, h, GL_BGR, GL_FLOAT, &image(0,0) );
      
      stringstream filename;
      filename << "frames/viewer" << setw(8) << setfill( '0' ) << index << ".tga";
      image.write( filename.str().c_str() );
      
      index++;
   }
      
   void Viewer :: display( void )
   {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      shader.enable();

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
      
      Quaternion    eye = Vector( 0., 0., -2.5*camera.zoom );
      Quaternion center = Vector( 0., 0., 0. );
      Quaternion     up = Vector( 0., 1., 0. );      
      gluLookAt(   eye[1],    eye[2],    eye[3],
                center[1], center[2], center[3],
                    up[1],     up[2],     up[3] );
      
      
      Quaternion r = camera.currentRotation();
      eye = r.conj() * eye * r;
      GLint uniformEye = glGetUniformLocation( shader, "eye" );
      glUniform3f( uniformEye, eye[1], eye[2], eye[3] );
      
      Quaternion light = Vector( -1., 1., -2. );
      light = r.conj() * light * r;
      GLint uniformLight = glGetUniformLocation( shader, "light" );
      glUniform3f( uniformLight, light[1], light[2], light[3] );
      
      camera.setView();
      callDisplayList();
      shader.disable();
      glutSwapBuffers();
   }
   
   void Viewer :: updateDisplayList( void )
   {
      if( surfaceDL )
      {
         glDeleteLists( surfaceDL, 1 );
         surfaceDL = 0;
      }
      
      surfaceDL = glGenLists( 1 );
      glNewList( surfaceDL, GL_COMPILE );
      setMeshMaterial();
      drawScene();
      glEndList();
   }
   
   void Viewer :: setGL( void )
   {
      glClearColor( .5, .5, .5, 1. );
      setLighting();
   }
   
   void Viewer :: setLighting( void )
   {
      GLfloat position[4] = { 20., 30., 40., 0. };
      glLightfv( GL_LIGHT0, GL_POSITION, position );
      glEnable( GL_LIGHT0 );
      glEnable( GL_NORMALIZE );
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
   
   void Viewer :: callDisplayList( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );
      glEnable( GL_DEPTH_TEST );
      glEnable( GL_LIGHTING );
      glCallList( surfaceDL );
      glPopAttrib();
   }
   
   void Viewer :: drawScene( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glEnable( GL_POLYGON_OFFSET_FILL );
      glPolygonOffset( 1., 1. );
      drawPolygons();
      glDisable( GL_POLYGON_OFFSET_FILL );
      
      if( renderWireframe ) drawWireframe();
      
      drawIsolatedVertices();
      
      drawSelectedVertices();
      
      glPopAttrib();
   }
   
   void Viewer :: drawPolygons( void )
   {
      if( not renderQuasiConformal )
      {
         glColor3d( 1., .5, .25 );
      }

      for( FaceCIter f  = mesh.faces.begin();
          f != mesh.faces.end();
          f ++ )
      {
         if( f->isBoundary() ) continue;
      
         if( renderQuasiConformal )
         {
            double value = faceQCDistortion( f );
            Vector color = qcColor( value );
            glColor3dv( &color[0] );
         }   
         
         glBegin( GL_POLYGON );
         if( render3D and renderWireframe )
         {
            Vector N = f->normal();
            glNormal3dv( &N[0] );
         }
         
         HalfEdgeCIter he = f->he;
         do
         {
            if( render3D and (not renderWireframe) )
            {
               Vector N = he->vertex->normal();
               glNormal3dv( &N[0] );
            }

            if( not render3D )
            {
               glNormal3d(0.,0.,1.);
            }
            
            if( render3D )
            {
               glVertex3dv( &he->vertex->position[0] );               
            }
            else
            {
               glVertex3dv( &he->vertex->texture[0] );
            }
            
            he = he->next;
         }
         while( he != f->he );
         glEnd();
      }
   }
   
   void Viewer :: drawWireframe( void )
   {
      shader.disable();
      glPushAttrib( GL_ALL_ATTRIB_BITS );
      
      glDisable( GL_LIGHTING );
      glColor4f( 0., 0., 0., 0.5 );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      
      glBegin( GL_LINES );
      for( EdgeCIter e  = mesh.edges.begin();
          e != mesh.edges.end();
          e ++ )
      {
            if( render3D )
            {
               glVertex3dv( &e->he->vertex->position[0] );
               glVertex3dv( &e->he->flip->vertex->position[0] );
            }
            else
            {
               glVertex3dv( &e->he->vertex->texture[0] );
               glVertex3dv( &e->he->flip->vertex->texture[0] );
            }
         
      }
      glEnd();
      
      glPopAttrib();
   }
   
   void Viewer :: drawIsolatedVertices( void )
   {
      glPushAttrib( GL_ALL_ATTRIB_BITS );
      
      glPointSize( 5 );
      glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
      glEnable( GL_POINT_SMOOTH );
      glColor3f( 1., 0., 0. );
      
      glBegin( GL_POINTS );
      for( VertexCIter v  = mesh.vertices.begin();
          v != mesh.vertices.end();
          v ++ )
      {
         if( v->isIsolated() )
         {
            if( render3D )
            {
               glVertex3dv( &v->position[0] );
            }
            else
            {
               glVertex3dv( &v->texture[0] );
            }
         }
      }
      glEnd();
      
      glPopAttrib();
   }
   
   void Viewer :: drawVertices( void )
   {
      for( VertexCIter v = mesh.vertices.begin();
          v != mesh.vertices.end();
          v ++ )
      {
         glLoadName(v->index);
         glBegin(GL_POINTS);
         if( render3D )
         {
            glVertex3dv( &v->position[0] );
         }
         else
         {
            glVertex3dv( &v->texture[0] );
         }
         glEnd();
      }
   }
   
   void Viewer :: drawSelectedVertices( void )
   {
      shader.disable();

      glPushAttrib( GL_ALL_ATTRIB_BITS );

      GLfloat  diffuse[4] = { 0.0, 0.0, 1.0, 1.0 };
      GLfloat specular[4] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat  ambient[4] = { 0.0, 0.0, 0.0, 1.0 };
      
      glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse  );
      glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular );
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient  );
      glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, 16.      );

      for( VertexCIter v = mesh.vertices.begin();
          v != mesh.vertices.end();
          v ++ )
      {
         if( v->tag ) 
         {
            double radius = v->isIsolated() ? 0.02 : 100;

            if ( not v->isIsolated() )
            {
               HalfEdgeIter he = v->he;
               do
               {
                  double len = render3D ? \
                     ( he->vertex->position - he->flip->vertex->position ).norm() : \
                     ( he->vertex->texture - he->flip->vertex->texture ).norm();

                  if ( radius > len ) radius = len;
                  he = he->flip->next;
               } while ( he != v->he );

               radius *= 0.5;
            }

            const double * vecPtr = render3D ? &v->position[0] : &v->texture[0];

            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glTranslatef( vecPtr[0], vecPtr[1], vecPtr[2] );
            glutSolidSphere( radius, 16, 16 );
            glPopMatrix();
         }
      }

      glPopAttrib();

      /*shader.disable();
      glPushAttrib( GL_ALL_ATTRIB_BITS );

      glEnable(GL_COLOR_MATERIAL);
      glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
      glEnable( GL_POINT_SMOOTH );
      glColor3f( 0., 0., 0.5 );
      glPointSize( 20 );

      glBegin(GL_POINTS);
      for( VertexCIter v = mesh.vertices.begin();
          v != mesh.vertices.end();
          v ++ )
      {
         if( v->tag ) 
         {
            if( render3D )
            {
               glVertex3dv( &v->position[0] );
            }
            else
            {
               glVertex3dv( &v->texture[0] );
            }
         }
      }
      glEnd();
      
      glPopAttrib();*/
   }

   void Viewer :: pickVertex(int x, int y)
   {
      int width  = glutGet(GLUT_WINDOW_WIDTH );
      int height = glutGet(GLUT_WINDOW_HEIGHT);
      if( x < 0 || x >= width || y < 0 || y >= height ) return;
      
      int bufSize = mesh.vertices.size();
      GLuint* buf = new GLuint[bufSize];
      glSelectBuffer(bufSize, buf);
      
      GLint viewport[4];
      GLdouble projection[16];
      glGetIntegerv( GL_VIEWPORT, viewport );
      glGetDoublev(GL_PROJECTION_MATRIX, projection);
      
      glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(0);
      
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix(x, viewport[3]-y, 10, 10, viewport);
      glMultMatrixd(projection);
      
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      drawVertices();
      glPopMatrix();
      
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      
      glMatrixMode(GL_MODELVIEW);
      long hits = glRenderMode(GL_RENDER);
      
      int index = -1;
      double min_z = 1.0e100;
      for( long i = 0; i < hits; ++i )
      {
         double distance = buf[4*i + 1];
         if( distance < min_z )
         {
            index = buf[4*i + 3];
            min_z = distance;
         }
      }
      delete[] buf;

      if (index >= 0)
      {
         mesh.vertices[index].toggleTag();
         updateDisplayList();
      }
   }
}

