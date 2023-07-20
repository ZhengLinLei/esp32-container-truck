# Importing the relevant libraries
import websockets
import asyncio

# Server data
PORT = 7890
print("Server listening on Port " + str(PORT))

# A set of connected ws clients
connected = []
trucks = []

# The main behavior function for this server
async def echo(websocket, path):
    # Store a copy of the connected client
    connected.append(["sender", websocket])
    # Handle incoming messages
    try:
        async for message in websocket:
            print("Received message from client: " + message)
            # Send a response to all connected clients except sender
     
            if message == "scan":
                print("Scanning...")
                # Change last client to receiver
                connected[-1][0] = "receiver"
                for conn in connected:
                    # Send to all connected clients a message
                    if conn[1] != websocket:
                        await conn[1].send(message)
            elif message == "load":
                print("Load")
                # for truck in trucks:
                #     await truck.send(message)
                for conn in connected:
                    await conn[1].send('load')
            elif message.startswith('{') and message.endswith('}'):
                # Send to receive client
                for conn in connected:
                    if conn[0] == "receiver":
                        await conn[1].send(message)
            elif message == "truck":
                print("A truck just connected")
                trucks.append(websocket)
            elif message == "container":
                print("A container just connected")
                trucks.append(websocket)
            else:
                print("A client just connected")

    # Handle disconnecting clients 
    except websockets.exceptions.ConnectionClosed as e:
        print("A client just disconnected")
        print(e)
    finally:
        # Remove the client from the connected list when it disconnects
        # Use map
        for i in range(len(connected)):
            if connected[i][1] == websocket:
                del connected[i]
                break
        


# Start the server
start_server = websockets.serve(echo, "192.168.1.2", PORT)
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()