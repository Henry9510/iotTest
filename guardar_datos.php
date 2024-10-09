<?php
$servername = "localhost";
$username = "root"; // Cambia si tienes un usuario diferente
$password = ""; // Cambia si tienes una contraseña
$dbname = "hub_datos"; // Nombre de la base de datos

// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Obtener los datos del sensor
$sensor_id = $_GET['sensor_id'];
$temperatura = $_GET['temperatura'];
$humedad = $_GET['humedad'];

// Insertar datos en la base de datos
$sql = "INSERT INTO sensor_data (sensor_id, temperatura, humedad, timestamp) VALUES ('$sensor_id', $temperatura, $humedad, NOW())";

if ($conn->query($sql) === TRUE) {
    echo "Datos guardados con éxito";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
