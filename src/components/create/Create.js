import React, {Component} from 'react';

export class Create extends Component {
  render() {
    return (
      <div className="row">
        <div className="col-sm-6">
          <form>
            <fieldset>
              <legend>Create new contract</legend>
              <div className="form-group">
                <label htmlFor="contractId">Contract ID:</label>
                <input type="text" className="form-control" id="contractId" aria-describedby="idHelp" placeholder="My-contract-123" />
                <small id="idHelp" className="form-text text-muted">0/15</small>
              </div>

              <div className="form-group">
                <label htmlFor="my">Your side (contractor):</label>
                <input type="text" className="form-control" id="my" aria-describedby="myHelp" placeholder="100.00" />
                <small id="myHelp" className="form-text text-muted">How much EOS will <strong>you</strong> (contractor) put in this contract?</small>
              </div>

              <div className="form-group">
                <label htmlFor="other">Other side (contracted):</label>
                <input type="text" className="form-control" id="other" aria-describedby="otherHelp" placeholder="100.00" />
                <small id="otherHelp" className="form-text text-muted">How much EOS will the contracted put in this contract?</small>
              </div>

              <button type="submit" className="btn btn-primary">Submit</button>
            </fieldset>
          </form>
        </div>
      </div>
    )
  }
}