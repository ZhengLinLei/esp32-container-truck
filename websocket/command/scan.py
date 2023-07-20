# Importing the relevant libraries
import websockets
import asyncio

from datetime import date

# The main function that will handle connection and communication 
# with the server
async def listen():
    url = "ws://192.168.1.2:7890"
    # Connect to the server
    async with websockets.connect(url) as ws:
        # Send scan message to server
        await ws.send("scan")
        # Stay alive forever, listening to incoming msgs
        while True:
            msg = await ws.recv()
            

            # Filename in today format ddmmYYYY.json
            today = date.today()
            filename = today.strftime("%d%m%Y")

            # Save to file
            # If exists remove all and write, other wise create it and start writing with [ and ]
            with open("./data/"+filename+".json", "a+") as f:
                # If file is empty
                if f.tell() == 0:
                    f.write("[")
                else:
                    # Remove last ]
                    f.seek(f.tell() - 1, 0)
                    f.truncate()
                    f.write(",")
                
                # Write the message
                f.write(msg)
                # Write ]
                f.write("]")
                

# Start the connection
asyncio.get_event_loop().run_until_complete(listen())