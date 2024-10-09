// script.js
document.getElementById('new-work-order').addEventListener('click', function() {
    alert('Funcionalidad para crear una nueva orden de trabajo.');
});

document.getElementById('generate-report').addEventListener('click', function() {
    document.getElementById('report-output').innerText = 'Informe generado exitosamente.';
});
