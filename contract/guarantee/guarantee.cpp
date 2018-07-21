#include "guarantee.hpp"

namespace Falci {

    void Guarantee::apply(account_name contract, account_name act) {
        switch (act) {
            case N(transfer): {
                onTransfer(contract, act);
                break;
            }

            case N(cancel): {
                auto tx = unpack_action_data<cancel_t>();
                cancel(tx.account_a, tx.id);
                break;
            }

            case N(update): {
                auto tx = unpack_action_data<update_t>();
                update(tx.account, tx.account_a, tx.id, tx.status);
                break;
            }
        }
    }

    void Guarantee::onTransfer(account_name contract, account_name act) {
        eosio_assert(act == N(transfer), "invalid action, use eosio.token::transfer");
        eosio_assert(contract == N(eosio.token), "invalid contract, use eosio.token");

        auto tx = unpack_action_data<currency::transfer>();

        if (tx.from == _self) {
            return;
        }
        eosio_assert( tx.quantity.symbol == asset().symbol, "asset must be system token" );
        eosio_assert( tx.quantity.is_valid(), "invalid asset" );
        eosio_assert( tx.quantity.amount > 0, "asset must be positive quantity" );

        string memo( tx.memo );
        string::size_type index = memo.find( ";" );

        if (index != string::npos) {
            std::string id     = memo.substr (0, index);
            std::string amount = memo.substr (index + 1);

            create(tx.from, std::atol(id.c_str()), tx.quantity, asset(std::atol(amount.c_str())));
        } else {
            pay(tx.from, memo, tx.quantity);
        }
    }

    void is_valid(const asset amount) {
        eosio_assert( amount.symbol == asset().symbol, "asset must be system token" );
        eosio_assert( amount.is_valid(), "invalid prize" );
        eosio_assert( amount.amount > 0, "prize must be positive quantity" );
    }

    void Guarantee::create(const account_name account_a, const uint64_t id, const asset amount_a, const asset amount_b) {
        require_auth(account_a);
        contractsIndex contracts(_self, account_a);

        // Validate the input
        is_valid(amount_a);
        is_valid(amount_b);

        auto itr = contracts.find(id);
        eosio_assert( itr == contracts.end(), "Contract already exists" );

        contracts.emplace(account_a, [&](auto& contract) {
            contract.id        = id;
            contract.account_a = account_a;
            contract.amount_a  = amount_a;
            contract.amount_b  = amount_b;
            contract.status_a  = 1;  // waiting
            contract.status_b  = 0; // account not defined
        });
    }
    
    void Guarantee::pay(const account_name account_b, const string memo, const asset amount_b) {
        require_auth(account_b);

        string::size_type index = memo.find( ":" );
        eosio_assert(index != string::npos, "invalid id");

        is_valid(amount_b);

        account_name account_a = ::eosio::string_to_name(memo.substr(0, index).c_str());
        uint64_t id            = std::atol(memo.substr (index + 1).c_str());

        contractsIndex contracts(_self, account_a);

        print(memo.c_str());
        print("\n");
        print(name{account_a});
        print("\n");
        print(id);
        print("\n");

        auto itr = contracts.find(id);
        eosio_assert( itr != contracts.end(), "Contract not found " );
        eosio_assert( itr->amount_b == amount_b, "Incorrect amount" );
        eosio_assert( itr->account_a != account_b, "You can't pay your own contract" );
        eosio_assert( itr->status_b == 0, "This contract already has a contracted" );

        // update the contract
        contracts.modify(itr, 0, [&]( auto& contract ) {
            contract.account_b = account_b;
            contract.status_b = 1; // waiting
        });
    }

    void Guarantee::cancel(const account_name account_a, const uint64_t id) {
        require_auth(account_a);
        contractsIndex contracts(_self, account_a);

        auto itr = contracts.find(id);
        eosio_assert( itr != contracts.end(), "contract not found" );
        eosio_assert( itr->status_b == 0 || (itr->status_a == itr->status_b && itr->status_a != 1), "contract already paid" );

        if (itr->status_b == 0) {
            // refund
            action(
                   permission_level{ _self, N(active) },
                   N(eosio.token), N(transfer),
                   std::make_tuple(_self, account_a, itr->amount_a, std::string("Refund"))
            ).send();
        }

        contracts.erase(itr);
    }

    void Guarantee::update(const account_name account, const account_name account_a, const uint64_t id, const uint64_t status) {
        require_auth(account);

        eosio_assert( status == 2 || status == 3, "Invalid status" );
        contractsIndex contracts(_self, account_a);

        auto itr = contracts.find(id);
        eosio_assert( itr != contracts.end(), "Contract not found" );
        eosio_assert( itr->account_a == account || itr->account_b == account, "It's not your contract" );
        eosio_assert( itr->status_a != itr->status_b || itr->status_a == 1, "Contract already finished" );

        uint64_t status_a = itr->status_a;
        uint64_t status_b = itr->status_b;

        if (itr->account_a == account) {
            status_a = status;
        } else {
            status_b = status;
        }

        // update the contract
        contracts.modify(itr, 0, [&]( auto& contract ) {
            contract.status_a = status_a;
            contract.status_b = status_b;
        });

        if (status_a == status_b) {
            // finish the contract

            // refund account A
            action(
                   permission_level{ _self, N(active) },
                   N(eosio.token), N(transfer),
                   std::make_tuple(_self, itr->account_a, itr->amount_a, std::string("Success"))
            ).send();

            // refund account B
            action(
                   permission_level{ _self, N(active) },
                   N(eosio.token), N(transfer),
                   std::make_tuple(_self, itr->account_b, itr->amount_b, std::string("Success"))
            ).send();
        }

    }
}

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
      Falci::Guarantee ex(receiver);
      ex.apply(code, action);
      eosio_exit(0);
    }
}


