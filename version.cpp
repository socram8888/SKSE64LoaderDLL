
#include "pch.h"
#include "version.h"

Version::Version() : major(0), minor(0), patch(0) {}

Version::Version(int maj, int min, int pat)
    : major(maj), minor(min), patch(pat) {}

Version::Version(const std::string& versionStr) {
    std::stringstream ss(versionStr);
    std::string token;

    try {
        if (!std::getline(ss, token, '.')) throw std::invalid_argument("Invalid version string");
        major = std::stoi(token);

        if (!std::getline(ss, token, '.')) throw std::invalid_argument("Invalid version string");
        minor = std::stoi(token);

        if (!std::getline(ss, token, '.')) throw std::invalid_argument("Invalid version string");
        patch = std::stoi(token);
    }
    catch (const std::exception&) {
        throw std::invalid_argument("Failed to parse version string: " + versionStr);
    }
}

int Version::Compare(const Version& other) const {
    if (major != other.major) return (major < other.major) ? -1 : 1;
    if (minor != other.minor) return (minor < other.minor) ? -1 : 1;
    if (patch != other.patch) return (patch < other.patch) ? -1 : 1;
    return 0;
}

bool Version::operator==(const Version& other) const { return Compare(other) == 0; }
bool Version::operator!=(const Version& other) const { return Compare(other) != 0; }
bool Version::operator<(const Version& other) const { return Compare(other) == -1; }
bool Version::operator<=(const Version& other) const { return Compare(other) <= 0; }
bool Version::operator>(const Version& other) const { return Compare(other) == 1; }
bool Version::operator>=(const Version& other) const { return Compare(other) >= 0; }

std::ostream& operator<<(std::ostream& os, const Version& v) {
    os << v.major << "." << v.minor << "." << v.patch;
    return os;
}
