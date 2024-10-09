<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "hub_datos";

// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Obtener los últimos 20 registros
$sql = "SELECT temperatura, humedad, tiempo_encendido, timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 20";
$result = $conn->query($sql);

// Arrays para almacenar los datos
$temperaturas = [];
$humedades = [];
$timestamps = [];

while($row = $result->fetch_assoc()) {
    $temperaturas[] = $row['temperatura'];
    $humedades[] = $row['humedad'];
    $timestamps[] = $row['timestamp'];
}

$conn->close(); // Cerrar conexión
?>

<!DOCTYPE html>
<html>
<head>
    <title>Gráfico de Datos del Sensor</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <h1>Gráfico de Datos del Sensor</h1>
    <canvas id="myChart" width="400" height="200"></canvas>

    <script>
        const ctx = document.getElementById('myChart').getContext('2d');
        const myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: <?php echo json_encode($timestamps); ?>,
                datasets: [
                    {
                        label: 'Temperatura (°C)',
                        data: <?php echo json_encode($temperaturas); ?>,
                        borderColor: 'rgb(255, 99, 132)',
                        fill: false
                    },
                    {
                        label: 'Humedad (%)',
                        data: <?php echo json_encode($humedades); ?>,
                        borderColor: 'rgb(54, 162, 235)',
                        fill: false
                    }
                ]
            },
            options: {
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: 'Fecha y Hora'
                        }
                    },
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: 'Valores'
                        }
                    }
                }
            }
        });
    </script>
</body>
</html>
