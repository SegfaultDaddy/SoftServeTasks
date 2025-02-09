#ifndef VECTOR_CHUNK_HPP
#define VECTOR_CHUNK_HPP

#include <vector>
#include <print>

template<typename T>
struct VectorChunk
{
    std::vector<T>::const_iterator begin;
    std::vector<T>::const_iterator end;
};

template<typename T>
std::vector<VectorChunk<T>> split(const std::vector<T>& data, const std::size_t parts)
{
    std::vector<VectorChunk<T>> chunks{};
    if(std::size(data) < parts)
    {
        chunks.emplace_back(std::begin(data), std::end(data));
        return chunks;
    }
    auto step{std::size(data) / parts};
    for(std::size_t i{0}; i < parts - 1; i += 1) 
    {
        auto begin{std::begin(data) + (i) * step};
        auto end{std::begin(data) + (i + 1) * step};
        chunks.emplace_back(begin, end);
    }
    auto begin{std::begin(data) + (parts - 1) * step};
    auto end{std::begin(data) + std::max(std::size(data), parts * step)};
    chunks.emplace_back(begin, end);
    return chunks;
}

#endif