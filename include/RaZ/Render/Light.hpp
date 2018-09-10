#pragma once

#ifndef RAZ_LIGHT_HPP
#define RAZ_LIGHT_HPP

#include <memory>

#include "RaZ/Math/Vector.hpp"
#include "RaZ/Math/Transform.hpp"

namespace Raz {

class Light;
using LightPtr = std::unique_ptr<Light>;

class PointLight;
using PointLightPtr = std::unique_ptr<PointLight>;

class DirectionalLight;
using DirectionalLightPtr = std::unique_ptr<DirectionalLight>;

class SpotLight;
using SpotLightPtr = std::unique_ptr<SpotLight>;

class Light : public Transform {
public:
  virtual Vec4f getHomogeneousPosition() const = 0;
  const Vec3f& getDirection() const { return m_direction; }
  Vec3f& getDirection() { return m_direction; }
  const Vec3f& getColor() const { return m_color; }
  Vec3f& getColor() { return m_color; }
  float getEnergy() const { return m_energy; }
  float getAngle() const { return m_angle; }

  void setEnergy(float energy) { m_energy = energy; }

  virtual ~Light() = default;

protected:
  Light(const Vec3f& position, float energy, const Vec3f& color) : m_energy{ energy }, m_color{ color } { m_position = position; }
  Light(const Vec3f& position, const Vec3f& direction, float energy, const Vec3f& color)
    : m_direction{ direction }, m_energy{ energy }, m_color{ color } { m_position = position; }
  Light(const Vec3f& position, const Vec3f& direction, float angle, float energy, const Vec3f& color)
    : m_direction{ direction }, m_angle{ angle }, m_energy{ energy }, m_color{ color } { m_position = position; }

  Vec3f m_direction {};
  float m_angle  = 0.f;
  float m_energy = 1.f;
  Vec3f m_color {};
};

class PointLight : public Light {
public:
  explicit PointLight(const Vec3f& position, float energy = 1.f, const Vec3f& color = Vec3f(1.f)) : Light(position, energy, color) {}

  Vec4f getHomogeneousPosition() const override { return Vec4f(m_position, 1.f); }

  template <typename... Args>
  static PointLightPtr create(Args&&... args) { return std::make_unique<PointLight>(std::forward<Args>(args)...); }
};

class DirectionalLight : public Light {
public:
  DirectionalLight(const Vec3f& direction, float energy = 1.f, const Vec3f& color = Vec3f(1.f), const Vec3f& position = Vec3f(0.f))
    : Light(position, direction, energy, color) {}

  Vec4f getHomogeneousPosition() const override { return Vec4f(m_position, 0.f); }

  template <typename... Args>
  static DirectionalLightPtr create(Args&&... args) { return std::make_unique<DirectionalLight>(std::forward<Args>(args)...); }
};

class SpotLight : public Light {
public:
  SpotLight(const Vec3f& position, const Vec3f& direction, float angle, float energy = 1.f, const Vec3f& color = Vec3f(1.f))
    : Light(position, direction, angle, energy, color) {}

  Vec4f getHomogeneousPosition() const override { return Vec4f(m_position, 1.f); }

  template <typename... Args>
  static SpotLight create(Args&&... args) { return std::make_unique<SpotLight>(std::forward<Args>(args)...); }
};

} // namespace Raz

#endif // RAZ_LIGHT_HPP
