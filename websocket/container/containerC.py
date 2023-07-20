# Importing the relevant libraries
# Importing the relevant libraries
import websockets
import asyncio

# The main function that will handle connection and communication 
# with the server
async def listen():
    url = "ws://192.168.1.2:7890"
    # Connect to the server
    async with websockets.connect(url) as ws:
        # Send container
        await ws.send("container")

        while True:
            msg = await ws.recv()
            print("Received msg: " + msg)
            if msg == "scan":
                print("Received scan message from server")
                # Send {"aaa":1}
                await ws.send('{"name":"Simulated-C", "coord": [0, 0], "capa":[70, 28]}')

# Start the connection
asyncio.get_event_loop().run_until_complete(listen())