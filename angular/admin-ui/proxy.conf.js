const PROXY_CONFIG = [
    {
        context: [
            "/login",
            "/logout",
            "/rest",
            "/fonts",
        ],
        target: "http://localhost:3000",
        secure: false,
        logLevel: "debug"
    },
    {
        context: [
            "/websocket",
        ],
        target: "ws://localhost:3000",
        secure: false,
        logLevel: "debug",
        ws: true
    }
]

module.exports = PROXY_CONFIG;
