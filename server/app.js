const express = require('express');
const axios = require('axios');
const cors = require('cors');
const app = express();
const port = 3000;

app.use(express.json());
// CORS configuration
// Replace "*" with your client's origin in production for security
app.use(cors({
  origin: 'http://localhost'  // Assuming your client runs on localhost
}));

app.get('/', (req, res) => {
  res.send('Hello from Express');
});

app.get('/api/data', (req, res) => {
  res.json({ message: 'Hello from the Express server!' });
});

// Route to fetch data from the C++ microservice
app.get('/cpp-service', async (req, res) => {
  try {
    const response = await axios.get('http://microservice:8080/');
    res.send(response.data);
  } catch (error) {
    console.error('Error fetching data from C++ service:', error);
    res.status(500).send('Error fetching data from C++ service');
  }
});

app.get('/hello', async (req, res) => {
  try {
    const response = await axios.get('http://microservice:8080/hello');
    res.send(response.data);
  } catch (error) {
    console.error('Error fetching data from C++ service:', error);
    res.status(500).send('Error fetching data from C++ service');
  }
});

app.post('/cpp-service-post', async (req, res) => {
  try {
    // Here you can add data to be sent to the C++ service
    const postData = { key: 'value' };
    const response = await axios.post('http://microservice:8080/data', postData);
    res.send(response.data);
  } catch (error) {
    console.error('Error posting data to C++ service:', error);
    res.status(500).send('Error posting data to C++ service');
  }
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
