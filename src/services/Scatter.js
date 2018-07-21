import * as Eos from 'eosjs';
import {config} from '../config';

const eosAccount = acc => acc.blockchain === 'eos';
const auth = account => ({authorization:[`${account.name}@${account.authority}`]});

export class ScatterService {
  static instance = new Promise((resolve) => {
    document.addEventListener('scatterLoaded', () => {
      const scatter = window.scatter;
      window.scatter = null;
      scatter.requireVersion(3.0);
      resolve(scatter);
    });
  });

  constructor() {

    // docker local
    this.network = config.network;

    this.eosOptions = {
      broadcast: true,
      sign: true,
      chainId: this.network.chainId
    };

    this.scatter = ScatterService.instance;
  }

  async login() {
    return this.scatter
      .then(scatter => scatter.getIdentity({accounts: [this.network]}))
      .then(id => id.accounts.find(eosAccount) || Promise.reject('Authentication failed'));
  }

  remove(id) {
    return this.scatter
      .then(scatter => {
        const eos = this.getEos(scatter);
        const account = scatter.identity.accounts.find(eosAccount);

        return eos.contract(config.account, {accounts: [this.network]})
          .then(contract => contract.cancel({
              account: account.name,
              id
            }, auth(account)
          ));
      });

  }

  transfer(quantity, memo) {
    return this.scatter
      .then(scatter => {
        const eos = this.getEos(scatter);
        const account = scatter.identity.accounts.find(eosAccount);

        return eos.contract('eosio.token', {accounts: [this.network]})
          .then(contract => contract.transfer({
              from: account.name,
              to: config.account,
              quantity, memo
            }, auth(account)
          ));
      });
  }

  getEos(scatter) {
    return scatter.eos(this.network, Eos, this.eosOptions, this.network.port === 443 ? 'https' : 'http');
  }

  getTableRows() {
    return this.scatter
      .then(scatter => {
        const eos = this.getEos(scatter);

        return eos.getTableRows({
          json: true,
          code: config.account,
          scope: config.account,
          table: config.table
        });
      })
  }
}