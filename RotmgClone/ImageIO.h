#pragma once

#include <vector>
#include <string>

unsigned int loadTexture(const char *path);

unsigned int loadCubemap(const char* path, const std::vector<std::string>& faces);