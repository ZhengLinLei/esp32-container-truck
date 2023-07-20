# Importing the relevant libraries
import websockets
import asyncio

# The main function that will handle connection and communication 
# with the server
async def listen():
    url = "ws://127.0.0.1:7890"
    # Connect to the server
    async with websockets.connect(url) as ws:
        # Send "truck"
        await ws.send("truck")
        
        while True:
            msg = await ws.recv()
            
            # Print
            print("Received load message from server")

# Start the connection
asyncio.get_event_loop().run_until_complete(listen())