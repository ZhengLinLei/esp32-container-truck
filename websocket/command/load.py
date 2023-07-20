# Importing the relevant libraries
import websockets
import asyncio

# The main function that will handle connection and communication 
# with the server
async def listen():
    url = "ws://192.168.1.2:7890"
    # Connect to the server
    async with websockets.connect(url) as ws:
        # Send scan message to server
        await ws.send("load")
        # Stay alive forever, listening to incoming msgs
        while True:
            msg = await ws.recv()
            
            print("Message received from server: ", msg)
                

# Start the connection
asyncio.get_event_loop().run_until_complete(listen())