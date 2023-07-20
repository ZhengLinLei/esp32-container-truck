window.addEventListener('load', () => {
    // Create a new WebSocket.
    const socket = new WebSocket('ws://192.168.1.2:7890');
    let codeTerminal = document.querySelector("#terminal");

    // ================ Helper Functions =================
    function updateCodeBlock(json) {
        let text = JSON.stringify(json, null, "\t");
        codeTerminal.innerHTML += text + "\n";
        Prism.highlightElement(codeTerminal);
    }

    // Connection opened
    socket.addEventListener('open', function (event) {
        let welcome = {
            "status": "success",
            "message": "Welcome to Team 11 Backend Control Center",
            "server": "connected",
            "host": "127.0.0.1",
            "port": 7890,
        }
        updateCodeBlock(welcome);
    });
    // Failed
    socket.addEventListener('error', function (event) {
        let welcome = {
            "status": "error",
            "message": "Failed to connect to server",
            "server": "disconnected",
            "error": event,
            "host": "127.0.0.1",
            "port": 7890,
        }
        updateCodeBlock(welcome);
    });
    document.querySelector("button.btn").addEventListener('click', () => {
        console.log("Send Scan");
        let welcome = {
            "status": "success",
            "message": "Scan started",
            "server": "connected",
        }
        updateCodeBlock(welcome);
        socket.send('scan');
    });
    document.querySelector("button.gen").addEventListener('click', () => {
        console.log("Send load");
        let welcome = {
            "status": "success",
            "message": "Load started",
            "server": "connected",
        }
        updateCodeBlock(welcome);
        socket.send('load');
    });

    // Listen for messages
    socket.addEventListener('message', function (event) {
        console.log(event.data);
        // Formate and write into code block
        updateCodeBlock(JSON.parse(event.data));
    });


    
});