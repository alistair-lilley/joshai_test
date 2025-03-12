# Light Monitor

This application written in C++ monitors a set of lights, whose information is hosted on a web server located at localhost:8080.

The web application supports GET, PUT, and POST operations.

## Compilation and running

```
./compile
./main
```

### GET

The web app allows you to GET the following urls

- localhost:8080/lights

this gets a summary of all the lights on the network.

- localhost:8080/lights/<id>

This gets the details of a specific light by device ID.

### PUT/POST

These allow you to update the light device information on a per-light basis, as well as add new lights.

PUT/POSTing to localhost:8080/lights/<id> updates a specific light, while PUT/POSTing to localhost:8080/lights will create a new light.


## The Monitor

This monitor continually runs GET on localhost:8080/lights to see if any new lights are posted, and then runs GET on localhost:8080/lights/<id> for each light ID to identify if any updates have been made to any of the light devices. It GETs the json values for the lights and checks them against its record, prints any updates that have occurred, and updates its record.
