<?php
$servername = "localhost";
$username = "root"; // Cambia si tienes un usuario diferente
$password = ""; // Cambia si tienes una contraseña
$dbname = "hub_datos"; // Nombre de tu base de datos

// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Consulta para obtener los datos
$sql = "SELECT id, sensor_id, temperatura, humedad, tiempo_encendido, timestamp FROM sensor_data ORDER BY timestamp DESC";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Datos del Sensor</title>
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
        }
        th, td {
            padding: 10px;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>Datos de Sensores Almacenados</h1>

    <table>
        <tr>
            <th>ID</th>
            <th>ID del Sensor</th>
            <th>Temperatura (°C)</th>
            <th>Humedad (%)</th>
            <th>Tiempo de Encendido (s)</th>
            <th>Fecha y Hora</th>
        </tr>

        <?php
        if ($result->num_rows > 0) {
            // Mostrar los datos en la tabla
            while($row = $result->fetch_assoc()) {
                echo "<tr>";
                echo "<td>" . $row["id"] . "</td>";
                echo "<td>" . $row["sensor_id"] . "</td>";
                echo "<td>" . $row["temperatura"] . "</td>";
                echo "<td>" . $row["humedad"] . "</td>";
                echo "<td>" . $row["tiempo_encendido"] . "</td>";
                echo "<td>" . $row["timestamp"] . "</td>";
                echo "</tr>";
            }
        } else {
            echo "<tr><td colspan='6'>No hay datos disponibles</td></tr>";
        }
        ?>

    </table>

    <?php $conn->close(); // Cerrar conexión ?>
</body>
</html>
