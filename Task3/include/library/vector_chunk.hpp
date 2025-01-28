#ifndef VECTOR_CHUNK_HPP
#define VECTOR_CHUNK_HPP

#include <vector>

template<typename T>
struct VectorChunk
{
    std::vector<T>::const_iterator begin;
    std::vector<T>::const_iterator end;
};

template<typename T>
std::vector<VectorChunk<T>> split(const std::vector<T>& data, std::size_t parts)
{
    std::vector<VectorChunk<T>> chunks{};

    if(std::size(data) < parts)
    {
        chunks.emplace_back(std::begin(data), std::end(data));
        return chunks;
    }

    auto step{std::size(data) / parts};
    for(std::size_t i{0}; i < std::size(data); i += step) 
    {
        auto begin{std::begin(data) + i};
        auto end{std::begin(data) + std::min(std::size(data), i + step)};
        chunks.emplace_back(begin, end);
    }

    return chunks;
}

#endif