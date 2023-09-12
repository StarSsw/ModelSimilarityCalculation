#include <dirent.h>
#include "VolumeSimilarity.h"

Similarities ComputeSimilarity(const char *file1, const char *file2) {
    MyMesh mesh1,mesh2;
    // read stl Model
    ReadStlModel(file1,mesh1);
    ReadStlModel(file2,mesh2);
    // genConvexMesh
    MyMesh convexMesh1,convexMesh2;
    GenConvexHull(mesh1,convexMesh1);
    GenConvexHull(mesh2,convexMesh2);
    double volume1 = ComputeVolume(convexMesh1);
    double volume2 = ComputeVolume(convexMesh2);
    double surface1 = ComputeSurface(convexMesh1);
    double surface2 = ComputeSurface(convexMesh2);
    double resV = (volume1 == 0 || volume2 == 0) ? 0 : std::min(volume1,volume2)/std::max(volume1,volume2);
    double resS = (surface1 == 0 || surface2 == 0) ? 0 : std::min(surface1,surface2)/std::max(surface1,surface2);
    return {resS,resV,file1,file2};
}

vector<Similarities> ComputeSimilarityFromDir(const char *file, std::string dirpath) {
    vector<Similarities>res;
    vector<std::string>filenames;
    MyMesh mesh;
    ReadStlModel(file,mesh);
    MyMesh convexMesh;
    GenConvexHull(mesh,convexMesh);
    double volume1 = ComputeVolume(convexMesh);
    double surface1 = ComputeSurface(convexMesh);
    // open the dir and find all files
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(dirpath.c_str()))){
        cout<<"Folder doesn't Exist!"<<endl;
        return res;
    }
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0 ){
            if(ends_with(ptr->d_name,".stl"))
                filenames.push_back(dirpath + "/" + ptr->d_name);
        }
    }
    closedir(pDir);
    std::sort(filenames.begin(),filenames.end());
    for(auto filename : filenames)
    {
        MyMesh meshCompared;
        ReadStlModel(filename.c_str(),meshCompared);
        MyMesh convexMeshCompared;
        GenConvexHull(meshCompared,convexMeshCompared);
        double volumeCompared = ComputeVolume(convexMeshCompared);
        double surfaceCompared = ComputeSurface(convexMeshCompared);
        double resV = (volume1 == 0 || volumeCompared == 0) ? 0 : std::min(volume1,volumeCompared)/std::max(volume1,volumeCompared);
        double resS = (surface1 == 0 || surfaceCompared == 0) ? 0 : std::min(surface1,surfaceCompared)/std::max(surface1,surfaceCompared);
        res.push_back({resS,resV,file,filename});
    }
    return res;
}

double ComputeVolume(MyMesh &mesh) {
    // 计算体心
    Point3d vCentre = {0.0,0.0,0.0};
    int faceCount = 0;
    for(int i = 0 ; i < mesh.FN() ; i++) if(!mesh.face[i].IsD())
    {
        MyFace* f = &mesh.face[i];
        Point3d fCentre = {0.0,0.0,0.0};
        for(int j = 0 ; j < 3 ; j++)
        {
            fCentre += f->P(j);
        }
        fCentre = fCentre / 3.0;
        vCentre += fCentre;
        faceCount++;
    }
    vCentre = vCentre / (double)faceCount;
    // 计算每一个面片组成的三棱锥体积
    double volume = 0.0;
    for(int i = 0 ; i < mesh.FN() ; i++) if(!mesh.face[i].IsD())
    {
        MyFace* f = &mesh.face[i];
        volume += TriVolume(f,vCentre);
    }
    return volume;
}

double ComputeSurface(MyMesh &mesh) {
    double surfaceArea = 0.0;
    for(int i = 0 ; i < mesh.FN() ; i++) if(!mesh.face[i].IsD())
    {
        MyFace *f = &mesh.face[i];
        surfaceArea += TriArea(f);
    }
    return surfaceArea;
}

double OutputConvexHull(const char *input, const char *output) {
    MyMesh mesh;
    // read stl Model
    ReadStlModel(input,mesh);
    // genConvexMesh
    MyMesh convexMesh;
    GenConvexHull(mesh,convexMesh);
    vcg::tri::io::ExporterSTL<MyMesh>::Save(convexMesh,output,false);
    return 0;
}

void ReadStlModel(const char *filename,MyMesh &mesh) {
    int mask1;
    vcg::tri::io::ImporterSTL<MyMesh>::Open(mesh,filename,mask1);
    vcg::tri::Clean<MyMesh>::RemoveDuplicateVertex(mesh);
    vcg::tri::Clean<MyMesh>::RemoveDegenerateVertex(mesh);
    vcg::tri::Clean<MyMesh>::RemoveDuplicateFace(mesh);
    vcg::tri::Allocator<MyMesh>::CompactEveryVector(mesh);
}

