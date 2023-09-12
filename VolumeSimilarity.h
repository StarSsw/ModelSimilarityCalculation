
#ifndef CONVEXVOLUME_VOLUMESIMILARITY_H
#define CONVEXVOLUME_VOLUMESIMILARITY_H

#include "mymesh.h"
#include "wrap/io_trimesh/import_stl.h"
#include "wrap/io_trimesh/export_obj.h"
#include "wrap/io_trimesh/export_stl.h"
#include <vcg/complex/algorithms/convex_hull.h>

typedef struct Similarities{
    double areaSimilarity;
    double volumeSimilarity;
    std::string file1;
    std::string file2;
}Similarities;

typedef struct ConvexHullAttr{
    double area;
    double volume;
    std::string filename;
}ConvexHullAttr;

// Calculate the volume similarity between two files
Similarities ComputeSimilarity(const char* file1,const char* file2);
// Compute a single model and all models under a folder
vector<Similarities> ComputeSimilarityFromDir(const char* file,std::string dirpath);
// Calculate the convexHull's attrs of a model
ConvexHullAttr ComputeConvexHullAttr(const char* filename);
vector<ConvexHullAttr> ComputeConvexHullAttrFromDir(std::string dirpath);
// Calculate the volume of a single model convex hull
double ComputeVolume(MyMesh &mesh);
// Calculate the surface area of a single model convex hull
double ComputeSurface(MyMesh &mesh);
// Output the obj file corresponding to the convex hull of the model
double OutputConvexHull(const char* input,const char* output);
// Read the model
void ReadStlModel(const char* filename,MyMesh &mesh);
// Generate the Convex hull
void GenConvexHull(MyMesh &mesh, MyMesh &convexHull);
// Calculate the triangle area
double TriArea(MyFace* f);
// Calculate the volume of the pyramid
double TriVolume(MyFace *f,Point3d &centre);

// save result
void SaveResult(vector<Similarities> &results,const char* output);
void SaveResult(Similarities &result,const char* output);
void SaveResult(vector<ConvexHullAttr> &results,const char* output);
void SaveResult(ConvexHullAttr &result,const char* output);
// check the files in the dir is stl or not
int ends_with(const char* str, const char* suffix);

#endif //CONVEXVOLUME_VOLUMESIMILARITY_H
