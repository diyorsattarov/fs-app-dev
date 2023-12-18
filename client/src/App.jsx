import React, { useState, useEffect } from 'react';
import reactLogo from './assets/react.svg';
import viteLogo from '/vite.svg';
import './App.css';

function App() {
  const [count, setCount] = useState(0);
  const [cppServiceData, setCppServiceData] = useState('');
  const [postData, setPostData] = useState('');  // Declare postData state

  useEffect(() => {
    fetch('http://localhost:3000/hello')
      .then(response => response.text())
      .then(data => {
        setCppServiceData(data);
      })
      .catch(error => {
        console.error('Error fetching data:', error);
      });
  }, []);

  const handlePostRequest = () => {
    fetch('http://localhost:3000/cpp-service-post', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ data: postData })
    })
    .then(response => response.text())
    .then(data => {
      console.log('Post response:', data);
    })
    .catch(error => {
      console.error('Error posting data:', error);
    });
  };

  return (
    <>
      <div>
        <input
          type="text"
          value={postData}
          onChange={(e) => setPostData(e.target.value)}
          placeholder="Enter data to send"
        />
        <button onClick={handlePostRequest}>Send POST Request</button>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        {/* Existing button */}
      </div>
      <p className="read-the-docs">
        {/* Existing content */}
      </p>
      {cppServiceData && <p>Data from C++ Service: {cppServiceData}</p>}
    </>
  );
}

export default App;