# wax-prng

PRNG contract for the WAX blockchain

## Idea

The [WAX ORNG](https://github.com/worldwide-asset-exchange/wax-orng) is an open source blockchain-native service that developers can easily integrate into their dApps, but it has a an issue: RAM usage

As of writing the ORNG contract leaves 120 bytes used in the `signvals.a` table after each call <i>_(112 for the index, 8 for the data)_</i>

For dApps with tens/hundreds of thousands of daily ORNG calls, those RAM costs start to become significant

## Usage

-   copy the `psuedorng.hpp` file to your project
-   call the `pseudorng::requestrand` method with 2 arguments `(min, max)`

## Disclaimer

This method of generating pseudo-random numbers is inherently unsafe because it uses on-chain sources of entropy (from the tables in the `eosio`, `atomicassets`, `atomicmarket` & `m.federation` contracts) which can be trivially predicted and used as an attack vector against your contract

<ins>**DO NOT**</ins> use this for any contracts that rely on randomization in a critical/financial way

This repo is just a proof-of-concept and is not meant to be used in a production environment
