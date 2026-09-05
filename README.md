# Project-Mote<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Project-Mote // Live Telemetry</title>
    <style>
        :root {
            --bg-color: #050505;
            --panel-bg: #111;
            --text-main: #00ffcc;
            --text-accent: #ff0055;
            --border-color: #333;
            --font-stack: 'Courier New', Courier, monospace;
        }

        body {
            background-color: var(--bg-color);
            color: var(--text-main);
            font-family: var(--font-stack);
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        header {
            width: 100%;
            max-width: 1000px;
            border-bottom: 2px solid var(--text-main);
            padding-bottom: 10px;
            margin-bottom: 30px;
            display: flex;
            justify-content: space-between;
            align-items: flex-end;
        }

        h1 {
            margin: 0;
            font-size: 2.5rem;
            text-transform: uppercase;
            letter-spacing: 2px;
        }

        .status-blink {
            color: var(--text-accent);
            animation: blink 1s step-end infinite;
            font-weight: bold;
        }

        @keyframes blink {
            50% { opacity: 0; }
        }

        .dashboard-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            width: 100%;
            max-width: 1000px;
        }

        .panel {
            background-color: var(--panel-bg);
            border: 1px solid var(--border-color);
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0, 255, 204, 0.1);
        }

        .panel-header {
            font-size: 0.9rem;
            color: #888;
            border-bottom: 1px solid #333;
            padding-bottom: 5px;
            margin-bottom: 15px;
            text-transform: uppercase;
        }

        .data-value {
            font-size: 3rem;
            font-weight: bold;
            margin: 0;
            text-shadow: 0 0 5px var(--text-main);
        }

        .unit {
            font-size: 1.2rem;
            color: #aaa;
        }

        .footer-logs {
            margin-top: 40px;
            width: 100%;
            max-width: 1000px;
            background: var(--panel-bg);
            padding: 15px;
            border-left: 4px solid var(--text-accent);
            font-size: 0.85rem;
            color: #aaa;
        }
    </style>
</head>
<body>

    <header>
        <div>
            <h1>Project-Mote // Cyber-Spider</h1>
            <div>Hack Club Stardance Mission | Lead Engineer: Aryabhart Tenjuku</div>
        </div>
        <div class="status-blink">SYSTEM ONLINE // DATA LINK ACTIVE</div>
    </header>

    <div class="dashboard-grid">
        <!-- BMV080 Laser Data -->
        <div class="panel">
            <div class="panel-header">BMV080 // Laser Altimeter</div>
            <div class="data-value"><span id="alt-data">14.2</span><span class="unit"> m</span></div>
        </div>

        <!-- BME688 Temp Data -->
        <div class="panel">
            <div class="panel-header">BME688 // Microclimate Temp</div>
            <div class="data-value"><span id="temp-data">22.4</span><span class="unit"> °C</span></div>
        </div>

        <!-- BME688 Pressure Data -->
        <div class="panel">
            <div class="panel-header">BME688 // Atmospheric Pressure</div>
            <div class="data-value"><span id="pres-data">1012.5</span><span class="unit"> hPa</span></div>
        </div>

        <!-- System Status -->
        <div class="panel">
            <div class="panel-header">XIAO ESP32-S3 // I2C Bus</div>
            <div class="data-value" style="color: #00ffcc; font-size: 2.2rem;">NOMINAL</div>
        </div>
    </div>

    <div class="footer-logs">
        > INITIALIZING FLIGHT SEQUENCE...<br>
        > I2C BUS [SDA/SCL] ESTABLISHED...<br>
        > SENSOR THERMAL OFFSET CALIBRATED...<br>
        > ACQUIRING LIVE TELEMETRY...
    </div>

    <script>
        // Add a little jitter to the numbers to make it look like live drone flight data!
        setInterval(() => {
            const tempBase = 22.4;
            const altBase = 14.2;
            const presBase = 1012.5;

            document.getElementById('temp-data').innerText = (tempBase + (Math.random() * 0.4 - 0.2)).toFixed(2);
            document.getElementById('alt-data').innerText = (altBase + (Math.random() * 0.8 - 0.4)).toFixed(2);
            document.getElementById('pres-data').innerText = (presBase + (Math.random() * 1.2 - 0.6)).toFixed(1);
        }, 800);
    </script>

</body>
</html>
