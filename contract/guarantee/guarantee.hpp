#include <eosiolib/asset.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>
#include <string>

namespace Falci {
    using namespace eosio;
    using std::string;

    class Guarantee : public contract {
        using contract::contract;

        public:
            Guarantee(account_name self)
              : contract(self){}

            void apply(account_name contract, account_name act);

            void onTransfer(const account_name contract, const account_name act);

            //@abi action
            void cancel(const account_name account_a, const uint64_t id);

            //@abi action
            void update(const account_name account, const account_name account_a, const uint64_t id, const uint64_t status);

        private:
            void create(const account_name account_a, const uint64_t id, const asset amount_a, const asset amount_b);

            void pay(const account_name account_b, const string memo, const asset amount_b);

            struct cancel_t {
                account_name account_a;
                uint64_t id;
            };

            struct update_t {
                account_name account;
                account_name account_a;
                uint64_t id;
                uint64_t status;
            };

            //@abi table contracts i64
            struct contracts {
                uint64_t     id;
                account_name account_a;
                account_name account_b;
                asset        amount_a;
                asset        amount_b;
                uint64_t     status_a;
                uint64_t     status_b;

                // Status:
                // 0: account not defined
                // 1: waiting
                // 2: approved
                // 3: rejected

                auto primary_key() const { return id; }

                EOSLIB_SERIALIZE(contracts, (id)(account_a)(account_b)(amount_a)(amount_b)(status_a)(status_b))
            };

            typedef multi_index<N(contracts), contracts> contractsIndex;
    };

}