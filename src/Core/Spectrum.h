#ifndef __PBR_SPECTRUM_H__
#define __PBR_SPECTRUM_H__

#include <PBR.h>
#include <PBRMath.h>

using namespace pbr::math;

namespace pbr {

    static const float Gamma = 2.4f; // 2.4 prev: 2.2

    class PBR_SHARED RGBSpectrum {
    public:
        float r, g, b;

        RGBSpectrum();
        RGBSpectrum(float r, float g, float b);
        explicit RGBSpectrum(float s);
        explicit RGBSpectrum(const Vec3& v);

        RGBSpectrum  operator+ (const RGBSpectrum& rgb) const;
        RGBSpectrum& operator+=(const RGBSpectrum& rgb);

        RGBSpectrum  operator- (const RGBSpectrum& rgb) const;
        RGBSpectrum& operator-=(const RGBSpectrum& rgb);

        RGBSpectrum  operator* (float scalar) const;
        RGBSpectrum& operator*=(float scalar);

        RGBSpectrum  operator* (const RGBSpectrum& rgb) const;
        RGBSpectrum& operator*=(const RGBSpectrum& rgb);

        RGBSpectrum  operator/ (float scalar) const;
        RGBSpectrum& operator/=(float scalar);

        RGBSpectrum  operator/ (const RGBSpectrum& scalar) const;
        RGBSpectrum& operator/=(const RGBSpectrum& scalar);

        float  operator[](uint32 idx) const;
        float& operator[](uint32 idx);

        float max() const;
        float min() const;

        void  clamp(float low, float high);
        bool  isBlack() const;
        float lum() const;

        static const RGBSpectrum BLACK;
    };

    // Standard input/ouput
    PBR_SHARED std::istream& operator>>(std::istream& is, RGBSpectrum& spectrum);
    PBR_SHARED std::ostream& operator<<(std::ostream& os, const RGBSpectrum& spectrum);

    PBR_SHARED RGBSpectrum operator*(float scalar, const RGBSpectrum& rgb);

    PBR_SHARED RGBSpectrum clamp(const RGBSpectrum& rgb, float low, float high);
    PBR_SHARED RGBSpectrum exp  (const RGBSpectrum& rgb);
    PBR_SHARED RGBSpectrum lerp (const RGBSpectrum& rgb1, const RGBSpectrum& rgb2, float t);

    /* ----------------------------------------------------------
            1931 CIE XYZ Color Matching Function
            http://cvrl.ioo.ucl.ac.uk/cmfs.htm
    ---------------------------------------------------------*/
    static PBR_CONSTEXPR uint32 CIENumSamples  = 471;
    static PBR_CONSTEXPR double CIELambdaStart = 360.0;
    static PBR_CONSTEXPR double CIELambdaEnd   = 830.0;
    static PBR_CONSTEXPR double CIEYIntegral   = 106.856895;

    PBR_SHARED extern const double CIELambdas[];
    PBR_SHARED extern const double CIESamplesX[];
    PBR_SHARED extern const double CIESamplesY[];
    PBR_SHARED extern const double CIESamplesZ[];
    PBR_SHARED extern const RGBSpectrum CIEXYZ[];

    inline PBR_SHARED RGBSpectrum XYZToRGB(RGBSpectrum xyz) {
        return RGBSpectrum(
             3.240479 * xyz.r + -1.537150 * xyz.g + -0.498535 * xyz.b,
            -0.969256 * xyz.r +  1.875991 * xyz.g +  0.041556 * xyz.b,
             0.055648 * xyz.r + -0.204043 * xyz.g +  1.057311 * xyz.b
        );
    }

    inline PBR_SHARED RGBSpectrum RGBToXYZ(RGBSpectrum rgb) {
        return RGBSpectrum(
            0.412453 * rgb.r + 0.357580 * rgb.g + 0.180423 * rgb.b,
            0.212671 * rgb.r + 0.715160 * rgb.g + 0.072179 * rgb.b,
            0.019334 * rgb.r + 0.119193 * rgb.g + 0.950227 * rgb.b
        );
    }

    class SpectralDistribution {
    public:
        SpectralDistribution(const std::vector<float>& lambdas, const std::vector<float>& vals);
        explicit SpectralDistribution(const std::string& filename);

        float linearInterpolate(float lambda) const;
        RGBSpectrum toRGB() const;

    private:
        std::size_t _num;
        std::vector<float> _lambdas;
        std::vector<float> _vals;
    };

    typedef RGBSpectrum Color;
}

#endif