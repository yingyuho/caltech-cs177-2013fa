#include <list>
using namespace std;

#include "Application.h"
#include "Quaternion.h"

namespace DDG
{
   // public
   void Application::run(Mesh& mesh)
   {
      if (not mesh.boundaries.empty())
      {
         std::cout << "Mesh has boundary" << std::endl;
         return;
      }

      if ( mesh.taggedVertices.size() == 0 )
      {
         std::cout << "No vertices are tagged" << std::endl;
         return;
      }

      balanceWinding(mesh);
      solveForConnection(mesh);
      transportVectorField(mesh);
   }

   void Application::balanceWinding(Mesh& mesh)
   {
      if ( mesh.taggedVertices.size() == 0 ) return;

      int euler = mesh.vertices.size() - mesh.edges.size() + mesh.faces.size();

      int totalWinding = 0;

      for ( std::list<int>::const_iterator it = mesh.taggedVertices.cbegin();
         it != mesh.taggedVertices.cend(); it++ )
      {
         totalWinding += mesh.vertices[*it].winding;
      }

      if ( euler == totalWinding ) return;

      // if there is no highlighted vertex, highlight the first tagged vertex 
      if ( mesh.hledVertices.size() == 0 )
         mesh.toggleVertexHL( mesh.taggedVertices.front() );

      // modify the highlighted vertex to balance winding numbers
      mesh.vertices[mesh.hledVertices.front()].winding += euler - totalWinding;
   }

   void Application::solveForConnection(Mesh& mesh)
   {
      const int V = mesh.vertices.size();
      const double PI = 3.141592653589793238462;
      const double TWOPI = 2. * PI;

      // get minus Gaussian curvature
      DenseMatrix<Real> b(V);
      b.zero(-TWOPI);

      for ( FaceCIter f = mesh.faces.cbegin(); f != mesh.faces.cend(); f++ )
      {
         HalfEdgeCIter he = f->he;

         VertexCIter v0 = he->vertex;   he = he->next;
         VertexCIter v1 = he->vertex;   he = he->next;
         VertexCIter v2 = he->vertex;

         Vector v01 = v1->position - v0->position;
         Vector v12 = v2->position - v1->position;
         Vector v20 = v0->position - v2->position;

         double th0 = atan2( cross(v20,v01).norm(), -dot(v20,v01) );
         double th1 = atan2( cross(v01,v12).norm(), -dot(v01,v12) );
         double th2 = atan2( cross(v12,v20).norm(), -dot(v12,v20) );

         b(v0->index) += th0;
         b(v1->index) += th1;
         b(v2->index) += th2;
      }

      // get winding number
      for ( std::list<int>::const_iterator it = mesh.taggedVertices.cbegin();
         it != mesh.taggedVertices.cend(); it++ )
      {
         b(*it) += TWOPI * mesh.vertices[*it].winding;
      }

      // find u, phi satisfying d⋆du = −K + 2πk, phi = ⋆du
      SparseMatrix<Real> d0, star1;
      ExteriorDerivative0Form<Real>::build(mesh, d0);
      HodgeStar1Form<Real>::build(mesh, star1);

      SparseMatrix<Real> L = d0.transpose() * star1 * d0;

      DenseMatrix<Real> u;

      solve(L, u, b);

      DenseMatrix<Real> phi = star1 * d0 * u;

      // put potential
      for ( int i = 0; i < V; i++ )
         mesh.vertices[i].potential = u(i);

      // put connection
      for ( int i = 0; i < mesh.edges.size(); i++ )
      {
         HalfEdgeIter he = mesh.edges[i].he;
         he->connection = phi(i);
         he->flip->connection = -phi(i);
      }
   }

