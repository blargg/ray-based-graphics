#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include "easypng.h"

using namespace std;

class animation {
  public:
    void addFrame(PNG const & img);
    void write(std::string filename);

  private:
    std::vector<PNG> frames;

	template <typename T>
	string to_string(const T & value);
	string getString(int i, int padToSameLengthAs);
	bool exists(const string & path);
};

#endif
