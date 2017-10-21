#pragma once

#ifndef RAZ_TEXTURE_HPP
#define RAZ_TEXTURE_HPP

#include <memory>
#include <string>

#include "GL/glew.h"
#include "RaZ/Utils/Image.hpp"

namespace Raz {

class Texture {
public:
  Texture() { glGenTextures(1, &m_index); }
  Texture(const std::string& fileName) : Texture() { load(fileName); }

  GLuint getIndex() const { return m_index; }
  const Image& getImage() const { return m_image; }

  void load(const std::string& fileName);
  void bind() const { glBindTexture(GL_TEXTURE_2D, m_index); }

  ~Texture() { glDeleteTextures(1, &m_index); }

private:
  GLuint m_index;
  Image m_image;
};

using TexturePtr = std::shared_ptr<Texture>;

} // namespace Raz

#endif // RAZ_TEXTURE_HPP
