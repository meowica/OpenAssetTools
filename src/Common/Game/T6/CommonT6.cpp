#include "CommonT6.h"

#include "Utils/Pack.h"

#include <algorithm>
#include <cmath>

using namespace T6;

PackedTexCoords Common::Vec2PackTexCoords(const float (&in)[2])
{
    return PackedTexCoords{pack32::Vec2PackTexCoordsUV(in)};
}

PackedUnitVec Common::Vec3PackUnitVec(const float (&in)[3])
{
    return PackedUnitVec{pack32::Vec3PackUnitVecThirdBased(in)};
}

GfxColor Common::Vec4PackGfxColor(const float (&in)[4])
{
    return GfxColor{pack32::Vec4PackGfxColor(in)};
}

void Common::Vec2UnpackTexCoords(const PackedTexCoords& in, float (&out)[2])
{
    pack32::Vec2UnpackTexCoordsUV(in.packed, out);
}

void Common::Vec3UnpackUnitVec(const PackedUnitVec& in, float (&out)[3])
{
    pack32::Vec3UnpackUnitVecThirdBased(in.packed, out);
}

void Common::Vec4UnpackGfxColor(const GfxColor& in, float (&out)[4])
{
    pack32::Vec4UnpackGfxColor(in.packed, out);
}

float Common::LinearToDbspl(const float linear)
{
    const auto db = 20.0f * std::log10(std::max(linear, 0.0000152879f));
    if (db > -95.0f)
        return db + 100.0f;

    return 0;
}

float Common::DbsplToLinear(const float dbsplValue)
{
    return std::pow(10.0f, (dbsplValue - 100.0f) / 20.0f);
}

float Common::HertzToCents(const float hertz)
{
    return 1200.0f * std::log2(hertz);
}

float Common::CentsToHertz(const float cents)
{
    return std::pow(2.0f, cents / 1200.0f);
}

float Common::RadToDeg(const float radians)
{
    return radians * (180.0f / M_PI);
}

void Common::ToEulerAnglesRad(const vec4_t* matrix, vec3_t* out)
{
    constexpr float GIMBAL_LOCK_THRESHOLD = 0.005f;

    const float a = asinf(-matrix[0].v[2]);
    const float cosA = cosf(a);

    if (fabsf(cosA) > GIMBAL_LOCK_THRESHOLD)
    {
        out->x = atan2f(matrix[1].v[2] / cosA, matrix[2].v[2] / cosA);
        out->y = a;
        out->z = atan2f(matrix[0].v[1] / cosA, matrix[0].v[0] / cosA);
    }
    else
    {
        out->x = atan2f(-matrix[2].v[1], matrix[1].v[1]);
        out->y = a;
        out->z = 0.0f;
    }
}

void Common::ToEulerAnglesDeg(const vec4_t* matrix, vec3_t* out)
{
    vec3_t euler_rad;
    ToEulerAnglesRad(matrix, &euler_rad);
    out->x = RadToDeg(euler_rad.x);
    out->y = RadToDeg(euler_rad.y);
    out->z = RadToDeg(euler_rad.z);
}
