#include <iostream>
#include "VolumeSimilarity.h"
#include "vector"

void printHelp()
{
    printf("********************************** functions **********************************\n");
    printf("1 [file1] [file2] (output)\n");
    printf("    Sample : 1 /usr/download/exp.stl /usr/download/sim.stl\n");
    printf("    Then   : output the Similarity about area and volume between two files\n");
    printf("    Sample : 1 /usr/download/exp.stl /usr/download/sim.stl /usr/download/res.txt\n");
    printf("    Then   : output the Similarity about area and volume between two files and save the result into the res.txt\n");
    printf("2 [file1] [dirpath] (output)\n");
    printf("    Sample : 1 /usr/download/exp.stl /usr/download/models\n");
    printf("    Then   : output the Similarity about area and volume between file1 and all models in the dirpath\n");
    printf("    Sample : 1 /usr/download/exp.stl /usr/download/models /usr/download/res.txt\n");
    printf("    Then   : output the Similarity about area and volume between file1 and all models in the dirpath\n");
    printf("           : and save the result into the res.txt\n");
    printf("3 [file1] (output)\n");
    printf("    Sample : 3 /usr/download/a.stl\n");
    printf("    Then   : output the area and volume of the convex hull of the a model\n");
    printf("    Sample : 3 /usr/download/a.stl /usr/res.txt\n");
    printf("    Then   : output the area and volume of the convex hull of the a model and save the result into res.txt\n");
    printf("4 [dir] (output)\n");
    printf("    Sample : 4 /usr/download/models\n");
    printf("    Then   : output the area and volume of the convex hull of each model in dir\n");
    printf("    Sample : 4 /usr/download/models /usr/res.txt\n");
    printf("    Then   : output the area and volume of the convex hull of each model in dir and output the result into res.txt\n");
    printf("5 [file1] [output]\n");
    printf("    Sample : 5 /usr/download/a.stl /usr/download/output.stl\n");
    printf("    Then   : output the convex hull of a.stl into the output path\n");
    printf("******************************** functions end ********************************\n");
}

int main(int argc, char** argv) {
    if(argc == 1)
    {
        printHelp();
        return 0;
    }
    // 获取要实现什么功能
    char *func = argv[1];
    // 分割需要有5个argc才是正确的
    if(strcmp(func,"1") == 0 && argc > 3)
    {
        if(argc < 6)
        {
            char* srcFile = argv[2];
            char* destFile = argv[3];
            Similarities res = ComputeSimilarity(srcFile,destFile);
            std::cout << res.areaSimilarity << " " << res.volumeSimilarity << std::endl;
            if(argc == 5)
            {
                char* output = argv[4];
                SaveResult(res,output);
            }
        }
        else
        {
            printHelp();
        }
    }
    else if(strcmp(func,"2") == 0 && argc > 3)
    {
        if(argc < 6)
        {
            char* srcFile = argv[2];
            char* dirpath = argv[3];
            vector<Similarities> res = ComputeSimilarityFromDir(srcFile,dirpath);
            for(auto it : res)
            {
                std::cout << it.areaSimilarity << " " << it.volumeSimilarity << std::endl;
            }
            if(argc == 5)
            {
                char* output = argv[4];
                SaveResult(res,output);
            }
        }
        else
        {
            printHelp();
        }
    }
    else if(strcmp(func,"3") == 0 && argc > 2)
    {
        if(argc < 5)
        {
            char* srcFile = argv[2];
            ConvexHullAttr result = ComputeConvexHullAttr(srcFile);
            std::cout << result.area << " " << result.volume << std::endl;
            if(argc == 4)
            {
                char* output = argv[3];
                SaveResult(result,output);
            }
        }
        else
        {
            printHelp();
        }
    }
    else if(strcmp(func,"4") == 0 && argc > 2)
    {
        if(argc < 5)
        {
            char* dir = argv[2];
            vector<ConvexHullAttr> result = ComputeConvexHullAttrFromDir(dir);
            for(auto it : result)
            {
                std::cout << it.area << " " << it.volume << std::endl;
            }
            if(argc == 4)
            {
                char* output = argv[3];
                SaveResult(result,output);
            }
        }
        else
        {
            printHelp();
        }
    }
    else if(strcmp(func,"5") == 0)
    {
        if(argc == 4)
        {
            char* srcFile = argv[2];
            char* outFile = argv[3];
            OutputConvexHull(srcFile,outFile);
        }
        else
        {
            printHelp();
        }
    }
    else
    {
        printHelp();
    }
    return 0;
}
