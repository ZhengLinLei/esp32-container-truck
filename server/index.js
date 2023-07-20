const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '/index.html'));
});

app.use('/static', express.static('public'));

app.listen(port, "192.168.1.2", () => {
    console.log(`Example app listening on port ${port}`);
});