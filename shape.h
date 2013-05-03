#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "vectre.h"
#include "point.h"

/**
 * @class Shape
 * @ingroupe Shapes
 *
 * @brief Virtual class for a 3d shape.
 * Allows objects to be moved and rotated. Defines the intersections with
 * rays.
 */
class Shape
{
	protected:
	/**
	 * @brief The location of the object 
	 */
	point<3> location;

	/**
	 * @brief The rotaion of the object around it's location in radians.\n
	 * rotation[0] is around the x-axis.\n
	 * rotation[1] is around the y-axis.\n
	 * rotation[2] is around the z-axis.
	 */
	point<3> rotation;

	public:
	/// Default constructor. Does nothing.
	Shape();

	Shape(const point<3> loc);

	/// Makes a shape at a certain point and rotation.
	Shape(const point<3> loc, const point<3> rotation);

	/// Virtual default constructor for Shapes
	virtual Shape* create() const = 0;
	/// Virtual copy constructor for Shapes.
	virtual Shape* clone() const = 0;

	/// Virtual deconstructor.
	virtual ~Shape(){ /* Do nothing */ }

	/**
	 * @brief Moves the shape to the given location.
	 * @param newLoc The location to move to.
	 */
	void move(const point<3>& newLoc);

	/**
	 * @brief Moves the shape in the given direction.
	 * @param dir The direction to move the object.
	 */
	void move(const vectre<3>& dir);

	/**
	 * @brief Sets the rotation to the given rotation.
	 * @param newRotation The rotation to align to.
	 */
	void setRotation(const point<3>& newRotation);

	/**
	 * @brief Rotate the shape by the given set of angles.
	 *
	 * Rotates the object realative to the current rotation.
	 *
	 * @param angles The angles to rotate the object by.
	 */
	void rotate(const point<3>& angles);

	/// Simply returns the location of the object
	point<3> getLocation() const;

	/// Gives the closest intersection in front of the ray.
	virtual double intersection(const ray<3>& viewRay) const = 0;
	/// Gives the normal vectre for the given point.
	virtual vectre<3> normal_vectre(const point<3>& surface) const = 0;

};
#endif
