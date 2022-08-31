#include <waxpseudorng.hpp>

ACTION waxpseudorng::callrand(uint64_t min, uint64_t max)
{
    auto rand_value = pseudorng::requestrand(min, max);

    action(permission_level { get_self(), name("active") }, get_self(), name("logrand"), make_tuple(rand_value))
        .send();
}

ACTION waxpseudorng::logrand(uint64_t result) { }
