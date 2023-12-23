#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>

std::vector<std::string> listFilesInDirectory(const std::string& path);
bool areFilesSame(const std::string& file1, const std::string& file2);
void copyFile(const std::string& sourcePath, const std::string& destPath);

#endif // FILE_OPERATIONS_H
