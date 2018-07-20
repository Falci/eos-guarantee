import React from 'react';
import {Link} from 'react-router-dom';

export function Home() {
  return (
    <div className="jumbotron">
      <h1 className="display-3">EOS Guarantee!</h1>
      <p className="lead">
        A simple EOS contract where each side deposit tokens as a guarantee that they will honor their deal.
      </p>
      <hr className="my-4" />
      <p>Agree on a value. Each side pay. The sender than can send the good and mark this contract as done. Once the buyer receive the item, he/she can also make the contract as done and both get their tokens back.</p>
      <p>Any problem? Don't worry. The other side will honor the deal, or his/her tokens will be locked here.</p>
      <p className="lead">
        <Link className="btn btn-primary btn-lg" to="/about" role="button">Learn more</Link>
        &nbsp;
        <Link className="btn btn-success btn-lg" to="/new" role="button">Create a contract</Link>
      </p>
    </div>
  );
}