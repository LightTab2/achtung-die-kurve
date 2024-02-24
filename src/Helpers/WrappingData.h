#pragma once
#include <concepts>

#include <gsl/gsl>

template <std::integral dataType>
class WrappingData final
{
    friend constexpr WrappingData<dataType> operator+(WrappingData<dataType> lhs, const WrappingData<dataType>& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend constexpr WrappingData<dataType> operator-(WrappingData<dataType> lhs, const WrappingData<dataType>& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend constexpr WrappingData<dataType> operator+(WrappingData<dataType> lhs, dataType rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend constexpr WrappingData<dataType> operator-(WrappingData<dataType> lhs, dataType rhs)
    {
        lhs -= rhs;
        return lhs;
    }
public:
    // Constructor
    ///
    /// \pre Expects(boundMax >= boundMin);
    constexpr WrappingData(dataType boundMin = 0, dataType boundMax = 1, dataType init = 0)
        : data_{ init },
          boundMin_{ boundMin },
          boundMax_{ boundMax },
          modulo_{ boundMax - boundMin + 1 }
    {
        Expects(boundMax >= boundMin);
        if (data_ < boundMin_)
            data_ = boundMin_;
        else if (data_ > boundMax_)
            data_ = boundMax_;
        Ensures(modulo_ > 0);
    }

    // Allow copy
    constexpr WrappingData(const WrappingData&)      = default;
    constexpr WrappingData& operator=(WrappingData&) = default;
    // Allow move
    constexpr WrappingData(WrappingData&&)            noexcept = default;
    constexpr WrappingData& operator=(WrappingData&&) noexcept = default;
    // Destructor
    ~WrappingData() = default;

    // Explicit and Implicit conversion
    constexpr operator dataType() const
    {
        return data_;
    }

    constexpr WrappingData<dataType>& operator++()
    {
        if (++data_ > boundMax_)
            data_ = boundMin_;
        return *this;
    }

    constexpr WrappingData<dataType> operator++(int)
    {
        WrappingData copyOld(*this);
        ++(*this);
        return copyOld;
    }

    constexpr WrappingData<dataType>& operator--()
    {
        if (--data_ < boundMin_)
            data_ = boundMax_;
        return *this;
    }

    constexpr WrappingData<dataType> operator--(int)
    {
        WrappingData copyOld(*this);
        --(*this);
        return copyOld;
    }

    constexpr WrappingData<dataType>& operator+=(dataType rhs)
    {
        data_ += rhs;
        if (data_ > boundMax_)
            data_ = boundMin_ + ((data_ - boundMax_ - 1) % modulo_);
        return *this;
    }

    constexpr WrappingData<dataType>& operator-=(dataType rhs)
    {
        data_ -= rhs;
        if (data_ < boundMin_)
            data_ = boundMax_ - ((boundMin_ - data_) % modulo_);
        return *this;
    }

    /// Copies the value, but not the bounds
    ///
    constexpr WrappingData<dataType>& operator=(dataType rhs)
    {
        data_ = rhs;
        if (data_ < boundMin_)
            data_ = boundMin_;
        else if (data_ > boundMax_)
            data_ = boundMax_;
    }
   
    constexpr bool operator==(dataType rhs)
    {
        return data_ == rhs;
    }
    constexpr bool operator!=(dataType rhs)
    {
        return !(data_ == rhs);
    }
    /// Copies the <b>bounds</b> from another \ref WrappingData
    /// 
    /// \pre Expects(boundedData.boundMax_ > boundedData.boundMin_);
    constexpr void setBounds(const WrappingData& boundedData)
    {
        Expects(boundedData.boundMax_ >= boundedData.boundMin_);

        boundMin_ = boundedData.boundMin_;
        boundMax_ = boundedData.boundMax_;
        modulo_   = boundMax_ - boundMin_ + 1;

        Ensures(modulo_ > 0);
    }
    
    ///
    /// \pre Expects(boundMax > boundMin);
    constexpr void setBounds(dataType boundMin, dataType boundMax)
    {
        Expects(boundMax >= boundMin);

        boundMin_ = boundMin;
        boundMax_ = boundMax;
        modulo_   = boundMax_ - boundMin_ + 1;

        Ensures(modulo_ > 0);
    }
   
    ///
    /// \pre Expects(boundMax_ >= boundMin);
    constexpr void setBoundMin(dataType boundMin)
    {
        Expects(boundMax_ >= boundMin);

        boundMin_ = boundMin;
        modulo_   = boundMax_ - boundMin_ + 1;

        Ensures(modulo_ > 0);
    }

    ///
    /// \pre Expects(boundMax >= boundMin_);
    constexpr void setBoundMax(dataType boundMax)
    {
        Expects(boundMax >= boundMin_);

        boundMax_ = boundMax;
        modulo_   = boundMax_ - boundMin_ + 1;

        Ensures(modulo_ > 0);
    }

private:
    dataType data_{};
    dataType boundMin_{};
    dataType boundMax_{};
    dataType modulo_{};
};