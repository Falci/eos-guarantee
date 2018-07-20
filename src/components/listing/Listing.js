import React, {Component} from 'react';
import {Link} from 'react-router-dom';

export class Listing extends Component {
  render() {
    return (
      <div>
        <h2>List pending contracts</h2>

        <div className="row">
          <div className="col-sm-4">
            <div className="form-group">
              <label htmlFor="contractor">Contractor:</label>
              <input type="text" className="form-control" id="contractor" aria-describedby="contractorHelp" placeholder="" />
              <small id="contractorHelp" className="form-text text-muted">The account that has created the contract.</small>
            </div>
          </div>
        </div>

        <table className="table table-hover">
          <thead>
          <tr>
            <th scope="col">ID</th>
            <th scope="col">Contractor</th>
            <th scope="col">Contracted</th>
            <th scope="col">&nbsp;</th>
          </tr>
          </thead>
          <tbody>
          <tr>
            <td>1</td>
            <td>100 EOS <span className="badge badge-info">Waiting</span></td>
            <td>100 EOS <span className="badge badge-warning">Not yet</span></td>
            <td><Link to={`/details/1`}>Details</Link></td>
          </tr>
          <tr>
            <td>2</td>
            <td>100 EOS <span className="badge badge-info">Waiting</span></td>
            <td>100 EOS <span className="badge badge-info">Waiting</span></td>
            <td><Link to={`/details/2`}>Details</Link></td>
          </tr>
          <tr>
            <td>3</td>
            <td>100 EOS <span className="badge badge-success">Approved</span></td>
            <td>100 EOS <span className="badge badge-danger">Rejected</span></td>
            <td><Link to={`/details/3`}>Details</Link></td>
          </tr>
          </tbody>
        </table>

        <div>
          <ul className="pagination">
            <li className="page-item disabled">
              <a className="page-link" href="#">&laquo;</a>
            </li>
            <li className="page-item active">
              <a className="page-link" href="#">1</a>
            </li>
            <li className="page-item">
              <a className="page-link" href="#">2</a>
            </li>
            <li className="page-item">
              <a className="page-link" href="#">3</a>
            </li>
            <li className="page-item">
              <a className="page-link" href="#">4</a>
            </li>
            <li className="page-item">
              <a className="page-link" href="#">5</a>
            </li>
            <li className="page-item">
              <a className="page-link" href="#">&raquo;</a>
            </li>
          </ul>
        </div>
      </div>
    )
  }
}