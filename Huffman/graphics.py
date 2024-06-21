import matplotlib.pyplot as plt
import pandas as pd

# Leer los datos desde el archivo CSV
df = pd.read_csv('Huffman/resultados.csv', sep=';')

# Convertir los tipos de datos
df['tamaño_en_MB'] = df['tamaño_ingreasado_en_MB'].astype(float)
df['tiempo_promedio'] = df['tiempo_promedio'].astype(float)

# Crear las curvas para codificación y decodificación
codificar = df[df['tipo_consulta'] == 'codificacion'].groupby('tamaño_en_MB')['tiempo_promedio'].mean()
decodificar = df[df['tipo_consulta'] == 'decodificacion'].groupby('tamaño_en_MB')['tiempo_promedio'].mean()

# Crear la figura y los ejes
fig, ax = plt.subplots(figsize=(8, 6))

# Graficar las curvas
ax.plot(codificar.index, codificar.values, marker='o', label='Codificar')
ax.plot(decodificar.index, decodificar.values, marker='o', label='Decodificar')

# Configurar los ejes y la leyenda
ax.set_xlabel('Tamaño en MB')
ax.set_ylabel('Tiempo promedio')
ax.legend()

# Mostrar la gráfica
plt.show()