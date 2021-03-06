#include <fstream>
#include <iostream>

#include "RaZ/Utils/FileUtils.hpp"
#include "RaZ/Utils/Image.hpp"
#include "RaZ/Utils/StrUtils.hpp"

namespace Raz {

void Image::read(const std::string& filePath, bool reverse) {
  std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);

  if (file) {
    const std::string format = StrUtils::toLowercaseCopy(FileUtils::extractFileExtension(filePath));

    if (format == "png")
      readPng(file, reverse);
    else if (format == "tga")
      readTga(file);
    else
      std::cerr << "Warning: '" + format + "' format is not supported, image ignored" << std::endl;
  } else {
    std::cerr << "Error: Couldn't open the file '" + filePath + "'" << std::endl;
  }
}

void Image::save(const std::string& filePath, bool reverse) const {
  std::ofstream file(filePath, std::ios_base::out | std::ios_base::binary);

  if (file) {
    const std::string format = StrUtils::toLowercaseCopy(FileUtils::extractFileExtension(filePath));

    if (format == "png")
      savePng(file, reverse);
    /*else if (format == "tga")
      saveTga(file);*/
    else
      std::cerr << "Warning: '" + format + "' format is not supported, image ignored" << std::endl;
  } else {
    throw std::runtime_error("Error: Unable to create a file as '" + filePath + "'; path to file must exist");
  }
}

} // namespace Raz
