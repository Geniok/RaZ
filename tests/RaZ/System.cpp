#include "catch/catch.hpp"
#include "RaZ/Entity.hpp"
#include "RaZ/Math/Transform.hpp"
#include "RaZ/Render/Mesh.hpp"
#include "RaZ/Utils/Window.hpp"
#include "RaZ/System.hpp"

class TestSystem : public Raz::System {
public:
  TestSystem() { m_acceptedComponents.setBit(Raz::Component::getId<Raz::Transform>()); } // [ 0 1 ]

  void update(float /* deltaTime */) override {}
};

TEST_CASE("System basic") {
  TestSystem testSystem {};

  // Window created to setup the OpenGL context, which Raz::Mesh needs to be instantiated
  // TODO: this window should not be created anymore when renderer will be externalized
  Raz::Window window(1, 1);

  Raz::EntityPtr mesh = Raz::Entity::create(0);
  mesh->addComponent<Raz::Mesh>();

  // If the system is supposed to contain the entity, link it
  // This operation is normally made into a World
  if (!(mesh->getEnabledComponents() & testSystem.getAcceptedComponents()).isEmpty())
    testSystem.linkEntity(mesh);

  REQUIRE_FALSE(testSystem.containsEntity(mesh));

  Raz::EntityPtr transform = Raz::Entity::create(1);
  transform->addComponent<Raz::Transform>();

  if (!(transform->getEnabledComponents() & testSystem.getAcceptedComponents()).isEmpty())
    testSystem.linkEntity(transform);

  REQUIRE(testSystem.containsEntity(transform));

  // Removing our Transform component, the entity shouldn't be processed by the system anymore
  transform->removeComponent<Raz::Transform>();

  // Unlink the entity if none of the components match
  if ((transform->getEnabledComponents() & testSystem.getAcceptedComponents()).isEmpty())
    testSystem.unlinkEntity(transform);

  REQUIRE_FALSE(testSystem.containsEntity(transform));

  // Creating an entity without components
  const Raz::EntityPtr emptyEntity = Raz::Entity::create(2);

  // The entity will not be linked since there's no component to be matched
  if (!(emptyEntity->getEnabledComponents() & testSystem.getAcceptedComponents()).isEmpty())
    testSystem.linkEntity(emptyEntity);

  REQUIRE_FALSE(testSystem.containsEntity(emptyEntity));
}