void GenConvexHull(MyMesh &mesh, MyMesh &convexHull) {
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(mesh);
    bool result = vcg::tri::ConvexHull<MyMesh, MyMesh>::ComputeConvexHull(mesh, convexHull);
    if (!result)
        std::cerr << "can not gen a convex hull!" << std::endl;
}

double TriArea(MyFace *f) {
    Point3d A = f->P(0);
    Point3d B = f->P(1);
    Point3d C = f->P(2);
    Eigen::Vector3d AB = {B.X()-A.X(),B.Y()-A.Y(),B.Z()-A.Z()};
    Eigen::Vector3d AC = {C.X()-A.X(),C.Y()-A.Y(),C.Z()-A.Z()};
    // S = |AB x AC| * 0.5
    double area = 0.5 * AB.cross(AC).norm();
    return area;
}

double TriVolume(MyFace *f, Point3d &centre) {
    Point3d A = f->P(0);
    Point3d B = f->P(1);
    Point3d C = f->P(2);
    Eigen::Vector3d AB = {B.X()-A.X(),B.Y()-A.Y(),B.Z()-A.Z()};
    Eigen::Vector3d AC = {C.X()-A.X(),C.Y()-A.Y(),C.Z()-A.Z()};
    Eigen::Vector3d normal = AB.cross(AC).normalized();
    // AX+BY+CZ+D=0
    double D = -normal.x() * A.X() - normal.y() * A.Y() - normal.z() * A.Z();
    // distance = Ax + By + Cz + D
    double h = std::abs(centre.X() * normal.x() + centre.Y() * normal.y() + centre.Z() * normal.z() + D);
    double volume = TriArea(f) * h / 3.0;
    return volume;
}

int ends_with(const char *str, const char *suffix) {
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    if (str_len < suffix_len) {
        return 0; // 字符串长度小于后缀长度，不可能以后缀结尾
    }
    return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

void SaveResult(vector<Similarities> &results, const char *output) {
    FILE *wfp = fopen(output, "w+");
    if (NULL == wfp) {
        perror("The Write Operator is failure , can not open the file!");
        return;
    }
    char buff[2048] = {0};
    for(auto it : results)
    {
        sprintf(buff, "%s %s %lf %lf\n", it.file1.c_str(),it.file2.c_str(),it.areaSimilarity,it.volumeSimilarity);
        fputs(buff, wfp);
    }
    fclose(wfp);
}

void SaveResult(Similarities &result, const char *output) {
    FILE *wfp = fopen(output, "w+");
    if (NULL == wfp) {
        perror("The Write Operator is failure , can not open the file!");
        return;
    }
    char buff[2048] = {0};
    sprintf(buff, "%s %s %lf %lf\n", result.file1.c_str(),result.file2.c_str(),result.areaSimilarity,result.volumeSimilarity);
    fputs(buff, wfp);
    fclose(wfp);
}

void SaveResult(vector<ConvexHullAttr> &results, const char *output) {
    FILE *wfp = fopen(output, "w+");
    if (NULL == wfp) {
        perror("The Write Operator is failure , can not open the file!");
        return;
    }
    char buff[2048] = {0};
    for(auto it : results)
    {
        sprintf(buff, "%s %lf %lf\n", it.filename.c_str(),it.area,it.volume);
        fputs(buff, wfp);
    }
    fclose(wfp);
}

void SaveResult(ConvexHullAttr &result, const char *output) {
    FILE *wfp = fopen(output, "w+");
    if (NULL == wfp) {
        perror("The Write Operator is failure , can not open the file!");
        return;
    }
    char buff[2048] = {0};
    sprintf(buff, "%s %lf %lf\n", result.filename.c_str(),result.area,result.volume);
    fputs(buff, wfp);
    fclose(wfp);
}

ConvexHullAttr ComputeConvexHullAttr(const char *filename) {
    MyMesh mesh;
    // read stl Model
    ReadStlModel(filename,mesh);
    // genConvexMesh
    MyMesh convexMesh;
    GenConvexHull(mesh,convexMesh);
    double volume = ComputeVolume(convexMesh);
    double surface = ComputeSurface(convexMesh);
    return {surface,volume,filename};
}

vector<ConvexHullAttr> ComputeConvexHullAttrFromDir(std::string dirpath) {
    vector<std::string> filenames;
    vector<ConvexHullAttr> res;
    // open the dir and find all files
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(dirpath.c_str()))){
        cout<<"Folder doesn't Exist!"<<endl;
        return res;
    }
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0 ){
            if(ends_with(ptr->d_name,".stl"))
                filenames.push_back(dirpath + "/" + ptr->d_name);
        }
    }
    closedir(pDir);
    std::sort(filenames.begin(),filenames.end());
    for(auto it : filenames)
    {
        auto it_res = ComputeConvexHullAttr(it.c_str());
        res.push_back(it_res);
    }
    return res;
}
