#pragma once

class Version {
public:
    int major;
    int minor;
    int patch;

    // Constructors
    Version();
    Version(int maj, int min, int pat);
    Version(const std::string& versionStr);

    // Comparison: returns -1 if this < other, 0 if equal, 1 if this > other
    int Compare(const Version& other) const;

    // Operator overloads
    bool operator==(const Version& other) const;
    bool operator!=(const Version& other) const;
    bool operator<(const Version& other) const;
    bool operator<=(const Version& other) const;
    bool operator>(const Version& other) const;
    bool operator>=(const Version& other) const;

    // Utility
    friend std::ostream& operator<<(std::ostream& os, const Version& v);
};
