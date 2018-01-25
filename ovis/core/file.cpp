#include <ovis/core/file.hpp>

#include <fstream>
#include <vector>

namespace ovis {
  
std::string LoadTextFile(const std::string& filename)
{
    std::ifstream file(filename);
    file.seekg(0, std::ios::end);
    
    std::vector<char> buffer(file.tellg());
    file.seekg(0, std::ios::beg);
    
    file.read(buffer.data(), buffer.size());
    
    return std::string(buffer.data(), buffer.data() + buffer.size());
}

}  // namespace ovis
