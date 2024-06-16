import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo CSV
data = pd.read_csv('Huffman/resultados.csv', sep=';')  # Asegúrate de reemplazar 'tu_archivo.csv' con la ruta a tu archivo

# Filtrar los datos para obtener solo los de tipo 'codificacion'
filtered_data = data[data['tipo_consulta'] == 'codificacion']

# Preparar los datos para la gráfica
x = filtered_data['largo_string_ingresado_en_MB']
y = filtered_data['espacio_codificado_en_MB']

# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(x, y, marker='o', linestyle='-', color='b')
plt.title('Espacio Codificado vs Largo del String Ingresado')
plt.xlabel('Largo del String Ingresado (MB)')
plt.ylabel('Espacio Codificado (MB)')
plt.grid(True)
plt.show()