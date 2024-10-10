#ifndef REDATAMLIB_BITARRREADER_HPP
#define REDATAMLIB_BITARRREADER_HPP

#include <vector>
#include <bitset>

#include <cstdint>      //  uint16_t, uint32_t

namespace RedatamLib {
using std::bitset;
using std::vector;

class BitArrayReader
{
public:
    BitArrayReader(size_t dataSize);
    ~BitArrayReader() = default;

    BitArrayReader(const BitArrayReader&) = delete;
    BitArrayReader& operator=(const BitArrayReader&) = delete;
    
    void ParseBits(vector<uint32_t>* results, uint32_t data);

private:
    size_t m_varSize;
    size_t m_remainderSize;
    bitset<32> m_mask;
    bitset<32> m_data;
    bitset<32> m_remainder;

    bitset<32> CreateMask(size_t size);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_BITARRREADER_HPP