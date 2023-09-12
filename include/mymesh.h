
#ifndef FILEEXCHANGE_MYMESH_H
#define FILEEXCHANGE_MYMESH_H

#include <iostream>
#include "vector"
#include "vcg/complex/complex.h"
#include "vcg/complex/used_types.h"
#include "vcg/simplex/vertex/component.h"
#include "vcg/simplex/face/component.h"
#include <vcg/complex/algorithms/update/halfedge_indexed.h>

using namespace vcg;
using namespace std;

class MyEdge;
class MyFace;
class MyVertex;
struct MyUsedTypes : public UsedTypes<	Use<MyVertex>   ::AsVertexType,
        Use<MyEdge>     ::AsEdgeType,
        Use<MyFace>     ::AsFaceType>{};

class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::Normal3f, vertex::VFAdj , vertex::VEAdj, vertex::Qualityf, vertex::BitFlags,  vertex::Mark ,vertex::CurvatureDirf>{};
class MyFace    : public Face< MyUsedTypes, face::Mark,  face::VertexRef, face::VFAdj , face::FEAdj , face::FFAdj, face::Normal3f, face::BitFlags > {};
class MyEdge    : public Edge<MyUsedTypes , edge::VEAdj , edge::VertexRef, edge::BitFlags>{};
class MyMesh    : public tri::TriMesh< vector<MyVertex>, vector<MyFace> , vector<MyEdge> > {};


//
/* Definition of a mesh of polygons that also supports half-edges
*/
class MyPFace;
class MyPVertex;
class MyPHEdge;
class MyPEdge;

struct MyPUsedTypes: public vcg::UsedTypes<vcg::Use<MyPVertex>  ::AsVertexType,
        vcg::Use<MyPEdge>	::AsEdgeType,
        vcg::Use<MyPHEdge>::AsHEdgeType,
        vcg::Use<MyPFace>	::AsFaceType
>{};

//class DummyEdge: public vcg::Edge<PolyUsedTypes>{};
class MyPVertex:public vcg::Vertex<	MyPUsedTypes,
        vcg::vertex::Coord3f,
        vcg::vertex::Normal3f,
        vcg::vertex::Mark,
        vcg::vertex::BitFlags,
        vcg::vertex::VHAdj>{} ;

class MyPEdge : public Edge<MyPUsedTypes>{};
class MyPHEdge : public HEdge< MyPUsedTypes, hedge::BitFlags,
        //hedge::HFAdj,		// pointer to the face
        //hedge::HOppAdj,	// pointer to the opposite edge
        //hedge::HVAdj,		// pointer to the vertex
        //hedge::HNextAdj,	// pointer to the next halfedge
        hedge::HEdgeData		// the previous 4 components (just more handy, you can comment this and uncomment the previous four lines)
        //,hedge::HPrevAdj	// pointer to the previous halfedge
>{};

class MyPFace:public vcg::Face<
        MyPUsedTypes
        ,vcg::face::PolyInfo // this is necessary  if you use component in vcg/simplex/face/component_polygon.h
        // It says "this class is a polygon and the memory for its components (e.g. pointer to its vertices
        // will be allocated dynamically")
        ,vcg::face::PFVAdj	 // Pointer to the vertices (just like FVAdj )
        ,vcg::face::PFVAdj
        ,vcg::face::PFFAdj	 // Pointer to edge-adjacent face (just like FFAdj )
        ,vcg::face::PFHAdj	 // Pointer its half -edges  ( you may need this if you use half edges)
        ,vcg::face::BitFlags // bit flags
        ,vcg::face::Normal3f // normal
> {};

class MyPMesh: public
             vcg::tri::TriMesh<
                     std::vector<MyPVertex>,	// the vector of vertices
                     std::vector<MyPFace >, 						// the vector of faces
                     std::vector<MyPHEdge>		,						// the vector of edges
                     std::vector<MyPEdge> 								// the vector of edges
             >{};

#endif //FILEEXCHANGE_MYMESH_H
