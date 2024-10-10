//  replaces DataBlock.cs
#ifndef REDATAMLIB_BYTEARRREADER_HPP
#define REDATAMLIB_BYTEARRREADER_HPP

#include <stdexcept>    //  std::out_of_range, std::length_error
#include <vector>
#include <string>
#include <cstddef>      //  std::byte, size_t
#include <cstdint>      //  uint16_t, uint32_t

namespace RedatamLib {
using std::length_error;
using std::out_of_range;
using std::string;
using std::vector;

class ByteArrayReader
{
public:
    ByteArrayReader();
    //  throws std::ios_base::failure if fails to open file
    ByteArrayReader(const string& filePath);

    ~ByteArrayReader() = default;

    size_t GetPos() const;
    size_t GetEndPos() const;
    
    //  throws std::out_of_range
    void SetPos(int newPos);
    void MovePos(int bytes);
    void MovePosTo(const string& subArr);

    //  str's length is between 0 and 128 bytes
    //  filterByContent checks if output is an alpha-numeric string (including whitespaces)
    bool TryReadStr(string* output, bool filterByContent = true);
    
    //  throws std::out_of_range
    string ReadString(size_t length);

    //  throws std::out_of_range if there isn't such string
    //  used to find variable names
    string GetFormerString();

    //  throws std::out_of_range; LE = little-endian, BE = big-endian
    unsigned char ReadByte();
    uint16_t ReadInt16LE();
    uint32_t ReadInt32LE();
    uint16_t ReadInt16BE();
    uint32_t ReadInt32BE();

private:
    vector<unsigned char> m_data;
    size_t m_currPos;
    size_t m_endPos;

    size_t FindNextMatch(const vector<unsigned char>& subArr, size_t len, size_t startPos);
    bool IsValidStr(const string& str);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_BYTEARRREADER_HPP