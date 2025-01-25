#include "any_type.hpp"

AnyType::AnyType()
    : data_{nullptr}
{
}

AnyType::AnyType(const AnyType& other)
    : data_{(other.data_ != nullptr)? other.data_->clone() : nullptr}
{
}

AnyType::AnyType(AnyType&& other)
    : data_{other.data_}
{
    other.data_ = nullptr;
}

AnyType::~AnyType()
{
    delete data_;
}

AnyType& AnyType::swap(AnyType& other)
{
    auto copy{data_};
    data_ = other.data_;
    other.data_ = copy;
    return *this;
}

AnyType& AnyType::operator=(const AnyType& other)
{
    AnyType{other}.swap(*this);
    return *this;
}

AnyType& AnyType::operator=(AnyType&& other)
{
    other.swap(*this);
    AnyType{}.swap(other);
    return *this;
}

bool AnyType::has_value() const noexcept
{
    return data_ != nullptr;
}

const std::type_info& AnyType::type() const noexcept
{
    return data_->type();
}

void AnyType::reset() noexcept
{
    AnyType{}.swap(*this);
    data_ = nullptr;
}