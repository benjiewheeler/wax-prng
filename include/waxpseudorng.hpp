#include "psuedorng.hpp"
#include <eosio/eosio.hpp>

using namespace eosio;
using namespace std;

/**
* Sample contract to demonstrate usage of psuedorng::requestrand
*/
CONTRACT waxpseudorng : public contract
{
public:
    using contract::contract;

    ACTION callrand(uint64_t min, uint64_t max);
    ACTION logrand(uint64_t result);

private:
};