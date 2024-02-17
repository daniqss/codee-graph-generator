const express = require('express');
const port = process.env.PORT ?? 8080;
const app = express();

app.use(express.static('results'));

app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
