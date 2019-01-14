#pragma once

#ifndef RAZ_SHAPE_HPP
#define RAZ_SHAPE_HPP

#include "RaZ/Component.hpp"
#include "RaZ/Math/Vector.hpp"

namespace Raz {

class Line;
class Plane;
class Sphere;
class Triangle;
class Quad;
class AABB;

class Shape : public Component {
public:
  /// Point containment check.
  /// \param point Point to be checked.
  /// \return True if the point is contained by the shape, false otherwise.
  virtual bool contains(const Vec3f& point) const = 0;
  /// Shape-line intersection check.
  /// \param line Line to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const Line& line) const = 0;
  /// Shape-plane intersection check.
  /// \param plane Plane to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const Plane& plane) const = 0;
  /// Shape-sphere intersection check.
  /// \param sphere Sphere to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const Sphere& sphere) const = 0;
  /// Shape-triangle intersection check.
  /// \param triangle Triangle to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const Triangle& triangle) const = 0;
  /// Shape-quad intersection check.
  /// \param quad Quad to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const Quad& quad) const = 0;
  /// Shape-AABB intersection check.
  /// \param aabb AABB to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  virtual bool intersects(const AABB& aabb) const = 0;
  /// Computes the projection of a point (closest point) onto the shape.
  /// \param point Point to compute the projection from.
  /// \return Point projected onto the shape.
  virtual Vec3f computeProjection(const Vec3f& point) const = 0;
  /// Computes the shape's centroid.
  /// \return Computed centroid.
  virtual Vec3f computeCentroid() const = 0;
};

/// Line segment defined by its two extremities' positions.
class Line : public Shape {
public:
  Line() = default;
  Line(const Vec3f& beginPos, const Vec3f& endPos) : m_beginPos{ beginPos }, m_endPos{ endPos } {}

  const Vec3f& getBeginPos() const { return m_beginPos; }
  const Vec3f& getEndPos() const { return m_endPos; }

  /// Point containment check.
  /// \param point Point to be checked.
  /// \return True if the point is located on the line, false otherwise.
  bool contains(const Vec3f& point) const override { return computeProjection(point) == point; }
  /// Line-line intersection check.
  /// \param line Line to check if there is an intersection with.
  /// \return True if both lines intersect each other, false otherwise.
  bool intersects(const Line& line) const override;
  /// Line-plane intersection check.
  /// \param plane Plane to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Plane& plane) const override;
  /// Line-sphere intersection check.
  /// \param sphere Sphere to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Sphere& sphere) const override;
  /// Line-triangle intersection check.
  /// \param triangle Triangle to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Triangle& triangle) const override;
  /// Line-quad intersection check.
  /// \param quad Quad to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Quad& quad) const override;
  /// Line-AABB intersection check.
  /// \param aabb AABB to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const AABB& aabb) const override;
  /// Computes the projection of a point (closest point) onto the line.
  /// The projected point is necessarily located on the line.
  /// \param point Point to compute the projection from.
  /// \return Point projected onto the line.
  Vec3f computeProjection(const Vec3f& point) const override;
  /// Computes the line's centroid, which is the point lying directly between the two extremities.
  /// \return Computed centroid.
  Vec3f computeCentroid() const override { return (m_beginPos + m_endPos) / 2.f; }
  /// Line length computation.
  /// To be used if actual length is needed; otherwise, prefer computeSquaredLength().
  /// \return Line's length.
  float computeLength() const { return (m_endPos - m_beginPos).computeLength(); }
  /// Line squared length computation.
  /// To be preferred over computeLength() for faster operations.
  /// \return Line's squared length.
  float computeSquaredLength() const { return (m_endPos - m_beginPos).computeSquaredLength(); }

private:
  Vec3f m_beginPos {};
  Vec3f m_endPos {};
};

/// Plane defined by a distance from [ 0; 0; 0 ] and a normal.
class Plane : public Shape {
public:
  explicit Plane(float distance, const Vec3f& normal = Axis::Y) : m_distance{ distance }, m_normal{ normal } {}
  explicit Plane(const Vec3f& position, const Vec3f& normal = Axis::Y) : m_distance{ position.computeLength() }, m_normal{ normal } {}
  Plane(const Vec3f& firstPoint, const Vec3f& secondPoint, const Vec3f& thirdPoint)
    : m_distance{ ((firstPoint + secondPoint + thirdPoint) / 3.f).computeLength() },
      m_normal{ (secondPoint - firstPoint).cross(thirdPoint - firstPoint).normalize() } {}

  float getDistance() const { return m_distance; }
  const Vec3f& getNormal() const { return m_normal; }

  /// Point containment check.
  /// \param point Point to be checked.
  /// \return True if the point is located on the plane, false otherwise.
  bool contains(const Vec3f& point) const override { return FloatUtils::checkNearEquality(m_normal.dot(point) - m_distance, 0.f); }
  /// Plane-line intersection check.
  /// \param line Line to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Line& line) const override { return line.intersects(*this); }
  /// Plane-plane intersection check.
  /// \param plane Plane to check if there is an intersection with.
  /// \return True if both planes intersect each other, false otherwise.
  bool intersects(const Plane& plane) const override;
  /// Plane-sphere intersection check.
  /// \param sphere Sphere to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Sphere& sphere) const override;
  /// Plane-triangle intersection check.
  /// \param triangle Triangle to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Triangle& triangle) const override;
  /// Plane-quad intersection check.
  /// \param quad Quad to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Quad& quad) const override;
  /// Plane-AABB intersection check.
  /// \param aabb AABB to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const AABB& aabb) const override;
  /// Computes the projection of a point (closest point) onto the plane.
  /// The projected point is necessarily located on the plane.
  /// \param point Point to compute the projection from.
  /// \return Point projected onto the plane.
  Vec3f computeProjection(const Vec3f& point) const override { return point - m_normal * (m_normal.dot(point) - m_distance); }
  /// Computes the plane's centroid, which is the point lying onto the plane at its distance from the center in its normal direction.
  /// \return Computed centroid.
  Vec3f computeCentroid() const override { return m_normal * m_distance; }

private:
  float m_distance {};
  Vec3f m_normal {};
};

/// Triangle defined by its three vertices' positions, presumably in counter-clockwise order.
class Triangle : public Shape {
public:
  Triangle() = default;
  Triangle(const Vec3f& firstPos, const Vec3f& secondPos, const Vec3f& thirdPos)
    : m_firstPos{ firstPos }, m_secondPos{ secondPos }, m_thirdPos{ thirdPos } {}

