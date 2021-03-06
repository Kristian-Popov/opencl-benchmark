#include <functional>
#include <memory>

#include "cl_benchmark_main.hpp"
#include "fixtures/cuboid_opencl_fixture.h"
#include "fixtures/factorial_opencl_fixture.h"

using namespace kpv::cl_benchmark;

namespace {
template <typename T>
struct OpenClTypeTraits {
    static const char* const short_description;
};

template <>
const char* const OpenClTypeTraits<float>::short_description = "single precision";
template <>
const char* const OpenClTypeTraits<double>::short_description = "double precision";

FixtureFamily CreateFactorialFixture(const PlatformList& platform_list, int32_t data_size) {
    FixtureFamily fixture_family;
    fixture_family.name = (boost::format("Factorial, %1% elements") % data_size).str();
    fixture_family.element_count = data_size;
    for (auto& platform : platform_list.OpenClPlatforms()) {
        for (auto& device : platform->GetDevices()) {
            fixture_family.fixtures.insert(
                std::make_pair<const FixtureId, std::shared_ptr<Fixture>>(
                    FixtureId(fixture_family.name, device, ""),
                    std::make_shared<kpv::FactorialOpenClFixture>(
                        std::dynamic_pointer_cast<OpenClDevice>(device), data_size)));
        }
    }
    return fixture_family;
}

template <typename T>
FixtureFamily CreateCuboidFixture(const PlatformList& platform_list, int32_t data_size) {
    FixtureFamily fixture_family;
    fixture_family.name = (boost::format("Cuboid, %1%, %2% cuboids") %
                           OpenClTypeTraits<T>::short_description % data_size)
                              .str();
    fixture_family.element_count = data_size;
    for (auto& platform : platform_list.OpenClPlatforms()) {
        for (auto& device : platform->GetDevices()) {
            fixture_family.fixtures.insert(
                std::make_pair<const FixtureId, std::shared_ptr<Fixture>>(
                    FixtureId(fixture_family.name, device, ""),
                    std::make_shared<kpv::CuboidOpenClFixture<T>>(
                        std::dynamic_pointer_cast<OpenClDevice>(device), data_size)));
        }
    }
    return fixture_family;
}
}  // namespace

using namespace ::std::placeholders;

REGISTER_FIXTURE("trivial-factorial", std::bind(&CreateFactorialFixture, _1, 100));
REGISTER_FIXTURE("trivial-factorial", std::bind(&CreateFactorialFixture, _1, 1000));
REGISTER_FIXTURE("trivial-factorial", std::bind(&CreateFactorialFixture, _1, 100000));
REGISTER_FIXTURE("trivial-factorial", std::bind(&CreateFactorialFixture, _1, 1000000));
REGISTER_FIXTURE("cuboid", std::bind(&CreateCuboidFixture<float>, _1, 100));
REGISTER_FIXTURE("cuboid", std::bind(&CreateCuboidFixture<float>, _1, 1000));
REGISTER_FIXTURE("cuboid", std::bind(&CreateCuboidFixture<double>, _1, 100000));
REGISTER_FIXTURE("cuboid", std::bind(&CreateCuboidFixture<double>, _1, 1000000));
