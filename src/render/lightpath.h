#ifndef SRC_RENDER_LIGHTPATH_H_
#define SRC_RENDER_LIGHTPATH_H_

#include <Eigen/Dense>
#include <vector>
#include <tuple>
#include "core/common.h"
#include "core/ray.h"
#include "core/properties.h"
using namespace Eigen;
using std::vector;

enum ShaderType { Diffuse, Specular, Transmissive, CameraShader };

struct PathPoint {
    Vector4d location;
    Vector4d normal;
    Properties properties;
    ShaderType shader;
};

struct CamPartialPath {
    bool exists;
    double samplex, sampley;
    ray cameraLocation;
    vector<PathPoint> bounces;
};

struct LightPartialPath {
    bool exists;
    Color emitted;
    Vector4d lightLocation;
    vector<PathPoint> bounces;
};

class LightPath {
 public:
    LightPath(Vector4d lightPoint, Color emittedLight,
            vector<PathPoint> bounces, Vector4d cameraPoint);
    LightPath(LightPartialPath lightPath, CamPartialPath camPath);

    std::tuple<Vector4d, Color> getLight();
    Vector4d getCameraPoint();
    Vector4d getCameraDirection();

    PathPoint getPoint(int index);
    // for a path point, get the direction that the light is coming from
    Vector4d getLightDirection(int index);
    // for a path point, get the direction that light is going out to
    Vector4d getViewDirection(int index);
    // for the segemnt (index - 1, index), calculate the geometric term
    double G(int index);
    // for the last segment leading into the camera, calculate the geometric
    // term
    double GCam();
    int size();

    /**
     * returns the eye side and light side partial paths that result from
     * deleting the path points from s to t
     *
     * the partial paths may be empty
     */
    std::tuple<LightPartialPath, CamPartialPath> deleteSubpath(int s, int t);

    void setClearPath(bool isclear);
    bool isClearPath();
    // record what part of the film this sample contributes to
    void setSampleLocation(double x, double y);
    // return the location of the sample on the film.
    // from 0 to 1
    double getSampleX();
    // return the location of the sample on the film.
    // from 0 to 1
    double getSampleY();

 private:
    vector<PathPoint> objectPoints;
    Vector4d lightLocation;
    Color emitted;
    Vector4d cameraLocation;
    double sampleX;
    double sampleY;
    bool clearPath;
};


#endif  // SRC_RENDER_LIGHTPATH_H_
