import React, { Component } from 'react';
import { Switch, Route } from 'react-router-dom';
import {Navbar} from './components/navbar/Navbar';
import {Footer} from './components/footer/Footer';
import {Home} from './components/home/Home';
import {Create} from './components/create/Create';
import {Details} from './components/details/Details';
import {Listing} from './components/listing/Listing';
import './App.css';

class App extends Component {
  render() {
    return (
      <div>
        <Navbar />
        <div className="container page">
          <Switch>
            <Route exact path='/' component={Home}/>
            <Route path='/new' component={Create}/>
            <Route path='/open' component={Listing}/>
            <Route path='/details' component={Details}/>
            {/*<Route path='/about' component={About}/>*/}
          </Switch>
        </div>
        <Footer />
      </div>
    );
  }
}

export default App;