  const Vec3f& getFirstPos() const { return m_firstPos; }
  const Vec3f& getSecondPos() const { return m_secondPos; }
  const Vec3f& getThirdPos() const { return m_thirdPos; }

  /// Point containment check.
  /// \param point Point to be checked.
  /// \return True if the point is located on the triangle, false otherwise.
  bool contains(const Vec3f& point) const override { return (computeProjection(point) == point); }
  /// Triangle-line intersection check.
  /// \param line Line to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Line& line) const override { return line.intersects(*this); }
  /// Triangle-plane intersection check.
  /// \param plane Plane to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Plane& plane) const override { return plane.intersects(*this); }
  /// Triangle-sphere intersection check.
  /// \param sphere Sphere to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Sphere& sphere) const override { return sphere.intersects(*this); }
  /// Triangle-triangle intersection check.
  /// \param triangle Triangle to check if there is an intersection with.
  /// \return True if both triangles intersect each other, false otherwise.
  bool intersects(const Triangle& triangle) const override;
  /// Triangle-quad intersection check.
  /// \param quad Quad to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Quad& quad) const override;
  /// Triangle-AABB intersection check.
  /// \param aabb AABB to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const AABB& aabb) const override;
  /// Computes the projection of a point (closest point) onto the triangle.
  /// The projected point is necessarily located on the triangle's surface.
  /// \param point Point to compute the projection from.
  /// \return Point projected onto the triangle.
  Vec3f computeProjection(const Vec3f& point) const override;
  /// Computes the triangle's centroid, which is the point lying directly between its three points.
  /// \return Computed centroid.
  Vec3f computeCentroid() const override { return (m_firstPos + m_secondPos + m_thirdPos) / 3.f; }

private:
  Vec3f m_firstPos {};
  Vec3f m_secondPos {};
  Vec3f m_thirdPos {};
};

/// Quad defined by its four vertices' positions, presumably in counter-clockwise order.
class Quad : public Shape {
public:
  Quad() = default;
  Quad(const Vec3f& leftTopPos, const Vec3f& rightTopPos, const Vec3f& rightBottomPos, const Vec3f& leftBottomPos)
    : m_leftTopPos{ leftTopPos }, m_rightTopPos{ rightTopPos }, m_rightBottomPos{ rightBottomPos }, m_leftBottomPos{ leftBottomPos } {}

  const Vec3f& getLeftTopPos() const { return m_leftTopPos; }
  const Vec3f& getRightTopPos() const { return m_rightTopPos; }
  const Vec3f& getRightBottomPos() const { return m_rightBottomPos; }
  const Vec3f& getLeftBottomPos() const { return m_leftBottomPos; }

  /// Point containment check.
  /// \param point Point to be checked.
  /// \return True if the point is located on the quad, false otherwise.
  bool contains(const Vec3f&) const override { throw std::runtime_error("Error: Not implemented yet."); }
  /// Quad-line intersection check.
  /// \param line Line to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Line& line) const override { return line.intersects(*this); }
  /// Quad-plane intersection check.
  /// \param plane Plane to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Plane& plane) const override { return plane.intersects(*this); }
  /// Quad-sphere intersection check.
  /// \param sphere Sphere to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Sphere& sphere) const override { return sphere.intersects(*this); }
  /// Quad-triangle intersection check.
  /// \param triangle Triangle to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const Triangle& triangle) const override { return triangle.intersects(*this); }
  /// Quad-quad intersection check.
  /// \param quad Quad to check if there is an intersection with.
  /// \return True if both quads intersect each other, false otherwise.
  bool intersects(const Quad& quad) const override;
  /// Quad-AABB intersection check.
  /// \param aabb AABB to check if there is an intersection with.
  /// \return True if both shapes intersect each other, false otherwise.
  bool intersects(const AABB& aabb) const override;
  /// Computes the projection of a point (closest point) onto the quad.
  /// The projected point is necessarily located on the quad's surface.
  /// \param point Point to compute the projection from.
  /// \return Point projected onto the quad.
  Vec3f computeProjection(const Vec3f& point) const override;
  /// Computes the quad's centroid, which is the point lying directly between its four points.
  /// \return Computed centroid.
  Vec3f computeCentroid() const override { return (m_leftTopPos + m_rightTopPos + m_rightBottomPos + m_leftBottomPos) / 4.f; }

private:
  Vec3f m_leftTopPos {};
  Vec3f m_rightTopPos {};
  Vec3f m_rightBottomPos {};
  Vec3f m_leftBottomPos {};
};

struct AABB {
  AABB() = default;
  AABB(const Vec3f& rightTopFrontPos, const Vec3f& leftBottomBackPos)
    : rightTopFrontPos{ rightTopFrontPos }, leftBottomBackPos{ leftBottomBackPos } {}

  Vec3f rightTopFrontPos {};
  Vec3f leftBottomBackPos {};
};

} // namespace Raz

#endif // RAZ_SHAPE_HPP
