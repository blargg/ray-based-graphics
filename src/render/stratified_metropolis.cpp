#include "render/stratified_metropolis.h"
#include "render/metropolis_light_transport.h"

StratifiedMetropolis::StratifiedMetropolis() {
}

void StratifiedMetropolis::setCamera(Camera c) {
    cam = c;
}

void StratifiedMetropolis::setObjects(vector<Drawable *> objList) {
    objTree.rebuildTree(objList);
}

void StratifiedMetropolis::setLightStrata(vector< vector<Drawable *> > strata) {
    lightStrata = strata;
}

void StratifiedMetropolis::setStrataFromVector(vector<Drawable *> lights) {
    lightStrata.clear();
    for (int i = 0; i < lights.size(); i++) {
        vector<Drawable *> stratum;
        stratum.push_back(lights[i]);
        lightStrata.push_back(stratum);
    }
}

void StratifiedMetropolis::sampleImage(Film *imageFilm, int numSamples) {
    int width = imageFilm->getWidth();
    int height = imageFilm->getHeight();

    int numSamplesPerStratum = numSamples / lightStrata.size();
    for (vector<Drawable *> &stratum : lightStrata) {
        Film currentSample(width, height);
        MetropolisRenderer m;
        m.setCamera(cam);
        m.setObjectsByKDTree(objTree);
        m.setLights(stratum);
        m.sampleImage(&currentSample, numSamplesPerStratum);

        imageFilm->addSamples(currentSample);
    }
}
