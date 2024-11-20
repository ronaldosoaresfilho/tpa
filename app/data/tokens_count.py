import re

# Abrir o arquivo e ler o conteúdo
with open('dataset.txt', 'r', encoding='utf-8') as file:
    content = file.read()

# Dividir o conteúdo em tokens usando uma expressão regular
tokens = re.findall(r'\S+', content)

# Contar o número de tokens
num_tokens = len(tokens)

print(f'O número de tokens no arquivo é: {num_tokens}')