#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

namespace atomicassetsrng
{
    static constexpr eosio::name ATOMICASSETS_ACCOUNT = eosio::name("atomicassets");

    struct FORMAT
    {
        std::string name;
        std::string type;
    };

    struct config_s
    {
        uint64_t asset_counter;
        int32_t template_counter;
        uint64_t offer_counter;
        std::vector<FORMAT> collection_format;
        std::vector<eosio::extended_symbol> supported_tokens;
    };

    typedef eosio::singleton<eosio::name("config"), config_s> config_tbl;

    config_s get_config()
    {
        config_tbl aa_conf(ATOMICASSETS_ACCOUNT, ATOMICASSETS_ACCOUNT.value);
        return aa_conf.get();
    }
}

namespace atomicmarketrng
{
    static constexpr eosio::name ATOMICMARKET_ACCOUNT = eosio::name("atomicmarket");

    struct counters_s
    {
        eosio::name counter_name;
        uint64_t counter_value;

        auto primary_key() const { return counter_name.value; }
    };

    typedef eosio::multi_index<eosio::name("counters"), counters_s> counters_tbl;

    counters_tbl get_counters()
    {
        return counters_tbl(ATOMICMARKET_ACCOUNT, ATOMICMARKET_ACCOUNT.value);
    }
}

namespace eosiorng
{
    static constexpr eosio::name EOSIO_ACCOUNT = eosio::name("eosio");

    struct eosio_global_1_s
    {
        uint64_t max_ram_size;
        uint64_t total_ram_bytes_reserved;
        int64_t total_ram_stake;
        eosio::block_timestamp_type last_producer_schedule_update;
        eosio::time_point last_pervote_bucket_fill;
        int64_t pervote_bucket;
        int64_t perblock_bucket;
        int64_t voters_bucket;
        float total_voteshare_change_rate;
        float total_unpaid_voteshare;
        eosio::time_point total_unpaid_voteshare_last_updated;
        uint32_t total_unpaid_blocks;
        int64_t total_activated_stake;
        eosio::time_point thresh_activated_stake_time;
        uint16_t last_producer_schedule_size;
        float total_producer_vote_weight;
        eosio::block_timestamp_type last_name_close;
    };

    struct eosio_global_2_s
    {
        uint16_t new_ram_per_block;
        eosio::block_timestamp_type last_ram_increase;
        eosio::block_timestamp_type last_block_num;
        float total_producer_votepay_share;
        uint8_t revision;
    };

    struct eosio_global_3_s
    {
        eosio::time_point last_vpay_state_update;
        float total_vpay_share_change_rate;
    };

    typedef eosio::singleton<eosio::name("global"), eosio_global_1_s> global_1_tbl;
    typedef eosio::singleton<eosio::name("global2"), eosio_global_2_s> global_2_tbl;
    typedef eosio::singleton<eosio::name("global3"), eosio_global_3_s> global_3_tbl;

    eosio_global_1_s get_global_1()
    {
        global_1_tbl eosio_g1(EOSIO_ACCOUNT, EOSIO_ACCOUNT.value);
        return eosio_g1.get();
    }

    eosio_global_2_s get_global_2()
    {
        global_2_tbl eosio_g2(EOSIO_ACCOUNT, EOSIO_ACCOUNT.value);
        return eosio_g2.get();
    }

    eosio_global_3_s get_global_3()
    {
        global_3_tbl eosio_g3(EOSIO_ACCOUNT, EOSIO_ACCOUNT.value);
        return eosio_g3.get();
    }
}

namespace mfederationrng
{
    static constexpr eosio::name M_FEDERATION_ACCOUNT = eosio::name("m.federation");
    static constexpr eosio::name PLANETS[6] = {
        eosio::name("eyeke.world"),
        eosio::name("kavian.world"),
        eosio::name("magor.world"),
        eosio::name("naron.world"),
        eosio::name("neri.world"),
        eosio::name("veles.world")};

    struct state_3_s
    {
        eosio::time_point last_fill_time;
        float fill_rate;
        eosio::asset bucket_total;
        eosio::asset mine_bucket;
    };

    typedef eosio::singleton<eosio::name("state3"), state_3_s> state_3_tbl;

    state_3_s get_state_3(const eosio::name &planet)
    {
        state_3_tbl state_3(M_FEDERATION_ACCOUNT, planet.value);
        return state_3.get();
    }
}

namespace pseudorng
{
    /**
     * Fetch entropy from various contract tables
     *
     * @return the entropy values
     */
    std::vector<uint64_t> fetch_entropy()
    {
        std::vector<uint64_t> values = {};
        // add this in case none of the other contracts are available
        values.push_back((uint64_t)eosio::current_time_point().elapsed.count());

        // check if `eosio` account exists and fetch entropy from it
        if (is_account(eosiorng::EOSIO_ACCOUNT))
        {
            auto g1_conf = eosiorng::get_global_1();
            values.push_back(g1_conf.max_ram_size);
            values.push_back(g1_conf.total_ram_bytes_reserved);
            values.push_back(g1_conf.total_ram_stake);
            values.push_back(g1_conf.total_voteshare_change_rate);
            values.push_back(g1_conf.total_unpaid_voteshare);
            values.push_back(g1_conf.total_unpaid_blocks);

            auto g2_conf = eosiorng::get_global_2();
            values.push_back(g2_conf.new_ram_per_block);
            values.push_back(g2_conf.total_producer_votepay_share);

            auto g3_conf = eosiorng::get_global_3();
            values.push_back(g3_conf.total_vpay_share_change_rate);
        }

        // check if `atomicassets` account exists and fetch entropy from it
        if (is_account(atomicassetsrng::ATOMICASSETS_ACCOUNT))
        {
            auto aa_conf = atomicassetsrng::get_config();
            values.push_back(aa_conf.asset_counter);
            values.push_back(aa_conf.offer_counter);
            values.push_back(aa_conf.template_counter);
        }

        // check if `atomicmarket` account exists and fetch entropy from it
        if (is_account(atomicmarketrng::ATOMICMARKET_ACCOUNT))
        {
            auto am_counters = atomicmarketrng::get_counters();
            for (auto itr = am_counters.begin(); itr != am_counters.end(); itr++)
            {
                values.push_back(itr->counter_value);
            }
        }

        // check if `m.federation` account exists and fetch entropy from it
        if (is_account(mfederationrng::M_FEDERATION_ACCOUNT))
        {
            for (auto &planet : mfederationrng::PLANETS)
            {
                auto state_3 = mfederationrng::get_state_3(planet);
                values.push_back(state_3.last_fill_time.elapsed.count());
                values.push_back(state_3.fill_rate);
                values.push_back(state_3.bucket_total.amount);
                values.push_back(state_3.mine_bucket.amount);
            }
        }

        return values;
    }

    /**
     * Generate a new random uint64
     *
     * @param min lower bound of the requested integer.
     * @param max upper bound of the requested integer (must be greater than min).
     *
     * @return The generated random int
     */
    uint64_t requestrand(uint64_t min, uint64_t max)
    {
        eosio::check(max > min, "max must be greater than min");

        uint64_t seed = 0;
        const auto &entropy = fetch_entropy();

        for (auto &v : entropy)
        {
            seed ^= v;
        }

        const uint64_t &random_int = seed;

        auto diff = max - min;
        auto rand_result = (random_int % diff) + min;

        return rand_result;
    }
}
