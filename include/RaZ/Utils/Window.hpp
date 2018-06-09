#pragma once

#ifndef RAZ_WINDOW_HPP
#define RAZ_WINDOW_HPP

#include <functional>
#include <memory>
#include <vector>

#include "glew/include/GL/glew.h"
#if defined(_WIN32)
#include "glew/include/GL/wglew.h"
#elif defined(__gnu_linux__)
#include "glew/include/GL/glxew.h"
#endif
#include "glfw/include/GLFW/glfw3.h"
#include "RaZ/Utils/Image.hpp"
#include "RaZ/Utils/Keyboard.hpp"

namespace Raz {

using KeyCallbacks = std::vector<std::pair<int, std::function<void()>>>;

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string& title = "", uint8_t AASampleCount = 1);

  unsigned int getWidth() const { return m_width; }
  unsigned int getHeight() const { return m_height; }

  void setTitle(const std::string& title) const { glfwSetWindowTitle(m_window, title.c_str()); }
  void setIcon(const Image& img) const;
  void setIcon(const std::string& fileName) const { setIcon(Image(fileName, true)); }
  void enableFaceCulling(bool value = true) const;
  void disableFaceCulling() const { enableFaceCulling(false); }
  void enableVerticalSync(bool value = true) const;
  void disableVerticalSync() const { enableVerticalSync(false); }
  void addKeyCallback(Keyboard::Key key, std::function<void()> func);
  void updateKeyCallbacks() const;
  bool run() const;
  void close() const { glfwTerminate(); }

  ~Window() { close(); }

private:
  unsigned int m_width {};
  unsigned int m_height {};
  KeyCallbacks m_keyCallbacks {};
  GLFWwindow* m_window {};
};

using WindowPtr = std::unique_ptr<Window>;

} // namespace Raz

#endif // RAZ_WINDOW_HPP
