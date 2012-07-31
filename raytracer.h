/**
 * @brief Class that maintains the scene objects and renders the scene.
 * Only renders individual rays. Pictures are rendered in a camera object,
 * wich would use this class to render a PNG.\n
 * NOTE: This class is pretty experimental. I don't really like how its set up and would like to change it.
 * For now I will just trust that the user of the class can manage the Drawable pointers properly.
 */

#include <vector>

#include "drawable.h"
#include "easypng.h"
#include "light.h"
#include "ray.h"

using std::vector;

class Raytracer
{
	public:
	/**
	 * @brief This is a list of all the objects in the scene.
	 * It is public and lets the user manage the memory how they want.\n
	 * NOTE: When the Raytracer gets deleted, it will not delete the objects in the list.
	 * This is left for the user to decide.
	 */
	vector<Drawable*> objList;
	/// This is a list of all the lights in the scene.
	vector<light> lightList;

	/// Clears the lights and the objects.
	void clear_scene();
	/// Deletes the objects and removes their pointers from the vector.
	void clear_objects();
	/// Clears the lights from the lightList
	void clear_lights();

	/// Renders the given ray and returns it's Color.
	Color getColor(const ray<3>& viewRay, int depth);

	private:

	/// Color set aside for debugging purposes.
	static const Color DEBUG_COLOR;

	/**
	 * @brief Helper function that finds the closest object and time for the given ray.
	 * This is mostly to improve the readablilty of the code.
	 * @pre closestObj and bestTime are negative values.
	 * @post closestObj is the index of the closest object (or negative if there is no intersecions) and bestTime is the time to intersection.
	 *
	 * @param closestObj The reference to the intex of the closest object intersection.
	 * @param bestTime The refecrence to the time for the ray to reach the intersection.
	 */
	void getClosestObject( const ray<3>& viewRay, int& closestObj, double& bestTime);
};