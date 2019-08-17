#include "project/project.h"

namespace ss {

const std::string& ToString(ValueType type) {
	static const std::string strings[] = { "bool", "int", "float", "Vec3" };
	return strings[int(type)];
}

std::string FindParentPath(const std::string& path) {
	size_t last_slash = path.find_last_of("\\/");
	return last_slash == std::string::npos ? "" : path.substr(0, last_slash);
}

std::string MakePath(const std::string& path, const std::string& filename) {
	return path.empty() ? filename : (path + "/" + filename);
}

}