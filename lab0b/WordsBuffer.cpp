#include "WordsBuffer.h"

using namespace words_buffer;

WordsBuffer::WordsBuffer(std::ifstream &in, std::array<std::string, maxBufferSize> words): in(in) {}


