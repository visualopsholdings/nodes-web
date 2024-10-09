import React from "react";
import logo from "./logo.svg";
import "./App.css";

function App() {
  const [data, setData] = React.useState(null);

  React.useEffect(() => {
    fetch("/rest/1.0/users/me")
      .then((res) => res.json())
      .then((data) => setData(data.fullname));
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <p>Welcome {!data ? "Loading..." : data}</p>
      </header>
    </div>
  );
}

export default App;