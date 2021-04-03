const express = require("express");
const fetch = require("node-fetch");

const app = express();
const port = 5000;

//flase = Unlocked
//True = locked
const state = false;

//Endpoint to get state of system
app.get("/state", (req, res) => {
  res.json({ state: state });
});

//Endpoint to toggle state
app.get("/toggle", async (req, res) => {
  try {
    //TODO: Upate with solenoid server ip later
    await fetch("");
    state = state ? false : true;
  } catch (error) {
    console.log("Can't reach solenoid server");
  }
  res.send("toggle route");
});

app.listen(port, () => {
  console.log(`Server started on localhost:${port}`);
});