   void Application::transportVectorField(Mesh& mesh)
   {
      std::vector<bool> visited(mesh.faces.size(), false);
      std::list<int> toVisit;

      // start from a face linked to the first tagged vertex
      HalfEdgeCIter he = mesh.vertices[mesh.taggedVertices.front()].he->next;
      he->face->direction = (he->next->vertex->position - he->vertex->position).unit();
      visited[he->face->index] = true;
      toVisit.push_back(he->face->index);

      while ( not toVisit.empty() )
      {
         // use BFS
         const int faceI = toVisit.front();
         toVisit.pop_front();

         const Face& face = mesh.faces[faceI];

         HalfEdgeIter he = face.he;
         do
         {
            FaceIter f = he->flip->face;
            // ignore visited faces
            if ( visited[f->index] ) { he = he->next; continue; }

            // tag as visited and enqueue
            visited[f->index] = true;
            toVisit.push_back(f->index);

            // convert vector to quaternion
            Quaternion p(face.direction);

            // rotate by the angle between two face normals
            Vector axis = ( he->next->vertex->position - he->vertex->position ).unit();
            double th = atan2( dot( cross( face.normal(), f->normal() ), axis ), 
                                    dot( face.normal(), f->normal() ) );

            Quaternion q( cos(th/2.), sin(th/2.) * axis );

            p = q * p * q.conj();

            // rotate by the connection
            axis = f->normal();
            th = he->connection;

            q = Quaternion( cos(th/2.), sin(th/2.) * axis );

            p = q * p * q.conj();

            // convert quaternion to vector
            std::copy(&p[1], 3 + &p[1], &f->direction[0]);

            cout << dot( f->direction, f->normal() ) << endl;

            // project to the surface, not really needed for double precision
            // f->direction = ( f->direction - f->normal() * dot(f->direction, f->normal()) ).unit();

            he = he->next;
         } while ( he != face.he );
      }
   }

   void Application::flatten(Mesh& mesh)
   {
      if (mesh.boundaries.empty())
      {
         std::cout << "Mesh has no boundary" << std::endl;
         return;
      }

      // double initial_area = mesh.area();
      // std::cout << "area: " << initial_area << std::endl;

      // create matrix for Lc
      const int V = mesh.vertices.size();
      SparseMatrix<Complex> Lc(V,V);

      // build energy
      buildEnergy(mesh, Lc);

      // compute the solution;
      DenseMatrix<Complex> x(V,1);
      x.randomize();

      SparseMatrix<Complex> star0;
      HodgeStar0Form<Complex>::build(mesh, star0);

      Lc += Complex(1E-8) * star0;

      smallestEigPositiveDefinite<Complex>(Lc, star0, x);

      // then assign the solution
      assignSolution(x, mesh);

      // rescale mesh for display convenience
      double scale = std::sqrt( mesh.area() / mesh.area2D() );
      normalizeMesh(scale, mesh);
   }

   // protected
   void Application::buildEnergy(const Mesh& mesh, SparseMatrix<Complex>& A) const
   {
      const int V = mesh.vertices.size();
      // initialize a sparse VxV complex matrix
      A = SparseMatrix<Complex>(V,V);

      // loop through all half-edges to get Delta/2 part
      for ( std::vector<HalfEdge>::const_iterator it = mesh.halfedges.cbegin(); \
         it != mesh.halfedges.cend(); it++ )
      {
         if ( it->onBoundary ) continue;

         Complex half_half_cot = Complex(it->cotan() / 4.);
         int i = it->vertex->index;
         int j = it->next->vertex->index;
         A(i,i) += half_half_cot;
         A(j,j) += half_half_cot;
         A(i,j) -= half_half_cot;
         A(j,i) -= half_half_cot;
      }

      /*SparseMatrix<Complex> d0, star1;

      ExteriorDerivative0Form<Complex>::build( mesh, d0 );
      HodgeStar1Form<Complex>::build( mesh, star1 );
      A = d0.transpose() * star1 * d0;*/

      // for each boundary face
      for ( std::vector<Face>::const_iterator it = mesh.boundaries.cbegin(); \
         it != mesh.boundaries.cend(); it++ )
      {
         // loop through all half-edges to get A(z) part
         HalfEdgeIter he = it->he;
         do {
            int i = he->vertex->index;
            int j = he->next->vertex->index;
            A(i,j) -= Complex(0.,1/4.);
            A(j,i) += Complex(0.,1/4.);
            he = he->next;
         } while( he != it->he );
      }
   }

   void Application::assignSolution(const DenseMatrix<Complex>& x, Mesh& mesh)
   {
      for( VertexIter v = mesh.vertices.begin(); v != mesh.vertices.end(); v ++ )
      {
         int i = v->index;
         const Complex& z = x(i,0);
         v->texture.x = z.re;
         v->texture.y = z.im;
         v->texture.z = 0.0;
      }
   }

   void Application::normalizeMesh(const double scale, Mesh& mesh)
   {
      Vector avg;
      for( VertexIter v = mesh.vertices.begin(); v != mesh.vertices.end(); v ++ )
      {
         avg += v->texture;
      }
      avg /= mesh.vertices.size();

      for( VertexIter v = mesh.vertices.begin(); v != mesh.vertices.end(); v ++ )
      {
         v->texture = scale*(v->texture - avg);
      }
   }
}
